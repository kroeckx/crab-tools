/*
 * Import the CRAB database into postgresql from the files provided by AGIV.
 *
 * Copyright (C) 2013 Kurt Roeckx <kurt@roeckx.be>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <shapefil.h>
#include <postgresql/libpq-fe.h>

static std::string dbf_dir;
static std::string shape_dir;

static PGconn *conn;

static void ImportTable(std::string DBFfile, std::string DBFFieldNames[], int DBFFields, std::string PGSQLFieldNames[], int PGSQLFields, std::string PGSQLTable, int coord = -1)
{
	DBFHandle hDBF = DBFOpen((dbf_dir + DBFfile).c_str(), "rb");
	if (!hDBF)
	{
		std::cerr << "Unable to open file " << DBFfile << std::endl;
		exit(1);
	}
	if (DBFGetFieldCount(hDBF) != DBFFields)
	{
		std::cerr << "Unexpected number of fields in " << DBFfile << std::endl;
		exit(1);
	}

	for (int i = 0; i < DBFFields; i++)
	{
		char FieldName[12];

		DBFGetFieldInfo(hDBF, i, FieldName, NULL, NULL);
		if (DBFFieldNames[i] != FieldName)
		{
			std::cerr << "Unexpected number of field name in " + DBFfile + ": ";
			std::cerr << FieldName << " != " << DBFFieldNames[i] << std::endl;
			exit(1);
		}
	}

	std::string copy_query = "COPY " + PGSQLTable + " (";
	for (int i = 0; i < PGSQLFields; i++)
	{
		copy_query += PGSQLFieldNames[i];
		if (i != PGSQLFields - 1)
		{
			copy_query += ", ";
		}
	}
	copy_query += ") FROM stdin";
	PGresult *res = PQexec(conn, copy_query.c_str());
	if (PQresultStatus(res) != PGRES_COPY_IN)
	{
		std::cerr << "Failed to start COPY: ";
                std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	PQclear(res);
	int rows = DBFGetRecordCount(hDBF);
	for (int row = 0; row < rows; row++)
	{
		if (DBFIsRecordDeleted(hDBF, row))
		{
			std::cerr << "Deleted row: " << row << std::endl;
			continue;
		}
		std::string copy;
		for(int field = 0; field < DBFFields; field++)
		{
			if (field == coord)
			{
				if (DBFIsAttributeNULL(hDBF, row, field) || DBFIsAttributeNULL(hDBF, row, field+1))
				{
					copy += "\\N";
					field++;
				}
				else
				{
					copy += std::string("SRID=31370;POINT(") + DBFReadStringAttribute(hDBF, row, field);
					field++;
					copy += std::string(" ") + DBFReadStringAttribute(hDBF, row, field) + ")";
				}
			}
			else if (DBFIsAttributeNULL(hDBF, row, field))
	                {
				copy += "\\N";
			}
			else
			{
				const char *data = DBFReadStringAttribute(hDBF, row, field);
				if (data[0] == '\0')
				{
					/* It's empty, treat is as NULL */
					copy += "\\N";
				}
				else
				{
					copy += data;
				}
			}
			if (field != DBFFields - 1)
			{
				copy += "\t";
			}
		}
		copy += "\n";
		if (PQputCopyData(conn, copy.c_str(), copy.size()) != 1) { std::cerr << "PQputCopyData() failed: " << std::endl; std::cerr << PQerrorMessage(conn); exit(1); }
	}
	if (PQputCopyEnd(conn, NULL) != 1)
	{
		std::cerr << "PQputCopyEnd() failed: ";
                std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	res = PQgetResult(conn);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		std::cerr << "COPY failed: ";
                std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	DBFClose(hDBF);
}

static void ImportShape(std::string shape_file, std::string DBFFieldNames[], int DBFFields, std::string PGSQLFieldNames[], int PGSQLFields, std::string PGSQLTable)
{
	SHPHandle hSHP = SHPOpen((shape_file).c_str(), "rb");
	if (!hSHP)
	{
		std::cerr << "Unable to open file " << shape_file << std::endl;
		exit(1);
	}
	int nEntities, nShapeType;
	SHPGetInfo(hSHP, &nEntities, &nShapeType, NULL, NULL);
	if (nShapeType != SHPT_POINT)
	{
		std::cerr << "Only point types are supported." << std::endl;
		exit(1);
	}
	
	std::string dbffile = shape_file + ".dbf";
	DBFHandle hDBF = DBFOpen(dbffile.c_str(), "rb");
	if (!hDBF)
	{
		std::cerr << "Unable to open file: " << dbffile << std::endl;
		exit(1);
	}
	if (DBFGetFieldCount(hDBF) != DBFFields)
	{
		std::cerr << "Unexpected number of fields in " << dbffile << std::endl;
		exit(1);
	}

	for (int i = 0; i < DBFFields; i++)
	{
		char FieldName[12];

		DBFGetFieldInfo(hDBF, i, FieldName, NULL, NULL);
		if (DBFFieldNames[i] != FieldName)
		{
			std::cerr << "Unexpected number of field name in " << dbffile << ": ";
			std::cerr << FieldName << " != " << DBFFieldNames[i] << std::endl;
			exit(1);
		}
	}

	int rows = DBFGetRecordCount(hDBF);
	if (rows != nEntities)
	{
		std::cerr << "Different number of rows in shapefile and dbase file: " << shape_file << std::endl;
		exit(1);
	}

	std::string copy_query = "COPY " + PGSQLTable + " (";
	for (int i = 0; i < PGSQLFields; i++)
	{
		copy_query += PGSQLFieldNames[i];
		if (i != PGSQLFields - 1)
		{
			copy_query += ", ";
		}
	}
	copy_query += ") FROM stdin";
	PGresult *res = PQexec(conn, copy_query.c_str());
	if (PQresultStatus(res) != PGRES_COPY_IN)
	{
		std::cerr << "Failed to start COPY: ";
                std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	PQclear(res);
	for (int row = 0; row < rows; row++)
	{
		std::string copy;
		for(int field = 0; field < DBFFields; field++)
		{
			if (DBFIsAttributeNULL(hDBF, row, field))
	                {
				copy += "\\N";
			}
			else
			{
				const char *data = DBFReadStringAttribute(hDBF, row, field);
				if (data[0] == '\0')
				{
					/* It's empty, treat is as NULL */
					copy += "\\N";
				}
				else
				{
					copy += data;
				}
			}
			copy += "\t";
		}

		SHPObject *pShape;
		pShape = SHPReadObject(hSHP, row);
		if (!pShape)
		{
			std::cerr << "Unable to read shape number: " << row << std::endl;
			exit(1);
		}
		if (pShape->nVertices != 1)
		{
			std::cerr << "Unexpected amount of vertextes row: " << row << std::endl;
			exit(1);
		}

		std::ostringstream pos;
		pos << std::fixed;
		pos.precision(2);
		pos << "SRID=31370;POINT(";	
		pos << pShape->padfX[0];
		pos << " ";
		pos << pShape->padfY[0];
		pos << ")";
		copy += pos.str();

		SHPDestroyObject(pShape);

		copy += "\n";
		if (PQputCopyData(conn, copy.c_str(), copy.size()) != 1) { std::cerr << "PQputCopyData() failed: " << std::endl; std::cerr << PQerrorMessage(conn); exit(1); }
	}
	if (PQputCopyEnd(conn, NULL) != 1)
	{
		std::cerr << "PQputCopyEnd() failed: ";
                std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	res = PQgetResult(conn);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		std::cerr << "COPY failed: ";
                std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	DBFClose(hDBF);
	SHPClose(hSHP);
}

static void ImportMunicipality()
{
	std::string DBFfile = "gem.dbf";
	std::string DBFFieldNames[] = { "ID", "NISGEMCODE", "TAALCODE", "TAALCODE2", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINBEW", "BEGINORG" };
	std::string PGSQLFieldNames[] = { "id", "nis_code", "language_id", "language2_id", "start_date", "end_date", "start_time", "start_operation_id", "start_organisation_id" };
	std::string PGSQLTable = "crab.municipality";

	ImportTable(DBFfile, DBFFieldNames, 9, PGSQLFieldNames, 9, PGSQLTable);
}

static void ImportStreetName()
{
	std::string DBFfile = "straatnm.dbf";
	std::string DBFFieldNames[] = { "ID", "NISGEMCODE", "STRAATNM", "TAALCODE", "STRAATNM2", "TAALCODE2", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW", "STRAATNM0" };
	std::string PGSQLFieldNames[] = { "id", "municipality_nis_code", "street_name", "language_id", "street_name2", "language2_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id", "street_name0" };
	std::string PGSQLTable = "crab.street_name";

	ImportTable(DBFfile, DBFFieldNames, 12, PGSQLFieldNames, 12, PGSQLTable);
}

static void ImportHouseNumber()
{
	std::string DBFfile = "huisnr.dbf";
	std::string DBFFieldNames[] = { "ID", "STRAATNMID", "HUISNR", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW", "HUISNRID0" };
	std::string PGSQLFieldNames[] = { "id", "street_name_id", "house_number", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id", "house_number_id0" };
	std::string PGSQLTable = "crab.house_number";

	ImportTable(DBFfile, DBFFieldNames, 9, PGSQLFieldNames, 9, PGSQLTable);
}

static void ImportTerrainObjects()
{
	std::string DBFfile = "terrobj.dbf";
	std::string DBFFieldNames[] = { "ID", "OBJID", "AARD", "X", "Y", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW", "KADGEMCODE" };
	std::string PGSQLFieldNames[] = { "id", "object_id", "object_type_id", "point", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id", "cadastre_municipality_code" };
	std::string PGSQLTable = "crab.terrain_object";

	ImportTable(DBFfile, DBFFieldNames, 11, PGSQLFieldNames, 10, PGSQLTable, 3);
}

static void ImportTerrainObjectHouseNumber()
{
	std::string DBFfile = "tobjhnr.dbf";
	std::string DBFFieldNames[] = { "ID", "TERROBJID", "HUISNRID", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "terrain_object_id", "house_number_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };

	std::string PGSQLTable = "crab.terrain_object_house_number";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportSubAddress()
{
	std::string DBFfile = "subadres.dbf";
	std::string DBFFieldNames[] = { "ID", "HUISNRID", "SUBADR", "AARD", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "house_number_id", "sub_address", "sub_address_type_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.sub_address";

	ImportTable(DBFfile, DBFFieldNames, 9, PGSQLFieldNames, 9, PGSQLTable);
}

static void ImportStreetObject()
{
	std::string DBFfile = "wegobj.dbf";
	std::string DBFFieldNames[] = { "ID", "OBJID", "AARD", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "object_id", "street_object_type_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.street_object";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportStreetSide()
{
	std::string DBFfile = "strkant.dbf";
	std::string DBFFieldNames[] = { "ID", "STRAATNMID", "WEGOBJID", "KANT", "BEGINPOS", "EINDPOS", "PARITEIT", "EERSTEHNR", "LAATSTEHNR", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "street_name_id", "street_object_id", "street_side_type_id", "begin_position", "end_position", "house_numbering_scheme_id", "first_house_number", "last_house_number", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.street_side";

	ImportTable(DBFfile, DBFFieldNames, 14, PGSQLFieldNames, 14, PGSQLTable);
}

static void ImportCadastreMunicipality()
{
	std::string DBFfile = "kadgem.dbf";
	std::string DBFFieldNames[] = { "ID", "KADGEMCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "cadastre_municipality_code", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.cadastre_municipality";

	ImportTable(DBFfile, DBFFieldNames, 7, PGSQLFieldNames, 7, PGSQLTable);
}

static void ImportCadastreMunicipalityName()
{
	std::string DBFfile = "kadgnm.dbf";
	std::string DBFFieldNames[] = { "ID", "KADGEMID", "KADGEMNM", "TAALCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "cadastre_municipality_id", "name", "language_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.cadastre_municipality_name";

	ImportTable(DBFfile, DBFFieldNames, 9, PGSQLFieldNames, 9, PGSQLTable);
}

static void ImportMunicipalityCadastreMunicipality()
{
	std::string DBFfile = "kadggem.dbf";
	std::string DBFFieldNames[] = { "ID", "KADGEMID", "GEMID", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "cadastre_municipality_id", "municipality_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.municipality_cadastre_municipality";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportMunicipalityName()
{
	std::string DBFfile = "gemnm.dbf";
	std::string DBFFieldNames[] = { "ID", "GEMID", "GEMNM", "TAALCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINBEW", "BEGINORG" };
	std::string PGSQLFieldNames[] = { "id", "municipality_id", "name", "language_id", "start_date", "end_date", "start_time", "start_operation_id", "start_organisation_id" };
	std::string PGSQLTable = "crab.municipality_name";

	ImportTable(DBFfile, DBFFieldNames, 9, PGSQLFieldNames, 9, PGSQLTable);
}

static void ImportPostCanton()
{
	std::string DBFfile = "postkan.dbf";
	std::string DBFFieldNames[] = { "ID", "PKANCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "postcode", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.post_canton";

	ImportTable(DBFfile, DBFFieldNames, 7, PGSQLFieldNames, 7, PGSQLTable);
}

static void ImportPostCantonName()
{
	std::string DBFfile = "postknm.dbf";
	std::string DBFFieldNames[] = { "ID", "POSTKANID", "POSTKANNM", "TAALCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "post_canton_id", "name", "language_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.post_canton_name";

	ImportTable(DBFfile, DBFFieldNames, 9, PGSQLFieldNames, 9, PGSQLTable);
}

static void ImportSubPostCanton()
{
	std::string DBFfile = "subkan.dbf";
	std::string DBFFieldNames[] = { "ID", "POSTKANID", "SUBKANNR", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINBEW", "BEGINORG" };
	std::string PGSQLFieldNames[] = { "id", "post_canton_id", "sub_canton_number", "start_date", "end_date", "start_time", "start_operation_id", "start_organisation_id" };
	std::string PGSQLTable = "crab.sub_post_canton";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportMunicipalitySubPostCanton()
{
	std::string DBFfile = "subkgem.dbf";
	std::string DBFFieldNames[] = { "ID", "SUBKANID", "GEMID", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINBEW", "BEGINORG" };
	std::string PGSQLFieldNames[] = { "id", "sub_post_canton_id", "municipality_id", "start_date", "end_date", "start_time", "start_operation_id", "start_organisation_id" };
	std::string PGSQLTable = "crab.municipality_sub_post_canton";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportHouseNumberPostCanton()
{
	std::string DBFfile = "pkancode.dbf";
	std::string DBFFieldNames[] = { "ID", "HUISNRID", "PKANCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "house_number_id", "postcode", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.house_number_post_canton";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportSubStreet()
{
	std::string DBFfile = "substr.dbf";
	std::string DBFFieldNames[] = { "ID", "STRAATCODE", "SUBKANCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "street_code", "postcode", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.sub_street";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportSubStreetName()
{
	std::string DBFfile = "substrnm.dbf";
	std::string DBFFieldNames[] = { "ID", "SUBSTRID", "SUBSTRNM", "TAALCODE", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "sub_street_id", "name", "language_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.sub_street_name";

	ImportTable(DBFfile, DBFFieldNames, 9, PGSQLFieldNames, 9, PGSQLTable);
}

static void ImportSubStreetStreetName()
{
	std::string DBFfile = "sstrstrn.dbf";
	std::string DBFFieldNames[] = { "ID", "STRAATNMID", "SUBSTRID", "BEGINDATUM", "EINDDATUM", "BEGINTIJD", "BEGINORG", "BEGINBEW" };
	std::string PGSQLFieldNames[] = { "id", "street_name_id", "sub_street_id", "start_date", "end_date", "start_time", "start_organisation_id", "start_operation_id" };
	std::string PGSQLTable = "crab.sub_street_street_name";

	ImportTable(DBFfile, DBFFieldNames, 8, PGSQLFieldNames, 8, PGSQLTable);
}

static void ImportAddressPositions()
{
	std::string shape_file = shape_dir + "CrabAdr";
	std::string DBFFieldNames[] = { "ID", "STRAATNMID", "STRAATNM", "HUISNR", "APPTNR", "BUSNR", "HNRLABEL", "NISCODE", "GEMEENTE", "POSTCODE", "HERKOMST" };
	std::string PGSQLFieldNames[] = { "id", "street_name_id", "street_name", "house_number", "appartement_number", "bus_number", "house_number_label", "nis_code", "municipality_name", "postcode", "source", "point" };

	std::string PGSQLTable = "crab.address_position";

	ImportShape(shape_file, DBFFieldNames, 11, PGSQLFieldNames, 12, PGSQLTable);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " dbase_dir shape_dir" << std::endl;
		exit(1);
	}
	dbf_dir = argv[1];
	shape_dir = argv[2];

	conn = PQconnectdb("service=gis");
	if (PQstatus(conn) != CONNECTION_OK)
	{
		std::cerr << "Connection to database failed: ";
		std::cerr << PQerrorMessage(conn);;
		PQfinish(conn);
		return 1;
	}

	/* The DBF files we get have data in latin1. */
	PQsetClientEncoding(conn, "LATIN1");

	PGresult *res;

#if 0
	res = PQexec(conn, "truncate crab.municipality, crab.street_name, crab.house_number, crab.terrain_object, crab.terrain_object_house_number, crab.sub_address, crab.street_object, crab.street_side, crab.cadastre_municipality, crab.cadastre_municipality_name, crab.municipality_cadastre_municipality, crab.municipality_name, crab.post_canton, crab.post_canton_name, crab.sub_post_canton, crab.municipality_sub_post_canton, crab.house_number_post_canton, crab.sub_street, crab.sub_street_name, crab.sub_street_street_name");
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		std::cerr << "Failed to start COPY: ";
                std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	PQclear(res);
#endif

	res = PQexec(conn, "BEGIN");
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		std::cerr << "BEGIN failed: ";
		std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	PQclear(res);

	/* The order is so that it should be able to import everything if there
	 * are foreign keys on the tables. */
	ImportMunicipality();
	ImportStreetName();
	ImportHouseNumber();
	ImportTerrainObjects();
	ImportTerrainObjectHouseNumber();
	ImportSubAddress();
	ImportStreetObject();
	ImportStreetSide();
	ImportCadastreMunicipality();
	ImportCadastreMunicipalityName();
	ImportMunicipalityCadastreMunicipality();
	ImportMunicipalityName();
	ImportPostCanton();
	ImportPostCantonName();
	ImportSubPostCanton();
	ImportMunicipalitySubPostCanton();
	ImportHouseNumberPostCanton();
	ImportSubStreet();
	ImportSubStreetName();
	ImportSubStreetStreetName();

	ImportAddressPositions();

	res = PQexec(conn, "COMMIT");
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		std::cerr << "COMMIT failed: ";
		std::cerr << PQerrorMessage(conn);
		exit(1);
	}
	PQclear(res);

	PQfinish(conn);

	return 0;
};


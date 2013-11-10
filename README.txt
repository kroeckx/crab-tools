Tools to work with the CRAB data from AGIV.

Copyright (C) 2013 Kurt Roeckx <kurt@roeckx.be>

CRAB
====

CRAB (Centraal Referentie AddressenBestand) is a database made by AGIV
that contains address information for Flanders.

Importing CRAB
==============

This will read the files as provided by AGIV and import them in
to a postgresql database.  AGIV provides 3 sources of data:
- CRAB address position:
  https://download.agiv.be/Producten/Detail?id=102&title=CRAB_adresposities

  This constains a structured database will almost all the information they
  have about addresses.  It can have multiple locations for the same address,
  or might not have a location for it at all.

  But it has various problems including that you can't tell which addresses
  exist and which don't, there might be no location for the address, it has
  multiple terrain objects for a house number and it's hard to known which one
  to look at in an automatic way.  There are also some problems with the data
  model.

  You need the dBASE version for the import tool.

- CRAB address list:
  https://download.agiv.be/Producten/Detail?id=206&title=CRAB_Adres

  This contains just a list of all addresses with what they have as best
  location of the address.  If they don't have a location they will have a best
  guess based on things like interpolation and indicate how they got it.  It
  also seems to know which addresses exist and don't exist that can't be found
  in the position database.  This is the best source for finding the location of
  an address and solves most of the problems with the position database.

  You need the shapefile version for the import tool.

- CRAB street list:
  https://download.agiv.be/Producten/Detail?id=72&title=CRAB_stratenlijst

  This contains just a list of streetnames without information about location
  and is a subset of the information in the address position database, together
  with the output of some queries that combine the results of those tables.

  We currently don't import these.

To import the data you first need to create the needed tables.  This can be done
with the crab_tables.sql file.  It doesn't have any of the constraints making it
faster to import the data.  Example of creating the tables:
$ psql service=gis -f crab_tables.sql

If you downloaded the the zip files and saves them in the positions/ and
addresslist/ directories you can use the following to import the data:
$ crab_import positions/dBASE/ addresslist/Shapefile/

Then you can add the constraints:
$ psql service=gis -f crab_constraints.sql



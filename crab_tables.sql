DROP SCHEMA crab cascade;
CREATE SCHEMA crab;

SET search_path = crab, pg_catalog;

CREATE TABLE cadastre_municipality (
    id integer NOT NULL,
    cadastre_municipality_code integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE cadastre_municipality_name (
    id integer NOT NULL,
    cadastre_municipality_id integer NOT NULL,
    name character varying(40),
    language_id character(2) NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE house_number (
    id integer NOT NULL,
    street_name_id integer NOT NULL,
    house_number character varying(11) NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer,
    start_operation_id integer NOT NULL,
    house_number_id0 character varying(37) NOT NULL
);

CREATE TABLE house_number_post_canton (
    id integer NOT NULL,
    house_number_id integer NOT NULL,
    postcode integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE house_numbering_scheme (
    id integer NOT NULL,
    name character varying
);

CREATE TABLE language (
    id character(2) NOT NULL,
    name character varying
);

CREATE TABLE municipality (
    id integer NOT NULL,
    nis_code integer NOT NULL,
    language_id character(2) NOT NULL,
    language2_id character(2),
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_operation_id integer NOT NULL,
    start_organisation_id integer NOT NULL
);

CREATE TABLE municipality_cadastre_municipality (
    id integer NOT NULL,
    cadastre_municipality_id integer NOT NULL,
    municipality_id integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE municipality_name (
    id integer NOT NULL,
    municipality_id integer NOT NULL,
    name character varying(40) NOT NULL,
    language_id character(2) NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_operation_id integer NOT NULL,
    start_organisation_id integer NOT NULL
);

CREATE TABLE municipality_sub_post_canton (
    id integer NOT NULL,
    sub_post_canton_id integer NOT NULL,
    municipality_id integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_operation_id integer NOT NULL,
    start_organisation_id integer NOT NULL
);

CREATE TABLE object_type (
    id integer NOT NULL,
    name character varying
);

CREATE TABLE operation (
    id integer NOT NULL,
    name character varying
);

CREATE TABLE organisation (
    id integer NOT NULL,
    name character varying
);

CREATE TABLE post_canton (
    id integer NOT NULL,
    postcode integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE post_canton_name (
    id integer NOT NULL,
    post_canton_id integer NOT NULL,
    name character varying(255) NOT NULL,
    language_id character(2) NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE street_object (
    id integer NOT NULL,
    object_id varchar(21) NOT NULL,
    street_object_type_id integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE street_name (
    id integer NOT NULL,
    municipality_nis_code integer NOT NULL,
    street_name character varying(80) NOT NULL,
    language_id character(2) NOT NULL,
    street_name2 character varying(80),
    language2_id character(2),
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL,
    street_name0 character varying(80)
);

CREATE TABLE street_object_type (
    id integer NOT NULL,
    name character varying
);

CREATE TABLE street_side (
    id integer NOT NULL,
    street_name_id integer NOT NULL,
    street_object_id integer NOT NULL,
    street_side_type_id integer NOT NULL,
    begin_position numeric(7,2) NOT NULL,
    end_position numeric(7,2),
    house_numbering_scheme_id integer,
    first_house_number character varying(20),
    last_house_number character varying(20),
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE street_side_type (
    id integer NOT NULL,
    name character varying
);

CREATE TABLE sub_address (
    id integer NOT NULL,
    house_number_id integer NOT NULL,
-- TODO: This shouldn't be allowed to be NULL, but the data we got contains some
-- NULLs
--    sub_address character varying(35) NOT NULL,
    sub_address character varying(35),
    sub_address_type_id integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE sub_address_type (
    id integer NOT NULL,
    name character varying
);

CREATE TABLE sub_post_canton (
    id integer NOT NULL,
    post_canton_id integer NOT NULL,
    sub_canton_number integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_operation_id integer NOT NULL,
    start_organisation_id integer NOT NULL
);

CREATE TABLE sub_street (
    id integer NOT NULL,
    street_code varchar(4) NOT NULL,
    postcode integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE sub_street_name (
    id integer NOT NULL,
    sub_street_id integer NOT NULL,
    name character varying(80),
    language_id character(2) NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE sub_street_street_name (
    id integer NOT NULL,
    street_name_id integer NOT NULL,
    sub_street_id integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE terrain_object (
    id integer NOT NULL,
    object_id character varying(21) NOT NULL,
    object_type_id integer NOT NULL,
    point public.geometry,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL,
    cadastre_municipality_code integer,
    CONSTRAINT enforce_dims_point CHECK ((public.st_ndims(point) = 2)),
    CONSTRAINT enforce_geotype_point CHECK (((public.geometrytype(point) = 'POINT'::text) OR (point IS NULL))),
    CONSTRAINT enforce_srid_point CHECK ((public.st_srid(point) = 31370))
);

CREATE TABLE terrain_object_house_number (
    id integer NOT NULL,
    terrain_object_id integer NOT NULL,
    house_number_id integer NOT NULL,
    start_date date NOT NULL,
    end_date date,
    start_time timestamp without time zone NOT NULL,
    start_organisation_id integer NOT NULL,
    start_operation_id integer NOT NULL
);

CREATE TABLE address_position (
    id integer NOT NULL,
    street_name_id integer NOT NULL,
    street_name varchar(80) NOT NULL,
    house_number varchar(11) NOT NULL,
    appartement_number varchar(11),
    bus_number varchar(11),
    house_number_label varchar(23) NOT NULL,
    nis_code integer NOT NULL,
    municipality_name varchar(40) NOT NULL,
    postcode integer NOT NULL,
    source varchar(60),
    point public.geometry NOT NULL,
    CONSTRAINT enforce_dims_point CHECK ((public.st_ndims(point) = 2)),
    CONSTRAINT enforce_geotype_point CHECK (((public.geometrytype(point) = 'POINT'::text) OR (point IS NULL))),
    CONSTRAINT enforce_srid_point CHECK ((public.st_srid(point) = 31370))
);

COPY house_numbering_scheme (id, name) FROM stdin;
1	Even
2	Odd
3	All
4	Unordered
5	No numbers
\.

COPY language (id, name) FROM stdin;
nl	Dutch
fr	French
de	German
\.

COPY object_type (id, name) FROM stdin;
1	Lot according to cadastre
2	Building according to GRB
3	Artwork accordign to GRB
4	Administrative lot according to GRB
5	Building according to municipality
99	Other
\.

COPY operation (id, name) FROM stdin;
1	Input in database
2	Filling in end date
3	Correction attributes
4	Removal from database
\.

COPY organisation (id, name) FROM stdin;
1	Municipality
2	Rijksregister
3	Administratie voor Algemene Patrimoniumdocumentatie (AAPD)
4	TeleAtlas
5	Agentschap voor Geografische Informatie Vlaanderen (AGIV)
6	Nationaal Geografisch Instituut (NGI)
7	bPost
8	NAVTEQ
9	Verrijkte Kruispuntbank voor Ondernemingen
99	Other
\.

COPY street_object_type (id, name) FROM stdin;
1	Street connection according to TeleAtlas
2	Street connection according to GRB
3	Waypoint according to GRB
4	Street connection according to NAVTEQ
5	Street connection according to municipality
99	Other
\.

COPY street_side_type (id, name) FROM stdin;
1	Left
2	Right
\.

COPY sub_address_type (id, name) FROM stdin;
1	Appartment number
2	Bus number
99	Other
\.


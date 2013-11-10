SET search_path = crab, pg_catalog;

ALTER TABLE ONLY cadastre_municipality
    ADD UNIQUE (cadastre_municipality_code);

ALTER TABLE ONLY cadastre_municipality_name
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY cadastre_municipality
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY house_number
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY house_number_post_canton
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY house_numbering_scheme
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY language
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY municipality_cadastre_municipality
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY municipality_name
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY municipality
    ADD UNIQUE (nis_code);

ALTER TABLE ONLY municipality
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY municipality_sub_post_canton
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY object_type
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY operation
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY organisation
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY post_canton_name
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY post_canton
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY post_canton
    ADD UNIQUE (postcode);

-- It's currently unclear if they should be unique or not, and currently aren't.
--ALTER TABLE ONLY street_object
--    ADD UNIQUE (object_id);

ALTER TABLE ONLY street_object
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY street_name
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY street_object_type
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY street_side
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY street_side_type
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY sub_address
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY sub_address_type
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY sub_post_canton
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY sub_street_name
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY sub_street
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY sub_street_street_name
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY terrain_object_house_number
    ADD PRIMARY KEY (id);

-- I was expecting them to be unique, but they're don't seem to be.
--ALTER TABLE ONLY terrain_object
--    ADD UNIQUE (object_id);

ALTER TABLE ONLY terrain_object
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY address_position
    ADD PRIMARY KEY (id);

ALTER TABLE ONLY cadastre_municipality_name
    ADD FOREIGN KEY (cadastre_municipality_id) REFERENCES cadastre_municipality(id);

ALTER TABLE ONLY cadastre_municipality_name
    ADD FOREIGN KEY (language_id) REFERENCES language(id);

ALTER TABLE ONLY cadastre_municipality_name
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY cadastre_municipality_name
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY cadastre_municipality
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY cadastre_municipality
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY house_number_post_canton
    ADD FOREIGN KEY (house_number_id) REFERENCES house_number(id);

ALTER TABLE ONLY house_number_post_canton
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY house_number_post_canton
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY house_number_post_canton
    ADD FOREIGN KEY (postcode) REFERENCES post_canton(postcode);

ALTER TABLE ONLY house_number
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY house_number
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY house_number
    ADD FOREIGN KEY (street_name_id) REFERENCES street_name(id);

ALTER TABLE ONLY municipality_cadastre_municipality
    ADD FOREIGN KEY (cadastre_municipality_id) REFERENCES cadastre_municipality(id);

ALTER TABLE ONLY municipality_cadastre_municipality
    ADD FOREIGN KEY (municipality_id) REFERENCES municipality(id);

ALTER TABLE ONLY municipality_cadastre_municipality
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY municipality_cadastre_municipality
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY municipality
    ADD FOREIGN KEY (language2_id) REFERENCES language(id);

ALTER TABLE ONLY municipality
    ADD FOREIGN KEY (language_id) REFERENCES language(id);

ALTER TABLE ONLY municipality_name
    ADD FOREIGN KEY (language_id) REFERENCES language(id);

ALTER TABLE ONLY municipality_name
    ADD FOREIGN KEY (municipality_id) REFERENCES municipality(id);

ALTER TABLE ONLY municipality_name
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY municipality_name
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY municipality
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY municipality
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY municipality_sub_post_canton
    ADD FOREIGN KEY (municipality_id) REFERENCES municipality(id);

ALTER TABLE ONLY municipality_sub_post_canton
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY municipality_sub_post_canton
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY municipality_sub_post_canton
    ADD FOREIGN KEY (sub_post_canton_id) REFERENCES sub_post_canton(id);

ALTER TABLE ONLY post_canton_name
    ADD FOREIGN KEY (language_id) REFERENCES language(id);

ALTER TABLE ONLY post_canton_name
    ADD FOREIGN KEY (post_canton_id) REFERENCES post_canton(id);

ALTER TABLE ONLY post_canton_name
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY post_canton_name
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY post_canton
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY post_canton
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY street_object
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY street_object
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY street_object
    ADD FOREIGN KEY (street_object_type_id) REFERENCES street_object_type(id);

ALTER TABLE ONLY street_name
    ADD FOREIGN KEY (language2_id) REFERENCES language(id);

ALTER TABLE ONLY street_name
    ADD FOREIGN KEY (language_id) REFERENCES language(id);

ALTER TABLE ONLY street_name
    ADD FOREIGN KEY (municipality_nis_code) REFERENCES municipality(nis_code);

ALTER TABLE ONLY street_name
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY street_name
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY street_side
    ADD FOREIGN KEY (house_numbering_scheme_id) REFERENCES house_numbering_scheme(id);

ALTER TABLE ONLY street_side
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY street_side
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY street_side
    ADD FOREIGN KEY (street_object_id) REFERENCES street_object(id);

ALTER TABLE ONLY street_side
    ADD FOREIGN KEY (street_name_id) REFERENCES street_name(id);

ALTER TABLE ONLY street_side
    ADD FOREIGN KEY (street_side_type_id) REFERENCES street_side_type(id);

ALTER TABLE ONLY sub_address
    ADD FOREIGN KEY (house_number_id) REFERENCES house_number(id);

ALTER TABLE ONLY sub_address
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY sub_address
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY sub_address
    ADD FOREIGN KEY (sub_address_type_id) REFERENCES sub_address_type(id);

ALTER TABLE ONLY sub_post_canton
    ADD FOREIGN KEY (post_canton_id) REFERENCES post_canton(id);

ALTER TABLE ONLY sub_post_canton
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY sub_post_canton
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY sub_street_name
    ADD FOREIGN KEY (language_id) REFERENCES language(id);

ALTER TABLE ONLY sub_street_name
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY sub_street_name
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY sub_street_name
    ADD FOREIGN KEY (sub_street_id) REFERENCES sub_street(id);

ALTER TABLE ONLY sub_street
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY sub_street
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY sub_street_street_name
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY sub_street_street_name
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY sub_street_street_name
    ADD FOREIGN KEY (street_name_id) REFERENCES street_name(id);

ALTER TABLE ONLY sub_street_street_name
    ADD FOREIGN KEY (sub_street_id) REFERENCES sub_street(id);

-- The postcode in sub_street is actually a sub post canton.  The value like it's here
-- is in no other table, but it's a combination of postcode from post_canton +
-- sub_canton_number from sub_post_canton
--ALTER TABLE ONLY sub_street
--    ADD FOREIGN KEY (postcode) REFERENCES post_canton(postcode);

ALTER TABLE ONLY terrain_object_house_number
    ADD FOREIGN KEY (house_number_id) REFERENCES house_number(id);

ALTER TABLE ONLY terrain_object_house_number
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY terrain_object_house_number
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY terrain_object_house_number
    ADD FOREIGN KEY (terrain_object_id) REFERENCES terrain_object(id);

ALTER TABLE ONLY terrain_object
    ADD FOREIGN KEY (cadastre_municipality_code) REFERENCES cadastre_municipality(cadastre_municipality_code);

-- TODO: This constraint should work, but the data we get contains some invalid
-- references
--ALTER TABLE ONLY terrain_object
--    ADD FOREIGN KEY (object_type_id) REFERENCES object_type(id);

ALTER TABLE ONLY terrain_object
    ADD FOREIGN KEY (start_operation_id) REFERENCES operation(id);

ALTER TABLE ONLY terrain_object
    ADD FOREIGN KEY (start_organisation_id) REFERENCES organisation(id);

ALTER TABLE ONLY address_position
    ADD FOREIGN KEY (street_name_id) REFERENCES street_name(id);

ALTER TABLE ONLY address_position
    ADD FOREIGN KEY (nis_code) REFERENCES municipality(nis_code);

ALTER TABLE ONLY address_position
    ADD FOREIGN KEY (postcode) REFERENCES post_canton(postcode);


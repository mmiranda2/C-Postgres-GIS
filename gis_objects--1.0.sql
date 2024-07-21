create type mypoint;

CREATE FUNCTION mypoint_in(cstring) RETURNS mypoint
    AS 'MODULE_PATHNAME', 'mypoint_in'
    LANGUAGE c IMMUTABLE;

CREATE FUNCTION mypoint_out(mypoint) RETURNS cstring
    AS 'MODULE_PATHNAME', 'mypoint_out'
    LANGUAGE c IMMUTABLE;

CREATE TYPE mypoint (
    internallength = 16,
    input = mypoint_in,
    output = mypoint_out,
    alignment = double
);

CREATE FUNCTION my_point_in_polygon(mypoint, mypoint[]) RETURNS boolean
    AS 'MODULE_PATHNAME', 'my_point_in_polygon'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION my_log() RETURNS varchar
    AS 'MODULE_PATHNAME', 'my_log'
    LANGUAGE C IMMUTABLE STRICT;

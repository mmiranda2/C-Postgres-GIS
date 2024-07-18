CREATE TYPE mypoint AS (
    lat double precision,
    lon double precision
);

CREATE TYPE mypolygon AS (
    points mypoint[]
);

CREATE FUNCTION make_mypolygon(points mypoint[]) RETURNS mypolygon
    AS $$
BEGIN
    RETURN ROW(points)::mypolygon;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION my_point_in_polygon(mypoint, mypolygon) RETURNS boolean
    AS 'MODULE_PATHNAME', 'my_point_in_polygon'
    LANGUAGE C IMMUTABLE STRICT;

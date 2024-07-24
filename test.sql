-- DROP EXTENSION gis_objects cascade;
CREATE EXTENSION gis_objects;

-- true
SELECT my_point_in_polygon(
    '(40.72, -74.02)'::mypoint, 
    ARRAY[
        '(40.71, -74.02)'::mypoint, 
        '(40.74, -74.00)'::mypoint, 
        '(40.74, -74.04)'::mypoint
    ]
);

-- false
SELECT my_point_in_polygon(
    '(40.72, -74.02)'::mypoint, 
    ARRAY[
        '(40.80, -74.02)'::mypoint, 
        '(40.84, -74.00)'::mypoint, 
        '(40.84, -74.04)'::mypoint
    ]
);

SELECT my_point_in_polygon(
    '(77.7, -77.7)'::mypoint, 
    make_mypolygon(ARRAY[
        '(40.71, -74.01)'::mypoint, 
        '(40.74, -74.03)'::mypoint, 
        '(40.75, -74.04)'::mypoint
    ])
);

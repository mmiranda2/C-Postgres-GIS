SELECT my_point_in_polygon(
    ROW(40.72, -74.02)::mypoint, 
    make_mypolygon(ARRAY[
        ROW(40.71, -74.01)::mypoint, 
        ROW(40.74, -74.03)::mypoint, 
        ROW(40.75, -74.04)::mypoint
    ])
);

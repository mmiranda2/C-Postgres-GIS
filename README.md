`$ make`

`$ sudo make install`

```
$ psql postgres
postgres=# CREATE EXTENSION gis_objects;
postgres=# SELECT my_point_in_polygon(
    ROW(40.7128, -74.0059)::mypoint, 
    make_mypolygon(ARRAY[
        ROW(40.7127, -74.0060)::mypoint, 
        ROW(40.7129, -74.0060)::mypoint, 
        ROW(40.7128, -74.0058)::mypoint
    ])
);
```

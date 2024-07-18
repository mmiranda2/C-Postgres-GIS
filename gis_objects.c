#include "postgres.h"
#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef struct {
    double lat;
    double lon;
} MyPoint;

typedef struct {
    int32 length;
    MyPoint points[FLEXIBLE_ARRAY_MEMBER];
} MyPolygon;

PG_FUNCTION_INFO_V1(my_point_in_polygon);

static bool my_point_in_polygon_internal(MyPoint *p, MyPolygon *poly) {
    int i, j, nvert = poly->length;
    bool c = false;

    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((poly->points[i].lon >= p->lon) != (poly->points[j].lon >= p->lon)) &&
            (p->lat <= (poly->points[j].lat - poly->points[i].lat) * (p->lon - poly->points[i].lon) / (poly->points[j].lon - poly->points[i].lon) + poly->points[i].lat)) {
            c = !c;
        }
    }

    return c;
}

Datum my_point_in_polygon(PG_FUNCTION_ARGS) {
    MyPoint *p = (MyPoint *) PG_GETARG_POINTER(0);
    MyPolygon *poly = (MyPolygon *) PG_GETARG_POINTER(1);

    bool result = my_point_in_polygon_internal(p, poly);

    PG_RETURN_BOOL(result);
}

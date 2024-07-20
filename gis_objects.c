#include "postgres.h"
#include "fmgr.h"
#include "utils/elog.h"
#include "utils/builtins.h"
#include <stdio.h>

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

static void log_my_point(MyPoint *p) {
    elog(INFO, "Point(%.2f, %.2f)", p->lat, p->lon);
}

static void log_my_polygon(MyPolygon *poly) {
    char buffer[1024];
    int offset = 0;
    offset += snprintf(buffer, sizeof(buffer), "Polygon[");
    for (int i = 0; i < poly->length; i++) {
        if (i > 0) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, ", ");
        }
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "Point(%.2f, %.2f)", poly->points[i].lat, poly->points[i].lon);
    }
    snprintf(buffer + offset, sizeof(buffer) - offset, "]");
    elog(INFO, "%s", buffer);
}

PG_FUNCTION_INFO_V1(mypoint_in);

Datum
mypoint_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    double lat, lon;
    MyPoint *result;

    if (sscanf(str, " ( %lf , %lf )", &lat, &lon) != 2)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("Invalid input syntax for type %s: \"%s\"",
                        "mypoint", str)));

    result = (MyPoint *) palloc(sizeof(MyPoint));
    result->lat = lat;
    result->lon = lon;
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(mypoint_out);

Datum
mypoint_out(PG_FUNCTION_ARGS)
{
    MyPoint *mypt = (MyPoint *) PG_GETARG_POINTER(0);
    char *result;

    result = psprintf("(%g,%g)", mypt->lat, mypt->lon);
    PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(my_point_in_polygon);

Datum
my_point_in_polygon(PG_FUNCTION_ARGS) {
    elog(INFO, "In my_point_in_polygon ...");

    MyPoint *p = (MyPoint *) PG_GETARG_POINTER(0);
    // MyPolygon *poly = (MyPolygon *) PG_GETARG_POINTER(1);

    log_my_point(p);
    // log_my_polygon(poly);

    // bool result = my_point_in_polygon_internal(p, poly);
    bool result = false;

    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(my_log);

Datum
my_log(PG_FUNCTION_ARGS) {
    elog(INFO, "In my_log ...");
    elog(WARNING, "Warning in my_log ...");
    text *result = cstring_to_text("some returned string");
    PG_RETURN_TEXT_P(result);
}

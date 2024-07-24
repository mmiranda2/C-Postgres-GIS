#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/elog.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef struct {
    double lat;
    double lon;
} MyPoint;

typedef struct {
    int length;
    MyPoint points[];
} MyPolygon;

// static void
// log_my_point_p(MyPoint *p)
// {
//     elog(INFO, "MyPoint(%.2f, %.2f)", p->lat, p->lon);
// }

static void
log_my_point(MyPoint p)
{
    elog(INFO, "MyPoint(%.2f, %.2f)", p.lat, p.lon);
}

static MyPolygon*
getarg_mypolygon(ArrayType *pg_array)
{
    int16 elmlen;
    bool elmbyval;
    char elmalign;
    int nitems;
    Datum *elem_values;
    bool *elem_nulls;

    get_typlenbyvalalign(ARR_ELEMTYPE(pg_array), &elmlen, &elmbyval, &elmalign);

    deconstruct_array(pg_array,
                      ARR_ELEMTYPE(pg_array),
                      elmlen, elmbyval, elmalign,
                      &elem_values, &elem_nulls, &nitems);

    MyPolygon *poly = (MyPolygon *) malloc(sizeof(MyPolygon));
    for (int i = 0; i < nitems; i++) 
    {
        if(elem_nulls[i])
        {
            return NULL;
        }
        poly->points[i] = *(MyPoint *) DatumGetPointer(elem_values[i]);
    }
    poly->length = nitems;
    return poly;
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
my_point_in_polygon(PG_FUNCTION_ARGS) 
{
    elog(INFO, "In my_point_in_polygon (07232024)...");

    MyPoint p = *(MyPoint *) PG_GETARG_POINTER(0);
    log_my_point(p);

    elog(INFO, "Parsing array ...");
    MyPolygon *poly = getarg_mypolygon(PG_GETARG_ARRAYTYPE_P(1));

    if(poly == NULL)
    {
        elog(INFO, "NULL element in array, exiting...");
        PG_RETURN_BOOL(false); 
    }

    bool result = false;
    int i, j, length = poly->length;
    MyPoint this_pt, prev_pt;
    for (i = 0, j = length - 1; i < length; j = i++)
    {
        prev_pt = poly->points[j];
        this_pt = poly->points[i];
        elog(INFO, "p (root point): MyPoint(%.2f, %.2f)", p.lat, p.lon);
        elog(INFO, "prev_pt: MyPoint(%.2f, %.2f)", prev_pt.lat, prev_pt.lon);
        elog(INFO, "this_pt: MyPoint(%.2f, %.2f)", this_pt.lat, this_pt.lon);

        if (((this_pt.lon >= p.lon) != (prev_pt.lon >= p.lon)) &&
            (p.lat <= (prev_pt.lat - this_pt.lat) * (p.lon - this_pt.lon) / (prev_pt.lon - this_pt.lon) + this_pt.lat)) 
        {
            elog(INFO, "Flipping result... from %d to %d\n", result, !result);
            result = !result;
        }
        else
        {
            elog(INFO, "Doing nothing... (result value is currently %d)\n", result);
        }
    }

    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(my_log);
Datum
my_log(PG_FUNCTION_ARGS) 
{
    elog(INFO, "In my_log ...");
    elog(WARNING, "Warning in my_log ...");
    text *result = cstring_to_text("some returned string");
    PG_RETURN_TEXT_P(result);
}

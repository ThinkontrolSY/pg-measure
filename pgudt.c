#include "postgres.h"

PG_MODULE_MAGIC;

typedef struct Measure
{
    double amount;
    char unit[];
} Measure;

PG_FUNCTION_INFO_V1(measure_in);

Datum measure_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    double amount;
    char unit[];
    Measure *result;

    if (sscanf(str, "%lf#%ls", &amount, unit) != 2)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for Measure: \"%s\"",
                        str)));

    result = (Measure *)palloc(sizeof(Measure));
    result->amount = amount;
    result->unit = unit;
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(measure_out);

Datum measure_out(PG_FUNCTION_ARGS)
{
    Measure *measure = (Measure *)PG_GETARG_POINTER(0);
    char *result;

    result = psprintf("%g#%g", measure->amount, measure->unit);
    PG_RETURN_CSTRING(result);
}
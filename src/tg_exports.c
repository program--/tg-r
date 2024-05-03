#include <R.h>
#include <Rinternals.h>
#include <R_ext/Visibility.h>
#include "R_ext/Rdynload.h"

#include "tg_interface.h"
#include "tg_altrep.h"

#define R_METHOD_DEF(name, argc) \
    { (#name), (DL_FUNC)(&name), (argc) }

#define R_METHOD_DEF_END \
    {NULL, NULL, 0}

static const R_CallMethodDef CallEntries[] = {
    R_METHOD_DEF(tg_R_read_wkb, 1),
    R_METHOD_DEF(tg_R_read_wkt, 1),
    R_METHOD_DEF(tg_R_read_geojson, 1),
    R_METHOD_DEF(tg_R_as_wkt, 1),
    R_METHOD_DEF(tg_R_typeof, 1),
    R_METHOD_DEF(tg_R_collection_to_list, 1),
    R_METHOD_DEF(tg_R_make_tgc, 1),
    R_METHOD_DEF_END
};

attribute_visible void R_init_tg(DllInfo* dll)
{
    InitTGCollectionClass(dll);

    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

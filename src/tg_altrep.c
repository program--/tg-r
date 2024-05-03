#define TG_ALTREP_IMPL
#include "tg_altrep.h"
#include "tg_interface.h"

static R_altrep_class_t tgc_class;

Rboolean tgc_Inspect(SEXP x, int pre, int deep, int pvec, void (*inspect_subtree)(SEXP, int, int, int))
{
    struct tg_geom* geom = tg_from_sexp(R_altrep_data1(x));

    Rprintf(
        "tg_collection [ptr: %p, len: %d]\n",
        geom,
        tg_geom_num_geometries(geom)
    );

    return TRUE;
}

R_xlen_t tgc_Length(SEXP x)
{
    return tg_geom_num_geometries(tg_from_sexp(R_altrep_data1(x)));
}

SEXP tgc_Duplicate(SEXP x, Rboolean deep)
{
    struct tg_geom* geom = tg_from_sexp(R_altrep_data1(x));
    struct tg_geom* new_geom = deep ? tg_geom_copy(geom) : tg_geom_clone(geom);
    return tg_to_sexp(new_geom);
}

void* tgc_Dataptr(SEXP x, Rboolean writeable)
{
    return tg_ext_geom_geometry_data(tg_from_sexp(R_altrep_data1(x)));
}

const void* tgc_Dataptr_or_null(SEXP x)
{
    return tg_ext_geom_geometry_data(tg_from_sexp(R_altrep_data1(x)));
}

SEXP tgc_Elt(SEXP x, R_xlen_t i)
{
    struct tg_geom* geom = tg_ext_geom_geometry_at_unsafe(tg_from_sexp(R_altrep_data1(x)), i);
    return tg_to_sexp(tg_geom_clone(geom));
}


void tgc_Set_elt(SEXP x, R_xlen_t i, SEXP v)
{
    struct tg_geom* collection = tg_from_sexp(R_altrep_data1(x));
    struct tg_geom* geom = tg_ext_geom_geometry_at_unsafe(collection, i);

    if (!Rf_inherits(v, "tg") || Rf_isNull(v)) {
        Rf_error("Class of type `%s` not assignable to `tg`", Rf_S3Class(v));
        return;
    }
    
    struct tg_geom* new_geom = (struct tg_geom*)R_ExternalPtrAddr(v);
    tg_ext_geom_geometry_data(collection)[i] = new_geom;
    tg_geom_free(geom);
}

void InitTGCollectionClass(DllInfo* dll)
{
    R_altrep_class_t cls = R_make_altlist_class("tgc", "tg", dll);
    tgc_class = cls;

    R_set_altrep_Inspect_method(cls, tgc_Inspect);
    R_set_altrep_Length_method(cls, tgc_Length);
    R_set_altrep_Duplicate_method(cls, tgc_Duplicate);
    R_set_altvec_Dataptr_method(cls, tgc_Dataptr);
    R_set_altvec_Dataptr_or_null_method(cls, tgc_Dataptr_or_null);
    R_set_altlist_Elt_method(cls, tgc_Elt);
    R_set_altlist_Set_elt_method(cls, tgc_Set_elt);
}

SEXP tg_R_make_tgc(SEXP collection)
{
    if (collection == R_NilValue) {
        return R_NilValue;
    }

    struct tg_geom* geom = tg_from_sexp(collection);
    if (geom == NULL) {
        return R_NilValue;
    }

    if (tg_geom_typeof(geom) != TG_GEOMETRYCOLLECTION) {
        Rf_error("Geometry must be a GeometryCollection to coerce to `tgc`.");
        return R_NilValue;
    }

    return R_new_altrep(tgc_class, tg_to_sexp(tg_geom_clone(geom)), R_NilValue);
}

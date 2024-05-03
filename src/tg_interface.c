#include "tg_interface.h"

void tg_free_sexp(SEXP x)
{
    tg_geom_free(tg_from_sexp(x));
}

SEXP tg_to_sexp(const struct tg_geom* geom)
{
    SEXP xp = PROTECT(R_MakeExternalPtr((void*)geom, R_NilValue, R_NilValue));
    R_RegisterCFinalizer(xp, tg_free_sexp);

    SEXP cls = PROTECT(Rf_allocVector(STRSXP, 2));
    SET_STRING_ELT(cls, 1, Rf_mkChar("tg"));
    switch (tg_geom_typeof(geom)) {
        case TG_POINT:
            SET_STRING_ELT(cls, 0, Rf_mkChar("tg_point"));
            break;
        case TG_LINESTRING:
            SET_STRING_ELT(cls, 0, Rf_mkChar("tg_linestring"));
            break;
        case TG_POLYGON:
            SET_STRING_ELT(cls, 0, Rf_mkChar("tg_polygon"));
            break;
        case TG_MULTIPOINT:
            SET_STRING_ELT(cls, 0, Rf_mkChar("tg_multipoint"));
            break;
        case TG_MULTILINESTRING:
            SET_STRING_ELT(cls, 0, Rf_mkChar("tg_multilinestring"));
            break;
        case TG_MULTIPOLYGON:
            SET_STRING_ELT(cls, 0, Rf_mkChar("tg_multipolygon"));
            break;
        case TG_GEOMETRYCOLLECTION:
            SET_STRING_ELT(cls, 0, Rf_mkChar("tg_geometrycollection"));
            break;
    }

    UNPROTECT(1); /* cls */
    Rf_setAttrib(xp, Rf_mkString("class"), cls);
    UNPROTECT(1); /* xp */
    return xp;
}

struct tg_geom* tg_from_sexp(SEXP x) {
    return (struct tg_geom*)R_ExternalPtrAddr(x);
}

SEXP tg_R_read_wkb(SEXP raw) {
    R_xlen_t nbytes = Rf_xlength(raw);
    Rbyte* bytes = RAW(raw);
    struct tg_geom* geom = tg_parse_wkb(bytes, nbytes);
    if (tg_geom_error(geom)) {
        Rf_error(tg_geom_error(geom));
        return R_NilValue;
    }

    return tg_to_sexp(geom);
}

// TODO: prevent potential overflow (use tg_parse_*n function)
SEXP tg_R_read_wkt(SEXP str) {
    const char* wkt = CHAR(STRING_ELT(str, 0));
    struct tg_geom* geom = tg_parse_wkt(wkt);
    if (tg_geom_error(geom)) {
        Rf_error(tg_geom_error(geom));
        return R_NilValue;
    }

    return tg_to_sexp(geom);
}

// TODO: prevent potential overflow (use tg_parse_*n function)
SEXP tg_R_read_geojson(SEXP str) {
    const char* geojson = CHAR(STRING_ELT(str, 0));

    struct tg_geom* geom = tg_parse_geojson(geojson);
    if (tg_geom_error(geom)) {
        Rf_error(tg_geom_error(geom));
        return R_NilValue;
    }

    return tg_to_sexp(geom);
}

SEXP tg_R_as_wkt(SEXP xp) {
    struct tg_geom* geom = tg_from_sexp(xp);

    char dst[BUFSIZ];
    size_t len = tg_geom_wkt(geom, dst, BUFSIZ);
    return Rf_ScalarString(Rf_mkCharLen(dst, len));
}

SEXP tg_R_typeof(SEXP xp) {
    struct tg_geom* geom = tg_from_sexp(xp);
    const char* type = tg_geom_type_string(tg_geom_typeof(geom));
    return Rf_mkString(type);
}

SEXP tg_R_collection_to_list(SEXP xp) {
    struct tg_geom* geom = tg_from_sexp(xp);
    int count = tg_geom_num_geometries(geom);

    SEXP result = PROTECT(Rf_allocVector(VECSXP, count));
    for (int i = 0; i < count; i++) {
        struct tg_geom* part = tg_geom_clone(tg_geom_geometry_at(geom, i));
        SET_VECTOR_ELT(result, i, tg_to_sexp(part));
    }
    UNPROTECT(1);
    return result;
}

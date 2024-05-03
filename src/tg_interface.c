#include "tg_interface.h"
#include <R_ext/Altrep.h>

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

struct tg_geom* tg_from_sexp(SEXP x)
{
    return (struct tg_geom*)R_ExternalPtrAddr(x);
}

SEXP tg_R_read_wkb(SEXP raw)
{
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
SEXP tg_R_read_wkt(SEXP str)
{
    const char* wkt = CHAR(STRING_ELT(str, 0));
    struct tg_geom* geom = tg_parse_wkt(wkt);
    if (tg_geom_error(geom)) {
        Rf_error(tg_geom_error(geom));
        return R_NilValue;
    }

    return tg_to_sexp(geom);
}

// TODO: prevent potential overflow (use tg_parse_*n function)
SEXP tg_R_read_geojson(SEXP str)
{
    const char* geojson = CHAR(STRING_ELT(str, 0));

    struct tg_geom* geom = tg_parse_geojson(geojson);
    if (tg_geom_error(geom)) {
        Rf_error(tg_geom_error(geom));
        return R_NilValue;
    }

    return tg_to_sexp(geom);
}

SEXP tg_R_as_wkt(SEXP xp)
{
    struct tg_geom* geom = tg_from_sexp(xp);

    char dst[BUFSIZ];
    size_t len = tg_geom_wkt(geom, dst, BUFSIZ);
    return Rf_ScalarString(Rf_mkCharLen(dst, len));
}

SEXP tg_R_typeof(SEXP xp)
{
    struct tg_geom* geom = tg_from_sexp(xp);
    const char* type = tg_geom_type_string(tg_geom_typeof(geom));
    return Rf_mkString(type);
}

SEXP tg_R_bbox(SEXP xp)
{
    struct tg_geom* geom = tg_from_sexp(xp);
    struct tg_rect bbox = tg_geom_rect(geom);
    SEXP result = PROTECT(Rf_allocVector(REALSXP, 4));
    SET_REAL_ELT(result, 0, bbox.min.x);
    SET_REAL_ELT(result, 1, bbox.min.y);
    SET_REAL_ELT(result, 2, bbox.max.x);
    SET_REAL_ELT(result, 3, bbox.max.y);
    UNPROTECT(1);
    return result;
}

SEXP tg_R_Elt_point(struct tg_geom* g, R_xlen_t i)
{
    if (i > 1 || i < 0) {
        return Rf_ScalarReal(R_NaReal);
    }

    struct tg_point pt = tg_geom_point(g);
    return Rf_ScalarReal(i == 0 ? pt.x : pt.y);
}

SEXP tg_R_Elt_line(struct tg_geom* g, R_xlen_t i)
{
    const struct tg_line* line = tg_geom_line(g);
    if (line == NULL || i < 0) {
        return R_NilValue;
    }

    struct tg_point pt = tg_line_point_at(line, i);
    return tg_to_sexp(tg_geom_new_point(pt));
}

SEXP tg_R_Elt_poly(struct tg_geom* g, R_xlen_t i)
{
    const struct tg_poly* poly = tg_geom_poly(g);
    if (poly == NULL || i < 0) {
        return R_NilValue;
    }

    const struct tg_ring* ring = NULL;

    if (i == 0) {
        ring = tg_poly_exterior(poly);
    } else {
        ring = tg_poly_hole_at(poly, i - 1);
    }

    return tg_to_sexp(tg_geom_new_linestring((const struct tg_line*)ring));
}

SEXP tg_R_Elt_multi(struct tg_geom* g, R_xlen_t i, enum tg_geom_type type)
{
    /* If there is an error, any branch will jump to
       label 'tg_R_Elt_multi_error_' below the return statement. */

    if (i < 0) {
        goto tg_R_Elt_multi_error_;
    }

    struct tg_geom* child = NULL;
    if (type == TG_MULTIPOINT) {
        if (i >= tg_geom_num_points(g)) {
            goto tg_R_Elt_multi_error_;
        }

        struct tg_point pt = tg_geom_point_at(g, i);
        child = tg_geom_new_point(pt);
    } else if (type == TG_MULTILINESTRING) {
        if (i >= tg_geom_num_lines(g)) {
            goto tg_R_Elt_multi_error_;
        }

        const struct tg_line* line = tg_geom_line_at(g, i);
        child = tg_geom_new_linestring(line);
    } else if (type == TG_MULTIPOLYGON) {
        if (i >= tg_geom_num_polys(g)) {
            goto tg_R_Elt_multi_error_;
        }
    
        const struct tg_poly* poly = tg_geom_poly_at(g, i);
        child = tg_geom_new_polygon(poly);
    } else /* type == TG_GEOMETRYCOLLECTION */ {
        if (i >= tg_geom_num_geometries(g)) {
            goto tg_R_Elt_multi_error_;
        }

        child = tg_geom_clone(tg_geom_geometry_at(g, i));
    }

    return tg_to_sexp(child);

tg_R_Elt_multi_error_:
    Rf_error("subscript %d out of bounds", i + 1);
    return R_NilValue;
}

SEXP tg_R_Elt(SEXP xp, SEXP index)
{
    R_xlen_t i = INTEGER(index)[0];
    struct tg_geom* geom = tg_from_sexp(xp);
    if (geom == NULL) {
        return R_NilValue;
    }

    switch(tg_geom_typeof(geom)) {
        case TG_POINT:
            return tg_R_Elt_point(geom, i);
        case TG_LINESTRING:
            return tg_R_Elt_line(geom, i);
        case TG_POLYGON:
            return tg_R_Elt_poly(geom, i);
        case TG_MULTIPOINT:
        case TG_MULTILINESTRING:
        case TG_MULTIPOLYGON:
        case TG_GEOMETRYCOLLECTION:
            return tg_R_Elt_multi(geom, i, tg_geom_typeof(geom));

        default:
            Rf_error("unknown geometry type: %s", tg_geom_type_string(tg_geom_typeof(geom)));
    };

    return R_NilValue;
}

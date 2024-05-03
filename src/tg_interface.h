#pragma once
#define STRICT_R_HEADERS
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include "tg.h"

SEXP            tg_to_sexp(const struct tg_geom* geom);
struct tg_geom* tg_from_sexp(SEXP x);
void            tg_free_sexp(SEXP x);
SEXP            tg_R_read_wkb(SEXP raw);
SEXP            tg_R_read_wkt(SEXP str);
SEXP            tg_R_read_geojson(SEXP str);
SEXP            tg_R_as_wkt(SEXP xp);
SEXP            tg_R_typeof(SEXP xp);
SEXP            tg_R_bbox(SEXP xp);

SEXP tg_R_Elt(SEXP xp, SEXP index);

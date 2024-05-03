#pragma once
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Altrep.h>
#include "tg_ext.h"

#ifndef TG_ALTREP_IMPL
/**
 * The tgc_class is an ALTREP(VECSXP) (list)
 * representing a geometry collection. The primary purpose
 * for this is to support list columns in R that directly use
 * `tg_geom_search()`, rather than coercing a VECSXP of external
 * pointers into a tg_geom Geometry Collection.
 *
 * However, note that a TG Collection has the same representation
 * as a `tg_geom` of type TG_GEOMETRYCOLLECTION, but semantically
 * they differ as the former represents multiple features (multiple rows),
 * whereas the latter represents a single feature (single row).
 *
 *     data1: SEXP External Pointer to the underlying tg_geom* object.
 *     data2: currently holds R_NilValue.
 */
extern R_altrep_class_t tgc_class;
#endif

void InitTGCollectionClass(DllInfo* dll);

SEXP tg_R_make_tgc(SEXP collection);

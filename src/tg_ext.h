#pragma once
/**
 * @file tg_ext.h
 * @brief Provides extensions on top of the C library `tg` for use in the R package `tg`.
 *
 * Functions defined in this header must prefix with 'tg_ext_' instead of 'tg_' to separate
 * extension function from core functions found in tg.
 */

#include "tg.h"

struct tg_geom** tg_ext_geom_geometry_data(const struct tg_geom* geom);
struct tg_geom* tg_ext_geom_geometry_at_unsafe(const struct tg_geom* geom, int index);

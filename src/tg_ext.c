#include "tg_ext.h"

/* base implementation */
#include "tg.c"
#include "tg.h"

/* extended implementation */
struct tg_geom** tg_ext_geom_geometry_data(const struct tg_geom* geom)
{
    if (geom && geom->head.base == BASE_GEOM &&
        geom->head.type == TG_GEOMETRYCOLLECTION &&
        geom->multi)
    {
        return geom->multi->geoms;
    }

    return NULL;
}

struct tg_geom* tg_ext_geom_geometry_at_unsafe(const struct tg_geom* geom, int index)
{
    if (geom && geom->head.base == BASE_GEOM && 
        geom->head.type == TG_GEOMETRYCOLLECTION && 
        geom->multi && index >= 0 && index <= geom->multi->ngeoms)
    {
        return geom->multi->geoms[index];
    }
    return NULL;
}

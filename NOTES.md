# `tg` and `tgc` S3 classes

The `tg` class is an external pointer to a `struct tg_geom` object.

The `tgc` class is an ALTLIST represented by an external pointer to a `struct tg_geom` of type `TG_GEOMETRYCOLLECTION` (may be narrowed to type of `TG_MULTI*` in the future, but might not matter for performance).

One of the primary benefits of this is using the embedded spatial index on the geometry collection, i.e. with `tg_geom_search`.

```
tg
  -> EXPTRSXP
    -> struct tg_geom*

tgc
  -> ALTLIST
    -> struct tg_geom*
      -> [struct tg_geom*, struct tg_geom*, ...]
```

The difference between the two is usage semantics.

- A `tg` object:
  - represents the geometry associated with a feature (i.e. `sfg` in sf).
  - is a value in a column in a `data.frame`.
- A `tgc` object:
  - represents multiple features (i.e. `sfc` in sf).
  - is a column in a `data.frame`.

# Extending the core C `tg` library

The C library of `tg` hides some implementation details that are needed for the implementation of the `tgc` class.
As a result, this package contains `tg_ext.{c,h}` files which embed the original `tg` code, and adds extension functions
as necessary. C code can use either `tg_ext.h` or `tg.h`, but `tg_ext.h` is required if extension functions are needed.
The Makevars prevents compiling `tg.c`, and instead `tg_ext.c` directly includes `tg.c` via C preprocessor directives.

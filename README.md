
<!-- README.md is generated from README.Rmd. Please edit that file -->

# tg

<!-- badges: start -->
<!-- badges: end -->

This package provides an interface to the [tg C
library](https://github.com/tidwall/tg) similar to
[sf](https://github.com/r-spatial/sf) and
[geos](https://github.com/paleolimbot/geos).

## Installation

You can install the development version of tg like so:

``` r
pak::pkg_install("program--/tg")
```

## Example Usage

Reading geometry from WKT:

``` r
tg::as_tg("POINT(1 1)")
#> POINT(1 1)
```

Reading geometry from a GeoJSON string:

``` r
gjson <- '
{ "type": "FeatureCollection",
  "features": [
    { "type": "Feature",
      "geometry": {"type": "Point", "coordinates": [102.0, 0.5]},
      "properties": {"prop0": "value0"}
      },
    { "type": "Feature",
      "geometry": {
        "type": "LineString",
        "coordinates": [
          [102.0, 0.0], [103.0, 1.0], [104.0, 0.0], [105.0, 1.0]
          ]
        },
      "properties": {
        "prop0": "value0",
        "prop1": 0.0
        }
      },
    { "type": "Feature",
       "geometry": {
         "type": "Polygon",
         "coordinates": [
           [ [100.0, 0.0], [101.0, 0.0], [101.0, 1.0],
             [100.0, 1.0], [100.0, 0.0] ]
           ]

       },
       "properties": {
         "prop0": "value0",
         "prop1": {"this": "that"}
         }
       }
    ]
  }
'

tg::as_tg(gjson)
#> GEOMETRYCOLLECTION(POINT(102 0.5),LINESTRING(102 0,103 1,104 0,105 1),POLYGON((100 0,101 0,101 1,100 1,100 0)))
```

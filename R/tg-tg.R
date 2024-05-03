#' Coerce to a TG geometry
#'
#' @param x An object interpretable of type character or raw.
#' @param crs An ESPG ID
#' @param ... unused
#' @return A TG geometry vector
#'
#' @seealso [tg_read_geojson()] for GeoJSON,
#'   [tg_read_wkt()] for WKT, and [tg_read_wkb()] for WKB
#'
#' @examples
#' as_tg_geometry("POINT(1.0 2.0)")
#'
#' gj <- '{"type":"Feature","geometry":{"type":"Point","coordinates":[125.6,10.1]},"properties":{"name":"Dinagat Islands"}}'
#' as_tg_geometry(gj)
#'
#' @rdname as_tg
#' @export
as_tg <- function(x, ...) {
    UseMethod("as_tg")
}

#' @rdname as_tg
#' @export
as_tg.default <- function(x, ...) {
    NULL
}

#' @rdname as_tg
#' @export
as_tg.tg <- function(x, ...) {
    x
}

#' @export
print.tg <- function(x, ...) {
    message(tg_as_wkt(x))
    invisible(x)
}

#' @export
is.tg <- function(x) {
    inherits(x, "tg")
}

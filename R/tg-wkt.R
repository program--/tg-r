#' Read WKT into TG geometry
#' @param x a WKT character vector
#' @rdname tg_read
#' @export
tg_read_wkt <- function(x, ..., crs = NULL) {
    .Call(`tg_R_read_wkt`, x)
}

#' Read a GeoJSON string into TG geometry
#' @param x GeoJSON string vector
#' @rdname tg_read
#' @export
tg_read_geojson <- function(x, ..., crs = NULL) {
    .Call(`tg_R_read_geojson`, x)
}

tg_as_wkt <- function(x) {
    .Call(`tg_R_as_wkt`, x)
}

#' @rdname as_tg
#' @export
as_tg.character <- function(x, ..., crs = NULL) {
    tryCatch(
        tg_read_wkt(x, ..., crs = crs),
        error = function(cond) {
            tryCatch(
                tg_read_geojson(x, ..., crs = crs),
                error = function(cond2) {
                    stop(
                        "Failed to parse \"",
                        x,
                        "\" as WKT or GeoJSON.",
                        call. = FALSE
                    )
                }
            )
        }
    )
}

#' @rdname as_tg
#' @export
as_tg.blob <- function(x, ..., crs = NULL) {
    NextMethod("as_tg", as.raw(x), ..., crs = crs)
}

#' @rdname as_tg
#' @export
as_tg.WKB <- function(x, ..., crs = NULL) {
    NextMethod("as_tg", as.raw(x), ..., crs = crs)
}

#' @rdname as_tg
#' @export
as_tg.raw <- function(x, ..., crs = NULL) {
    tg_read_wkb(x, ..., crs = crs)
}

#' Read WKB into TG geometry
#' @param x a WKB raw vector
#' @rdname tg_read
#' @export
tg_read_wkb <- function(x, ..., crs = NULL) {
    .Call(`tg_R_read_wkb`, x)
}

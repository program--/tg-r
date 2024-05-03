as_tgc <- function(x, ...) {
    UseMethod("as_tgc")
}

as_tgc.default <- function(x, ...) {
    NULL
}

as_tgc.tgc <- function(x, ...) {
    x
}

as_tgc.tg_geometrycollection <- function(x, ...) {
    .Call(`tg_R_make_tgc`, ptr)
}

as_tgc.tg <- function(x, ...) {
    stop("Not implemented")
}

#' @export
is.tgc <- function(x) {
    inherits(x, "tgc")
}

#' @method as.list tgc
#' @export
as.list.tgc <- function(x, ...) {
    x
}

#' Coerce to a TG geometry collection
#' @rdname as_tgc
#' @export
as_tgc <- function(x, ...) {
    UseMethod("as_tgc")
}

#' @rdname as_tgc
#' @export
as_tgc.default <- function(x, ...) {
    NULL
}

#' @rdname as_tgc
#' @export
as_tgc.tgc <- function(x, ...) {
    x
}

#' @rdname as_tgc
#' @export
as_tgc.tg_geometrycollection <- function(x, ...) {
    .tgc <- .Call(`tg_R_make_tgc`, x)
    class(.tgc) <- "tgc"
    .tgc
}

#' @rdname as_tgc
#' @export
as_tgc.tg <- function(x, ...) {
    stop("Not implemented")
}

#' @rdname as_tgc
#' @export
as_tgc.data.frame <- function(x, geom_col, ...) {
    NULL
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

#' @method as.data.frame tgc
#' @export
as.data.frame.tgc <- function(x, ..., optional = FALSE) {
    if (!optional) {
        NextMethod()
    } else {
        structure(x, row.names = c(NA, length(x)), class = "data.frame")
    }
}

#' @export
`[.tgc` <- function(x, i) {
    x[[i]]
}

#' @method format tgc
#' @export
format.tgc <- function(x, ..., header_only = TRUE) {
    bb <- tg_bbox(x)
    header <- paste0(
        "<tg collection, bounds: (",
        format(bb[1]), " ",
        format(bb[2]), ", ",
        format(bb[3]), " ",
        format(bb[4]), ")>"
    )

    if (header_only) {
        return(header)
    }

    strs <- paste(
        vapply(x, format, FUN.VALUE = character(1), ..., USE.NAMES = FALSE),
        collapse = " "
    )

    paste(header, strs, sep = "\n", collapse = "\n")
}

#' @method print tgc
#' @export
print.tgc <- function(x, ...) {
    cat(format(x, ..., header_only = TRUE), "\n", sep = "")
    for (g in x) {
        cat(format(g, ...), "\n")
    }
    invisible(x)
}

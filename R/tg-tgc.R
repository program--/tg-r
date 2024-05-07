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
        structure(list(x), row.names = c(NA, length(x)), class = "data.frame")
    }
}

#' @method format tgc
#' @export
format.tgc <- function(x, ...) {
    vapply(x, format, FUN.VALUE = character(1), ..., USE.NAMES = FALSE)
}

#' @method print tgc
#' @export
print.tgc <- function(x, ...) {
    bb <- tg_bbox(x)
    header <- paste0(
        "<tg collection, bounds: (",
        format(bb[1]), " ",
        format(bb[2]), ", ",
        format(bb[3]), " ",
        format(bb[4]), ")>"
    )

    cat(header, sep = "\n")
    for (g in x) {
        cat(format(g, ...), "\n")
    }
    invisible(x)
}

#' pulled from geos::str.geos_geometry
#' @method str tgc
#' @export
str.tgc <- function(object, ..., indent.str = "", width = getOption("width")) {
    if (length(object) == 0) {
        cat(paste0(" ", class(object)[1], "[0]\n"))
        return(invisible(object))
    }

    width     <- width - nchar(indent.str) - 2
    length    <- min(length(object), ceiling(width / 5))
    formatted <- sapply(object[seq_len(length)], format, trim = TRUE)

    title <- paste0(" ", class(object)[1], "[1:", length(object), "]")
    cat(paste0(
        title,
        " ",
        strtrim(paste0(formatted, collapse = ", "), width - nchar(title)),
        "\n"
    ))
    invisible(object)
}

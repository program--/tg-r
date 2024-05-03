#' Get bounding box of a TG geometry
#' @export
tg_bbox <- function(x, ...) {
    x <- as_tg(x)
    bbox <- .Call(`tg_R_bbox`, x)
    names(bbox) <- c("xmin", "ymin", "xmax", "ymax")
    bbox
}

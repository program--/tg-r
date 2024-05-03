#!/usr/bin/env Rscript

.version <- "0.4.0"
.url <- paste0(
    "https://github.com/tidwall/tg/archive/refs/tags/v",
    .version,
    ".tar.gz"
)

archive::archive_extract(
    archive = .url,
    dir     = "src",
    files   = c("tg.c", "tg.h"),
    strip_components = 1L
)

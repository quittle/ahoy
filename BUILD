# Copyright (c) 2016, 2018 Dustin Doloff
# Licensed under Apache License v2.0

load(":internal.bzl", "CC_WARNINGS")

AHOY_HEADERS = glob([ "include/**/*.h" ])

cc_inc_library(
    name = "ahoy",
    deps = [ ":ahoy_internal" ],
    hdrs = AHOY_HEADERS,
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "ahoy_internal",
    srcs = glob([ "src/*", "src/**/*" ]),
    hdrs = AHOY_HEADERS,
    # This can't use copts because when Ahoy is included as a third-party package, ahoy_internal
    # will fail to compile as the headers will no longer be in their expected place.
    includes = [ "include" ],
    copts = CC_WARNINGS,
    visibility = [ "//:__subpackages__" ],
)

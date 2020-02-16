# Copyright (c) 2016, 2018 Dustin Toff
# Licensed under Apache License v2.0

load(":internal.bzl", "CC_WARNINGS")

AHOY_HEADERS = glob(["include/**/*.h"])

cc_library(
    name = "ahoy",
    hdrs = AHOY_HEADERS,
    visibility = ["//visibility:public"],
    deps = [":ahoy_internal"],
)

cc_library(
    name = "ahoy_internal",
    srcs = glob([
        "src/*",
        "src/**/*",
    ]),
    hdrs = AHOY_HEADERS,
    copts = CC_WARNINGS + ["-O3"],
    # This can't use copts because when Ahoy is included as a third-party package, ahoy_internal
    # will fail to compile as the headers will no longer be in their expected place.
    includes = ["include"],
    visibility = ["//:__subpackages__"],
)

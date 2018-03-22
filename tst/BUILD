# Copyright (c) 2018 Dustin Doloff
# Licensed under Apache License v2.0

load("//:internal.bzl", "CC_WARNINGS")

cc_test(
    name = "unit_tests",
    srcs = glob([ "**/*.cc" ]),
    deps = [
        "//:ahoy_internal",
        "@com_google_googletest//:gtest_main",
    ],
    timeout = "short",
    copts = CC_WARNINGS,
)

# Copyright (c) 2018 Dustin Toff
# Licensed under Apache License v2.0

load("//:internal.bzl", "CC_WARNINGS")

cc_test(
    name = "unit_tests",
    timeout = "short",
    srcs = glob(["**/*.cc"]),
    copts = CC_WARNINGS,
    deps = [
        "//:ahoy_internal",
        "@com_google_googletest//:gtest_main",
    ],
)

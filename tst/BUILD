# Copyright (c) 2018 Dustin Doloff
# Licensed under Apache License v2.0

cc_test(
    name = "unit_tests",
    srcs = glob([ "**/*.cc" ], exclude = ["parser_ut.cc"]),
    deps = [
        "//:ahoy_internal",
        "@com_google_googletest//:gtest_main",
    ],
    timeout = "short",
)

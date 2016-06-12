# Copyright (c) 2016 Dustin Doloff
# Licensed under Apache License v2.0

AHOY_HEADERS = [
    "include/ahoy/actualized_parameter.h",
    "include/ahoy/ahoy_all.h",
    "include/ahoy/formal_parameter.h",
    "include/ahoy/named_formal_parameter.h",
    "include/ahoy/newline.h",
    "include/ahoy/parse_result.h",
    "include/ahoy/parser.h",
]

cc_inc_library(
    name = "ahoy",
    deps = [":ahoy_internal"],
    hdrs = AHOY_HEADERS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "ahoy_internal",
    srcs = [
        "src/actualized_parameter.cc",
        "src/formal_parameter.cc",
        "src/named_formal_parameter.cc",
        "src/newline.cc",
        "include/ahoy/parser.h",
    ],
    hdrs = AHOY_HEADERS,
    copts = ["-Iinclude"],
    includes = ["include"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "tst/actualized_parameter_ut.cc",
        "tst/formal_parameter_ut.cc",
        "tst/named_formal_parameter_ut.cc",
        "tst/newline_ut.cc",
        "tst/parse_result_ut.cc",
        "tst/parser_ut.cc",
    ],
    copts = ["-Iinclude"],
    deps = [
        ":ahoy_internal",
        "@gtest//:gtest",
    ],
    timeout = "short",
)

cc_binary(
    name = "sample",
    srcs = [ "tst/sample_main.cc" ],
    deps = [ ":ahoy" ],
)

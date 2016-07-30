# Copyright (c) 2016 Dustin Doloff
# Licensed under Apache License v2.0

AHOY_HEADERS = [
    "include/ahoy/ahoy_all.h",
    "include/ahoy/arg_generator.h",
    "include/ahoy/arg_size.h",
    "include/ahoy/args.h",
    "include/ahoy/formal_parameter.h",
    "include/ahoy/newline.h",
    "include/ahoy/options.h",
    "include/ahoy/parser.h",
    "include/ahoy/static_assert_helper.h",
    "include/ahoy/type.h",
]

cc_library(
    name = "ahoy",
    deps = [":ahoy_internal"],
    hdrs = AHOY_HEADERS,
    includes = ["include"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "ahoy_internal",
    srcs = [
        "src/args.cc",
        "src/arg_generator.cc",
        "src/formal_parameter.cc",
        "src/newline.cc",
        "src/parser.cc",
        "src/type.cc",
    ],
    hdrs = AHOY_HEADERS,
    copts = [ "-Iinclude" ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "tst/arg_generator_ut.cc",
        "tst/args_ut.cc",
        "tst/formal_parameter_ut.cc",
        "tst/newline_ut.cc",
        "tst/parser_manual_ut.cc",
        "tst/parser_ut.cc",
        "tst/static_assert_helper_ut.cc",
        "tst/type_ut.cc",
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
    srcs = [ "examples/sample_main.cc" ],
    deps = [ ":ahoy" ],
)

cc_library(
    name = "ahoy",
    deps = [":ahoy_internal"],
    includes = [
        "include",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "ahoy_internal",
    srcs = [
        "src/arg.cc",
        "src/named_arg.cc",
        "src/newline.cc",
        "src/param.cc",
        "include/ahoy/parser.h",
    ],
    hdrs = [
        "include/ahoy/arg.h",
        "include/ahoy/named_arg.h",
        "include/ahoy/newline.h",
        "include/ahoy/param.h",
        "include/ahoy/parse_result.h",
        "include/ahoy/parser.h",
    ],
    copts = ["-Iinclude"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "tst/arg_ut.cc",
        "tst/named_arg_ut.cc",
        "tst/newline_ut.cc",
        "tst/param_ut.cc",
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
)

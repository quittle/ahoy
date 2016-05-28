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
        "src/param.cc",
        "include/ahoy/parser.h",
    ],
    hdrs = [
        "include/ahoy/arg.h",
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
        "tst/param_ut.cc",
        "tst/parse_result_ut.cc",
        "tst/parser_ut.cc",
    ],
    copts = ["-Iinclude"],
    linkopts = [
        "-lm",
    ],
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

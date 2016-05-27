extra_copts = [
    "-Wall",
    "-Werror",
    "-Wextra",
    "-Wpedantic",
    "-Woverloaded-virtual",
    "-Wsequence-point",
    "-Wunused",
    "-Wno-unused-function",
]

cc_library(
    name = "ahoy",
    srcs = [
        "src/arg.cc",
        "src/param.cc",
        # "src/parser.cc",
    ],
    hdrs = [
        "include/ahoy/arg.h",
        "include/ahoy/param.h",
        "include/ahoy/parser.h",
    ],
    copts = extra_copts,
    includes = [
        "include",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "tst/arg_ut.cc",
        "tst/parser_ut.cc",
        "tst/param_ut.cc",
    ],
    copts = extra_copts,
    linkopts = [
        "-lm",
    ],
    deps = [
        ":ahoy",
        "@gtest//:gtest",
    ],
    timeout = "short",
)

cc_binary(
    name = "sample",
    srcs = [ "tst/sample_main.cc" ],
)

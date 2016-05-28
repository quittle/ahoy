cc_library(
    name = "ahoy",
    srcs = [
        "src/arg.cc",
        "src/param.cc",
    ],
    hdrs = [
        "include/ahoy/arg.h",
        "include/ahoy/param.h",
        "include/ahoy/parser.h",
        "include/ahoy/parse_result.h",
    ],
    includes = [
        "include",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "tst/arg_ut.cc",
        "tst/param_ut.cc",
        "tst/parser_ut.cc",
        "tst/parse_result_ut.cc",
    ],
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

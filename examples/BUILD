# Copyright (c) 2018 Dustin Doloff
# Licensed under Apache License v2.0

load("//:internal.bzl", "CC_WARNINGS")

cc_binary(
    name = "sample",
    srcs = [ "sample_main.cc" ],
    copts = CC_WARNINGS,
    deps = [ "//:ahoy" ],
)

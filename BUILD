load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "quaternion",
    srcs = ["quaternion.cpp"],
    hdrs = ["quaternion.h"],
)

cc_test(
    name = "quaternion_test",
    srcs = ["quaternion_test.cpp"],
    deps = [
        ":quaternion",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

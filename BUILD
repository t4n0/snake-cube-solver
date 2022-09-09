load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "math_constants",
    srcs = ["math_constants.cpp"],
    hdrs = ["math_constants.h"],
)

cc_library(
    name = "quaternion",
    srcs = ["quaternion.cpp"],
    hdrs = ["quaternion.h"],
)

cc_test(
    name = "quaternion_test",
    srcs = ["quaternion_test.cpp"],
    deps = [
        ":math_constants",
        ":quaternion",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "cube",
    srcs = ["main.cpp"],
    deps = [
        ":math_constants",
        ":quaternion",
    ],
)

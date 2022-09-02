load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "vector3d",
    srcs = ["vector3d.cpp"],
    hdrs = ["vector3d.h"],
)

cc_test(
    name = "vector3d_test",
    srcs = ["vector3d_test.cpp"],
    deps = [
        ":vector3d",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
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
        ":quaternion",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "cube",
    srcs = ["main.cpp"],
    deps = [":quaternion"],
)

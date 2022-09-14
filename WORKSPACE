load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.11.0",
)

BUILD_MATPLOT = """
cc_library(
    name = "cimg",
    hdrs = ["source/3rd_party/cimg/CImg.h"],
    strip_include_prefix = "source/3rd_party/cimg",
)

cc_library(
    name = "nodesoup",
    srcs = glob([
        "source/3rd_party/nodesoup/**/*.cpp",
        "source/3rd_party/nodesoup/**/*.hpp",
    ]),
    hdrs = ["source/3rd_party/nodesoup/include/nodesoup.hpp"],
    strip_include_prefix = "source/3rd_party/nodesoup/include/",
)

cc_library(
    name = "matplot",
    srcs = glob(
        include = ["source/matplot/**/*.cpp"],
        exclude = ["**/*opengl*"],
    ),
    hdrs = glob(
        include = ["source/matplot/**/*.h"],
        exclude = ["**/*opengl*"],
    ),
    strip_include_prefix = "source",
    visibility = ["//visibility:public"],
    deps = [
        ":cimg",
        ":nodesoup",
    ],
)"""

new_git_repository(
    name = "matplot",
    branch = "master",
    build_file_content = BUILD_MATPLOT,
    remote = "https://github.com/alandefreitas/matplotplusplus",
)

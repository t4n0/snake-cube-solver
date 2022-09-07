load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.11.0",
)

git_repository(
    name = "googlebenchmark",
    remote = "https://github.com/google/benchmark",
    tag = "v1.5.2",
)

BUILD_SCIPLOT = """
cc_library(
    name = "sciplot",
    hdrs = glob(["sciplot/**/*.hpp"]),
    includes = ["."],
    visibility = ["//visibility:public"],
)"""

new_git_repository(
    name = "sciplot",
    build_file_content = BUILD_SCIPLOT,
    commit = "69fd01ed491c9de271f70982433012aa53a0d271",
    remote = "https://github.com/sciplot/sciplot",
    shallow_since = "1661173888 +0200",
)

workspace(name = "org_cloudabi_cloudlibc")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "org_cloudabi_bazel_toolchains_cloudabi",
    commit = "6cda0c4219e3d71e1fe26faa946f19d837b42dc8",
    remote = "https://github.com/NuxiNL/bazel-toolchains-cloudabi.git",
)

load("@org_cloudabi_bazel_toolchains_cloudabi//:toolchains.bzl", "toolchains_cloudabi_dependencies")

toolchains_cloudabi_dependencies()

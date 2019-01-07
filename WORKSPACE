workspace(name = "org_cloudabi_cloudlibc")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "org_cloudabi_bazel_toolchains_cloudabi",
    commit = "4eb362f2e4d8a5e29ac4b6638ef9d4b9fd312b32",
    remote = "https://github.com/NuxiNL/bazel-toolchains-cloudabi.git",
)

load("@org_cloudabi_bazel_toolchains_cloudabi//:toolchains.bzl", "toolchains_cloudabi_dependencies")

toolchains_cloudabi_dependencies()

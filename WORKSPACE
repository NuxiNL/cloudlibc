workspace(name = "org_cloudabi_cloudlibc")

# TODO(ed): Use a toolchain stored in Git.
local_repository(
    name = "org_cloudabi_bazel_toolchains_cloudabi",
    path = "/home/ed/projects/bazel-toolchains-cloudabi",
)

load("@org_cloudabi_bazel_toolchains_cloudabi//:toolchains.bzl", "toolchains_cloudabi_dependencies")

toolchains_cloudabi_dependencies()

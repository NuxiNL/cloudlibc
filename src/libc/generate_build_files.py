#!/usr/bin/env python3

# generate_build_files -- Generate Bazel build files for libc.
#
# This script was used to generate the initial set of BUILD.bazel files
# in this directory, based on source files present in directories.
#
# TODO(ed): Remove this script once the Bazel migration is complete.

import io
import os
import subprocess

procs = []


def open_and_format(filename):
    """Writes a BUILD file to disk and reformats it using buildifier."""
    proc = subprocess.Popen(
        ["buildifier"], stdin=subprocess.PIPE, stdout=open(filename, "w")
    )
    procs.append(proc)
    return io.TextIOWrapper(proc.stdin, encoding="UTF-8")


def is_package(path):
    """Determines whether a directory needs a BUILD.bazel file."""
    for f in os.listdir(path):
        if f.endswith(".c") or f.endswith(".cc") or f.endswith(".h"):
            return True
    return False


def generate_build_file(package, path):
    """Generates a BUILD.bazel file inside a package."""
    tests = []
    srcs = []
    for root, dirs, files in os.walk(path, topdown=False):
        for fn in files:
            p = os.path.relpath(os.path.join(root, fn), path)
            if fn.endswith("_test.c"):
                tests.append(p[:-7])
            elif fn.endswith(".c") or fn.endswith(".cc") or fn.endswith(".h"):
                srcs.append(p)
    if srcs:
        yield package

    if tests or srcs:
        with open_and_format(os.path.join(path, "BUILD.bazel")) as f:
            if tests:
                f.write(
                    """
load("@org_cloudabi_bazel_toolchains_cloudabi//:cc.bzl", "cc_test_cloudabi")
"""
                )

            if srcs:
                f.write(
                    """
cc_library(
   name = "%(name)s",
   srcs = %(srcs)s,
   visibility = ["//src/libc:__pkg__"],
)
"""
                    % {"name": os.path.basename(path), "srcs": repr(sorted(srcs))}
                )
            if tests:
                f.write(
                    """
[cc_test_cloudabi(
   name = test + "_test",
   srcs = [test + "_test.c"],
   deps = ["//src/libc/testing:testing_main"],
) for test in %s]
"""
                    % repr(sorted(tests))
                )


def traverse_packages(package, path):
    """Traverses directory, looking for packages."""
    if is_package(path):
        yield from generate_build_file(package, path)
    else:
        for f in os.listdir(path):
            p = os.path.join(path, f)
            if os.path.isdir(p):
                yield from traverse_packages("%s/%s" % (package, f), p)


# Generate the top-level BUILD.bazel file.
with open_and_format("BUILD.bazel") as f:
    f.write(
        """
cc_library(
    name = "libc",
    deps = %s,
    visibility = ["//visibility:public"],
)
"""
        % repr(sorted(traverse_packages("//src/libc", ".")))
    )

for proc in procs:
    proc.wait()

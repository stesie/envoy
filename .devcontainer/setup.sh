#!/usr/bin/env bash

. ci/setup_cache.sh
trap - EXIT

BAZELRC_FILE=~/.bazelrc bazel/setup_clang.sh /opt/llvm
echo "build --config=rbe-toolchain-clang" >> ~/.bazelrc
echo "build --symlink_prefix=/" >> ~/.bazelrc
echo "build ${BAZEL_BUILD_EXTRA_OPTIONS}" | tee -a ~/.bazelrc

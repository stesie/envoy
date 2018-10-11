#!/bin/bash -e

VERSION=0.15.0
SHA256=769b9757644a8ec9c4b07369fda4a3e6592639a1338a7a03225ceeedbc760b45

# buildifier
curl --location --output /usr/local/bin/buildifier https://github.com/bazelbuild/buildtools/releases/download/"$VERSION"/buildifier \
  && echo "$SHA256" '/usr/local/bin/buildifier' | sha256sum --check \
  && chmod +x /usr/local/bin/buildifier

# GCC for everything.
export CC=gcc
export CXX=g++
CXX_VERSION="$(${CXX} --version | grep ^g++)"
if [[ "${CXX_VERSION}" != "${EXPECTED_CXX_VERSION}" ]]; then
  echo "Unexpected compiler version: ${CXX_VERSION}"
  exit 1
fi

PREFIX=/envoy_deps_gcc_opt
export THIRDPARTY_DEPS=$PREFIX/deps
export THIRDPARTY_SRC=$PREFIX/thirdparty
export THIRDPARTY_BUILD=$PREFIX/thirdparty_build

mkdir -p $PREFIX
cp /deps.BUILD $PREFIX/BUILD
echo 'workspace(name = "envoy_deps")' > $PREFIX/WORKSPACE

DEPS=$(python <(cat /bazel-prebuilt/bazel/target_recipes.bzl; \
  echo "print ' '.join(\"${THIRDPARTY_DEPS}/%s.dep\" % r for r in set(TARGET_RECIPES.values()))"))
echo "Building opt deps ${DEPS}"
"$(dirname "$0")"/build_and_install_deps.sh ${DEPS}

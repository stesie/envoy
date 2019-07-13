workspace(name = "envoy")

load("//bazel:api_repositories.bzl", "envoy_api_dependencies")

envoy_api_dependencies()

load("//bazel:repositories.bzl", "GO_VERSION", "envoy_dependencies")
load("//bazel:cc_configure.bzl", "cc_configure")

envoy_dependencies()

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

cc_configure()

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")

go_rules_dependencies()

go_register_toolchains(go_version = GO_VERSION)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "bazel_toolchains",
    sha256 = "68e7678473090542e679ce7e6aa8a3ba5669577dede2b404f9865d556bd99f10",
    strip_prefix = "bazel-toolchains-0.28.0",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-toolchains/archive/0.28.0.tar.gz",
        "https://github.com/bazelbuild/bazel-toolchains/archive/0.28.0.tar.gz",
    ],
)

load("@bazel_toolchains//rules:rbe_repo.bzl", "rbe_autoconfig")
rbe_autoconfig(
    name = "rbe_ubuntu_clang",
    registry = "gcr.io",
    repository = "envoy-ci/envoy-build",
    digest = "sha256:9dbe1cba2b3340d49a25a1d286c8d49083ec986a6fead27f487e80ca334f065f",
)

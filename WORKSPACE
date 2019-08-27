workspace(name = "envoy")

load("//bazel:api_binding.bzl", "envoy_api_binding")

envoy_api_binding()

load("//bazel:api_repositories.bzl", "envoy_api_dependencies")

envoy_api_dependencies()

load("//bazel:repositories.bzl", "envoy_dependencies")

envoy_dependencies()

load("//bazel:dependency_imports.bzl", "envoy_dependency_imports")

envoy_dependency_imports()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

commit = "000b9c7ff87e58454fbb96849c3be5ac7461ca5a"
http_archive(
    name = "rpi_bazel",
    url = "https://github.com/mjbots/rpi_bazel/archive/{}.zip".format(commit),
    sha256 = "5e567936abefe572a38a507222dd4b7952ebe9ae530069bde855d6de3c51aee0",
    strip_prefix = "rpi_bazel-{}".format(commit),
    patches = ["//bazel:rpi_bazel.patch"],
    patch_args = ["-p1"],
)
load("@rpi_bazel//tools/workspace:default.bzl",
     rpi_bazel_add = "add_default_repositories")
rpi_bazel_add()


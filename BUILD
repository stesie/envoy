licenses(["notice"])  # Apache 2

exports_files(["VERSION"])

# /home/lizan/.cache/bazel/_bazel_lizan/1e01450ce718e05f04ac13414198f1bc/external/bazel_toolchains/configs/ubuntu16_04_clang/1.1/BUILD:253:1
platform(
  name = "rbe_ubuntu16.04",
  constraint_values = [
      "@bazel_tools//platforms:x86_64",
      "@bazel_tools//platforms:linux",
      "@bazel_tools//tools/cpp:clang",
#      "@bazel_toolchains//constraints:xenial",
#      "@bazel_toolchains//constraints/sanitizers:support_msan"
  ],
  remote_execution_properties = """
  properties: {
    name: "container-image"
    value:"docker://gcr.io/getenvoy-package/rbe-ubuntu-xenial@sha256:b456c7966e0eb447a6911f9e77686cde63288899280c08dd9f9edfa3c8386944"
  }""",
)


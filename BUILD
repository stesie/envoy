load("@protobuf_git//:protobuf.bzl", "cc_proto_library")

exports_files(["source/precompiled/precompiled.h"])

genrule(
    name = "envoy-ratelimit-proto",
    srcs = [
        "source/common/ratelimit/ratelimit.proto",
    ],
    outs = [
        "source/common/generated/ratelimit.proto",
    ],
    cmd = "cp $(SRCS) $@",
)

cc_proto_library(
    name = "envoy-ratelimit-pb",
    srcs = [
        "source/common/generated/ratelimit.proto",
    ],
    default_runtime = "//external:protobuf",
    protoc = "//external:protoc",
    include = "source",
)

genrule(
    name = "envoy-test-proto",
    srcs = [
        "test/proto/helloworld.proto",
    ],
    outs = [
        "test/generated/helloworld.proto",
    ],
    cmd = "cp $(SRCS) $@",
)

cc_proto_library(
    name = "envoy-test-pb",
    srcs = [
        "test/generated/helloworld.proto",
    ],
    default_runtime = "//external:protobuf",
    protoc = "//external:protoc",
    include = "test",
)

cc_binary(
    name = "deps_test",
    srcs = [
        "source/precompiled/precompiled.h",
        "deps_test.cc",
    ],
    copts = [
        "-I./include",
        "-I./source",
        "-include source/precompiled/precompiled.h",
    ],
    linkopts = [
        "-levent",
        "-levent_openssl",
        "-levent_pthreads",
        "-lhttp_parser",
        "-ljansson",
        "-lssl",
        "-lcrypto",
        "-lnghttp2",
        "-llightstep_core_cxx11",
        "-lprotobuf",
        "-lpthread",
        "-lanl",
        "-lrt",
        "-ldl",
    ],
    linkstatic=1,
    deps = [
        ":envoy-ratelimit-pb",
        "//external:spdlog",
    ],
)

genrule(
    name = "envoy-version",
    srcs = glob([
        ".git/**/*",
    ]),
    tools = [
        "tools/gen_git_sha.sh",
    ],
    outs = [
        "source/common/version_generated.cc",
    ],
    cmd = "touch $@ && $(location tools/gen_git_sha.sh) . $@",
)

cc_library(
    name = "envoy-common",
    srcs = glob([
        "source/**/*.cc",
        "source/**/*.h",
        "include/**/*.h",
    ], exclude=["source/exe/main.cc"]) + [
        "source/common/version_generated.cc",
    ],
    copts = [
        "-I./include",
        "-I./source",
        "-include source/precompiled/precompiled.h",
    ],
    linkopts = [
        "-lpthread",
        "-lanl",
        "-lrt",
    ],
    linkstatic=1,
    alwayslink=1,
    deps = [
        ":envoy-ratelimit-pb",
        "//external:boringssl_ssl",
        "//external:boringssl_crypto",
        "@boringssl//:bio_deprecit",
        "//external:nghttp2",
        "//external:spdlog",
        "//external:tclap",
        "//external:lightstep",
        "//external:event",
        "//external:protobuf",
        "//external:http_parser",
        "//external:rapidjson",
        "//external:event_pthreads",
    ],
)

cc_binary(
    name = "envoy",
    srcs = [
        "source/exe/main.cc",
    ],
    copts = [
        "-I./include",
        "-I./source",
        "-include source/precompiled/precompiled.h",
    ],
    deps = [
        ":envoy-common",
    ],
    linkstatic=1,
)

cc_test(
    name = "envoy-test",
    srcs = glob([
        "test/**/*.cc",
        "test/**/*.h",
    ]),
    data = glob([
        "generated/**/*",
        "test/**/*",
    ]),
    copts = [
        "-I./include",
        "-I./test",
        "-include test/precompiled/precompiled_test.h",
    ],
    deps = [
        ":envoy-common",
        ":envoy-test-pb",
        "//external:gtest",
    ],
    linkstatic=1,
)

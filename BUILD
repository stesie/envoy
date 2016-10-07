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

cc_library(
    name = "envoy-common",
    srcs = glob([
        "source/**/*.cc",
        "source/**/*.h",
        "include/**/*.h",
    ], exclude=["source/exe/main.cc"]),
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
    alwayslink=1,
    deps = [
        ":envoy-ratelimit-pb",
        "//external:spdlog",
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
    linkopts = [
        "-lgtest",
        "-lgmock",
    ],
    deps = [
        ":envoy-common",
        ":envoy-test-pb",
    ],
    linkstatic=1,
)

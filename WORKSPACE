git_repository(
    name = "protobuf_git",
    commit = "e8ae137c96444ea313485ed1118c5e43b2099cf1",  # v3.0.0
    remote = "https://github.com/google/protobuf.git",
)

bind(
    name = "protoc",
    actual = "@protobuf_git//:protoc",
)

bind(
    name = "protobuf",
    actual = "@protobuf_git//:protobuf",
)

bind(
    name = "cc_wkt_protos",
    actual = "@protobuf_git//:cc_wkt_protos",
)

bind(
    name = "cc_wkt_protos_genproto",
    actual = "@protobuf_git//:cc_wkt_protos_genproto",
)

bind(
    name = "protobuf_compiler",
    actual = "@protobuf_git//:protoc_lib",
)

bind(
    name = "protobuf_clib",
    actual = "@protobuf_git//:protobuf_lite",
)

new_git_repository(
    name = "spdlog_git",
    remote = "https://github.com/gabime/spdlog.git",
    commit = "319a62d73f0d6fd8262c606b071499be26f472b1",
    build_file = "BUILD.spdlog",
)

bind(
    name = "spdlog",
    actual = "@spdlog_git//:spdlog",
)



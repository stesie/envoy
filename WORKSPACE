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
    build_file = "third_party/BUILD.spdlog",
)

bind(
    name = "spdlog",
    actual = "@spdlog_git//:spdlog",
)

new_http_archive(
    name = "libevent_git",
    url = "https://github.com/libevent/libevent/releases/download/release-2.0.22-stable/libevent-2.0.22-stable.tar.gz",
    strip_prefix = "libevent-2.0.22-stable",
    build_file = "third_party/BUILD.libevent",
)

bind(
    name = "event",
    actual = "@libevent_git//:event",
)

bind(
    name = "event_pthreads",
    actual = "@libevent_git//:event_pthreads",
)

new_http_archive(
    name = "tclap_tar",
    url = "https://sourceforge.net/projects/tclap/files/tclap-1.2.1.tar.gz/download",
    type = "tar.gz",
    strip_prefix = "tclap-1.2.1",
    build_file = "third_party/BUILD.tclap",
)

bind(
    name = "tclap",
    actual = "@tclap_tar//:tclap",
)

new_git_repository(
    name = "lightstep_git",
    remote = "https://github.com/lightstep/lightstep-tracer-cpp.git",
    commit = "8d138d3739f4e89e27166427958f1e5bd7de2325",
    build_file = "third_party/BUILD.lightstep",
)


bind(
    name = "lightstep",
    actual = "@lightstep_git//:lightstep_core",
)

new_git_repository(
    name = "http_parser_git",
    remote = "https://github.com/nodejs/http-parser.git",
    commit = "9b0d5b33ebdaacff1dadd06bad4e198b11ff880e",
    build_file = "third_party/BUILD.http-parser",
)

bind(
    name = "http_parser",
    actual = "@http_parser_git//:http_parser",
)

"""
new_git_repository(
    name = "jansson_git",
    remote = "https://github.com/akheron/jansson.git",
    commit = "ee27b7e3ddf6853a698af163c8e0627fa295091b",
    build_file = "third_party/BUILD.jansson",
)
"""

new_http_archive(
    name = "jansson_git",
    url = "http://www.digip.org/jansson/releases/jansson-2.7.tar.gz",
    strip_prefix = "jansson-2.7",
    build_file = "third_party/BUILD.jansson",
)

bind(
    name = "jansson",
    actual = "@jansson_git//:jansson",
)

new_http_archive(
    name = "nghttp2_tar",
    url = "https://github.com/nghttp2/nghttp2/releases/download/v1.14.0/nghttp2-1.14.0.tar.gz",
    strip_prefix = "nghttp2-1.14.0",
    build_file = "third_party/BUILD.nghttp2",
)

bind(
    name = "nghttp2",
    actual = "@nghttp2_tar//:nghttp2",
)

new_git_repository(
    name = "googletest_git",
    remote = "https://github.com/google/googletest.git",
    commit = "ec44c6c1675c25b9827aacd08c02433cccde7780",
    build_file = "third_party/BUILD.googletest",
)

bind(
    name = "gtest",
    actual = "@googletest_git//:gtest",
)

git_repository(
    name = "boringssl",
    commit = "efaf893bf5f6ce2c663e15b617fcef613ef5d081",
    remote = "https://boringssl.googlesource.com/boringssl",
)

bind(
    name = "boringssl_crypto",
    actual = "@boringssl//:crypto"
)

bind(
    name = "boringssl_ssl",
    actual = "@boringssl//:ssl"
)

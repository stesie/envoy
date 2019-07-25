load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")

GO_VERSION = "1.12.5"

def envoy_dependency_imports():
    rules_foreign_cc_dependencies()
    go_rules_dependencies()
    go_register_toolchains(go_version = GO_VERSION)
    switched_rules_by_language(
        name = "com_google_googleapis_imports",
        cc = True,
        go = True,
        grpc = True,
        rules_override = {
            "py_proto_library": "@envoy_api//bazel:api_build_system.bzl",
        },
    )


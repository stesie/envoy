# Generated file, do not modify by hand
# Generated by 'rbe_ubuntu_gcc_gen' rbe_autoconfig rule
"""Definitions to be used in rbe_repo attr of an rbe_autoconf rule  """
toolchain_config_spec0 = struct(config_repos = [], create_cc_configs = True, create_java_configs = True, env = {"BAZEL_COMPILER": "clang", "BAZEL_LINKLIBS": "-l%:libstdc++.a", "BAZEL_LINKOPTS": "-lm:-static-libgcc", "CC": "clang", "CXX": "clang++", "PATH": "/usr/sbin:/usr/bin:/sbin:/bin:/usr/lib/llvm-8/bin"}, java_home = "/usr/lib/jvm/java-8-openjdk-amd64", name = "clang")
toolchain_config_spec1 = struct(config_repos = [], create_cc_configs = True, create_java_configs = True, env = {"BAZEL_COMPILER": "gcc", "BAZEL_LINKLIBS": "-l%:libstdc++.a", "BAZEL_LINKOPTS": "-lm:-static-libgcc", "CC": "gcc", "CXX": "g++", "PATH": "/usr/sbin:/usr/bin:/sbin:/bin:/usr/lib/llvm-8/bin"}, java_home = "/usr/lib/jvm/java-8-openjdk-amd64", name = "gcc")
_TOOLCHAIN_CONFIG_SPECS = [toolchain_config_spec0,toolchain_config_spec1]
_BAZEL_TO_CONFIG_SPEC_NAMES = {"0.28.0": ["clang", "gcc"]}
LATEST = "sha256:9dbe1cba2b3340d49a25a1d286c8d49083ec986a6fead27f487e80ca334f065f"
_CONTAINER_TO_CONFIG_SPEC_NAMES = {"sha256:9dbe1cba2b3340d49a25a1d286c8d49083ec986a6fead27f487e80ca334f065f": ["clang", "gcc"]}
_DEFAULT_TOOLCHAIN_CONFIG_SPEC = toolchain_config_spec0
TOOLCHAIN_CONFIG_AUTOGEN_SPEC = struct(
        bazel_to_config_spec_names_map = _BAZEL_TO_CONFIG_SPEC_NAMES,
        container_to_config_spec_names_map = _CONTAINER_TO_CONFIG_SPEC_NAMES,
        default_toolchain_config_spec = _DEFAULT_TOOLCHAIN_CONFIG_SPEC,
        latest_container = LATEST,
        toolchain_config_specs = _TOOLCHAIN_CONFIG_SPECS,
    )
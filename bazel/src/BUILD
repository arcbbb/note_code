cc_library(
    name = "version",
    srcs = ["version.cpp"],
)

cc_library(
    name = "liba",
    srcs = ["liba.cpp"],
)

cc_library(
    name = "libc",
    srcs = ["libc.cpp"],
)

cc_binary(
    name = "hello",
    srcs = ["hello.cpp"],
    deps =  select({
            ":use_liba": [":liba"],
            ":use_libc": [":libc"],
            "//conditions:default": [":libc"],
            }) + [":version"],
)

# Configurable attributes
# bazel build //label:target -c opt
# bazel build //label:target -c dbg
config_setting(
    name = "use_liba",
    values = {"compilation_mode" : "opt"},
)

config_setting(
    name = "use_libc",
    values = {"compilation_mode" : "dbg"},
)

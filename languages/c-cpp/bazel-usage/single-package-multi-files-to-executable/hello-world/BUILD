cc_library(
    name = "build-echo",
    srcs = ["echo.cc"],
    hdrs = ["echo.h"],
)

cc_binary(
    name="build-main",
    srcs=["main.cc"],
    deps = [
        "//hello-world:build-echo"
    ],
)
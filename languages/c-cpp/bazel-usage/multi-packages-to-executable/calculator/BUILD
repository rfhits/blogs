cc_library (
    name = "echo",
    srcs = ["echo.cc"],
    hdrs = ["echo.h"],
)


cc_binary(
    name = "calculator",
    srcs = ["main.cc"],
    deps = [
        ":echo",
        "//math:math"
    ],
)

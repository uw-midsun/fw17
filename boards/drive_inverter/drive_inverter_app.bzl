load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("//bazel:transitions.bzl", "midsun_transition_binary")

LINKER_SCRIPT = "//boards/drive_inverter:linker.ld"

def drive_inverter_app(name, srcs = [], deps = [], **kwargs):
    binary_name = name + "_cc"
    cc_binary(
        name = binary_name,
        srcs = srcs,
        additional_linker_inputs = [LINKER_SCRIPT],
        linkopts = [
            "-Wl,-T $(location {})".format(LINKER_SCRIPT),
        ],
        tags = ["manual"],
        deps = deps + ["@cmsis-device-g4//:startup_stm32g474xx"],
        **kwargs
    )

    midsun_transition_binary(
        name = name,
        binary = ":{}".format(binary_name),
        extra_defines = [
            "STM32G474xx",
        ],
        target_platform = "//bazel/platforms:stm32g4xx",
    )

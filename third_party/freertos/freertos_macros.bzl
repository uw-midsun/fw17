load("@rules_cc//cc:defs.bzl", "cc_library")

def freertos_config(name, config, target_compatible_with = []):
    header_name = "_{name}_header".format(name = name)
    native.genrule(
        name = header_name,
        srcs = [config],
        outs = ["{name}/FreeRTOSConfig.h".format(name = name)],
        cmd = "cp $< $@",
    )
    cc_library(
        name = name,
        hdrs = [header_name],
        strip_include_prefix = name,
        target_compatible_with = target_compatible_with,
    )

def freertos_kernel(name, port_path, config, target_compatible_with = []):
    port_path = port_path.rstrip("/")
    port_srcs = native.glob(["{path}/*.c".format(path = port_path)])

    kernel_hdrs = native.glob([
        "include/*.h",
        "{path}/*.h".format(path = port_path),
    ])

    cc_library(
        name = name,
        hdrs = kernel_hdrs,
        includes = [
            "include",
            port_path,
        ],
        srcs = [
            "croutine.c",
            "event_groups.c",
            "list.c",
            "queue.c",
            "stream_buffer.c",
            "tasks.c",
            "timers.c",
        ] + port_srcs,
        target_compatible_with = target_compatible_with,
        deps = [config],
    )

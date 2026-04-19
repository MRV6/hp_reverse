includedirs {
    "vendor/kiero",

    "vendor/minhook/include",

    "vendor/imgui",
    "vendor/imgui/backends"
}

files {
    "vendor/imgui/*.cpp",
    "vendor/imgui/backends/imgui_impl_dx12.cpp",
    "vendor/imgui/backends/imgui_impl_win32.cpp",

    "vendor/kiero/kiero.cpp",

    "vendor/minhook/**"
}
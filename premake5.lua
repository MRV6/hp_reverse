workspace "hp_reverse"
    configurations { "Debug", "Release" }
    architecture "x64"

project "hp_reverse"
    kind "SharedLib"
    language "C++"

    targetdir "bin/%{cfg.buildcfg}"
    targetname "hp_reverse"
    targetextension ".dll"

    cppdialect "C++17"

    includedirs { "includes" }

    files { "srcs/*.cpp", "includes/*.h" }

    include "vendor.lua"

    links { "d3d12", "dxgi", "d3dcompiler" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
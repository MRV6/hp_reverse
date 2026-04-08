workspace "hp_reverse"
    configurations { "Debug", "Release" }
    architecture "x64"

project "hp_reverse"
    kind "SharedLib"
    language "C++"

    targetdir "bin/%{cfg.buildcfg}"
    targetname "hp_reverse"
    targetextension ".dll"

    includedirs { "includes" }

    files { "includes/**.h", "srcs/**.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
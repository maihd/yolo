local ROOT_DIR = path.getabsolute(".")
local BUILD_DIR = path.join(ROOT_DIR, "Build")

workspace "Yolo"
do
    language "C++"
    location (BUILD_DIR)

    configurations { "Debug", "Release" }

    platforms { "x32", "x64" }

    startproject "Yolo.Test"

    flags {
        "NoPCH",
        "NoRuntimeChecks",
        "ShadowedVariables",
        "LinkTimeOptimization",

        --"FatalWarnings",
        --"FatalLinkWarnings",
        "FatalCompileWarnings",
    }

    cppdialect "C++11"
    staticruntime "On"
    omitframepointer "On"

    filter "configurations:Debug"
    do
        flags {
        }
    end

    filter {}
end

project "Yolo"
do
    kind "StaticLib"

    defines {
        "GLEW_STATIC"
    }

    links {
        "OpenGL32"
    }

    includedirs {
        path.join(ROOT_DIR, "Include"),
        
        path.join(ROOT_DIR, "ThirdParty/Sources/glew-2.1.0/include"),
    }

    files {
        path.join(ROOT_DIR, "Include/Yolo/*.h"),
        path.join(ROOT_DIR, "Include/Yolo/**/*.h"),

        path.join(ROOT_DIR, "Sources/*.h"),
        path.join(ROOT_DIR, "Sources/**/*.h"),

        path.join(ROOT_DIR, "Sources/*.c"),
        path.join(ROOT_DIR, "Sources/**/*.c"),

        path.join(ROOT_DIR, "Sources/*.cc"),
        path.join(ROOT_DIR, "Sources/**/*.cc"),

        path.join(ROOT_DIR, "ThirdParty/Sources/glew-2.1.0/src/glew.c"),
    }

    filter {}
end

project "Yolo.Test"
do
    kind "ConsoleApp"

    links {
        "Yolo"
    }

    includedirs {
        path.join(ROOT_DIR, "Include"),
    }

    files {
        path.join(ROOT_DIR, "Tests/*.cc"),
        path.join(ROOT_DIR, "Tests/**/*.cc"),   
    }

    filter {}
end
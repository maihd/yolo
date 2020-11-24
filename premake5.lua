local ROOT_DIR = path.getabsolute(".")
local BUILD_DIR = path.join(ROOT_DIR, "Build")
local THIRDPARTY_DIR = path.join(ROOT_DIR, "ThirdParty")

local function filedirs(dirs)
    if type(dirs) == "string" then
        files {
            path.join(ROOT_DIR, dirs, "*.h"),
            path.join(ROOT_DIR, dirs, "*.c"),
            path.join(ROOT_DIR, dirs, "*.cc"),
            path.join(ROOT_DIR, dirs, "*.cpp"),
        }
    elseif type(dirs) == "table" then
        for _, dir in ipairs(dirs) do
            filedirs(dir)
        end
    end
end

workspace "Yolo"
do
    language "C++"
    location (BUILD_DIR)

    configurations { "Debug", "Release" }

    platforms { "x32", "x64" }


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

    rtti "Off"
    exceptionhandling "Off"

    startproject "Yolo.Test"

    filter {}
end

project "Yolo"
do
    kind "StaticLib"

    defines {
        "GLEW_STATIC",
        --"AL_LIBTYPE_STATIC",
    }

    links {
        "SDL2",
        "OpenAL32",
    }

    includedirs {
        path.join(ROOT_DIR, "Include"),
        
        path.join(THIRDPARTY_DIR, "Include"),
        path.join(THIRDPARTY_DIR, "Sources/stb"),
        path.join(THIRDPARTY_DIR, "Sources/glew-2.1.0/include"),
    }

    files {
        path.join(THIRDPARTY_DIR, "Sources/glew-2.1.0/src/glew.c"),
    }

    filedirs {
        "Include/Yolo",
        "Include/Yolo/Heap",

        "Sources",
        "Sources/Heap",
        "Sources/Imgui",
    }

    filter { "platforms:x32" }
    do
        libdirs {
            path.join(THIRDPARTY_DIR, "Libs/Win32"),
        }

        postbuildcommands {
            
        }
    end

    filter { "platforms:x64" }
    do
        libdirs {
            path.join(THIRDPARTY_DIR, "Libs/Win64"),
        }

        postbuildcommands {
        }
    end

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

project "Spaneon"
do
    kind "ConsoleApp"

    links { 
        "Yolo"
    }

    includedirs {
        path.join(ROOT_DI, "Include"),
    }

    files {
        path.join(ROOT_DIR, "Games/Spaneon/Sources/*.h"),
        path.join(ROOT_DIR, "Games/Spaneon/Sources/**/*.h"),

        path.join(ROOT_DIR, "Games/Spaneon/Sources/*.cc"),
        path.join(ROOT_DIR, "Games/Spaneon/Sources/**/*.cc"),
    }
end
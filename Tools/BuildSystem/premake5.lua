local ROOT_DIR = path.getabsolute("../..")
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
    location (path.join(BUILD_DIR, _ACTION))

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

    cppdialect "C++14"
    staticruntime "On"
    omitframepointer "On"

    rtti "Off"
    exceptionhandling "Off"

    startproject "Yolo.Test"

    filter { "configurations:Debug" }
    do
        optimize "Off"
    end

    filter { "configurations:Release" }
    do
        optimize "Full"
    end

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
        "Include/Concurrency",
        "Include/Container",
        "Include/Graphics",
        "Include/Math",
        "Include/Misc",
        "Include/System",
        "Include/Text",

        "Sources",
        "Sources/Misc",
        "Sources/Text",
        "Sources/Imgui",
        "Sources/System",
        "Sources/Graphics",
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
        path.join(ROOT_DIR, "Tests/*.cpp"),
        path.join(ROOT_DIR, "Tests/**/*.cpp"),   
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

        path.join(ROOT_DIR, "Games/Spaneon/Sources/*.cpp"),
        path.join(ROOT_DIR, "Games/Spaneon/Sources/**/*.cpp"),
    }
end

project "Examples.Animation"
do
    kind "ConsoleApp"

    links { 
        "Yolo"
    }

    includedirs {
        path.join(ROOT_DI, "Include"),
    }

    files {
        path.join(ROOT_DIR, "Examples/Animation/*.h"),
        path.join(ROOT_DIR, "Examples/Animation/**/*.h"),

        path.join(ROOT_DIR, "Examples/Animation/*.cpp"),
        path.join(ROOT_DIR, "Examples/Animation/**/*.cpp"),
    }
end

project "Examples.ECS"
do
    kind "ConsoleApp"

    links {
        "Yolo"
    }

    includedirs {
        path.join(ROOT_DIR, "Include")
    }

    filedirs {
        path.join(ROOT_DIR, "Examples/ECS"),
        path.join(ROOT_DIR, "Examples/ECS/Tests"),
        path.join(ROOT_DIR, "Examples/ECS/Framework"),
        path.join(ROOT_DIR, "Examples/ECS/Systems"),
        path.join(ROOT_DIR, "Examples/ECS/Components"),
    }

    filter {}
end

project "Examples.TodoMVC"
do
    kind "ConsoleApp"

    links {
        "Yolo"
    }

    includedirs {
        path.join(ROOT_DIR, "Include")
    }

    filedirs {
        path.join(ROOT_DIR, "Examples/TodoMVC")
    }

    filter {}
end

project "Examples.Audios"
do
    kind "ConsoleApp"

    links {
        "Yolo"
    }

    includedirs {
        path.join(ROOT_DIR, "Include")
    }

    filedirs {
        path.join(ROOT_DIR, "Examples/Audios")
    }

    filter {}
end

project "Examples.ImGuiWidgets"
do
    kind "ConsoleApp"

    links {
        "Yolo"
    }

    includedirs {
        path.join(ROOT_DIR, "Include")
    }

    filedirs {
        path.join(ROOT_DIR, "Examples/ImGuiWidgets")
    }

    filter {}
end

-- project "Examples.SpineView"
-- do
--     kind "ConsoleApp"

--     links {
--         "Yolo"
--     }

--     includedirs {

--     }
-- end
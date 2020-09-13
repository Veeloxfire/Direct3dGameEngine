workspace "Direct3dGameEngine"
	configurations { "Debug", "Release" }
    platforms { "Win32" }
    startproject "Engine"

project "Utility"
    location "%{prj.name}"
    kind "None"

    files {
        "premake5.lua",
        "Engine/Log.txt",
        "%{prj.name}/app.manifest",
    }
   

project "Engine"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    location "%{prj.name}"
    warnings "Extra"

    shadermodel "5.0"
    shaderentry "main"

    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "bin/%{cfg.platform}/%{cfg.buildcfg}-obj/"

    files {
        "%{prj.name}/shaders/*.hlsli",
        "%{prj.name}/shaders/*_ps.hlsl",
        "%{prj.name}/shaders/*_vs.hlsl",
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp",
    }

    includedirs { "%{prj.name}/src" }    

    filter "files:**_vs.hlsl"
        shadertype "Vertex"
        
    filter "files:**_ps.hlsl"
        shadertype "Pixel"
    
    filter "platforms:Win32"
        defines { "WINDOWS" }
        system "Windows"
        architecture "x86"


    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
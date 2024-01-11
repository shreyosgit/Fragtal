workspace "FragTal"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	startproject "FragTal"

arch = "%{cfg.architecture}"
config = "%{cfg.buildcfg}"

project "FragTal"
	location "FragTal"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir ("bin/" .. arch .. "/" .. config)
	objdir ("bin/intermediates/" .. arch .. "/" .. config )

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		
		"%{prj.name}/ImGUI/**.h",
		"%{prj.name}/ImGUI/**.cpp",

		"%{prj.name}/Text Editor/**.h",
		"%{prj.name}/Text Editor/**.cpp",

		"%{prj.name}/UI/**.h"
	}

	includedirs
	{
		"FragTal/deps/include"
	}
	
	libdirs
	{
		"FragTal/deps/lib/" .. arch
	}

	--postbuildcommands
	--{
	--	"xcopy /Q /E /Y /I \"%{wks.location}\\%{prj.name}\\font\\*.ttf\" \"%{cfg.targetdir}\\font\\\"",
	--	"xcopy /Q /E /Y /I \"%{wks.location}\\%{prj.name}\\src\\Shader.cpp\" \"%{cfg.targetdir}\\src\\\"",
	--	"xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\*.ini\" \"%{cfg.targetdir}\\\"",
	--	"xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\*.dll\" \"%{cfg.targetdir}\\\"",
	--	"xcopy /Q /E /Y /I \"%{wks.location}\\deps\\include\\glm\" \"%{cfg.targetdir}\\glm\\\""
	--}

	postbuildcommands
	{
		-- Check if glm directory doesn't exist in the target directory before copying
		"IF NOT EXIST \"%{cfg.targetdir}\\font\\\" xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\font\\*.ttf\" \"%{cfg.targetdir}\\font\\\"",  -- Copy Fonts
		"IF NOT EXIST \"%{cfg.targetdir}\\icon\\\" xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\icon\\*.ttf\" \"%{cfg.targetdir}\\icon\\\"",  -- Copy Icons
		"IF NOT EXIST \"%{cfg.targetdir}\\UI\\\"   xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\UI\\*.h\" \"%{cfg.targetdir}\\UI\\\"",        -- Copy UI header file
    	"IF NOT EXIST \"%{cfg.targetdir}\\deps\\\" xcopy /Q /E /Y /I \"%{wks.location}\\%{prj.name}\\deps\\include\\glm\" \"%{cfg.targetdir}\\deps\\include\\glm\\\"", -- Copy GLM Math Library '/Y' for whole directory
		"IF NOT EXIST \"%{cfg.targetdir}\\temp\\\" xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\temp\\*.txt\" \"%{cfg.targetdir}\\temp\\\"", -- Copy Error.txt file
    	"xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\src\\Shader.cpp\" \"%{cfg.targetdir}\\src\\\"", -- Copy Shader.cpp file
		"xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\src\\Shader.h\"   \"%{cfg.targetdir}\\src\\\"", -- Copy Shader.h file
    	"xcopy /Q /Y /I \"%{wks.location}\\%{prj.name}\\*.ini\"           \"%{cfg.targetdir}\\\""       -- Copy .ini file
	}


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
	defines { "DEBUG" }
	runtime "Debug"
	symbols "On"
	optimize "Off"
	buildoptions "/MTd"
	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2test.lib"
	}
	kind "ConsoleApp"  -- Make debug mode a console app

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		symbols "Off"
		optimize "Speed"  -- Enable runtime optimization here
		buildoptions "/MT"
		links
		{
			"SDL2.lib",
			"SDL2main.lib",
		}
		kind "WindowedApp"  -- Make release mode a windowed app

	vstudio = {  }

	vstudio.toolset = "latest"

	vstudio.vc2022 = {
		includeDirs = { 
			"deps/include"
		}
	}

	vstudio.vc2022["Debug"] = {
		libDirs = { 
			"deps/lib"
		},
		links = { 
			"SDL2.lib",
			"SDL2main.lib",
			"SDL2test.lib"
		}
	}

	vstudio.vc2022["Release"] = {
		libDirs = { 
			"deps/lib"
		},
		links = {
			"SDL2.lib",
			"SDL2main.lib"
		}
	}
-------------------------------------------
-- Premake script for the Chis project,  --
-- a game developed by Octo Games (2019) --
-- Néstor Cabrero Martín (06/05/2019)    --
-------------------------------------------

-------------------------------------------
-- Paths							     --
-------------------------------------------
OGRE_ROOT = "Dependencies/ogre-1.11.5/"				-- Directory for the Ogre Library
CEGUI_ROOT = "Dependencies/cegui-0.8.7/"			-- Directory for the CEGUI library
BULLET_ROOT = "Dependencies/bullet3-2.88/"			-- Directory for the Bullet library
FMOD_ROOT = "Dependencies/FMOD/"					-- Directory for the FMOD library

-------------------------------------------
-- Solutions						     --
-------------------------------------------
workspace "Chis"
	configurations { "Debug", "Release"}			-- Build target configurations
	platforms { "x32" }								-- Build target platforms

	startproject "Chis"								-- The startup project

	language "C++"									-- Programming language for the projects
	debugdir "bin/"									-- Working directory for the projects
	targetdir "bin/"								-- Where the projects generate their binaries
	objdir "obj/%{prj.name}/%{cfg.buildcfg}"		-- Where the projects generate their intermediates 

	filter { "configurations:Debug" }				-- Filter by configuration (Debug)
		defines { "_DEBUG" }						-- Macros defined in this configuration
		symbols "On"								-- Symbols activated
		targetsuffix "_d"							-- Add a suffix to the debug output file

	filter { "configurations:Release" }				-- Filter by configuration (Release)
		defines { "NDEBUG" }						-- Macros defined in this configuration
		optimize "On"								-- Build optimization activated

	filter "system:windows"							-- Filter by system (Windows)
		cppdialect "C++17"							-- Version of the programming language
		systemversion "latest"						-- Toolset for the projects

	filter {} 										-- Reset filters

	files {
		"src/%{prj.name}/*.c",
		"src/%{prj.name}/*.h",
		"src/%{prj.name}/*.hpp",
		"src/%{prj.name}/*.cpp"
	}

	vpaths {
      ["Header Files/*"] = { 
        "src/%{prj.name}/**.h", 
        "src/%{prj.name}/**.hpp"
      },
      ["Source Files/*"] = { 
        "src/%{prj.name}/**.c", 
        "src/%{prj.name}/**.cpp" 
      },
}

	-------------------------------------------
	-- Projects							     --
	-------------------------------------------
	project "RenderSystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs {
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include"
		}

	project "PhysicSystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			BULLET_ROOT .. "src",
			"src/ECSystem",
			"src/RenderSystem"
		}

	project "MapSystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			"src/ECSystem",
			"src/RenderSystem"
		}

	project "InputSystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			"src/ECSystem",
			"src/RenderSystem"
		}

	project "GUISystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			CEGUI_ROOT .. "cegui/include",
			CEGUI_ROOT .. "build/cegui/include",
			"src/ECSystem",
			"src/InputSystem",
			"src/RenderSystem"
		}

	project "ECSystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			"src/RenderSystem"
		}

	project "AudioSystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			FMOD_ROOT .. "lowlevel/inc",
			FMOD_ROOT .. "fsbank/inc",
			FMOD_ROOT .. "studio/inc"
		}

	project "AISystem"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			"src/ECSystem",
			"src/MapSystem",
			"src/RenderSystem"
		}

	project "Engine"
		location "proj/%{prj.name}"
		kind "StaticLib"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			BULLET_ROOT .. "src",
			CEGUI_ROOT .. "cegui/include",
			CEGUI_ROOT .. "build/cegui/include",
			FMOD_ROOT .. "lowlevel/inc",
			FMOD_ROOT .. "fsbank/inc",
			FMOD_ROOT .. "studio/inc",
			"src/ECSystem",
			"src/AISystem",
			"src/GUISystem",
			"src/MapSystem",
			"src/InputSystem",
			"src/AudioSystem",
			"src/PhysicSystem",
			"src/RenderSystem"
		}

		links {
			"ECSystem",
			"AISystem",
			"GUISystem",
			"MapSystem",
			"InputSystem",
			"AudioSystem",
			"PhysicSystem",
			"RenderSystem"			
		}

	project "Chis"
		location "proj/%{prj.name}"
		kind "ConsoleApp"

		includedirs	{
			OGRE_ROOT .. "Build/include",
			OGRE_ROOT .. "OgreMain/include",
			OGRE_ROOT .. "Components/Bites/include",
			OGRE_ROOT .. "Build/Dependencies/include",
			BULLET_ROOT .. "src",
			CEGUI_ROOT .. "cegui/include",
			CEGUI_ROOT .. "build/cegui/include",
			FMOD_ROOT .. "lowlevel/inc",
			FMOD_ROOT .. "fsbank/inc",
			FMOD_ROOT .. "studio/inc",
			"src/ECSystem",
			"src/AISystem",
			"src/GUISystem",
			"src/MapSystem",
			"src/InputSystem",
			"src/AudioSystem",
			"src/PhysicSystem",
			"src/RenderSystem",
			"src/Engine"
		}

		libdirs { 
			OGRE_ROOT .. "Build/lib/%{cfg.buildcfg}",
			OGRE_ROOT .. "Build/Dependencies/lib",
			CEGUI_ROOT .. "Build/lib",
			BULLET_ROOT .. "Build/lib/%{cfg.buildcfg}",
			FMOD_ROOT .. "lowlevel/lib",
			FMOD_ROOT .. "fsbank/lib",
			FMOD_ROOT .. "studio/lib",
			"bin/"
		}

		links {
			"Engine",
		}

		filter { "configurations:Debug" }
			links {
				"GUISystem_d.lib",
				"InputSystem_d.lib",
				"RenderSystem_d.lib",
				"fmodL_vc.lib",
				"PhysicSystem_d.lib",
				"AudioSystem_d.lib",
				"ECSystem_d.lib",
				"SDL2.lib",
				"SDL2main.lib",
				"OgreMain_d.lib",
				"LinearMath_Debug.lib",
				"BulletCollision_Debug.lib",
				"BulletDynamics_Debug.lib",
				"MapSystem_d.lib",
				"AISystem_d.lib",
				"CEGUIBase-0_d.lib",
				"CEGUIOgreRenderer-0_d.lib",
				"OIS_d.lib",
				"OgreBites_d.lib",
				"Engine_d.lib"
			}

		filter { "configurations:Release" }
			links {
				"GUISystem.lib",
				"InputSystem.lib",
				"RenderSystem.lib",
				"AudioSystem.lib",
				"PhysicSystem.lib",
				"fmod_vc.lib",
				"ECSystem.lib",
				"SDL2.lib",
				"SDL2main.lib",
				"OgreMain.lib",
				"LinearMath.lib",
				"BulletCollision.lib",
				"BulletDynamics.lib",
				"MapSystem.lib",
				"AISystem.lib",
				"CEGUIBase-0.lib",
				"CEGUIOgreRenderer-0.lib",
				"OIS.lib",
				"OgreBites.lib",
				"Engine.lib"
			}
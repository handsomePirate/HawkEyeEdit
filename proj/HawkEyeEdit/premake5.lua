project "HawkEyeEdit"
	kind "ConsoleApp"
	staticruntime "off"
	language "C++"
	cppdialect "C++17"
	location ""
	targetdir "../../build/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
	files { 
		"../../src/**.hpp",
		"../../src/**.cpp"
	}

	flags {
		"MultiProcessorCompile"
	}

	includedirs {
		"../../ext/imgui",
		"../../ext/imgui/examples/libs/glfw/include",
		"../../ext/imgui/examples/libs/gl3w"
	}

	links {
		"imgui",
		"../../ext/imgui/examples/libs/glfw/lib-vc2010-64/glfw3.lib",
		"opengl32.lib"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"

	filter {}

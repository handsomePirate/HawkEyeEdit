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
		"../../src/**.cpp",
		"../../ext/imgui/backends/imgui_impl_glfw.cpp",
		"../../ext/imgui/backends/imgui_impl_opengl3.cpp",
		"../../ext/imgui/examples/libs/gl3w/GL/gl3w.c"
	}

	flags {
		"MultiProcessorCompile"
	}

	includedirs {
		SoftwareCoreInclude,
		"../../ext/imgui",
		"../../ext/imgui/examples/libs/glfw/include",
		"../../ext/imgui/examples/libs/gl3w",
		"../../ext/imgui/backends"
	}

	links {
		"SoftwareCore",
		"imgui",
		"../../ext/imgui/examples/libs/glfw/lib-vc2010-64/glfw3.lib",
		"opengl32.lib",
		"gdi32.lib",
		"shell32.lib"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"

	filter {}

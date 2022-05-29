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
		"../../ext/imgui-node-editor/external/gl3w/source/gl3w.c",
		"../../ext/imgui-node-editor/examples/application/source/glfw/imgui_impl_glfw_gl3.h",
		"../../ext/imgui-node-editor/examples/application/source/glfw/imgui_impl_glfw_gl3.cpp"
	}

	flags {
		"MultiProcessorCompile"
	}

	includedirs {
		SoftwareCoreInclude,
		ImguiInclude,
		ImguiNodeEditorInclude,
		"../../ext/imgui-node-editor/external/glfw/include",
		"../../ext/imgui-node-editor/external/gl3w/Include",
		"../../ext/imgui-node-editor/examples/application/include",
		"../../ext/imgui-node-editor/examples/application/source/glfw"
	}

	links {
		"SoftwareCore",
		"ImguiNodeEditor",
		"../../ext/imgui-node-editor/external/glfw/lib-vc2010-64/glfw3.lib",
		"opengl32.lib"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"

	filter {}

#include <application.h>
#include <imgui_impl_glfw_gl3.h>
#include <imgui_node_editor.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <SoftwareCore/DefaultLogger.hpp>
#include <iostream>

void Print(const char* message, Core::LoggerSeverity severity)
{
	const char* traceSev = "[Trace] ";
	const char* debugSev = "[Debug] ";
	const char* infoSev = "[Info] ";
	const char* warnSev = "[Warn] ";
	const char* errorSev = "[Error] ";
	const char* fatalSev = "[Fatal] ";

	switch (severity)
	{
	case Core::LoggerSeverity::Trace:
		std::cout << traceSev;
		break;
	case Core::LoggerSeverity::Debug:
		std::cout << debugSev;
		break;
	case Core::LoggerSeverity::Info:
		std::cout << infoSev;
		break;
	case Core::LoggerSeverity::Warn:
		std::cout << warnSev;
		break;
	case Core::LoggerSeverity::Error:
		std::cout << errorSev;
		break;
	case Core::LoggerSeverity::Fatal:
		std::cout << fatalSev;
		break;
	}

	std::cout << message;
}

static void glfw_error_callback(int error, const char* description)
{
	CoreLogError(DefaultLogger, "Glfw Error %d: %s", error, description);
}

int main(int argc, char* argv[])
{
	DefaultLogger.SetNewOutput(&Print);

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		CoreLogFatal(DefaultLogger, "Could not initialize glfw.");
		return 1;
	}

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "HawkEyeEdit", NULL, NULL);
	if (!window)
	{
		CoreLogFatal(DefaultLogger, "Could not start glfw window.");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (gl3wInit())
	{
		CoreLogFatal(DefaultLogger, "Could not initialize gl3w.");
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfwGL3_Init(window, true);

	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Application_Initialize();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);
		ImGui::Begin("Content", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus);

		Application_Frame();

		ImGui::End();

		ImGui::Render();
		int displayWidth, displayHeight;
		glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
		glViewport(0, 0, displayWidth, displayHeight);
		glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		glfwSwapBuffers(window);
	}

	Application_Finalize();

	ImGui_ImplGlfwGL3_Shutdown();

	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}

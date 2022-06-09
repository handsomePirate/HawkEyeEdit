#pragma once
#include <imgui.h>

const char* Application_GetName();
void Application_Initialize();
void Application_Load(const char* yamlFileName, bool exists);
void Application_Finalize();
void Application_AddOutputNode(const ImVec2& position);
void Application_AddInputNode(const ImVec2& position);
void Application_Frame();

#pragma once

#include <Windows.h>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <regex>

#include "SDL_Template.h"
#include "..\ImGUI\imconfig.h"
#include "..\ImGUI\imgui_internal.h"
#include "..\ImGUI\imstb_rectpack.h"
#include "..\ImGUI\imstb_textedit.h"
#include "..\ImGUI\imstb_truetype.h"
#include "..\Text Editor\TextEditor.h"
#include "..\UI\Icons.h"
#include "..\UI\ini.h"

namespace Utils {
    void PrintVec2(const char* vecName, glm::vec2 vec);
    void PrintVec2(const char* vecName, ImVec2 vec);
	void PrintVec3(const char* vecName, glm::vec3 vec);
	void PrintVec4(const char* vecName, glm::vec4 vec);
	void SaveFile(const char* filepath, auto textToSave);
	void CompileFile(std::string errorFilePath);
}

typedef glm::vec4(*MainImageType)(glm::vec2, glm::vec2, float);

struct IconState {
	// Icons
	bool playbutton;
	bool expandbutton;
};

class AppGUI 
{
public:
	AppGUI();
	~AppGUI();

	void Run();

	//ImVec2 GetAppWindowSize() const { return ImVec2(app_WindowWidth, app_WindowHeight); }

	const char* fileToEdit = "src\\Shader.cpp";
	const char* dllName = "Shader.dll";
	std::wstring wdllName = L"Shader.dll";
	std::string exeFilePath = " ";
	std::string errorFilePath = "temp\\Error.txt";
	std::string iconConfigPath = "UI\\IconConfig.ini";
	std::string editorConfigPath = "UI\\EditorConfig.ini";
private:
	SDL_Template sdl_instance;
	//std::vector<glm::ui8_tvec4> ImageData;

	const char* app_Title;
	int app_WindowWidth, app_WindowHeight;

	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

	TextEditor editor;

	/// For Runtime Compilation Error
	std::string fileContent;

	/// ImGui Custom Font Variables
	ImFont* font1;
	ImFont* font2;
	float fontSize;

	/// ImGui Custom Icon Variables
	ImFont* icon;
	float iconSize;
	IconState icons;

	/// Viewport Resize
	ImVec2 viewportSize = { 0, 0 };
	ImVec2 viewportPos = { 0, 0 };
	bool viewportExpand = false;
	bool viewportShrink = false;

	/*bool exitfullScreen = false;
	bool fullScreen = false;*/

	/// Load DLL at Runtime
	bool LoadDLL();

	ImGuiIO& Init();
	void InitEditor();
	void RenderEditor(ImGuiIO& io);
	void LoadFont(ImGuiIO& io, float fontSize);
	void LoadIcons(ImGuiIO& io, float fontSize);
	bool LoadIconState(std::string file, bool& icon1, bool& icon2);
	bool WriteIconState(std::string file, bool icon1, bool icon2);
	void Draw();
	void RenderUI();
	void RenderLoop();
	SDL_Texture* RenderImage(ImVec2 windowSize);
};

//glm::vec4 mainImage(glm::vec2 fragCoord, glm::vec2 iResolution, float iTime);

#ifdef __cplusplus
extern "C" {
	#endif

	glm::vec4 mainImage(glm::vec2 fragCoord, glm::vec2 iResolution, float iTime);

	#ifdef __cplusplus
}
#endif
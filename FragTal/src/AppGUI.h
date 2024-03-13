#pragma once

#include <thread>

#include "SDL_Template.h"

#include "..\ImGUI\imconfig.h"
#include "..\ImGUI\imgui_internal.h"
#include "..\ImGUI\imstb_rectpack.h"
#include "..\ImGUI\imstb_textedit.h"
#include "..\ImGUI\imstb_truetype.h"

#include "..\Text Editor\TextEditor.h"

#include "..\Utils\Utils.h"

#include "..\UI\Theme.h"
#include "..\UI\Icons.h"
#include "..\UI\Ini.h"

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

	const char* fileToEdit = "src\\Shader.cpp";
	const char* dllName = "Shader.dll";
	std::wstring wdllName = L"Shader.dll";
	std::string exeFilePath = " ";
	std::string errorFilePath = "temp\\Error.txt";
	std::string iconConfigPath = "UI\\IconConfig.ini";
	std::string editorConfigPath = "UI\\EditorConfig.ini";
	std::string appUIConfigPath = "UI\\FragTal.ini";

private:
	SDL_Template sdl_instance;
	//std::vector<glm::ui8_tvec4> ImageData;

	const char* app_Title;
	int app_WindowWidth, app_WindowHeight;

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
	bool viewportExpand = false;
	bool viewportShrink = false;
	bool viewportPlay = false;
	bool iswindowSizeChanged = false;

	void LoadDLLPath();
	bool LoadDLL();
	std::string LoadINIPath();
	void LoadFont(ImGuiIO& io, float fontSize);
	void LoadIcons(ImGuiIO& io, float fontSize);
	bool LoadIconState(std::string file, bool& icon1, bool& icon2);
	bool WriteIconState(std::string file, bool icon1, bool icon2);

	void InitEditor();
	ImGuiIO& Init();
	void RenderLoop();
	void RenderUI();
	void RenderEditor(ImGuiIO& io);
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
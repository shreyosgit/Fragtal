#pragma once

#include <stdio.h>
#include <dinput.h>
#include <tchar.h>
#include <cstdlib>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <streambuf>

#include "SDL\SDL.h"
#include "glm\glm.hpp"

#include "..\ImGUI\imgui.h"
#include "..\ImGUI\imgui_impl_sdl2.h"
#include "..\ImGUI\imgui_impl_sdlrenderer2.h"

namespace glm {
	typedef tvec4<uint8_t> ui8_tvec4;
}

enum struct WindowState {ACTIVE, EXIT};

class SDL_Template
{
public:
	SDL_Template();
	~SDL_Template();

	void sdl_CreateWindow(const char* w_title, int w_width, int w_height);
	void sdl_CreateTexture(int u, int v);
	void sdl_DeleteTexture();
	void sdl_DrawTexture(int u, int v, std::vector<glm::ui8_tvec4>& PixelData);
	void sdl_RenderBegin();
	void sdl_RenderEnd();
	void sdl_InputEvent();

	WindowState sdl_WindowState;

	SDL_Window* sdl_WindowPtr;
	SDL_Renderer* sdl_RendererPtr;
	SDL_Texture* sdl_TexturePtr;

	int sdl_TextureSize;
private:
	const char* sdl_WindowTitle;
	int sdl_WindowWidth, sdl_WindowHeight;
	
	
	SDL_Rect sdl_ScrrenSize;

	void sdl_Init();
	void sdl_PrintError(std::string ErrorList);
	void sdl_Quit();
};


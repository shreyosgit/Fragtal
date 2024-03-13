#pragma once

#include <Windows.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <xstring>

#include "glm\glm.hpp"

#include "..\ImGUI\imgui.h"

#include "Timer.h"

namespace Utils {

	void PrintVec2(const char* vecName, glm::vec2 vec);
	void PrintVec2(const char* vecName, ImVec2 vec);
	void PrintVec3(const char* vecName, glm::vec3 vec);
	void PrintVec4(const char* vecName, glm::vec4 vec);
	void SaveFile(const char* filepath, std::string textToSave);
	std::string LoadTxtFile(std::string& filePath);
	std::wstring ConvertStringtoWstring(std::string FilePath);
	std::string ConvertWStringtoString(std::wstring FilePath);
	const char* BoolToString(bool value);
	bool StringToBool(const std::string& str);
	void CompileFileWithConsole(std::string errFilePath);
	void CompileFileWithoutConsole(std::string errFilePath, std::wstring dllPath, bool& isRecompiled);

}

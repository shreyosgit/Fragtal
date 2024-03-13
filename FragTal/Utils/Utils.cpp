#include "Utils.h"


namespace Utils {

    void PrintVec2(const char* vecName, glm::vec2 vec) {
        std::cout << vecName << std::endl;
        std::cout << "X => " << vec.r << std::endl;
        std::cout << "Y => " << vec.g << std::endl;
    }
    void PrintVec2(const char* vecName, ImVec2 vec) {
        std::cout << vecName << std::endl;
        std::cout << "X => " << vec.x << std::endl;
        std::cout << "Y => " << vec.y << std::endl;
    }
    void PrintVec3(const char* vecName, glm::vec3 vec) {
        std::cout << vecName << std::endl;
        std::cout << "X => " << vec.r << std::endl;
        std::cout << "Y => " << vec.g << std::endl;
        std::cout << "Z => " << vec.b << std::endl;
    }
    void PrintVec4(const char* vecName, glm::vec4 vec) {
        std::cout << vecName << std::endl;
        std::cout << "X => " << vec.r << std::endl;
        std::cout << "Y => " << vec.g << std::endl;
        std::cout << "Z => " << vec.b << std::endl;
        std::cout << "W => " << vec.a << std::endl;
    }
    void SaveFile(const char* filepath, std::string textToSave) {
        std::ofstream myfile(filepath);
        if (myfile.is_open())
        {
            myfile << textToSave;
            myfile.close();
        }
        else {
            std::cout << "Unable to open file" << std::endl;
        }
    }
    std::string LoadTxtFile(std::string& filePath) {

        std::ifstream file(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string fileContent = buffer.str();

        return fileContent;
    }

    std::wstring ConvertStringtoWstring(std::string FilePath) {
        return std::wstring(FilePath.begin(), FilePath.end());
    }

    std::string ConvertWStringtoString(std::wstring FilePath) {
        return std::string(FilePath.begin(), FilePath.end());
    }

    const char* BoolToString(bool value) {
        return value ? "true" : "false";
    }

    bool StringToBool(const std::string& str) {
        std::string lowercaseStr;
        for (char c : str) {
            lowercaseStr += std::tolower(c);
        }
        if (lowercaseStr == "1") {
            return true;
        }
        return false;
    }

    /// <param name="errorFilePath"> Takes a string of the file path where compilation error will be saved.</param>
    /// Note: Security concerns and Outdated to be removed!
    void CompileFileWithConsole(std::string errFilePath) {
        if (!std::filesystem::exists(errFilePath)) {
            std::cout << "Temp File Not Found!" << std::endl;
        }

        std::string compileCommand = "g++ -std=c++2a -O3 -shared -o Shader.dll -I../../deps/include src/Shader.cpp 2> " + errFilePath;
        std::system(compileCommand.c_str());
    }

    /// <summary>
    /// Found this code from Here: https://cboard.cprogramming.com/windows-programming/109024-createprocess-plus-command-line.html
    /// , https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw
    /// , https://stackoverflow.com/questions/515309/what-does-cmd-c-mean
    /// </summary>
    /// <param name="errorFilePath"> Takes a string of the file path where compilation error will be saved.</param>
    void CompileFileWithoutConsole(std::string errFilePath, std::wstring dllPath, bool& isRecompiled) {

        if (!std::filesystem::exists(errFilePath)) {
            std::cout << "Temp File Not Found!" << std::endl;

            isRecompiled = false;
            return;
        }

        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::wstring werrFilePath = Utils::ConvertStringtoWstring(errFilePath);
        std::wstring app = L"cmd.exe /C g++ -std=c++2a -O3 -march=native -flto -shared -o \"" + dllPath + L"\" -I..\\..\\deps\\include src\\Shader.cpp 2> " + werrFilePath;

        if (CreateProcess( /// Creating a separate process for compilation
            NULL,
            const_cast<LPWSTR>(app.c_str()),
            NULL,
            NULL,
            FALSE,
            CREATE_NO_WINDOW,
            NULL,
            NULL,
            &si,
            &pi)) {

            // Note: Close handles to the process and thread immediately after starting
            WaitForSingleObject(pi.hProcess, INFINITE);
        }
        else {
            std::cout << "Error: Failed to launch the process!" << GetLastError() << std::endl;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        isRecompiled = true;
    }
}
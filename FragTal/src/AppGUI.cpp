#include "AppGUI.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

namespace {

    //  These have Internal Linkage & Static storage duration

    std::wstring gdllPath = L" ";

    bool isDllLoaded = false;

    MainImageType mainImageFromDLL = NULL;
    HINSTANCE handle = NULL;

    std::vector<glm::ui8_tvec4> ImageData;

    void ProcessPixels(int startY, int endY, ImVec2 windowSize, glm::vec2 iResolution, float iTime);
}

AppGUI::AppGUI() : sdl_instance(), icons() {
	app_Title = "FragTal";
    app_WindowWidth = 2560;
    app_WindowHeight = 1370;

	// ImGUI State Variables
	show_demo_window = true;
	show_another_window = false;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // ImGUI Font Setting Variables
    font1 = nullptr;
    font2 = nullptr;
    fontSize = 23.0f;

    icon = nullptr;
    iconSize = 23.0f;

    fileContent = " ";
}
AppGUI::~AppGUI() {

}

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
    void SaveFile(const char* filepath, auto textToSave) {
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
    std::string LoadTxtFile() {

        std::ifstream file("temp/Error.txt");
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
    void CompileFileWithoutConsole(std::string errFilePath) {
        
        if (!std::filesystem::exists(errFilePath)) {
            std::cout << "Temp File Not Found!" << std::endl;
            return;
        }

        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::wstring werrFilePath = Utils::ConvertStringtoWstring(errFilePath);
        std::wstring app = L"cmd.exe /C g++ -std=c++2a -O3 -march=native -flto -shared -o \"" + gdllPath + L"\" -I..\\..\\deps\\include src\\Shader.cpp 2> " + werrFilePath;
        
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

            // Close handles to the process and thread immediately after starting
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            std::cout << "Failed to launch the process " << GetLastError()  << std::endl;
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}

ImGuiIO& AppGUI::Init() {
	// Creating SDL window and rendering context
	sdl_instance.sdl_CreateWindow(app_Title, app_WindowWidth, app_WindowHeight);

	// Create ImGUI Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Pass ImGui flags
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable;

    io.IniFilename = "FragTal.ini";

	// Setup Dear ImGui style
    ImGui::StyleColorsCustom();
    // Load the Custom Font
    LoadFont(io, fontSize);
    // Load the Custom Icons
    LoadIcons(io, iconSize);

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(sdl_instance.sdl_WindowPtr, sdl_instance.sdl_RendererPtr);
	ImGui_ImplSDLRenderer2_Init(sdl_instance.sdl_RendererPtr);

    // Initialize the Editor
    InitEditor();

    /// To-Do: If the DLL is generating runtime error in the middle and the application crashes as we are loading dll everytime at initialization, this will lock the application where rebuiling the app would not work 
    /// Fix: Implement a DLL check in another environment or maybe add a failsafe or fallback system, where the app will fallback to it's last working version
    // Loading the DLL
    LoadDLL();

	return io;
}

void AppGUI::InitEditor() {
    // TEXT EDITOR SAMPLE
    auto lang = TextEditor::LanguageDefinition::CPlusPlus();

    editor.SetLanguageDefinition(lang);
    //editor.SetPalette(TextEditor::GetLightPalette());

    // error markers
    /*TextEditor::ErrorMarkers markers;
    markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
    markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
    editor.SetErrorMarkers(markers);*/

    // "breakpoint" markers
    //TextEditor::Breakpoints bpts;
    //bpts.insert(24);
    //bpts.insert(47);
    //editor.SetBreakpoints(bpts);

    //	static const char* fileToEdit = "test.cpp";

    {
        std::ifstream t(fileToEdit);
        if (t.good())
        {
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            editor.SetText(str);
        }
    }

}

void AppGUI::LoadFont(ImGuiIO& io, float fontSize) {
    /*ImFontConfig fontConfig;
    fontConfig.OversampleH = 2;
    fontConfig.OversampleV = 2;*/

    font1 = io.Fonts->AddFontFromFileTTF("font/bmonofont-i18n.ttf", fontSize); // , &fontConfig
    //io.FontGlobalScale = 0.5f;
    font2 = io.Fonts->AddFontFromFileTTF("font/Cascadia.ttf", fontSize * 2.0f);
    io.Fonts->Build();
}

void AppGUI::LoadIcons(ImGuiIO& io, float fontSize) {

    //float baseFontSize = fontSize; // 13.0f is the size of the default font. Change to the font size you use.
    //float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    //// merge in icons from Font Awesome
    //static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    //ImFontConfig icons_config;
    //icons_config.MergeMode = true;
    //icons_config.PixelSnapH = true;
    //icons_config.GlyphMinAdvanceX = iconFontSize;
    //io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);
    //// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
    //// in an imgui window somewhere...
    ////ImGui::Text(ICON_FA_ADDRESS_BOOK "  Paint"); // use string literal concatenation
    //// outputs a paint brush icon and 'Paint' as a string.

    float baseFontSize = fontSize; // 13.0f is the size of the default font. Change to the font size you use.
    //float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN, ICON_MAX, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = baseFontSize;
    icon = io.Fonts->AddFontFromFileTTF(FRAGTAL_FONT, baseFontSize, &icons_config, icons_ranges);
    io.Fonts->Build();
}

void AppGUI::Run() {
    RenderLoop();
}

bool AppGUI::LoadIconState(std::string file, bool& icon1, bool& icon2) {

    // first, create a file instance
    mINI::INIFile configFile(file);

    // next, create a structure that will hold data
    mINI::INIStructure ini;

    // now we can read the file
    bool readSuccess = configFile.read(ini);

    // read a value
    std::string iconstring1 = ini.get("iconstate").get("playbutton");
    std::string iconstring2 = ini.get("iconstate").get("expandbutton");

    icon1 = Utils::StringToBool(iconstring1);
    icon2 = Utils::StringToBool(iconstring2);

    return readSuccess;
}

bool AppGUI::WriteIconState(std::string file, bool icon1, bool icon2) {
    std::ofstream configFile(file, std::ios::binary | std::ios::out);

    if (configFile.good() && configFile.is_open()) {
        
        configFile << "[iconstate]" << std::endl;
        configFile << "playbutton = " << icon1 << std::endl;
        configFile << "expandbutton = " << icon2 << std::endl;
        configFile.close();

        return true;
	}
	return false;
}

void AppGUI::RenderLoop() {

    ImGuiIO& io = Init(); (void)io;

    // Load the Icon state From Disk here
    if (!LoadIconState(std::string(exeFilePath + "\\" + iconConfigPath), icons.playbutton, icons.expandbutton)) {
      fileContent = "Error: Icon State variables cannot be read!";
    }

    while (sdl_instance.sdl_WindowState == WindowState::ACTIVE) {

        // start = std::chrono::high_resolution_clock::now();

        sdl_instance.sdl_RenderBegin();
        sdl_instance.sdl_InputEvent();

        /// Viewport window resizing
        if (viewportShrink) {
            ImGui::LoadIniSettingsFromDisk(std::string(exeFilePath + "\\" + editorConfigPath).c_str());
            viewportShrink = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        RenderUI();

        ImGui::EndFrame();
        ImGui::Render();

        SDL_RenderSetScale(sdl_instance.sdl_RendererPtr, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(sdl_instance.sdl_RendererPtr, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        //SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        sdl_instance.sdl_RenderEnd();

		// end = std::chrono::high_resolution_clock::now();
		// duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    }

    // Write the Icon state to Disk before cleanup here
    if (!WriteIconState(std::string(exeFilePath + "\\" + iconConfigPath), icons.playbutton, icons.expandbutton)) {
        std::cout << "Error: Icon Config could not be saved!" << std::endl;
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void AppGUI::RenderUI() {
    { 
        // Docking
        {
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::Begin("DockSpace Demo", nullptr, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // Submit the DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            else
            {
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
                ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
                ImGui::SameLine(0.0f, 0.0f);
                if (ImGui::SmallButton("click here"))
                    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Options"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                    ImGui::MenuItem("Padding", NULL, &opt_padding);
                    ImGui::Separator();

                    if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
                    if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
                    if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
                    if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                    if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                    if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                    ImGui::Separator();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            /// Viewport window resizing
            if (viewportExpand) {

                ImGui::SaveIniSettingsToDisk(std::string(exeFilePath + "\\" + editorConfigPath).c_str());

                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
                ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
                ImGui::SetNextWindowBgAlpha(0.0f);
                ImGui::SetNextWindowCollapsed(false);
                ImGui::SetNextWindowFocus();

                viewportExpand = false;
            }

            /// Viewport
            ImGui::Begin(" Viewport ", nullptr, ImGuiWindowFlags_NoScrollbar);
            {

                // Get Viewpost Window Size
                ImVec2 windowSize = ImGui::GetWindowSize();
                // Get Viewpost Window Position
                ImVec2 windowPos = ImGui::GetWindowPos();

                ImGui::Image(reinterpret_cast<void*>(RenderImage(windowSize)), windowSize);

                /// Performance Overlays

                /// Last Image Render Time Overlay
                {
                    // Set Overlay Window Dimensions
                    float width = 215.0f;
                    float height = 45.0f;

                    // Set Overlay Window Position
                    float left = windowPos.x + 20.0f;
                    float top = windowPos.y + 50.0f;
                    ImGui::SetNextWindowPos(ImVec2(left, top));

                    ImGui::BeginChild("Frame Render Time Window", ImVec2(width, height), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
                    {
                        ImDrawList* dl = ImGui::GetWindowDrawList();
                        dl->AddRectFilled(
                            ImVec2(windowPos.x, windowPos.y),                         // Top-Left Corner of the overlay window
                            ImVec2(windowPos.x + windowSize.x, windowPos.y + 200.0f), // Top-Right Corner of the overlay window
                            IM_COL32(0, 0, 0, 100),                                   // Rect color the transparency level (0 to 255)
                            NULL                                                      // Corner Radius
                        );

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                        ImGui::SetCursorPos(ImVec2(0.0f, 10.0f));
                        ImGui::Text(" Image: %.3f ms", io.DeltaTime * 1000.0f);
                        ImGui::PopStyleColor();
                    }
                    ImGui::EndChild();
                }
                /// FPS Overlay
                {
                    // Set Overlay Window Position
                    // Offset = Viewport_WindowPosition + Viewpost_WindowSize - 164.0f - Why 164.0f? taking 16.0f gap from width 148.0f
                    ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x - 164.0f, windowPos.y + 50.0f));

                    ImGui::BeginChild("Frame Per Second Window", ImVec2(148.0f, 45.0f), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
                    {
                        ImDrawList* dl = ImGui::GetWindowDrawList();
                        dl->AddRectFilled(
                            ImVec2(windowPos.x, windowPos.y),                         // Top-Left Corner of the overlay window
                            ImVec2(windowPos.x + windowSize.x, windowPos.y + 200.0f), // Top-Right Corner of the overlay window
                            IM_COL32(0, 0, 0, 100),                                   // Rect color the transparency level (0 to 255)
                            NULL                                                      // Corner Radius
                        );

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                        ImGui::SetCursorPos(ImVec2(0.0f, 10.0f));
                        ImGui::Text(" Fps: %.2f ", 1.0f / io.DeltaTime);
                        ImGui::PopStyleColor();
                    }
                    ImGui::EndChild();
                }
                /// Viewport Control Overlay
                {
                    // Set Overlay Window Position
                    // Offset = Viewport_WindowPosition + Viewpost_WindowSize - 164.0f - Why 164.0f? taking 16.0f gap from width 148.0f

                    float width = windowSize.x;
                    float height = 45.0f;

                    ImGui::SetNextWindowPos(ImVec2(windowPos.x, windowPos.y + windowSize.y - height));

                    ImGui::BeginChild(" Viewport Control ", ImVec2(width, height), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
                    {
                        ImDrawList* dl = ImGui::GetWindowDrawList();
                        dl->AddRectFilled( 
                            ImVec2(windowPos.x, windowPos.y + windowSize.y - height),       // Top-Left Corner of the overlay window
                            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y), // Top-Right Corner of the overlay window
                            IM_COL32(0, 0, 0, 100),                                         // Rect color the transparency level (0 to 255)
                            NULL                                                            // Corner Radius
                        );

						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.0f)); // Adjust hover color and transparency here
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.3333f, 0.0745f, 0.8f)); // Adjust hover color and transparency here

						ImGui::PushFont(icon);

                        // For Play & Pause Button
						ImGui::SetCursorPos(ImVec2(30.0f, -10.0f));
                        if (icons.playbutton) {
                            if (ImGui::Button(ICON_PLAY)) {
                                icons.playbutton = false;
                            }
                        }
                        else {
                            // Stop the rendering cycle
                            if (ImGui::Button(ICON_PAUSE)) {
                                icons.playbutton = true;
                            }
                        }
                        // For Viewport Compress and Expand Button
                        ImGui::SetCursorPos(ImVec2(windowSize.x - 60.0f, -10.0f)); // The default cursor position is set on here: SetNextWindowPos(ImVec2(windowPos.x, windowPos.y + windowSize.y - height)

                        if (icons.expandbutton) {
                            if (viewportExpand = ImGui::Button(ICON_EXPAND)) {
                                viewportSize = windowSize;
                                viewportPos = windowPos;
                                icons.expandbutton = false;
                            }
                        }
                        else {
                            // Enlarge the viewport size to the application windowsize
                            if (viewportShrink = ImGui::Button(ICON_COMPRESS)) {
                                icons.expandbutton = true;
                            }
                        }
                        ImGui::PopFont();

                        ImGui::PushFont(font1);
                        ImGui::SetCursorPos(ImVec2(90.0f, 9.0f));
                        ImGui::Text(" %.0f x %.0f ", windowSize.x, windowSize.y);
                        ImGui::PopFont();
						ImGui::PopStyleColor(3);
                    }
                    ImGui::EndChild();
                }

            }
            ImGui::End();

            /// Editor
            RenderEditor(io);

            /// Output
            ImGui::Begin(" Output ");
            {
                ImGui::Text(fileContent.c_str());
            }
            ImGui::End();

        }// Docking

        ImGui::End();
	}
}

bool AppGUI::LoadDLL() {

    wchar_t exePath[MAX_PATH]; // Use wide-character string
    GetModuleFileNameW(nullptr, exePath, MAX_PATH); // Use GetModuleFileNameW

    // Extract directory from the path
    std::wstring exeDirectory = std::wstring(exePath);
    size_t pos = exeDirectory.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        exeDirectory = exeDirectory.substr(0, pos);
    }
    
    std::wstring dllPath = exeDirectory + L"\\" + wdllName;
    
    // Store exe and dll path to make it available globally
    ::gdllPath = dllPath;
    exeFilePath = Utils::ConvertWStringtoString(exeDirectory);

    // Load the DLL
    ::handle = LoadLibrary((LPCWSTR)dllPath.c_str());

    if (!::handle) {

        fileContent = "Error: Unable to load " + std::string(dllName);
        // Handle the error (e.g., return or take appropriate action)
        return false;
    }
    // Obtain function pointer to mainImage from DLL
    ::mainImageFromDLL = reinterpret_cast<MainImageType>(GetProcAddress(::handle, "mainImage"));
    if (!::mainImageFromDLL) {

        fileContent = "Error: Undefined function 'mainImage()'\n";
        // Handle the error (e.g., return or take appropriate action)
        return false;
    }
    return true;
}

bool operator==(const ImVec2& lval, const ImVec2& rval) {
    if (lval.x == rval.x && lval.y == rval.y) {
        return true;
    }
    else {
        return false;
    }
}

void ::ProcessPixels(int startY, int endY, ImVec2 windowSize, glm::vec2 iResolution, float iTime) {
    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < windowSize.x; ++x) {
            glm::vec2 coord = glm::vec2((float)x, (float)windowSize.y - (float)y);

            if (::mainImageFromDLL) {
                ImageData[x + y * windowSize.x] = ::mainImageFromDLL(coord, iResolution, iTime);
            }
            else {
                ImageData[x + y * windowSize.x] = mainImage(coord, iResolution, iTime);
            }
        }
    }
}

SDL_Texture* AppGUI::RenderImage(ImVec2 windowSize) {

    static ImVec2 lastWindowSize = { 0, 0 };

    // On initialization
    if (lastWindowSize == ImVec2(0, 0)) {
        // Resize the buffer
        ImageData.resize(static_cast<size_t>(windowSize.x) * static_cast<size_t>(windowSize.y));

        // Create a new texture with the changed size
        sdl_instance.sdl_CreateTexture(windowSize.x, windowSize.y);
    }
    // On viewport resize
    else if (windowSize.x != lastWindowSize.x || windowSize.y != lastWindowSize.y) {
        // Destroy old SDL texture
        sdl_instance.sdl_DeleteTexture();

        // Resize the buffer
        ImageData.resize(static_cast<size_t>(windowSize.x) * static_cast<size_t>(windowSize.y));

        // Create a new texture with the changed size
        sdl_instance.sdl_CreateTexture(windowSize.x, windowSize.y);
    }

    // Update the windowsize
    lastWindowSize = windowSize;

    glm::vec2 iResolution = glm::vec2(windowSize.x, windowSize.y);
    float iTime = ImGui::GetTime();

    //auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    int numThreads = 10; // Number of threads
    int rowsPerThread = windowSize.y / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startY = i * rowsPerThread;
        int endY = (i == numThreads - 1) ? windowSize.y : startY + rowsPerThread; // Ensure all rows are processed

        threads.push_back(std::thread(::ProcessPixels, startY, endY, windowSize, iResolution, iTime));
    }
    for (auto& thread : threads) {
        thread.join();
    }

    /*auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Elapsed Time: " << duration.count() << " milliseconds" << std::endl;*/

    sdl_instance.sdl_DrawTexture(windowSize.x, windowSize.y, ImageData);
    return sdl_instance.sdl_TexturePtr;
}

void AppGUI::RenderEditor(ImGuiIO& io) {

    auto cpos = editor.GetCursorPosition();
    /// Editor
    ImGui::Begin(" Text Editor ", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S)) && io.KeyCtrl) {

        // Unload the DLL
        FreeLibrary(::handle);

		// Save the text to file location
		Utils::SaveFile(fileToEdit, editor.GetText());

		// Uses CreateProcessW() for runtime compilation
		Utils::CompileFileWithoutConsole(errorFilePath);

		if (!std::filesystem::exists(errorFilePath)) {
			// Create Error File if it doesn't exists
			std::ofstream errorFile(errorFilePath);
		}

		// Load Text File to show in ImGui Output Window
		fileContent = Utils::LoadTxtFile();

		// Reload the DLL
		LoadDLL();
    }

    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "Ctrl-S")) {

                // Unload the DLL
                FreeLibrary(::handle);

                // Save the text to file location
                Utils::SaveFile(fileToEdit, editor.GetText());
                
                // Uses std::system() for runtime compilation
                Utils::CompileFileWithConsole(errorFilePath);

                if (!std::filesystem::exists(errorFilePath)) {
                    // Create Error File if it doesn't exists
                    std::ofstream errorFile(errorFilePath);
                }

                // Load Text File to show in ImGui Output Window
                fileContent = Utils::LoadTxtFile();

                // Reload the DLL
                LoadDLL();
            }
            /*if (ImGui::MenuItem("Quit", "Alt-F4"))
                break;*/
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            bool ro = editor.IsReadOnly();
            if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                editor.SetReadOnly(ro);
            ImGui::Separator();

            if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
                editor.Undo();
            if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
                editor.Redo();

            ImGui::Separator();

            if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
                editor.Copy();
            if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
                editor.Cut();
            if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
                editor.Delete();
            if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                editor.Paste();

            ImGui::Separator();

            if (ImGui::MenuItem("Select all", nullptr, nullptr))
                editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Dark palette"))
                editor.SetPalette(TextEditor::GetDarkPalette());
            if (ImGui::MenuItem("Light palette"))
                editor.SetPalette(TextEditor::GetLightPalette());
            if (ImGui::MenuItem("Retro blue palette"))
                editor.SetPalette(TextEditor::GetRetroBluePalette());
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s",
        cpos.mLine + 1,                               // Current Line Position of Text Cursor
        cpos.mColumn + 1,                             // Current Column Position of Text Cursor
        editor.GetTotalLines(),
        editor.IsOverwrite() ? "Ovr" : "Ins",         // Editing Mode
        editor.CanUndo() ? "*" : " ",
        editor.GetLanguageDefinition().mName.c_str(),
        fileToEdit);                                  // File Path of the Loaded File

    ImGui::PushFont(font2);
    editor.Render(" TextEditor ");
    ImGui::PopFont();

    ImGui::End();
}
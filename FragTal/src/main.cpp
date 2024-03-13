#include "AppGUI.h"

int main(int argc, char* argv[]) {
	AppGUI myApp;
	myApp.Run();

	return 0;
}

//#include <iostream>

//int main(int argc, char* argv[]) {
//
//	using namespace std::literals::chrono_literals;
//
//	auto start = std::chrono::high_resolution_clock::now();
//
//	while (true) {
//		//std::this_thread::sleep_for(1s);
//		auto end = std::chrono::high_resolution_clock::now();
//		std::chrono::duration<float> duration = (end - start);
//		std::cout << "Elapsed Time: " << duration.count() << " milliseconds\n";
//	}
//	return 0;
//}

// To-Do today:
// 
// -------------Date: 28-12-2023
// 
// 1. Handle all the path releted issues for Visual Studio Config and also release mode exe [Done]
// 2. Make the application performant (Don't need to load the function pointer from for every function call, do it only while the build command is used) [Done]
// 3. Implement the command promt into the ImGui application and catch error from it [Done]
// 4. Work on the memory size issue as it inceased as the viewport size is increased but doesn't decreases when the viewport size decreases [Done]
// 
// -------------Date: 3-01-2024
// 
// 5. Handle the runtime error handling issue to not let the app get locked out [Done]
// 6. The user might change the mainImage function so make sure it's there or show an error [Done]
// 7. Add performance overlay on viewport [Done]
//
// -------------Date: 13-01-2024
//
// 8. Make all the path loading in one single function call []
// 9. Work on making the whole code clean []
//10. Work on the high DPI issue and the SDL_WINDOW_FULLSCREEN issue [Done]
//11. Work on a custom titlebar []
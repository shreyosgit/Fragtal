#include "AppGUI.h"

int main(int argc, char* argv[]) {
	AppGUI myApp;
	myApp.Run();

	return 0;
}

// To-Do today:
// -------------Date: 28-12-2023
// 1. Handle all the path releted issues for Visual Studio Config and also release mode exe [Done]
// 2. Make the application performant (Don't need to load the function pointer from for every function call, do it only while the build command is used) [Done]
// 3. Implement the command promt into the ImGui application and catch error from it [Done]
// 4. Work on the memory size issue as it inceased as the viewport size is increased but doesn't decreases when the viewport size decreases [Done]
// 
// -------------Date: 3-01-2024
// 
// 5. Handle the runtime error handling issue to not let the app get locked out []
// 6. The user might change the mainImage function so make sure it's there or show an error [Kinda Done]
// 7. Add performance overlay on viewport
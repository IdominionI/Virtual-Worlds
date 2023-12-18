// To test example apps etc use --------- 1111111111
// To test and run development app use +++++++++++++

//#include "ofApp.h" // ----- 11111111

//#include <Universal_FW/Im_Node_Editor/Im_node_editor.h> // +++++++++++ 

//#include "VW/test.h"

#include "VW/Editor/Main_Window/main_window.h"

//========================================================================
int main() {
	//ofSetupOpenGL(1024,800,OF_WINDOW);			// <-------- setup the GL context

	// Create main application window

	//ofApp *app = new ofApp(); //----- 11111111

	//ImNe_node_editor_class *app = new ImNe_node_editor_class();// +++++++++++ 
	//test_class *app = new test_class();// +++++++++++ 

	main_window_class* app = new main_window_class();// +++++++++++ 

	ofGLFWWindowSettings settings_main;
	settings_main.setGLVersion(4, 5);
	settings_main.setSize(1400,1200);
	settings_main.setPosition(glm::vec2(100, 100));
	settings_main.resizable = true;
	settings_main.windowMode = ofWindowMode::OF_WINDOW;
	//shared_ptr<ofAppBaseWindow> main_window = ofCreateWindow(settings_main);
	app->main_window = ofCreateWindow(settings_main);

	// Following makes no difference for geometry shaders
	//if (!ofGLCheckExtension("GL_ARB_geometry_shader4") && !ofGLCheckExtension("GL_EXT_geometry_shader4") && !ofIsGLProgrammableRenderer()) {
	//	ofLogFatalError() << "geometry shaders not supported on this graphics card";
	//	return 1;
	//}


	//app->main_window = ofCreateWindow(settings_main).get();

	//app->mainGLFW_ptr = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();

	//app->imgui_init(app->mainGLFW_ptr);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofRunApp(new ofApp());

	ofRunApp(app);

}

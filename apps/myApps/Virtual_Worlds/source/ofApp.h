#pragma once

#include "ofMain.h"

#include <ImGuiEx/ImGui_utils.h>

#include <Universal_FW/Im_Node_editor/ImNe_window.h>
#include <Universal_FW/Im_Node_editor/ImNe_app.h>

class ofApp : public ofBaseApp {

	~ofApp() {
		ImGui_ui_context.ImGui_end();
	}

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// point cloud example test
	ofEasyCam cam;
	ofMesh mesh;
	ofImage img;

	// for multiple appd
	shared_ptr<ofAppBaseWindow> main_window = NULL;
	//ImNe_Window_class ImNe_Window;
	GLFWwindow* ImNe_window_pointer;
	shared_ptr <ImNe_App_class>    ImNe_App;

	// ImGui
	GLFWwindow* mainGLFW_ptr;
	ImGui_ui_context_class ImGui_ui_context;

	// test ImNe_Window_class
	ImNe_Window_class* ImNe_Window;
	ImNe_Window_class* ImNe_Window2, * ImNe_Window3, * ImNe_Window31;
};

#include "ofApp.h"

//#include "Application/app2.h"

//--------------------------------------------------------------
void ofApp::setup() {

	mainGLFW_ptr = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();

	ImGui_ui_context.ImGui_init(mainGLFW_ptr);

	//point cloud test
	// ----------------------------------
	ofSetVerticalSync(true);

	// load an image from disk
	img.load("linzer.png");

	// we're going to load a ton of points into an ofMesh
	mesh.setMode(OF_PRIMITIVE_POINTS);

	// loop through the image in the x and y axes
	int skip = 4; // load a subset of the points
	for (int y = 0; y < img.getHeight(); y += skip) {
		for (int x = 0; x < img.getWidth(); x += skip) {
			ofColor cur = img.getColor(x, y);
			if (cur.a > 0) {
				// the alpha value encodes depth, let's remap it to a good depth range
				float z = ofMap(cur.a, 0, 255, -300, 300);
				cur.a = 255;
				mesh.addColor(cur);
				glm::vec3 pos(x, y, z);
				mesh.addVertex(pos);
			}
		}
	}

	ofEnableDepthTest();
	glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
	glPointSize(3); // make the points bigger

	// ----------------------------------
	// Create a second application window testing
	//ImNe_Window_class ImNe_Window;

	//ImNe_Window_class *ImNe_Window = new ImNe_Window_class;
	ImNe_Window = new ImNe_Window_class;
	if (ImNe_Window == NULL) {
		std::cout << "ImNe_window_pointer == NULL" << std::endl;
		return;
	}
	ImNe_Window->create_new_ImNE_window(main_window);

	std::cout << "ImNe_window_pointer != NULL" << std::endl;
	shared_ptr<ImNe_App_class> guiApp(new ImNe_App_class);
	ImNe_App = guiApp;

	ImNe_App->ImNe_window = ImNe_Window->app_base_window;

	ofRunApp(ImNe_Window->app_base_window, ImNe_App);

	// 2222222222222222222222222222222222222222

	ImNe_Window2 = new ImNe_Window_class;
	if (ImNe_Window2 == NULL) {
		std::cout << "ImNe_window_pointer2 == NULL" << std::endl;
		return;
	}
	ImNe_Window2->create_new_ImNE_window(ImNe_Window->app_base_window, ImNe_Window);

	std::cout << "ImNe_window_pointer2 != NULL" << std::endl;
	shared_ptr<ImNe_App_class> guiApp2(new ImNe_App_class);
	ImNe_App = guiApp2;

	ImNe_App->ImNe_window = ImNe_Window2->app_base_window;
	ImNe_Window2->ImNe_App = ImNe_App;

	ofRunApp(ImNe_Window2->app_base_window, ImNe_App);

	// 3333333333333333333333333333333333333333
	ImNe_Window3 = new ImNe_Window_class;
	if (ImNe_Window3 == NULL) {
		std::cout << "ImNe_window_pointer3 == NULL" << std::endl;
		return;
	}
	ImNe_Window3->create_new_ImNE_window(ImNe_Window->app_base_window, ImNe_Window2);

	std::cout << "ImNe_window_pointer3 != NULL" << std::endl;
	shared_ptr<ImNe_App_class> guiApp3(new ImNe_App_class);
	ImNe_App = guiApp3;

	ImNe_App->ImNe_window = ImNe_Window3->app_base_window;
	ImNe_Window3->ImNe_App = ImNe_App;

	ofRunApp(ImNe_Window3->app_base_window, ImNe_App);


	// 3111111111111111111111111111111
	ImNe_Window31 = new ImNe_Window_class;
	if (ImNe_Window31 == NULL) {
		std::cout << "ImNe_window_pointer31 == NULL" << std::endl;
		return;
	}
	ImNe_Window31->create_new_ImNE_window(ImNe_Window->app_base_window, ImNe_Window3);

	std::cout << "ImNe_window_pointer31 != NULL" << std::endl;
	shared_ptr<ImNe_App_class> guiApp31(new ImNe_App_class);
	ImNe_App = guiApp31;

	ImNe_App->ImNe_window = ImNe_Window31->app_base_window;
	ImNe_Window31->ImNe_App = ImNe_App;

	ofRunApp(ImNe_Window31->app_base_window, ImNe_App);
	/*
		ofGLFWWindowSettings settings;


		settings.setGLVersion(4, 5);
		settings.setSize(1024, 768);
		settings.setPosition(glm::vec2(1000, 500));
		//settings.resizable = true;
		settings.windowMode = ofWindowMode::OF_WINDOW;

		if (main_window != NULL) {
			settings.shareContextWith = main_window;

			shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

			shared_ptr<ofApp2> guiApp(new ofApp2);
			app2 = guiApp;

			app2->guiWindow = guiWindow;

			ofRunApp(guiWindow, app2);
		}
	*/

}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	//point cloud test
	// ----------------------------------
	// !!!! ofBackgroundGradient does not work here for multiple windows!!!!!!
	// !!!! wasted whole afternoon thinking problem originated !!!!
	// !!!! from the opengl window surface not being assigned correctly !!!
	//ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);
	//ofBackgroundGradient(ofColor(64), ofColor(0));

	ofSetBackgroundColor(100, 50, 150);

	// even points can overlap with each other, let's avoid that
	cam.begin();
	ofScale(2, -2, 2); // flip the y axis and zoom in a bit
	ofRotateYDeg(90);
	ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
	mesh.draw();

	//glPointSize(10);
	//glLineWidth(2.5);
	//glColor3f(1.0, 0.0, 0.0);
	//glBegin(GL_LINES);
	//glVertex3f(0.0, 0.0, 0.0);
	//glVertex3f(5.0, 5.0, 0.0);
	//glEnd();

	cam.end();


	glfwMakeContextCurrent(mainGLFW_ptr);
	ImGui_ui_context.ImGui_pre_render();

	// Following uncommented when investigating ImGui widget examples
	ImGui::Begin;
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
	ImGui::End;
	// ----------------------------------

	ImGui_ui_context.ImGui_post_render();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	std::cout << "keyPressed 0001" << std::endl;
	if (ImNe_Window != NULL) ImNe_Window->delete_ImNE_window_children();
	//ImNe_Window2 = NULL;
	std::cout << "keyPressed 0002" << std::endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

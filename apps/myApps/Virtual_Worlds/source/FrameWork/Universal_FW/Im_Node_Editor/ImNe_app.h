#pragma once

#include "ofMain.h"

class ImNe_App_class : public ofBaseApp {

public:
	void setup() {
		ofSetVerticalSync(true);
	}

	void update(){}

	void draw(){
		ofBackgroundGradient(ofColor(64), ofColor(0));
	}

	void exit() {}

	void keyPressed(int key){}
	void keyReleased(int key){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
	void mouseEntered(int x, int y){}
	void mouseExited(int x, int y){}
	void windowResized(int w, int h){}
	void dragEvent(ofDragInfo dragInfo){}
	void gotMessage(ofMessage msg){}

	shared_ptr<ofAppBaseWindow> ImNe_window = NULL;

};
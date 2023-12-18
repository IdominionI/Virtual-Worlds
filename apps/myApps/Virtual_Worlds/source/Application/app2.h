#pragma once

#include "ofMain.h"

class ofApp2 : public ofBaseApp {

public:
	void setup();
	//{
	//	ofSetVerticalSync(true);
	//	mesh.load("data/lofi-bunny.ply");
	//}

	void update();// {}

	void draw();
	//{
	//	ofBackgroundGradient(ofColor(64), ofColor(0));

	//	ofSetColor(255);
	//	cam.begin();

	//	ofSetColor(ofColor::gray);
	//	mesh.drawWireframe();

	//	glPointSize(2);
	//	ofSetColor(ofColor::white);
	//	mesh.drawVertices();
	//	cam.end();

	//	int n = mesh.getNumVertices();
	//	float nearestDistance = 0;
	//	glm::vec2 nearestVertex;
	//	int nearestIndex = 0;
	//	glm::vec3 mouse(mouseX, mouseY, 0);
	//	for (int i = 0; i < n; i++) {
	//		glm::vec3 cur = cam.worldToScreen(mesh.getVertex(i));
	//		float distance = glm::distance(cur, mouse);
	//		if (i == 0 || distance < nearestDistance) {
	//			nearestDistance = distance;
	//			nearestVertex = cur;
	//			nearestIndex = i;
	//		}
	//	}

	//	ofSetColor(ofColor::gray);
	//	ofDrawLine(nearestVertex, mouse);

	//	ofNoFill();
	//	ofSetColor(ofColor::yellow);
	//	ofSetLineWidth(2);
	//	ofDrawCircle(nearestVertex, 4);
	//	ofSetLineWidth(1);

	//	glm::vec2 offset(10, -10);
	//	ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
	//}

	void keyPressed(int key) ;//{}
	void keyReleased(int key);//{}
	void mouseMoved(int x, int y);//{}
	void mouseDragged(int x, int y, int button);//{}
	void mousePressed(int x, int y, int button);//{}
	void mouseReleased(int x, int y, int button);//{}
	void mouseEntered(int x, int y);//{}
	void mouseExited(int x, int y);//{}
	void windowResized(int w, int h);//{}
	void dragEvent(ofDragInfo dragInfo);//{}
	void gotMessage(ofMessage msg);//{}

	// point picker example test
	ofMesh mesh;
	ofEasyCam cam;

	shared_ptr<ofAppBaseWindow> guiWindow = NULL;

};
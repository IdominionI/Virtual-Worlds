#include "app2.h"

void ofApp2::setup() {
	ofSetVerticalSync(true);
	mesh.load("lofi-bunny.ply");
}

void ofApp2::update() {

}

void ofApp2::draw()
{
	//guiWindow->makeCurrent();
	ofBackgroundGradient(ofColor(64), ofColor(0));

	ofSetColor(255);
	cam.begin();

	ofSetColor(ofColor::gray);
	mesh.drawWireframe();

	glPointSize(2);
	ofSetColor(ofColor::white);
	mesh.drawVertices();
	cam.end();

	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	glm::vec2 nearestVertex;
	int nearestIndex = 0;
	glm::vec3 mouse(mouseX, mouseY, 0);
	for (int i = 0; i < n; i++) {
		glm::vec3 cur = cam.worldToScreen(mesh.getVertex(i));
		float distance = glm::distance(cur, mouse);
		if (i == 0 || distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}

	ofSetColor(ofColor::gray);
	ofDrawLine(nearestVertex, mouse);

	ofNoFill();
	ofSetColor(ofColor::yellow);
	ofSetLineWidth(2);
	ofDrawCircle(nearestVertex, 4);
	ofSetLineWidth(1);

	glm::vec2 offset(10, -10);
	ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);

}

//--------------------------------------------------------------
void ofApp2::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp2::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp2::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp2::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp2::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp2::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp2::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp2::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp2::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp2::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp2::dragEvent(ofDragInfo dragInfo) {

}

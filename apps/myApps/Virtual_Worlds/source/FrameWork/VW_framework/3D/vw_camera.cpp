//#include "vw_camera_class.h"
#include "vw_camera.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "ofGraphicsBaseTypes.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/gtx/vector_angle.hpp"
#include <limits>

//#include <glm/geometric.hpp>

// when an vw_camera_class is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
static const float minDifference = 0.1e-5f;

// this is the default on windows os
static const unsigned long doubleclickTime = 200;

//----------------------------------------
vw_camera_class::vw_camera_class() {
	reset();
	sensitivityTranslate = { 1,1,1 };
	sensitivityRot = { 1,1,1 };

	addInteraction(TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT, doTranslationKey);
	addInteraction(TRANSFORM_ROTATE, OF_MOUSE_BUTTON_LEFT);
	addInteraction(TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
	addInteraction(TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_MIDDLE);

}
//----------------------------------------
void vw_camera_class::update(ofEventArgs& args) {
	if (this->viewport.isZero()) {
		viewport = getViewport();
	}
	if (!bDistanceSet && bAutoDistance) {
		setDistance(getImagePlaneDistance(viewport), true);
	}
	if (bMouseInputEnabled && events) {
		if (events->getMousePressed()) {
			updateMouse(glm::vec2(events->getMouseX(), events->getMouseY()));
		}
		if (currentTransformType == TRANSFORM_ROTATE) {
			updateRotation();
		}
		else if (currentTransformType == TRANSFORM_TRANSLATE_XY ||
			currentTransformType == TRANSFORM_TRANSLATE_Z ||
			currentTransformType == TRANSFORM_SCALE) {
			updateTranslation();
		}
	}
}

//----------------------------------------
void vw_camera_class::begin(const ofRectangle& _viewport) {
	if (!bEventsSet) {
		setEvents(ofEvents());
	}
	viewport = _viewport;
	vw_camera_base_class::begin(viewport);
}

//----------------------------------------
void vw_camera_class::reset() {
	target.resetTransform();

	target.setPosition(0, 0, 0);
	lookAt(target);

	resetTransform();
	setPosition(0, 0, lastDistance);

	rot = { 0,0,0 };
	translate = { 0,0,0 };

	if (bAutoDistance) {
		bDistanceSet = false;
	}
	bApplyInertia = false;
	currentTransformType = TRANSFORM_NONE;
}

//----------------------------------------
void vw_camera_class::setTarget(const glm::vec3& targetPoint) {
	target.setPosition(targetPoint);
	lookAt(target);
}

//----------------------------------------
void vw_camera_class::setTarget(ofNode& targetNode) {
	target = targetNode;
	lookAt(target);
}

//----------------------------------------
const ofNode& vw_camera_class::getTarget() const {
	return target;
}

//----------------------------------------
void vw_camera_class::setDistance(float distance) {
	setDistance(distance, true);
}

//----------------------------------------
void vw_camera_class::setDistance(float distance, bool save) {//should this be the distance from the camera to the target?
	if (distance > 0.0f) {
		if (save) {
			this->lastDistance = distance;
		}
		setPosition(target.getPosition() + (distance * getZAxis()));
		bDistanceSet = true;
	}
}

//----------------------------------------
float vw_camera_class::getDistance() const {
	return glm::distance(target.getPosition(), getPosition());
}

//----------------------------------------
void vw_camera_class::setAutoDistance(bool bAutoDistance) {
	this->bAutoDistance = bAutoDistance;
	if (bAutoDistance) {
		bDistanceSet = false;
	}
}

//----------------------------------------
void vw_camera_class::setDrag(float drag) {
	this->drag = drag;
}

//----------------------------------------
float vw_camera_class::getDrag() const {
	return drag;
}

//----------------------------------------
void vw_camera_class::setTranslationKey(char key) {
	doTranslationKey = key;
}

//----------------------------------------
char vw_camera_class::getTranslationKey() const {
	return doTranslationKey;
}

//----------------------------------------
void vw_camera_class::enableMouseInput() {
	if (!bMouseInputEnabled && events) {
		listeners.push(events->update.newListener(this, &vw_camera_class::update));
		listeners.push(events->mousePressed.newListener(this, &vw_camera_class::mousePressed));
		listeners.push(events->mouseReleased.newListener(this, &vw_camera_class::mouseReleased));
		listeners.push(events->mouseScrolled.newListener(this, &vw_camera_class::mouseScrolled));
	}
	// if enableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = true;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}

//----------------------------------------
void vw_camera_class::disableMouseInput() {
	if (bMouseInputEnabled && events) {
		listeners.unsubscribeAll();
	}
	// if disableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = false;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}
//----------------------------------------
bool vw_camera_class::getMouseInputEnabled() const {
	return bMouseInputEnabled;
}

//----------------------------------------
void vw_camera_class::setEvents(ofCoreEvents& _events) {
	// If en/disableMouseInput were called within ofApp::setup(),
	// bMouseInputEnabled will tell us about whether the camera
	// mouse input needs to be initialised as enabled or disabled.
	// we will still set `events`, so that subsequent enabling
	// and disabling can work.

	// we need a temporary copy of bMouseInputEnabled, since it will 
	// get changed by disableMouseInput as a side-effect.
	bool wasMouseInputEnabled = bMouseInputEnabled;// || !events;
	disableMouseInput();
	events = &_events;
	if (wasMouseInputEnabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enableMouseInput();
	}
	bEventsSet = true;
}

//----------------------------------------
void vw_camera_class::setRotationSensitivity(const glm::vec3& sensitivity) {
	sensitivityRot = sensitivity;
}
//----------------------------------------
void vw_camera_class::setRotationSensitivity(float x, float y, float z) {
	setRotationSensitivity({ x,y,z });
}
//----------------------------------------
void vw_camera_class::setTranslationSensitivity(const glm::vec3& sensitivity) {
	sensitivityTranslate = sensitivity;
}
//----------------------------------------
void vw_camera_class::setTranslationSensitivity(float x, float y, float z) {
	sensitivityTranslate = { x,y,z };
}

//----------------------------------------
void vw_camera_class::enableMouseMiddleButton() {
	bEnableMouseMiddleButton = true;
}

//----------------------------------------
void vw_camera_class::disableMouseMiddleButton() {
	bEnableMouseMiddleButton = false;
}

//----------------------------------------
bool vw_camera_class::getMouseMiddleButtonEnabled() const {
	return bEnableMouseMiddleButton;
}

//----------------------------------------
glm::vec3 vw_camera_class::up() const {
	if (bRelativeYAxis) {
		if (bApplyInertia) {
			return getYAxis();
		}
		else {
			return lastPressAxisY;
		}
	}
	else {
		return upAxis;
	}
}

//----------------------------------------
void vw_camera_class::setRelativeYAxis(bool relative) {
	bRelativeYAxis = relative;
}

//----------------------------------------
bool vw_camera_class::getRelativeYAxis() const {
	return bRelativeYAxis;
}

//----------------------------------------
void vw_camera_class::setUpAxis(const glm::vec3& _up) {
	upAxis = _up;
}

//----------------------------------------
const glm::vec3& vw_camera_class::getUpAxis() const {
	return upAxis;
}

//----------------------------------------
void vw_camera_class::enableInertia() {
	doInertia = true;
}

//----------------------------------------
void vw_camera_class::disableInertia() {
	doInertia = false;
}

//----------------------------------------
bool vw_camera_class::getInertiaEnabled() const {
	return doInertia;
}

//----------------------------------------
void vw_camera_class::updateTranslation() {
	if (bApplyInertia) {
		translate *= drag;
		if (std::abs(translate.x) <= minDifference && std::abs(translate.y) <= minDifference && std::abs(translate.z) <= minDifference) {
			translate = { 0,0,0 };
			bApplyInertia = false;
			currentTransformType = TRANSFORM_NONE;

			bIsScrolling = false;
			return;
		}
		move((getXAxis() * translate.x) + (getYAxis() * translate.y) + (getZAxis() * translate.z));
	}
	if (currentTransformType == TRANSFORM_TRANSLATE_XY ||
		currentTransformType == TRANSFORM_TRANSLATE_Z ||
		currentTransformType == TRANSFORM_SCALE) {
		if (getOrtho()) {
			//In ortho mode moving along the z axis has no effect besides clipping.
			// Instead, scale is applied to achieve the effect of getting near or far from the target.
			glm::vec3 mousePre;
			bool bDoScale = (currentTransformType == TRANSFORM_SCALE || currentTransformType == TRANSFORM_TRANSLATE_Z);
			if (bDoScale) {
				mousePre = screenToWorld(glm::vec3((bIsScrolling ? mouseAtScroll : lastPressMouse), 0));
			}
			move(glm::vec3(lastPressAxisX * translate.x) + (lastPressAxisY * translate.y));
			if (bDoScale) {
				setScale(getScale() + translate.z);
				// this move call is to keep the scaling centered below the mouse.
				move(mousePre - screenToWorld(glm::vec3((bIsScrolling ? mouseAtScroll : lastPressMouse), 0)));
			}
		}
		else {
			move(glm::vec3(lastPressAxisX * translate.x) + (lastPressAxisY * translate.y) + (lastPressAxisZ * translate.z));
		}
	}
	if (bIsScrolling) {
		//this it to avoid the transformation to keep on after scrolling ended.
		currentTransformType = TRANSFORM_NONE;
		bIsScrolling = false;
	}
}

//----------------------------------------
void vw_camera_class::updateRotation() {
	if (bApplyInertia) {
		rot *= drag;
		if (std::abs(rot.x) <= minDifference && std::abs(rot.y) <= minDifference && std::abs(rot.z) <= minDifference) {
			rot = { 0,0,0 };
			bApplyInertia = false;
			currentTransformType = TRANSFORM_NONE;
			return;
		}

	}
	if (bApplyInertia) {
		curRot = glm::angleAxis(rot.z, getZAxis()) * glm::angleAxis(rot.y, up()) * glm::angleAxis(rot.x, getXAxis());
		rotateAround(curRot, target.getGlobalPosition());
		rotate(curRot);
	}
	else {
		curRot = glm::angleAxis(rot.z, lastPressAxisZ) * glm::angleAxis(rot.y, up()) * glm::angleAxis(rot.x, lastPressAxisX);
		setOrientation(curRot * lastPressOrientation);
		setPosition(curRot * (lastPressPosition - target.getGlobalPosition()) + target.getGlobalPosition());
	}
}

//----------------------------------------
void vw_camera_class::setControlArea(const ofRectangle& _controlArea) {
	controlArea = _controlArea;
}

//----------------------------------------
void vw_camera_class::clearControlArea() {
	controlArea = ofRectangle();
}

//----------------------------------------
ofRectangle vw_camera_class::getControlArea() const {
	if (controlArea.isZero()) {
		if (viewport.isZero()) {
			return getRenderer()->getCurrentViewport();
		}
		return viewport;
	}
	return controlArea;
}

//----------------------------------------
void vw_camera_class::mousePressed(ofMouseEventArgs& mouse) {
	ofRectangle area = getControlArea();
	if (area.inside(mouse.x, mouse.y)) {
		lastPressMouse = mouse;
		prevMouse = mouse;
		lastPressAxisX = getXAxis();
		lastPressAxisY = getYAxis();
		lastPressAxisZ = getZAxis();
		lastPressPosition = vw_camera_base_class::getGlobalPosition();
		lastPressOrientation = vw_camera_base_class::getGlobalOrientation();

		currentTransformType = TRANSFORM_NONE;
		if (events) {
			for (const auto& i : interactions) {
				if (i.mouseButton == mouse.button && ((i.key == -1) || events->getKeyPressed(i.key))) {
					currentTransformType = i.transformType;
					break;
				}
			}
		}
		if (currentTransformType == TRANSFORM_ROTATE) {
			bInsideArcball = glm::length(mouse - glm::vec2(area.getCenter())) < std::min(area.width / 2, area.height / 2);
		}
		bApplyInertia = false;
	}
}

//----------------------------------------
void vw_camera_class::mouseReleased(ofMouseEventArgs& mouse) {
	ofRectangle area = getControlArea();

	if (area.inside(mouse)) {
		// Check if it's double click
		unsigned long curTap = ofGetElapsedTimeMillis();
		if (lastTap != 0 && curTap - lastTap < doubleclickTime) {
			reset();
			return;
		}
		lastTap = curTap;
	}

	if (doInertia) {
		bApplyInertia = true;
	}
	else {
		currentTransformType = TRANSFORM_NONE;
		rot = { 0,0,0 };
		translate = { 0,0,0 };
	}
}
//----------------------------------------
void vw_camera_class::mouseScrolled(ofMouseEventArgs& mouse) {
	ofRectangle area = getControlArea();
	if (area.inside(mouse)) {
		mouseVel = mouse - prevMouse;
		prevMouse = mouse;
		if (doInertia) {
			bApplyInertia = true;
		}
		lastPressPosition = vw_camera_base_class::getGlobalPosition();
		lastPressAxisZ = getZAxis();
		if (getOrtho()) {
			translate.z = sensitivityScroll * mouse.scrollY / viewport.height;
			mouseAtScroll = mouse;
		}
		else {
			translate.z = mouse.scrollY * 30 * sensitivityTranslate.z * (getDistance() + std::numeric_limits<float>::epsilon()) / area.height;
		}
		currentTransformType = TRANSFORM_SCALE;
		bIsScrolling = true;
	}
}

//----------------------------------------
void vw_camera_class::updateMouse(const glm::vec2& mouse) {
	ofRectangle area = getControlArea();
	int vFlip = (isVFlipped() ? -1 : 1);

	rot = { 0,0,0 };
	translate = { 0,0,0 };
	switch (currentTransformType) {
	case TRANSFORM_ROTATE:
		mouseVel = mouse - lastPressMouse;
		if (bInsideArcball) {
			rot.x = vFlip * -mouseVel.y * sensitivityRot.x * glm::pi<float>() / std::min(area.width, area.height);
			rot.y = -mouseVel.x * sensitivityRot.y * glm::pi<float>() / std::min(area.width, area.height);
		}
		else {
			glm::vec2 center(area.getCenter());
			rot.z = sensitivityRot.z * -vFlip * glm::orientedAngle(glm::normalize(mouse - center),
				glm::normalize(lastPressMouse - center));
		}
		break;
	case TRANSFORM_TRANSLATE_XY:
		mouseVel = mouse - prevMouse;
		if (getOrtho()) {
			translate.x = -mouseVel.x * getScale().z;
			translate.y = vFlip * mouseVel.y * getScale().z;
		}
		else {
			translate.x = -mouseVel.x * sensitivityTranslate.x * 0.5f * (getDistance() + std::numeric_limits<float>::epsilon()) / area.width;
			translate.y = vFlip * mouseVel.y * sensitivityTranslate.y * 0.5f * (getDistance() + std::numeric_limits<float>::epsilon()) / area.height;
		}
		break;
	case TRANSFORM_TRANSLATE_Z:
		mouseVel = mouse - prevMouse;
		if (getOrtho()) {
			translate.z = mouseVel.y * sensitivityScroll / area.height;
		}
		else {
			translate.z = mouseVel.y * (sensitivityTranslate.z * 0.7f) * (getDistance() + std::numeric_limits<float>::epsilon()) / area.height;
		}
		break;
	default:
		break;
	}
	prevMouse = mouse;
}
//----------------------------------------
void vw_camera_class::addInteraction(TransformType type, int mouseButton, int key) {
	if (!hasInteraction(mouseButton, key)) {
		interactions.push_back(interaction(type, mouseButton, key));
	}
	else {
		ofLogNotice("vw_camera_class") << "Can not add interaction. It already exists";
	}
}
//----------------------------------------
void vw_camera_class::removeInteraction(TransformType type, int mouseButton, int key) {
	ofRemove(interactions, [&](interaction& i) { return i.transformType == type && i.mouseButton == mouseButton && i.key == key; });
}
//----------------------------------------
bool vw_camera_class::hasInteraction(int mouseButton, int key) {
	for (const auto& i : interactions) {
		if (i.mouseButton == mouseButton && i.key == key) {
			return true;
		}
	}
	return false;
}
//----------------------------------------
bool vw_camera_class::hasInteraction(TransformType type, int mouseButton, int key) {
	for (const auto& i : interactions) {
		if (i.transformType == type && i.mouseButton == mouseButton && i.key == key) {
			return true;
		}
	}
	return false;
}
//----------------------------------------
void vw_camera_class::removeAllInteractions() {
	interactions.clear();
}
//----------------------------------------
void vw_camera_class::onPositionChanged() {
	if (!bDistanceSet && bAutoDistance) {
		bDistanceSet = true;
	}
}

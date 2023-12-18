#include "vw_camera.h" // +++++++++++++++++++
#include "ofGraphics.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "of3dGraphics.h"

using std::shared_ptr;

#include "ofGLProgrammableRenderer.h" // ++++++++++++++++

//----------------------------------------
vw_camera_base_class::vw_camera_base_class() :
	isOrtho(false),
	fov(60),
	nearClip(0),
	farClip(0),
	lensOffset(0.0f, 0.0f),
	forceAspectRatio(false),
	aspectRatio(4. / 3.),
	vFlip(false)
{
}

//----------------------------------------
vw_camera_base_class::~vw_camera_base_class() {}

//----------------------------------------
void vw_camera_base_class::setFov(float f) {
	fov = f;
	camera_settings.fov = f;
}

//----------------------------------------
void vw_camera_base_class::setNearClip(float f) {
	nearClip = f;
	camera_settings.nearClip = f;
}

//----------------------------------------
void vw_camera_base_class::setFarClip(float f) {
	farClip = f;
	camera_settings.farClip = f;
}

//----------------------------------------
void vw_camera_base_class::setLensOffset(const glm::vec2& lensOffset) {
	this->lensOffset = lensOffset;
	camera_settings.lensOffset = lensOffset;
}

//----------------------------------------
void vw_camera_base_class::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
	setForceAspectRatio(true);
	camera_settings.aspectRatio = aspectRatio;
	camera_settings.forceAspectRatio = true;
}

//----------------------------------------
void vw_camera_base_class::setForceAspectRatio(bool forceAspectRatio) {
	this->forceAspectRatio = forceAspectRatio;
	camera_settings.forceAspectRatio = forceAspectRatio;
}

//----------------------------------------
void vw_camera_base_class::setupPerspective(bool _vFlip, float fov, float nearDist, float farDist, const glm::vec2& lensOffset) {
	ofRectangle orientedViewport = getRenderer()->getNativeViewport();
	float eyeX = orientedViewport.width / 2;
	float eyeY = orientedViewport.height / 2;
	float halfFov = glm::pi<float>() * fov / 360.0f;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;

	if (nearDist == 0) nearDist = dist / 10.0f;
	if (farDist == 0) farDist = dist * 10.0f;

	setFov(fov);
	setNearClip(nearDist);
	setFarClip(farDist);
	setLensOffset(lensOffset);
	setForceAspectRatio(false);

	setPosition(eyeX, eyeY, dist);
	lookAt({ eyeX,eyeY,0.f }, { 0.f,1.f,0.f });
	vFlip = _vFlip;
}

//----------------------------------------
void vw_camera_base_class::setupOffAxisViewPortal(const glm::vec3& topLeft, const glm::vec3& bottomLeft, const glm::vec3& bottomRight) {
	auto bottomEdge = bottomRight - bottomLeft; // plane x axis
	auto leftEdge = topLeft - bottomLeft; // plane y axis
	auto bottomEdgeNorm = glm::normalize(bottomEdge);
	auto leftEdgeNorm = glm::normalize(leftEdge);
	auto bottomLeftToCam = this->getPosition() - bottomLeft;

	auto cameraLookVector = glm::cross(leftEdgeNorm, bottomEdgeNorm);
	auto cameraUpVector = glm::cross(bottomEdgeNorm, cameraLookVector);

	lookAt(cameraLookVector + this->getPosition(), cameraUpVector);

	//lensoffset
	glm::vec2 lensOffset;
	lensOffset.x = -glm::dot(bottomLeftToCam, bottomEdgeNorm) * 2.0f / glm::length(bottomEdge) + 1.0f;
	lensOffset.y = -glm::dot(bottomLeftToCam, leftEdgeNorm) * 2.0f / glm::length(leftEdge) + 1.0f;
	setLensOffset(lensOffset);
	setAspectRatio(glm::length(bottomEdge) / glm::length(leftEdge));
	auto distanceAlongOpticalAxis = fabs(glm::dot(bottomLeftToCam, cameraLookVector));
	setFov(2.0f * glm::degrees(atan((glm::length(leftEdge) / 2.0f) / distanceAlongOpticalAxis)));
}


//----------------------------------------
void vw_camera_base_class::setVFlip(bool vflip) {
	vFlip = vflip;
	camera_settings.vFlip = vflip;
}

//----------------------------------------
bool vw_camera_base_class::isVFlipped() const {
	return vFlip;
}

//----------------------------------------
void vw_camera_base_class::enableOrtho() {
	isOrtho = true;
	camera_settings.isOrtho = true;
}

//----------------------------------------
void vw_camera_base_class::disableOrtho() {
	isOrtho = false;
	camera_settings.isOrtho = false;
}

//----------------------------------------
bool vw_camera_base_class::getOrtho() const {
	return isOrtho;
}

//----------------------------------------
float vw_camera_base_class::getImagePlaneDistance(const ofRectangle& viewport) const {
	return viewport.height / (2.0f * tanf(glm::pi<float>() * fov / 360.0f));
}

//----------------------------------------
void vw_camera_base_class::begin(const ofRectangle& viewport) {
	// VWCUSTOM MMMMMMMMMMMMMMMMMMMMMMMMMMMM
	//ofGetCurrentRenderer()->bind(*this, viewport);// --------------- need to change ofGLProgrammableRenderer to accomadate this class

	ofGLProgrammableRenderer *renderer = dynamic_cast<ofGLProgrammableRenderer *>(ofGetCurrentRenderer().get());
	renderer->pushView();
	renderer->viewport(viewport);
	renderer->setOrientation(renderer->get_matrixStack().getOrientation(), isVFlipped());
	renderer->matrixMode(OF_MATRIX_PROJECTION);
	renderer->loadMatrix(getProjectionMatrix(viewport));
	renderer->matrixMode(OF_MATRIX_MODELVIEW);
	renderer->loadViewMatrix(getModelViewMatrix());
	renderer->set_current_eye_pos(getPosition());

	// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}

// if begin(); pushes first, then we need an end to pop
//----------------------------------------
void vw_camera_base_class::end() {
	// VWCUSTOM MMMMMMMMMMMMMMMMMMMMMMMMMMMM
	//ofGetCurrentRenderer()->unbind(*this); // -------------------- need to change ofGLProgrammableRenderer to accomadate this class 
	
	
	ofGLProgrammableRenderer* renderer = dynamic_cast<ofGLProgrammableRenderer*>(ofGetCurrentRenderer().get());

	renderer->popView();
	// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}

//----------------------------------------
glm::mat4 vw_camera_base_class::getProjectionMatrix(const ofRectangle& viewport) const {
	// autocalculate near/far clip planes if not set by user
	const_cast<vw_camera_base_class*>(this)->calcClipPlanes(viewport);

	if (isOrtho) {
		return glm::translate(glm::mat4(1.0), { -lensOffset.x, -lensOffset.y, 0.f }) * glm::ortho(
			-viewport.width / 2,
			+viewport.width / 2,
			-viewport.height / 2,
			+viewport.height / 2,
			nearClip,
			farClip
		);
	}
	else {
		float aspect = forceAspectRatio ? aspectRatio : viewport.width / viewport.height;
		auto projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
		projection = glm::translate(glm::mat4(1.0), { -lensOffset.x, -lensOffset.y, 0.f }) * projection;
		return projection;
	}
}

//----------------------------------------
glm::mat4 vw_camera_base_class::getModelViewMatrix() const {
	return glm::inverse(getGlobalTransformMatrix());
}

//----------------------------------------
glm::mat4 vw_camera_base_class::getModelViewProjectionMatrix(const ofRectangle& viewport) const {
	return getProjectionMatrix(viewport) * getModelViewMatrix();
}

//----------------------------------------
glm::vec3 vw_camera_base_class::worldToScreen(glm::vec3 WorldXYZ, const ofRectangle& viewport) const {
	auto CameraXYZ = worldToCamera(WorldXYZ, viewport);

	glm::vec3 ScreenXYZ;
	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;
	ScreenXYZ.z = CameraXYZ.z;

	return ScreenXYZ;
}

//----------------------------------------
glm::vec3 vw_camera_base_class::screenToWorld(glm::vec3 ScreenXYZ, const ofRectangle& viewport) const {
	//convert from screen to camera
	glm::vec3 CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f * (ScreenXYZ.y - viewport.y) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;

	return cameraToWorld(CameraXYZ, viewport);
}

//----------------------------------------
glm::vec3 vw_camera_base_class::worldToCamera(glm::vec3 WorldXYZ, const ofRectangle& viewport) const {
	auto MVPmatrix = getModelViewProjectionMatrix(viewport);
	if (vFlip) {
		MVPmatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.f, -1.f, 1.f)) * MVPmatrix;
	}
	auto camera = MVPmatrix * glm::vec4(WorldXYZ, 1.0);
	return glm::vec3(camera) / camera.w;

}

//----------------------------------------
glm::vec3 vw_camera_base_class::cameraToWorld(glm::vec3 CameraXYZ, const ofRectangle& viewport) const {
	auto MVPmatrix = getModelViewProjectionMatrix(viewport);
	if (vFlip) {
		MVPmatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.f, -1.f, 1.f)) * MVPmatrix;
	}
	auto world = glm::inverse(MVPmatrix) * glm::vec4(CameraXYZ, 1.0);
	return glm::vec3(world) / world.w;
}

//----------------------------------------
void vw_camera_base_class::calcClipPlanes(const ofRectangle& viewport) {
	// autocalculate near/far clip planes if not set by user
	if (nearClip == 0 || farClip == 0) {
		float dist = getImagePlaneDistance(viewport);
		nearClip = (nearClip == 0) ? dist / 100.0f : nearClip;
		farClip = (farClip == 0) ? dist * 10.0f : farClip;
	}
}

ofRectangle vw_camera_base_class::getViewport() const {
	return getRenderer()->getCurrentViewport();
}

shared_ptr<ofBaseRenderer> vw_camera_base_class::getRenderer() const {
	if (!renderer) {
		return ofGetCurrentRenderer();
	}
	else {
		return renderer;
	}
}

void vw_camera_base_class::setRenderer(shared_ptr<ofBaseRenderer> _renderer) {
	renderer = _renderer;
}

void vw_camera_base_class::drawFrustum(const ofRectangle& viewport) const {
	ofPushMatrix();

	// In World Space, the camera frustum is the "capped pyramid" which 
	// contains everything which can be seen from a camera. 
	//
	// In Clip Space, the frustum is defined as a box with dimensions: -1, -1, -1 to +1, +1, +1.
	//
	// Much simpler. 
	//
	// We therefore want to draw our frustum as if in Clip Space. 
	// For this, we must find a matrix which will transform Clip Space 
	// back to World Space. 


	// Note: globalTransformMatrix == inverse(modelViewMatrix)
	glm::mat4 clipSpaceToWorld = getGlobalTransformMatrix() * glm::inverse(getProjectionMatrix(viewport));

	// Move into Clip Space - this matrix will transform anything we 
	// draw from Clip Space to World Space
	ofMultMatrix(clipSpaceToWorld);

	// Anything we draw now is transformed from Clip Space back to World Space

	ofPushStyle();
	ofNoFill();
	ofDrawBox(0, 0, 0, 2.f, 2.f, 2.f); // In Clip Space, the frustum is standardised to be a box of dimensions -1,1 in each axis
	ofPopStyle();

	ofPopMatrix();
}

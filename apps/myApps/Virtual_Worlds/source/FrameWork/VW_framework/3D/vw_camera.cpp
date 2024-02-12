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

	addInteraction(TransformType::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT, doTranslationKey);
	addInteraction(TransformType::TRANSFORM_ROTATE, OF_MOUSE_BUTTON_LEFT);
	addInteraction(TransformType::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
	addInteraction(TransformType::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_MIDDLE);


	//Keys for camera movement
	add_key_interaction(ACTION_TYPE_MOVEMENT_FORWARD,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_FORWARD, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_BACKWARD,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_BACKWARD, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_RIGHT,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_RIGHT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_LEFT,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_LEFT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_UP,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_UP, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_DOWN,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_DOWN, NO_KEY_MODIFIER);

	//Keys for camera rotation
	add_key_interaction(ACTION_TYPE_PITCH_UP,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_PITCH_UP, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_PITCH_DOWN,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_PITCH_DOWN, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_YAW_LEFT,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_YAW_LEFT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_YAW_RIGHT,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_YAW_RIGHT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_ROLL_LEFT,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_ROLL_LEFT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_ROLL_RIGHT,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_ROLL_RIGHT, NO_KEY_MODIFIER);

	
	add_key_interaction(ACTION_TYPE_CAMERA_ALIGN_XY_PLANE,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_CAMERA_ALIGN_PLANE, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_CAMERA_ALIGN_XZ_PLANE,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_CAMERA_ALIGN_PLANE, ALT_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_CAMERA_ALIGN_YZ_PLANE,ofKeyEventArgs::Type::Pressed, KEY_INTERACTION_ENABLED, KEY_CAMERA_ALIGN_PLANE, CONTROL_KEY_MODIFIER);
}

//----------------------------------------
// 
// VWCUSTOM ADD +++++++++++++++++++++++++++++++++
	
void vw_camera_class::move_in_global_direction(glm::vec3 direction) { // not tested
	glm::vec3 new_position = getPosition() + direction * movement_multiplier;
	setPosition(new_position);
	assign_last_mouse_translations();
}

void vw_camera_class::move_in_local_direction(glm::vec3 direction) { // not tested
	truck(direction.x * movement_multiplier);
	boom(direction.y * movement_multiplier);
	dolly(direction.z * movement_multiplier);
	assign_last_mouse_translations();
}

void vw_camera_class::move_forward() {
	dolly(-movement_factor);
	assign_last_mouse_translations();
}

void vw_camera_class::move_backward() {
	dolly(movement_factor);
	assign_last_mouse_translations();
}

void vw_camera_class::move_left() {
	truck(-movement_factor);
	assign_last_mouse_translations();
}

void vw_camera_class::move_right(){
	truck(movement_factor);
	assign_last_mouse_translations();
} 

void vw_camera_class::move_up() {
	boom(-movement_factor);
	assign_last_mouse_translations();
}

void vw_camera_class::move_down() {
	boom(movement_factor);
	assign_last_mouse_translations();
}

// ###################### Camera alignment ###########################
void vw_camera_class::align_to_plane(camera_ailgnment_type_enum camera_ailgnment) { // not tested
	glm::vec3 alignment_vector = { 0.0f,0.0f,0.0f };
	
	glm::vec3 mPosition = getPosition();
	glm::vec3 mlook_at  = glm::normalize(getLookAtDir());

	float look_at_distance = glm::length(mlook_at)*10.0f;

	if (look_at_distance == 0.0f) {
		std::cout << "Cannot align to requested plane as camera is looking at itself in its own location.\n";
		return;
	}

//std::cout << "vw_camera_class::align_to_plane 0000 : " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;

	mlook_at += mPosition;

//std::cout << "vw_camera_class::align_to_plane 1111: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;

	switch (camera_ailgnment) {
		case camera_ailgnment_type_enum::yz_plane: alignment_vector = { mPosition.x,mlook_at.y,mlook_at.z}; break;
		case camera_ailgnment_type_enum::xz_plane: alignment_vector = { mlook_at.x,mPosition.y,mlook_at.z }; break;
		case camera_ailgnment_type_enum::xy_plane: alignment_vector = { mlook_at.x,mlook_at.y,mPosition.z }; break;
	}

//std::cout << "vw_camera_class::align_to_plane 2222: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<alignment_vector.x <<":"<< alignment_vector.y<<":"<<alignment_vector.z<<":" << std::endl;

	lookAt(alignment_vector);

	mlook_at  = getLookAtDir();
//std::cout << "vw_camera_class::align_to_plane 3333: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;


	assign_last_mouse_translations();
}

	
	
	
	
	
// END VWCUSTOM ADD +++++++++++++++++++++++++++++++++

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
		if (currentTransformType == TransformType::TRANSFORM_ROTATE) {
			updateRotation();
		}
		else if (currentTransformType == TransformType::TRANSFORM_TRANSLATE_XY ||
			currentTransformType == TransformType::TRANSFORM_TRANSLATE_Z ||
			currentTransformType == TransformType::TRANSFORM_SCALE) {
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
	currentTransformType = TransformType::TRANSFORM_NONE;
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

// VWCUSTOM ADD ++++++++++++
void vw_camera_class::enable_keyboard_input(){
	if (!keyboard_input_enabled && events) {
		key_listeners.push(events->keyPressed.newListener(this, &vw_camera_class::key_pressed));
		key_listeners.push(events->keyReleased.newListener(this, &vw_camera_class::key_released));
	}

	keyboard_input_enabled = true;
}

void vw_camera_class::disable_keyboard_input(){
	if (keyboard_input_enabled && events) {
		key_listeners.unsubscribeAll();
	}

	keyboard_input_enabled = false;
}


void vw_camera_class::key_pressed(ofKeyEventArgs &keyEvent) {
//std::cout <<" vw_camera_class::keyPressed : 000 : " << id << "  :  " << getPosition().x << " : " << getPosition().y << " : " << getPosition().z << std::endl;
	if (!is_key_interaction_enabled(keyEvent.type, keyEvent.keycode, keyEvent.modifiers)) return;
	int interaction_id = get_key_interaction_id(keyEvent.type, keyEvent.keycode, keyEvent.modifiers);
	perform_key_pressed_action(interaction_id);
}

void vw_camera_class::key_released(ofKeyEventArgs &keyEvent) {
//std::cout <<" vw_camera_class::keyReleased : 000 : " << id << "  :  " << getPosition().x << " : " << getPosition().y << " : " << getPosition().z << std::endl;
	if (!is_key_interaction_enabled(keyEvent.type, keyEvent.keycode, keyEvent.modifiers)) return;
	int interaction_id = get_key_interaction_id(keyEvent.type, keyEvent.keycode, keyEvent.modifiers);
	perform_key_released_action(interaction_id);
}

void vw_camera_class::perform_key_pressed_action(int interaction_id) {
//char c = keyEvent.key;
//std::cout <<" vw_camera_class::perform_key_action  : " << c << " ::: " << keyEvent.key << "  :  " << keyEvent.keycode << " : " << keyEvent.scancode << " : " << keyEvent.codepoint << std::endl;

	//switch (keyEvent.keycode) {
	switch (interaction_id) {
	//switch (keyEvent.key) {
		////case ACTION_TYPE_EXIT_VIEWER: save_viewer_state(); close(); exit(0); break;
		////case ACTION_TYPE_TOGGLE_FIRST_PERSON: toggle_first_person_mode();            break;

		// movement this needs to be decided what are the plane space
		case ACTION_TYPE_MOVEMENT_FORWARD:  if (!(movement == camera_movement_mode_enum::xz_plane)) move_forward();  break;
		case ACTION_TYPE_MOVEMENT_BACKWARD: if (!(movement == camera_movement_mode_enum::xz_plane)) move_backward(); break;
		case ACTION_TYPE_MOVEMENT_RIGHT:    if (!(movement == camera_movement_mode_enum::yz_plane)) move_right();    break;
		case ACTION_TYPE_MOVEMENT_LEFT:     if (!(movement == camera_movement_mode_enum::yz_plane)) move_left();     break;
		case ACTION_TYPE_MOVEMENT_UP:       if (!(movement == camera_movement_mode_enum::xy_plane)) move_up();       break;
		case ACTION_TYPE_MOVEMENT_DOWN:     if (!(movement == camera_movement_mode_enum::xy_plane)) move_down();     break;

		case ACTION_TYPE_PITCH_UP:          if (pitch_rotation) tiltDeg(0.5);  break;
		case ACTION_TYPE_PITCH_DOWN:	    if (pitch_rotation) tiltDeg(-0.5); break;
		case ACTION_TYPE_YAW_LEFT:          if (yaw_rotation) panDeg(0.5);     break;
		case ACTION_TYPE_YAW_RIGHT:		    if (yaw_rotation) panDeg(-0.5);    break;
		case ACTION_TYPE_ROLL_LEFT:         if (roll_rotation) roll(0.5);      break;
		case ACTION_TYPE_ROLL_RIGHT:		if (roll_rotation) roll(-0.5);     break;

		// interface tools ::: TO DO ???? 
		////case ACTION_TYPE_DRAW_AXIS:           toggle_display_axis();        break;
		////case ACTION_TYPE_DRAW_GRID:           toggle_display_grid();	    break;
		////case ACTION_TYPE_DRAW_XY_GRID:        toggle_display_xy_grid();     break;
		////case ACTION_TYPE_DRAW_XZ_GRID:        toggle_display_xz_grid();     break;
		////case ACTION_TYPE_DRAW_YZ_GRID:        toggle_display_yz_grid();     break;
		////case ACTION_TYPE_DISPLAY_FPS:         toggle_FPS_display();	        break;
		////case ACTION_TYPE_DISPLAY_CAMERA_INFO: toggle_camera_info_display(); break;
		////case ACTION_TYPE_DISPLAY_CROSS_HAIRS: toggle_cross_hairs_display(); break;
		////case ACTION_TYPE_ENABLE_TEXT:         toggle_enable_all_text();     break;
		////case ACTION_TYPE_SAVE_SCREENSHOT:     save_snapShot();              break;

			// Camera alignment
		case ACTION_TYPE_CAMERA_ALIGN_XY_PLANE: align_to_plane(camera_ailgnment_type_enum::xy_plane); break;
		case ACTION_TYPE_CAMERA_ALIGN_XZ_PLANE: align_to_plane(camera_ailgnment_type_enum::xz_plane); break;
		case ACTION_TYPE_CAMERA_ALIGN_YZ_PLANE: align_to_plane(camera_ailgnment_type_enum::yz_plane); break;

		//Add more keyboard-mouse interactions here
	}
}


void vw_camera_class::perform_key_released_action(int interaction_id) {

}
//END VWCUSTOM ADD+++++++++++


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

	// VWCUSTOM ADD ++++++++++++
	bool was_key_board_input_enabled = keyboard_input_enabled;// || !events;

	disable_keyboard_input();
	if (was_key_board_input_enabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enable_keyboard_input();
	}
	//END VWCUSTOM ADD+++++++++++

	bEventsSet = true;
}

//----------------------------------------
void vw_camera_class::setRotationSensitivity(const glm::vec3& sensitivity) {
	//sensitivityRot = sensitivity;
	sensitivityRot = sensitivity*rotation_speed;// VWCUSTOM Modify MMMMM
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
			currentTransformType = TransformType::TRANSFORM_NONE;

			bIsScrolling = false;
			return;
		}
		move((getXAxis() * translate.x) + (getYAxis() * translate.y) + (getZAxis() * translate.z));
	}
	if (currentTransformType == TransformType::TRANSFORM_TRANSLATE_XY ||
		currentTransformType == TransformType::TRANSFORM_TRANSLATE_Z ||
		currentTransformType == TransformType::TRANSFORM_SCALE) {
		if (getOrtho()) {
			//In ortho mode moving along the z axis has no effect besides clipping.
			// Instead, scale is applied to achieve the effect of getting near or far from the target.
			glm::vec3 mousePre;
			bool bDoScale = (currentTransformType == TransformType::TRANSFORM_SCALE || currentTransformType == TransformType::TRANSFORM_TRANSLATE_Z);
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
		currentTransformType = TransformType::TRANSFORM_NONE;
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
			currentTransformType = TransformType::TRANSFORM_NONE;
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
//std::cout <<" vw_camera_class::mousePressed :000 : " << id << "  :  " << getPosition().x << " : " << getPosition().y << " : " << getPosition().z << std::endl;

	ofRectangle area = getControlArea();
	if (area.inside(mouse.x, mouse.y)) {
		lastPressMouse = mouse;
		prevMouse = mouse;
		lastPressAxisX = getXAxis();
		lastPressAxisY = getYAxis();
		lastPressAxisZ = getZAxis();
		lastPressPosition = getGlobalPosition();
		lastPressOrientation = getGlobalOrientation();

		currentTransformType = TransformType::TRANSFORM_NONE;
		if (events) {
			for (const auto& i : interactions) {
				if (i.mouseButton == mouse.button && ((i.key == -1) || events->getKeyPressed(i.key))) {
					currentTransformType = i.transformType;
					break;
				}
			}
		}
		if (currentTransformType == TransformType::TRANSFORM_ROTATE) {
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
		currentTransformType = TransformType::TRANSFORM_NONE;
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
		currentTransformType = TransformType::TRANSFORM_SCALE;
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
	case TransformType::TRANSFORM_ROTATE:
		mouseVel = mouse - lastPressMouse;
		if (bInsideArcball) {
			//rot.x = vFlip * -mouseVel.y * sensitivityRot.x * glm::pi<float>() / std::min(area.width, area.height);
			rot.x = vFlip * -mouseVel.y * sensitivityRot.x * glm::pi<float>() * rotation_speed/ std::min(area.width, area.height);// VWCUSTOM Modify MMMMM
			//rot.y = -mouseVel.x * sensitivityRot.y * glm::pi<float>() / std::min(area.width, area.height);
			rot.y = -mouseVel.x * sensitivityRot.y * glm::pi<float>() * rotation_speed/ std::min(area.width, area.height);// VWCUSTOM Modify MMMMM
		}
		else {
			glm::vec2 center(area.getCenter());
			rot.z = sensitivityRot.z *rotation_speed* -vFlip * glm::orientedAngle(glm::normalize(mouse - center),glm::normalize(lastPressMouse - center));// VWCUSTOM Modify MMMMM
			//rot.z = sensitivityRot.z * -vFlip * glm::orientedAngle(glm::normalize(mouse - center),glm::normalize(lastPressMouse - center));
		}
		break;
	case TransformType::TRANSFORM_TRANSLATE_XY:
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
	case TransformType::TRANSFORM_TRANSLATE_Z:
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

// VWCUSTOM ADD ++++++++++++++++++++++++++++
	//----------------------------------------
void vw_camera_class::add_key_interaction(int interaction_id,ofKeyEventArgs::Type key_event_type,bool enabled, int keycode, int modifier ) {
	if (!has_key_interaction(key_event_type,keycode,modifier)) {
		key_interaction_struct_type key_interaction;
		key_interaction.type           = key_event_type;
		key_interaction.enabled        = enabled;
		key_interaction.keycode        = keycode;
		key_interaction.modifiers      = modifier;
		key_interaction.interaction_id = interaction_id;

		key_interactions.push_back(key_interaction);
	}
	else {
		ofLogNotice("vw_camera_class") << "Can not add interaction. It already exists";
	}
}
//----------------------------------------
void vw_camera_class::remove_key_interaction(ofKeyEventArgs::Type key_event_type, int keycode, int modifier) {
	for (int i = 0; i < key_interactions.size() ;i++) {
		if (key_interactions[i].type == key_event_type && key_interactions[i].keycode == keycode && key_interactions[i].modifiers == modifier) {
			key_interactions.erase(key_interactions.begin() + i);
		}
	}
}

//----------------------------------------
bool vw_camera_class::has_key_interaction(ofKeyEventArgs::Type key_event_type, int keycode, int modifier) {
	for (key_interaction_struct_type &key_interaction : key_interactions) {
			if (key_interaction.type == key_event_type && key_interaction.keycode == keycode && key_interaction.modifiers == modifier)  {
			return true;
		}
	}
	return false;
}

bool vw_camera_class::is_key_interaction_enabled(ofKeyEventArgs::Type key_event_type, int keycode, int modifier) {
	for (key_interaction_struct_type &key_interaction : key_interactions) {
		if (key_interaction.type == key_event_type && key_interaction.keycode == keycode && key_interaction.modifiers == modifier && key_interaction.enabled )  {
			return true;
		}
	}
	return false;
}

void vw_camera_class::enable_key_interaction(ofKeyEventArgs::Type key_event_type, int keycode, int modifier) {
	for (key_interaction_struct_type &key_interaction : key_interactions) {
		if (key_interaction.type == key_event_type && key_interaction.keycode == keycode && key_interaction.modifiers == modifier)  {
			key_interaction.enabled = true;
		}
	}
}

void vw_camera_class::disable_key_interaction(ofKeyEventArgs::Type key_event_type, int keycode, int modifier) {
	for (key_interaction_struct_type &key_interaction : key_interactions) {
		if (key_interaction.type == key_event_type && key_interaction.keycode == keycode && key_interaction.modifiers == modifier)  {
			key_interaction.enabled = false;
		}
	}
}

int vw_camera_class::get_key_interaction_id(ofKeyEventArgs::Type key_event_type, int keycode, int modifier) {
	for (key_interaction_struct_type &key_interaction : key_interactions) {
		if (key_interaction.type == key_event_type && key_interaction.keycode == keycode && key_interaction.modifiers == modifier)  {
			return key_interaction.interaction_id;
		}
	}
	return -1;
}

//----------------------------------------
void vw_camera_class::remove_all_key_interactions() {
	key_interactions.clear();
}

void vw_camera_class::remove_all_press_key_interactions() {
	for (int i = key_interactions.size()-1; i > -1 ;i--) {
		if (key_interactions[i].type == ofKeyEventArgs::Type::Pressed) {
			key_interactions.erase(key_interactions.begin() + i);
		}
	}
}

void vw_camera_class::remove_all_release_key_interactions() {
	for (int i = key_interactions.size()-1; i > -1 ;i--) {
		if (key_interactions[i].type == ofKeyEventArgs::Type::Released) {
			key_interactions.erase(key_interactions.begin() + i);
		}
	}
}
	//----------------------------------------

// END VWCUSTOM ADD ++++++++++++++++++++++++++++


//----------------------------------------
void vw_camera_class::onPositionChanged() {
	if (!bDistanceSet && bAutoDistance) {
		bDistanceSet = true;
	}
}

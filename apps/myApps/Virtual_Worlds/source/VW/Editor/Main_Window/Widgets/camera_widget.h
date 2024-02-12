#pragma once

#include <ImGuiEx/imgui_widgets.h>

//#include <Source/Graphics_Engine/Scene/Scene_objects/camera_object.h>
#include <VW_Framework/3D/vw_camera.h>

/*
					CAMERA PROPOERTIES WIDGET
	
	A widget class that displays editable camera parameters or changes
	the camera behavior such as to define the user interaction with the
	scene camera or how the scene is depicted on the computer screen. 
*/


class camera_poperties_widget_class {
public:
	camera_poperties_widget_class() {}
	~camera_poperties_widget_class() {}

	vw_camera_class *camera = nullptr;

	bool display() {
		if (camera == NULL) {
std::cout <<"camera_poperties_widget_class::display : Selected Camera == NULL\n";
			return false;
		}

//std::cout <<"camera_poperties_widget_class::display : Selected Camera != NULL\n";

		float x_pos = 10.0f, y_pos = 50.0f;

		const char *camera_movement_items[] = {"None", "XY-Plane", "XZ-Plane", "YZ-Plane"};// This cannot be defined outside this function	

		//title("Viewer Camera");

		y_pos += 25;
		text("Translation       Orientation", x_pos + 35, y_pos);

		x_pos = 20; y_pos += 20;

		text("X", x_pos, y_pos);
		float_input("###cxt", translation.x, x_pos + 30, y_pos, 75.0f);
		text("Pitch", x_pos+120, y_pos);
		float_input("###cop",pitch, x_pos + 170.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y", x_pos , y_pos);
		float_input("###cyt", translation.y, x_pos + 30, y_pos, 75.0f);
		text("Yaw", x_pos + 120, y_pos);
		float_input("###coy", yaw, x_pos + 170.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Z", x_pos, y_pos);
		float_input("###czt", translation.z, x_pos + 30, y_pos, 75.0f);
		text("Roll", x_pos + 120, y_pos);
		float_input("###cor", roll, x_pos + 170.0f, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Translate###ctb", x_pos + 27, y_pos, 80, 20)) {
//std::cout <<"camera_poperties_widget_class::display :000 : " << camera->id << "  :  "<< camera->getPosition().x << " : " << camera->getPosition().y << " : "<< camera->getPosition().z << std::endl;
			camera->setPosition(translation);
			camera->assign_last_mouse_translations();// Need this so does not revert to last location using mouse interaction
//std::cout <<"camera_poperties_widget_class::display :111 : " << camera->id << "  :  "<< camera->getPosition().x << " : " << camera->getPosition().y << " : "<< camera->getPosition().z << std::endl;
		}

		if (ex_button("Orientate###cob", x_pos + 167, y_pos, 80, 20)) {
			camera->setOrientation({ pitch, yaw, roll });
			camera->assign_last_mouse_translations();// Need this so does not revert to last location using mouse interaction
		}

		y_pos += 25;
		if (ex_button("Relative###ctrb", x_pos + 27, y_pos, 80, 20)) {
//std::cout <<"camera_poperties_widget_class::display :2222 : " << camera->id << "  :  "<< camera->getPosition().x << " : " << camera->getPosition().y << " : "<< camera->getPosition().z << std::endl;
			camera->move(translation);
			camera->assign_last_mouse_translations();// Need this so does not revert to last location using mouse interaction
//std::cout <<"camera_poperties_widget_class::display :3333 : " << camera->id << "  :  "<< camera->getPosition().x << " : " << camera->getPosition().y << " : "<< camera->getPosition().z << std::endl;
		}

		if (ex_button("Relativee###corb", x_pos + 167, y_pos, 80, 20)) {
			glm::vec3 c_orientation = camera->getOrientationEulerDeg();
			camera->setOrientation({ pitch + c_orientation.x, c_orientation.y + yaw,c_orientation.z + roll });
			camera->assign_last_mouse_translations();// Need this so does not revert to last location using mouse interaction
		}

		y_pos += 35;
		x_pos = 10.0f;
		text("Set Camera Alignment", x_pos + 75, y_pos);
		y_pos += 20;
		x_pos = 5;
		if (ex_button("XY-Plane###caxy", x_pos + 27, y_pos, 80, 20)) {
			camera->align_to_plane(camera_ailgnment_type_enum::xy_plane);
		}

		if (ex_button("XZ-Plane###caxz", x_pos + 110, y_pos, 80, 20)) {
			camera->align_to_plane(camera_ailgnment_type_enum::xz_plane);
		}

		if (ex_button("YZ-Plane###cayz", x_pos + 193, y_pos, 80, 20)) {
			camera->align_to_plane(camera_ailgnment_type_enum::yz_plane);
		}

		y_pos += 30;
		text("Camera Movement\n  Restriction", x_pos + 100, y_pos);
		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(75);
		if (ImGui::Combo("###cmr", &camera_restrictions, &Funcs::ItemGetter, camera_movement_items, IM_ARRAYSIZE(camera_movement_items))) {
			switch (camera_restrictions) {
				case 0: camera->movement = camera_movement_mode_enum::free;     break;
				case 1: camera->movement = camera_movement_mode_enum::xy_plane; break;
				case 2: camera->movement = camera_movement_mode_enum::xz_plane; break;
				case 3: camera->movement = camera_movement_mode_enum::yz_plane; break;
			}
		}

		y_pos += 30;
		text("Camera Rotation", x_pos + 90, y_pos);
		y_pos += 20;

		ImGui::SetCursorPosX(x_pos + 50);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Pitch###crp", &camera->pitch_rotation);
		ImGui::SetCursorPosX(x_pos + 130);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Yaw###cry", &camera->yaw_rotation);
		ImGui::SetCursorPosX(x_pos + 210);
		ImGui::SetCursorPosY(y_pos);
		ImGui::Checkbox("Roll###crr", &camera->roll_rotation);

		x_pos = 40; y_pos += 30;
		text("Movement Factor", x_pos, y_pos+3);
		float_input("###cmf", camera->movement_factor, x_pos + 150, y_pos, 50.0f);
		x_pos = 40; y_pos += 30;
		text("Mouse Sensitivity", x_pos, y_pos+3);
		float_input("###cms", camera->rotation_speed, x_pos + 150, y_pos, 50.0f);// need to change this to a slider with min 0.05 to 10.0
		x_pos = 40; y_pos += 30;
		text("Mouse Scroll", x_pos, y_pos+3);
		float_input("###cmsc", camera->sensitivityScroll, x_pos + 150, y_pos, 50.0f);// need to change this to a slider with min 0.05 to 10.0

		y_pos += 25;
		text("Look At Location", x_pos + 45, y_pos);

		x_pos = 95; y_pos += 20;

		text("X", x_pos, y_pos);
		float_input("###cllx", camera->look_at_location.x, x_pos + 30, y_pos, 75.0f);

		y_pos += 20;
		text("Y", x_pos, y_pos);
		float_input("###clly", camera->look_at_location.y, x_pos + 30, y_pos, 75.0f);

		y_pos += 20;
		text("Z", x_pos, y_pos);
		float_input("###cllz", camera->look_at_location.z, x_pos + 30, y_pos, 75.0f);

		y_pos += 25;
		x_pos = 5;
		if (ex_button("Look At###clab", x_pos + 120, y_pos, 75, 20)) {
			camera->lookAt(look_at_location);
			camera->assign_last_mouse_translations();
		}

		x_pos = 5;
		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("Orbit\nRotation###cro", &camera->orbital_rotation)) {
			camera->assign_last_mouse_translations();
		}
			
		y_pos += 50;
		text("field of view", x_pos, y_pos);
		ImGui::SetCursorPosX(x_pos+120);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("###fov", &camera->fov, 1.0f, 175.0f, "%.2f")) {
			camera->camera_settings.farClip = camera->fov;
		}

		y_pos += 30;
		text("Aspect Ratio", x_pos, y_pos);
		ImGui::SetCursorPosX(x_pos+120);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("###ar", &camera->aspectRatio, 1.0f, 20.0f, "%.2f")) {
			camera->setAspectRatio(camera->aspectRatio);
		}

		y_pos += 30;
		text("Near clip", x_pos, y_pos);
		ImGui::SetCursorPosX(x_pos+120);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("###nc", &camera->nearClip, 0.0001f, 10000.0f, "%.2f")) {
			camera->camera_settings.nearClip = camera->nearClip;
		}

		y_pos += 30;
		text("Far clip", x_pos, y_pos);
		ImGui::SetCursorPosX(x_pos+120);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("###fc", &camera->farClip, 1.0f, 100000.0f, "%.2f")) {
			camera->camera_settings.farClip = camera->farClip;
		}

		x_pos = 5;
		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("Orthographic View ###cov", &otho_view)) {
			bool camera_ortho_view = camera->getOrtho();
			if (camera_ortho_view != otho_view) {
				if (otho_view)
					camera->enableOrtho();
				else
					camera->disableOrtho();
			}
		}

		return true;
	}


private:
	glm::vec3 translation      = { 0.0f,0.0f,0.0f };
	glm::vec3 look_at_location = { 0.0f,0.0f,0.0f };
	float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
	int   camera_restrictions = 0;
	//float fov, nearclip, farclip, aspect_ratio;
	bool  otho_view;


	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
	void define_camera_movement_restrictions() {

	}

};
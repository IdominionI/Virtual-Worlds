#pragma once
#include <string>
#include <vector>

#include <VW_framework/3D/vw_camera.h>

#include "../App/global.h" // Need to check that indeed unique ids are generated and stored

#include "../Widgets/parameter_widget_base.h"

inline id_key_manager_class<idu_type> camera_id_key;// This needed as global so all catagories in all editors have a unique category id

class scene_cameras_manager_class {
public:
	scene_cameras_manager_class() {
		selected_camera = add_camera();// Need at least one camera in the scene to be able to view things
		selected_camera->display_active = true;
	}

	~scene_cameras_manager_class() {
		for (size_t i = cameras.size()-1; i > -1; i--) {
			remove_camera(cameras[i]->id);
		}

		cameras.clear();
		selected_camera = NULL;
	}

	std::vector<vw_camera_class *>  cameras;
	vw_camera_class *selected_camera = NULL;

	// Camera data managemement;

	vw_camera_class *add_camera(bool new_camera_selected = true) {
		vw_camera_class *new_camera = new vw_camera_class;
		if (new_camera != NULL) {
			new_camera->id = camera_id_key.get_available_id_key();
			cameras.push_back(new_camera);
			if (new_camera_selected) {
				selected_camera = new_camera;
				selected_camera->display_active = true;
			}
		}
		return new_camera;
	}

	vw_camera_class *add_camera(vw_camera_settings_struct_type camera_settings,bool new_camera_selected = true) {
		vw_camera_class *new_camera = add_camera(new_camera_selected);
		if(new_camera != NULL) new_camera->define_camera_settings(camera_settings);
		return new_camera;
	}

	bool remove_camera(vw_camera_class *camera_to_remove) {
		if (camera_to_remove != NULL) {
			return remove_camera(camera_to_remove->id);
		}
		return false;
	}

	bool remove_camera(id_type camera_id) {
		if (cameras.size() < 2) return false; // Need at least one camera in the scene to be able to view things
		
		int index = get_camera_index(camera_id);
		if (index > -1) {
			delete cameras[index];
			cameras.erase(cameras.begin() + index);
			camera_id_key.assign_free_id_key(camera_id);// Free entity ID number to be reused when a new entity is created
			return true;
		}
		return false;
	}

	vw_camera_class *get_camera(id_type camera_id) {
		for (vw_camera_class *camera : cameras) {
			if (camera->id == camera_id) return camera;
		}
		return NULL;
	}

	bool enable_camera(id_type camera_id, bool enable = true) {
		vw_camera_class *camera = get_camera(camera_id);
		if (camera != NULL) {
			camera->enabled = enable;
			return true;
		}
		return false;
	}

	bool select_camera(id_type camera_id) {
		vw_camera_class *camera = get_camera(camera_id);
		if (camera != NULL) {
			//selected_camera->display_active = false;
			selected_camera = camera;
			//selected_camera->display_active = true;
			return true;
		}
		return false;
	}

	// !!!!!!!!!!!!!!!! Scene cameras management !!!!!!!!!!!!!!!!!!!!!!!!!

	// Much more needs to be done here
	void scene_cameras_render_setup() {
		for (vw_camera_class* camera : cameras) {
			if (camera->display_active) {
				camera->begin();
			}
		}
	}

	void disable_camera_mouse_input() {
		for (vw_camera_class* camera : cameras) {
			if (camera->display_active) {
				camera->disableMouseInput();
			}
		}
	}

	void enable_camera_mouse_input() {
		for (vw_camera_class* camera : cameras) {
			if (camera->display_active) {
				camera->enableMouseInput();
			}
		}
	}

	void disable_camera_keyboard_input() {
		for (vw_camera_class* camera : cameras) {
			if (camera->display_active) {
				camera->disable_keyboard_input();
			}
		}
	}

	void enable_camera_keyboard_input() {
		for (vw_camera_class* camera : cameras) {
			if (camera->display_active) {
				camera->enable_keyboard_input();
			}
		}
	}

	// Function to display cameras overlays on screen
	void scene_cameras_render() {
		for (vw_camera_class* camera : cameras) {
			if (camera->display_overlay) {
				camera->drawFrustum();
			}
		}
	}

	void scene_cameras_render_exit() {
		for (vw_camera_class* camera : cameras) {
			if (camera->display_active) {
				camera->end();
			}
		}
	}

	void display_paramater_data_widget(vw_camera_class *camera) {
		ImGui::SetCursorPosX(100);
		ImGui::SetCursorPosY(100);
		ImGui::Text("Scene Camera Parameters");
	};//To DO

	parameter_widget_base_class   parameter_widget;
	// node_editor_base_class          node_widget;


protected:

private:
	int get_camera_index(id_type camera_id) {
		for (size_t i=0; i < cameras.size(); i++) {
			if (cameras[i]->id == camera_id) return i;
		}
		return -1;
	}



};
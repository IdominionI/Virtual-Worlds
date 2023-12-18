#pragma once

#include <string>
#include <vector>

//#include <VW_framework/3D/vw_light.h>
#include <ofLight.h>

#include "../App/global.h" // Need to check that indeed unique ids are generated and stored

#include "../Widgets/parameter_widget_base.h"

class scene_lights_manager_class {
public:
	// Strangely this constructor causes app to crash !!!!!!
	// No idea why 
	scene_lights_manager_class() {
		//selected_light = add_light();
		//selected_light->light_is_on = false;
	}

	~scene_lights_manager_class() {
		for (size_t i = lights.size()-1; i > -1; i--) {
			remove_light(lights[i]->id);
		}

		lights.clear();
		selected_light = NULL;
	}

	std::vector<ofLight*>  lights;
	ofLight *selected_light = NULL;

	// Lights data management

	ofLight* add_light(bool new_light_selected = true) {
		ofLight *new_light = new ofLight;
		if (new_light != nullptr) {
			new_light->id = globalc::get_available_entity_id();
			new_light->light_is_on = true;
			lights.push_back(new_light);
			if (new_light_selected) selected_light = new_light;
		}
		return new_light;
	}

	ofLight* add_light(ofLight::Data light_settings, bool new_light_selected = true) {
		ofLight* new_light = add_light(new_light_selected);
		if (new_light != NULL) new_light->define_light_settings(light_settings);
		return new_light;
	}

	bool remove_light(ofLight* light_to_remove) {
		if (light_to_remove != NULL) {
			return remove_light(light_to_remove->id);
		}
		return false;
	}

	bool remove_light(id_type light_id) {
		if (lights.size() < 2) return false; // Need at least one light in the scene to be able to view things

		int index = get_light_index(light_id);
		if (index > -1) {
			delete lights[index];
			lights.erase(lights.begin() + index);
			globalc::assign_free_entity_id(light_id);// Free entity ID number to be reused when a new entity is created
			return true;
		}
		return false;
	}

	ofLight* get_light(id_type light_id) {
		for (ofLight* light : lights) {
			if (light->id == light_id) return light;
		}
		return NULL;
	}

	bool enable_light(id_type light_id, bool enable = true) {
		ofLight* light = get_light(light_id);
		if (light != NULL) {
			light->enabled = enable;
			return true;
		}
		return false;
	}

	bool select_light(id_type light_id) {
		ofLight* light = get_light(light_id);
		if (light != NULL) {
			selected_light = light;
			return true;
		}
		return false;
	}

	// !!!!!!!!!!!!!!!! Scene lights management !!!!!!!!!!!!!!!!!!!!!!!!!

	// Much more needs to be done here
	void scene_lights_render_setup() {
		ofEnableDepthTest();
		ofEnableLighting();

		for (ofLight *light : lights) {
			if (light->light_is_on) {
				light->enable();
			}
		}

	}

	// Function to display lights overlays on screen
	void scene_lights_render() {
		for (ofLight *light : lights) {
			if (light->display_overlay) {
				light->render_overlay();
			}
		}
	}

	void scene_lights_render_exit() {
		for (ofLight* light : lights) {
			if (light->light_is_on) {
				light->disable();
			}
		}

		ofDisableLighting();
		ofDisableDepthTest();
	}

	void display_paramater_data_widget(ofLight *light) {};// To DO

	parameter_widget_base_class   parameter_widget;
	// node_editor_base_class          node_widget;

protected:

private:
	int get_light_index(id_type light_id) {
		for (size_t i = 0; i < lights.size(); i++) {
			if (lights[i]->id == light_id) return i;
		}
		return -1;
	}

};
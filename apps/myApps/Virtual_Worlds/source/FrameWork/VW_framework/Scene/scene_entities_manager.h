#pragma once
/*
				SCENE ENTITY DB MANAGER CLASS

	Manager class to perform functions to entity scene objects
	required by the application, or to retrieve data of a
	particular entity id of a given entity data type.
*/
// ********

//#include "objects_category_basis.h"

//#include <VW/Common/definitions.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

//#include "../3D/vw_node.h"
#include "ofNode.h"

#include "scene_cameras.h"
#include "scene_lights.h"
#include "scene_object_categories.h"

//enum class entity_object_type_enum {camera, light, object};

inline id_key_manager_class<idu_type> category_id_key;// This needed as global so all catagories in all editors have a unique category id

class scene_entities_manager_class {
public:
	~scene_entities_manager_class() {
		clear_scene_objects();
	}

	scene_cameras_manager_class                   scene_cameras;
	scene_lights_manager_class                    scene_lights;
	std::vector<vw_scene_objects_category_class > scene_objects;

	log_panel_class* log_panel = NULL;

	int selected_object_category = INVALID_ID;

	//bool define_new_entity_category(id_type entity_category_id) {
	int define_new_entity_category(string_type category_name, string_type category_description = "") {
		if (get_objects_category_index(category_name) > -1) {
			// message category exits
			std::cout << "ERROR : Cannot define new Object category as it exists\n" << std::endl;
			return -1;
		}

		vw_scene_objects_category_class scene_objects_of_category;
		idu_type objects_category_id = category_id_key.get_available_id_key();

		scene_objects_of_category.objects_category_id  = objects_category_id;
		scene_objects_of_category.category_name        = category_name;
		scene_objects_of_category.category_description = category_description;
		
		scene_objects.push_back(scene_objects_of_category);
//printf("scene_entities_db_manager2_class:define_new_entity_category : %i\n", scene_objects.size());
		return objects_category_id;

	}

	bool add_object(vw_object_base_class *entity_object, id_type entity_category_id) {
		//if (entity_category_id > -1 && entity_id > -1) {
		if (entity_category_id > -1 && entity_object != NULL) {

			int index = get_category_index(entity_category_id);

			if (index < 0) {
//printf("scene_entities_db_manager2_class : add_new_entity :: scene_objects_of_category == NULL\n");
				return false;
			}

//printf("scene_entities_db_manager2_class : add_new_entity :: %i : %i  \n", scene_objects[index]->objects_category_id, entity_id);
			return scene_objects[index].add_object(entity_object);
		}

		return false;
	}

	bool delete_object(id_type entity_id, id_type entity_category_id) {
		if (entity_category_id > -1 && entity_id > -1) {
			int index = get_category_index(entity_category_id);
			if (index < 0) {
				// error message
				return false;
			}

			return scene_objects[index].delete_object(entity_id);
		}

		return false;
	}


	// +++++++++++++++++++++++++

	void display_entity_parameters(id_type current_selected_entity_type_id, id_type current_selected_entity_category_type_id, id_type current_selected_entity_id) {
		switch (current_selected_entity_type_id) {
			case ENTITY_TYPE_CAMERA : {
				vw_camera_class *camera = get_camera(current_selected_entity_id);
				if (camera != nullptr) {
					scene_cameras.display_paramater_data_widget(camera);
				}
			} ;break;

			case ENTITY_TYPE_LIGHT: {
				ofLight *light= get_light(current_selected_entity_id);
				if (light != nullptr) {
					scene_lights.display_paramater_data_widget(light);
				}
			}; break;

			case ENTITY_TYPE_OBJECT: {
//cout << "scene_entities_manager_class : display_entity_parameters : ENTITY_TYPE_OBJECT 00000 : " << current_selected_entity_id << " : " << current_selected_entity_category_type_id <<std::endl;
				vw_object_base_class *object = get_object_of_category(current_selected_entity_id, current_selected_entity_category_type_id);
				if (object != nullptr) {
//cout << "scene_entities_manager_class : display_entity_parameters : ENTITY_TYPE_OBJECT 11111 : object != nullptr" << std::endl;
					object->display_paramater_data_widget();
				}
			}; break;
		}

	}

	vw_scene_objects_category_class get_objects_of_category(string_type category_name) {
		int category_index = get_category_index(category_name);
		vw_scene_objects_category_class vw_scene_objects_category;

		if (category_index > INVALID_ID) {
			vw_scene_objects_category = scene_objects[category_index];
		}

		return vw_scene_objects_category;
	}

	vw_object_base_class *get_object_of_category(id_type entity_id, id_type entity_category_id) {
		if (entity_category_id > -1 && entity_id > -1) {
			int index = get_category_index(entity_category_id);
			if (index < 0) {
//cout << "scene_entities_manager_class : get_entity_of_category :: scene_objects_of_category == NULL\n";
				return NULL;
			}

			return scene_objects[index].get_entity_object(entity_id);
		}

		return NULL;
	}

	vw_camera_class *get_camera(id_type camera_id) {
		return scene_cameras.get_camera(camera_id);
	}

	ofLight *get_light(id_type light_id) {
		return scene_lights.get_light(light_id);
	}

	// +++++++++++++++++++++++++

	int get_category_index(string_type category_name) {
//cout << "scene_entities_manager_class : get_category_index 0000 : " << scene_objects.size() << std::endl;
		for (int i = 0; i < scene_objects.size(); i++) {
//cout << "scene_entities_manager_class : get_category_index 1111 : " << scene_objects[i].objects_category_id << " : " << category_name << std::endl;
			if (scene_objects[i].category_name == category_name)
				return i;
		}

		return -1;
	}

	int get_category_index(id_type entity_category_id) {
//cout << "scene_entities_manager_class : get_category_index AAAA : " << scene_objects.size() << std::endl;
		for (int i = 0; i < scene_objects.size(); i++) {
//cout << "scene_entities_manager_class : get_category_index BBBB : " << scene_objects[i].objects_category_id << " : " << entity_category_id << std::endl;
			if (scene_objects[i].objects_category_id == entity_category_id)
				return i;
		}

		return -1;
	}

	void delete_objects_of_category(id_type entity_category_id) {
		if (entity_category_id > -1) {
			int index = get_category_index(entity_category_id);
			if (index < 0) {
				// error message
				return;
			}

			scene_objects[index].clear_objects();
		}
		category_id_key.assign_free_id_key(entity_category_id);
	}

	void clear_scene_objects() {
		for (vw_scene_objects_category_class objects_of_category : scene_objects) {
			category_id_key.assign_free_id_key(objects_of_category.objects_category_id);
			objects_of_category.clear_objects();
		}

		scene_objects.clear();
	}


	int get_objects_category_index(string_type category_name) {
		for (int i = 0; i < scene_objects.size(); i++) {
//std::cout << "scene_entities_manager_class :  get_objects_category_index : " << i << " : " << scene_objects[i].category_name << " : " << category_name << std::endl;
			if (scene_objects[i].category_name == category_name)
				return i;
		}

		return -1;
	}

	void render_objects() {
		for (vw_scene_objects_category_class category_objects : scene_objects) {
			category_objects.render_objects();
		}
	}

	void render_overlays() {
		// Render camera and light functions here
		scene_lights.scene_lights_render();
		scene_cameras.scene_cameras_render();
	}

protected:
private:
	//id_key_manager_class<idu_type> category_id_key;

};

#pragma once

#include "../Object/vw_object_base.h"

#include "scene_settings.h" // +++++
#include "scene_cameras.h"
#include "scene_lights.h"

#include <FrameWork/Universal_FW/Kernal/id_key_manager.h>

inline id_key_manager_class<idu_type> entity_id_key;// This needed as global so all entities in all editors have a unique entity id

class vw_scene_objects_category_class {
public:
	id_type     objects_category_id  = INVALID_ID;
	string_type category_name        = "";
	string_type category_description = "";
	//geometry_type_enum      geometry_type = geometry_type_enum::undefined;   //not sure if needed
	//base_geometry_type_enum base_geometry_type = base_geometry_type_enum::D3;//not sure if needed

	std::vector<vw_object_base_class *> category_objects;

	vw_object_base_class *selected_object = NULL;

	vw_object_base_class *create_new_category_object( ) {
		vw_object_base_class *new_object = new vw_object_base_class;
		if (new_object == NULL) return false;
		new_object->object_category_id = objects_category_id;
		return new_object;
	}

	bool  add_object(vw_object_base_class* entity_object) {
		if (entity_object == NULL) return false;
		if (entity_object->object_category_id != objects_category_id) return false;
		entity_object->id = get_available_entity_id();
		category_objects.push_back(entity_object);
		return true;
	}

	bool  delete_object(id_type entity_id) {
//std::cout << "vw_scene_objects_category_class:delete_object : AAAAA0000 : " << category_objects.size() << std::endl;
		for (size_t i = 0; i < category_objects.size(); i++) {
			if (category_objects[i]->id == entity_id) {
				//category_objects[i]->delete_object();
				delete category_objects[i];
				category_objects.erase(category_objects.begin() + i);
				assign_free_entity_id(entity_id);// Free entity ID number to be reused when a new entity is created
//std::cout << "vw_scene_objects_category_class:delete_object : AAAAA1111 : " << category_objects.size() << std::endl;
				return true;
			}
		}

		return false;
	}

	bool  delete_object(vw_object_base_class *entity_object) {
		if (entity_object == NULL) return false;
		return delete_object(entity_object->id);
	}

	vw_object_base_class* get_entity_object(id_type entity_id) {
		for (vw_object_base_class* object : category_objects) {
			if (object->id == entity_id) return object;
		}

		return NULL;
	}

	void  clear_objects() {
		for (size_t i = category_objects.size() - 1; i > -1; i--) {
			assign_free_entity_id(category_objects[i]->id);// Free entity ID number to be reused when a new entity is created
			delete_object(category_objects[i]);
			category_objects.erase(category_objects.begin() + i);
		}
	}

	void update_objects() {
		for (vw_object_base_class *object : category_objects) {
			object->update_object();
		}
	}

	int get_object_index(id_type object_id) {
//printf("voxel_hcp_scene_objects_class :: get_object_index %i \n", size());
		for (int i = 0; i < category_objects.size(); i++) {
//printf("voxel_hcp_scene_objects_class :: get_object_index %i : %i\n", i, at(i)->object_id);
			if (category_objects.at(i)->id == object_id) return i;
		}

		return -1;
	}

	bool get_shader_parameters(id_type object_id, material_struct_type &shader_parameters) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		shader_parameters = category_objects[object_index]->get_shader_parameters();

		//voxel_hcp_object_class* voxel_hcp_object = at(object_index);
		//shader_parameters = voxel_hcp_object->voxel_object_data.shader_parameters;

		return true;
	}

	bool save_generation_data(id_type object_id, string_type file_pathname) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		return category_objects[object_index]->save_generation_data(file_pathname);

		//voxel_function_import_export.voxel_generator_parameters = voxel_hcp_object->voxel_object_data.voxel_generator_parameters;
		//return voxel_function_import_export.save_voxel_generated_function(file_pathname);
	}

	bool import_generation_data(id_type object_id, string_type file_pathname) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		return category_objects[object_index]->import_generation_data(file_pathname);

		//voxel_hcp_object_class* voxel_hcp_object = at(object_index);
		//return voxel_function_import_export.import_voxel_genereated_function(voxel_hcp_object->voxel_object_data.voxel_generator_parameters, file_pathname);
	}

	bool rename_object_id(id_type old_object_id, id_type new_object_id) {
		int object_index = get_object_index(old_object_id);
		if (object_index > -1) {
			category_objects.at(object_index)->id = new_object_id;
			return true;
		}
		return false;
	}

	// -----------------------------------------------------------

	//void display_current_object_parameter_widget() {
	//	if (current_object == nullptr) return;

	//	current_object->parameter_widget.display_parameters_widget();
	//}

	// -----------------------------------------------------------

	void render_objects(scene_settings_struct_type scene_settings,scene_cameras_manager_class scene_cameras,scene_lights_manager_class scene_lights) {
//	void render_objects() {
		vw_camera_class *selected_camera = scene_cameras.selected_camera;

		if (!selected_camera) { 
			std::cout <<"Scene_Object_Categories : Selected Camera = NULL\n";
			return; 
		}
//else std::cout <<"Scene_Object_Categories : Selected Camera != NULL\n";

		for (vw_object_base_class *object : category_objects) {
			if (!object) continue;

			if (object->display_object) {
				// update additional user universal variables :
				GLint object_shader_id = object->geometry->shader->getProgram();

				scene_settings.update_shader_uniforms(object_shader_id);// ++++++
				selected_camera->update_shader_uniforms(object_shader_id);// enable camera details to be accessed by glsl code
//if (!scene_lights.scene_global_light) {
//std::cout <<"Scene_Object_Categories :!scene_lights.scene_global_light == NULL\n";
//}
//else
//std::cout <<"Scene_Object_Categories :!scene_lights.scene_global_light != NULL\n";

				scene_lights.update_global_shader_uniforms(object_shader_id);

				// require here update object shader for scene lights
				object->material.update_shader_uniforms(object_shader_id); // ++++++
				object->update_shader_variables();// ++++++
				object->geometry->render();
			}

			if (object->display_bounding_limits) {
				object->draw_bounding_limits();
			}

			if (object->gizmo_display != node_gizmo_display_enum::none) { // since 99.9% of the time this is the case and to avoid the folowing switch test
				switch (object->gizmo_display) {
					case node_gizmo_display_enum::axis:        object->draw_axis_gizmo();        break;
					case node_gizmo_display_enum::orientation: object->draw_orientation_gizmo(); break;
					case node_gizmo_display_enum::scale:       object->draw_scale_gizmo();       break;
				}
			}

			if (object->display_normals) object->draw_normals();
		}
	}


protected:

private:
	int get_entity_object_index(id_type entity_id) {
		for (size_t i = 0; i < category_objects.size();i++) {
			if (category_objects[i]->id == entity_id) return i;
		}

		return -1;
	}

	idu_type get_available_entity_id() { return entity_id_key.get_available_id_key(); }
	void    assign_free_entity_id(idu_type entity_id) { entity_id_key.assign_free_id_key(entity_id); }
	void    reinitialise() { entity_id_key.reinitialise(); }

	//id_key_manager_class<idu_type> entity_id_key;
};
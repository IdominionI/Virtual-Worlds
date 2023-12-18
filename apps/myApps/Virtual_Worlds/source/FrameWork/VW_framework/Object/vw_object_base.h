#pragma once

#include "ofNode.h"

#include <VW_framework/Shader/shader_components.h>
#include "../Widgets/parameter_widget_base.h"

class vw_object_base_class : public ofNode{
public:
	//vw_object_base_class() {}
	//~vw_object_base_class() {}

	int  object_category_id = INVALID_ID;
	int  object_type_id     = INVALID_ID;

	bool display_object = true;
	bool active_object  = false;

	// Need to use a void pointer so as to allow objects of same category 
	// but different data structures to be stored in the same vector array
	// Thus to use need to use and access need to use dynamic casting.
	//void                        *object_data;
	//vw_object_base_class        *object_data;
	parameter_widget_base_class *parameter_widget = nullptr;// parameter_widget needs to be a pointer to work
	// node_editor_base_class          node_widget;

	virtual bool define_geometry_data() { return false; }
	virtual bool update_object() { return false; }
	virtual void delete_object() {}// ++++

	virtual void clear_shader_variables() {}
	virtual void update_shader() {}
	virtual void update_shader_variables() {}

	virtual bool perform_animation_frame_functions() { return false; }
	
	virtual void display_paramater_data_widget() {
		//if (object_data == NULL) {
		//	// ERROR message
		//	return;
		//}

		if (parameter_widget == NULL) {
			// ERROR message
//std::cout << "vw_object_base_class : display_paramater_data_widget : parameter_widget == NULL\n";
			return;
		}
//std::cout << "vw_object_base_class : display_paramater_data_widget\n";
		parameter_widget->current_selected_object_id = id;
		parameter_widget->object_to_execute          = this;

		parameter_widget->display_parameters_widget();
//std::cout << "vw_object_base_class : display_paramater_data_widget\n";;
	}

	virtual void update_boundary_limits() {} // This needs to be within ofNode geometry class

	// add more virtual functions as needed 

	virtual material_struct_type get_shader_parameters() { 
		material_struct_type a; 
		return a;
	};

	virtual bool save_generation_data(std::string file_pathname) { return false; }// Change (std::string to std::path
	virtual bool import_generation_data(std::string file_pathname) { return false; }// Change (std::string to std::path

protected:


private:
};

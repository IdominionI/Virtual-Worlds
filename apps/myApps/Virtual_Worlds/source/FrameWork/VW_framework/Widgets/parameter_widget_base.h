#pragma once

#include <VW_framework/Types/vw_type_definitions.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

class vw_object_base_class;

class parameter_widget_base_class {
public:
	parameter_widget_base_class() {}
	~parameter_widget_base_class() {}

	id_type               current_selected_object_id = INVALID_ID;
	vw_object_base_class *object_to_execute          = NULL;

	log_panel_class       *log_panel                 = NULL;

	virtual void display_parameters_widget() {};

	virtual void set_parameter_widget_object_data() {};
	
	// add more virtual functions as required here

protected:

private:
};
#pragma once

#include <VW_framework/Types/vw_type_definitions.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../../Editor/Widgets/hex_generation_selection_widget.h"
#include "hex_surface_editor_generator_widget.h"

class hex_surface_object_class;
class hex_surface_generation_editor_widget_class : public hex_surface_generation_selection_widget_class {
public:
	bool display_as_points = true;

	hex_surface_editor_generation_widget_class hex_surface_generation_widget;

	void display();
	void change_voxels_display();
	void update_hex_size();

private:
	float hex_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	void display_hex_surface_generation_widget(hex_surface_object_class *hex_surface_object_to_execute, id_type current_selected_object_id);

};
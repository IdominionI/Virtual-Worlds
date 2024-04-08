#pragma once

#include <VW_framework/Types/vw_type_definitions.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../../Editor/Widgets/hcp_voxel_generation_basis_widget.h"
#include "hcp_voxel_editor_generator_widget.h"

class voxel_hcp_object_class;
class hcp_voxel_generation_editor_widget_class : public hcp_voxel_generation_basis_widget_class {
public:
	bool display_as_points = true;

	voxel_hcp_editor_generation_widget_class voxel_hcp_generation_widget;

	void display();
	void change_voxels_display();
	void update_voxel_size();

private:
	float voxel_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	void display_voxel_generation_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id);

};
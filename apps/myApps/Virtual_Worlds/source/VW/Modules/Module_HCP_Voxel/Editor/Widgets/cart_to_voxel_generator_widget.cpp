#include "cart_to_voxel_generator_widget.h"

#include <glm/gtc/random.hpp>

#include <FrameWork/Kernels/imgui-docking/imgui.h>
#include <FrameWork/Universal_FW/assimp/Importer.hpp>
#include <FrameWork/Universal_FW/assimp/postprocess.h>
#include <FrameWork/Universal_FW/assimp/scene.h>

#include <VW/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <VW/Editor/Tools/dialogs.h>
#include <VW/Editor/Interface/IconsFontAwesome.h>

#include "../../Object/voxel_hcp_object.h"

void cart_to_voxel_widget_class::display() {
	if (voxel_hcp_object_to_execute == NULL) {
		return;
	}

	const char* random_items[] = { "Ball", "Sphere" };// This cannot be defined outside this function	

	float x_pos = 10.0f, y_pos = 180.0f;

	text("PLY File : ", x_pos, y_pos);
	//if (ex_button(ply_file_name + "###ctovb01", x_pos + 120, y_pos, 150, 20))
	if (ex_button(cart_to_hcp_voxel_generator_parameters.ply_file_name + "###ctovb01", x_pos + 120, y_pos, 150, 20))
		get_ply_file();

	y_pos += 25;

	text("Random Generaton :", x_pos, y_pos);

	ImGui::SetCursorPosX(x_pos + 170);
	ImGui::SetCursorPosY(y_pos);
	ImGui::SetNextItemWidth(75);
	ImGui::Combo("###ctovrgen", &cart_to_hcp_voxel_generator_parameters.random_type, &Funcs::ItemGetter, random_items, IM_ARRAYSIZE(random_items));

	y_pos += 25;
	text("Start             End", x_pos + 80, y_pos);

	x_pos = 20; y_pos += 20;

	text("X-axis", x_pos, y_pos);
	float_input("###cvxstart", cart_to_hcp_voxel_generator_parameters.x_start, x_pos + 60, y_pos, 75.0f);
	float_input("###cvxend", cart_to_hcp_voxel_generator_parameters.x_end, x_pos + 190.0f, y_pos, 75.0f);

	y_pos += 20;
	text("Y-axis", x_pos, y_pos);
	float_input("###cvystart", cart_to_hcp_voxel_generator_parameters.y_start, x_pos + 60, y_pos, 75.0f);
	float_input("###cvyend", cart_to_hcp_voxel_generator_parameters.y_end, x_pos + 190.0f, y_pos, 75.0f);

	y_pos += 20;
	text("Z-axis", x_pos, y_pos);
	float_input("###cvzstart", cart_to_hcp_voxel_generator_parameters.z_start, x_pos + 60, y_pos, 75.0f);
	float_input("###cvzend", cart_to_hcp_voxel_generator_parameters.z_end, x_pos + 190.0f, y_pos, 75.0f);

	y_pos += 30;
	text("Coordinate Generation", x_pos + 65, y_pos);
	y_pos += 25;
	text("ply file", x_pos + 60, y_pos);
	text("Function", x_pos + 180, y_pos);
	y_pos += 25;

	ImGui::SetCursorPosX(x_pos + 70);
	ImGui::SetCursorPosY(y_pos);
	ImGui::RadioButton("###ampcrb", &cart_to_hcp_voxel_generator_parameters.generation_type, 0);
	ImGui::SetCursorPosX(x_pos + 190);
	ImGui::SetCursorPosY(y_pos);
	ImGui::RadioButton("###amfsrb", &cart_to_hcp_voxel_generator_parameters.generation_type, 1);

	y_pos += 30;
	text("Number points", x_pos + 30, y_pos);
	integer_input("###ctvnpoints", cart_to_hcp_voxel_generator_parameters.number_of_points, x_pos + 160, y_pos, 75.0f);

	y_pos += 30;
	text("Scale/radius", x_pos + 30, y_pos);
	float_input("###ctvtrad", cart_to_hcp_voxel_generator_parameters.ctov_scale, x_pos + 160, y_pos, 75.0f);

	y_pos += 30;
	if (ex_button("Define Cartesian Data", x_pos + 60, y_pos, 180, 20))
		define_cartesian_data();

	y_pos += 60;
	text("Voxel Size", x_pos + 30, y_pos);
	float_input("###ctvvsize", cart_to_hcp_voxel_generator_parameters.voxel_size, x_pos + 160, y_pos, 75.0f);

	y_pos += 30;
	if (ex_button("Generate Voxels", x_pos + 70, y_pos, 150, 20))
		cartesian_to_voxel();

	y_pos += 30;
	text("Data Range", x_pos, y_pos);
	integer_input("###ctvmindr", cart_to_hcp_voxel_generator_parameters.min_data_value, x_pos + 100, y_pos, 45.0f);
	integer_input("###ctvmaxdr", cart_to_hcp_voxel_generator_parameters.max_data_value, x_pos + 190, y_pos, 45.0f);

	y_pos += 30;

	if (ex_button(ICON_FA_UPLOAD " Save###gs", x_pos + 35, y_pos, 100, 20))
		save_generation_parameters();

	if (ex_button(ICON_FA_DOWNLOAD " Load" "###gdl", x_pos + 165, y_pos, 100, 20))
		load_generation_parameters();
}

void cart_to_voxel_widget_class::define_cartesian_data() {
	if (cart_to_hcp_voxel_generator_parameters.generation_type == 0)
		import_ply_file_to_cartesian_array();
	else
		generate_cartesian_array();

	define_default_voxel_limits();
}

void cart_to_voxel_widget_class::import_ply_file_to_cartesian_array() {
	if (FW::filetools::file_exists(cart_to_hcp_voxel_generator_parameters.ply_file_pathname)) {
		if (!import_file(cart_to_hcp_voxel_generator_parameters.ply_file_pathname)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not import file %s\n", cart_to_hcp_voxel_generator_parameters.ply_file_pathname);
			return;
		}
		else
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : File %s imported successfuly\n", cart_to_hcp_voxel_generator_parameters.ply_file_pathname);
	}
	else
		printf("File of path %s does not exist\n", cart_to_hcp_voxel_generator_parameters.ply_file_pathname.c_str());
}

void cart_to_voxel_widget_class::generate_cartesian_array() {
	if (cart_to_hcp_voxel_generator_parameters.number_of_points <= 0) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Have specified zero points to generate cartesian data\n");
		return;
	}

	if (cart_to_hcp_voxel_generator_parameters.ctov_scale <= 0.0) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Have test radius invalid value\n");
		return;
	}

	float x_range = cart_to_hcp_voxel_generator_parameters.x_end - cart_to_hcp_voxel_generator_parameters.x_start;
	float y_range = cart_to_hcp_voxel_generator_parameters.y_end - cart_to_hcp_voxel_generator_parameters.y_start;
	float z_range = cart_to_hcp_voxel_generator_parameters.z_end - cart_to_hcp_voxel_generator_parameters.z_start;

	if (x_range == 0 && y_range == 0 && z_range == 0) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Have one or more of the data ranges to generate cartesian data array equal to zero\n");
		return;
	}

	glm::vec3 r_coord;

	cartesian_point_data.reset();
	/*
			// folowing for testing only
			for (int i = -10; i < 10; i++) {
				//for (int j = -10; j < 10; j++) {
					int inc = 2;
					for (int w = 0; w < inc; w++) {
						r_coord.x = float(i) + float(w)/float(inc);
						//r_coord.y = float(j) + float(w) / float(inc);
						r_coord.y = 0.0;
						r_coord.z = 0.0;

						cartesian_point_data.add_coordinate(r_coord);
					}
				//}
			}
	*/

	// use srand() for different outputs
	srand(int(time(0)));

	for (int i = 0; i < cart_to_hcp_voxel_generator_parameters.number_of_points; i++) {
		switch (cart_to_hcp_voxel_generator_parameters.random_type) {
			case 0:  r_coord = glm::ballRand(cart_to_hcp_voxel_generator_parameters.ctov_scale);      break;
			case 1:  r_coord = glm::sphericalRand(cart_to_hcp_voxel_generator_parameters.ctov_scale); break;
			default: r_coord = glm::ballRand(cart_to_hcp_voxel_generator_parameters.ctov_scale);		 break;
		}
		cartesian_point_data.add_coordinate(r_coord);
	}

}

void cart_to_voxel_widget_class::define_default_voxel_limits() {
	cart_to_hcp_voxel_generator_parameters.x_start = cartesian_point_data.min_x, cart_to_hcp_voxel_generator_parameters.x_end = cartesian_point_data.max_x;
	cart_to_hcp_voxel_generator_parameters.y_start = cartesian_point_data.min_y, cart_to_hcp_voxel_generator_parameters.y_end = cartesian_point_data.max_y;
	cart_to_hcp_voxel_generator_parameters.z_start = cartesian_point_data.min_z, cart_to_hcp_voxel_generator_parameters.z_end = cartesian_point_data.max_z;
}

void cart_to_voxel_widget_class::define_generation_parameters() {
	voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_start = cart_to_hcp_voxel_generator_parameters.x_start;
	voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_end   = cart_to_hcp_voxel_generator_parameters.x_end;
	voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_start = cart_to_hcp_voxel_generator_parameters.y_start;
	voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_end   = cart_to_hcp_voxel_generator_parameters.y_end;
	voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_start = cart_to_hcp_voxel_generator_parameters.z_start;
	voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_end   = cart_to_hcp_voxel_generator_parameters.z_end;

	voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.resolution_step = cart_to_hcp_voxel_generator_parameters.voxel_size;
}


void cart_to_voxel_widget_class::cartesian_to_voxel(bool notification) {
//	printf("cart_to_voxel_widget_class :: in cartesian_to_voxel\n");

	if (current_selected_entity_id == -1) {
//std::cout << "cart_to_voxel_widget_class :: current_selected_entity_id   == -1 #####\n";
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
		return;
	}
	if (voxel_hcp_object_to_execute == NULL) {
//std::cout << "cart_to_voxel_widget_class :: voxel_hcp_object_to_execute == NULL #####\n";
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
		return;
	}

	define_generation_parameters();
	voxel_hcp_object_to_execute->voxel_object_data.create_voxel_matrix();
	int matrix_size = int(voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size());

	glm::ivec3 matrix_dimension = voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension; // tresting only
//std::cout << "cart_to_voxel_widget_class :: cartesian_to_voxel :" <<  matrix_dimension.x << ":" << matrix_dimension.y << ":" << matrix_dimension.z =  ;
	for (glm::vec3 coordinate : cartesian_point_data.coordinates) {
		int index = voxel_hcp_object_to_execute->voxel_object_data.index_of_voxel_cell_with_cartesian_coord(coordinate.x, coordinate.y, coordinate.z);

//std::cout << "cart_to_voxel_widget_class :: INDEX !!!!  :" <<index =  ;
		if (index > -1 && index < matrix_size) {
			voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data[index]++;// add one to number of points in the voxel cell
		}
	}
//printf("cart_to_voxel_widget_class :: execute_voxel_function 3333\n");

	voxel_hcp_object_to_execute->set_voxel_value_range(cart_to_hcp_voxel_generator_parameters.min_data_value, cart_to_hcp_voxel_generator_parameters.max_data_value);// need to define values for min/max voxel value range or have incorrect to misleading display
	voxel_hcp_object_to_execute->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("cart_to_voxel_widget_class :: execute_voxel_function 4444\n");

			//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		shader_class shader;

		// **** Update voxel hcp shader variable values define  to be used in all voxel hcp shaders as default
		float vox_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * voxel_scale_value;

		shader.set_f1(shader.getProgram(), vox_size, "voxSize");

		shader.set_ivec3(shader.getProgram(), voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension, "voxel_matrix_dimension");

		shader.set_vec3(shader.getProgram(), voxel_hcp_object_to_execute->voxel_object_data.matrix_origin, "voxel_origin");

		float voxel_hcp_z_increment = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * 2.0f * sqrt(6.0f) / 3.0f;

		shader.set_f1(shader.getProgram(), voxel_hcp_z_increment, "voxel_hcp_z_increment");

		//voxel surface display data
		float v_min = (float)voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.min_surface_value;

		shader.set_f1(shader.getProgram(), v_min, "voxel_min_surface_display_value");

		float v_max = (float)voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.max_surface_value;

		shader.set_f1(shader.getProgram(), v_max, "voxel_max_surface_display_value");

	if (notification)
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel Creation From Function Completed.\n");


}

void cart_to_voxel_widget_class::execute_voxel_function(bool notification) {
	printf("cart_to_voxel_widget_class :: in execute_voxel_function\n");

}

//------------------------------------------------------------
	void cart_to_voxel_widget_class::get_ply_file() {
		char const* patterns[] = { "*.ply","*.obj","*.stl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No .ply file selected to perform voxel generation\n");
			return;
		}
		else
			printf("expression_file_pathname != NULL %s \n", file_pathname);

		cart_to_hcp_voxel_generator_parameters.ply_file_pathname = file_pathname;
		cart_to_hcp_voxel_generator_parameters.ply_file_name = vwDialogs::get_filename(file_pathname, "\\");
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bool cart_to_voxel_widget_class::import_file(const std::string& filepath) {
		const uint32_t cMeshImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_ValidateDataStructure;

		Assimp::Importer Importer;

		const aiScene* pScene = Importer.ReadFile(filepath.c_str(),
			cMeshImportFlags);

		if (pScene && pScene->HasMeshes())
		{
			cartesian_point_data.reset();

			auto* mesh = pScene->mMeshes[0];

			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				glm::vec3 mPos;
				mPos = { mesh->mVertices[i].x * cart_to_hcp_voxel_generator_parameters.ctov_scale, mesh->mVertices[i].y * cart_to_hcp_voxel_generator_parameters.ctov_scale ,mesh->mVertices[i].z * cart_to_hcp_voxel_generator_parameters.ctov_scale };

				cartesian_point_data.add_coordinate(mPos);
			}
			return true;
		}

		return false;
	}

	void cart_to_voxel_widget_class::save_generation_parameters() {
//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = { "*.cvgp" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file defined to save voxel generation parameter data to\n");
			return;
		}
//else
	//printf("save_generation_parameters != NULL %s \n", file_pathname);
	//printf("save_generation_parameters != NULL  \n");

		//define_paramter_data();

		voxel_function_import_export.cart_to_hcp_voxel_generator_parameters = cart_to_hcp_voxel_generator_parameters;

		voxel_function_import_export.save_cart_to_hcp_voxel(file_pathname);
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO : voxel generation parameter data saved to file\n %s\n", file_pathname);
	}

	void cart_to_voxel_widget_class::load_generation_parameters() {
//printf("load button clicked\n");// replace with clear variables


		char const* patterns[] = { "*.cvgp" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to import voxel generation parameter data from.\n");
			return;
		}
//else
//	printf("load_generation_parameters != NULL  \n");

		voxel_function_import_export.import_cart_to_voxel_generated_function(cart_to_hcp_voxel_generator_parameters, file_pathname);

		//define_generatione_variables();

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Voxel generation parameter data imported from file\n %s\n", file_pathname);
	}
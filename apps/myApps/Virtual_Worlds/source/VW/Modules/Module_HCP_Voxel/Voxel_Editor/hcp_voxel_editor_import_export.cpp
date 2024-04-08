#include "hcp_voxel_editor_import_export_.h"

#include <Universal_FW/Tools/dialogs.h> // Seems cannot have or use this in a .h file. Get already has a body compile errors

bool hcp_voxel_editor_import_export_class::export_voxel_volume(std::string file_pathname,voxel_object_data_class &voxel_object_data) {

		if (file_pathname.size() == 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel volume :: Cannot export voxel volume : No file pathname defined\n");
			vwDialogs::message_box("ERROR : Export voxel volume :: Cannot export voxel volume : No file pathname defined");
			return false;
		}


		if (voxel_object_data.voxel_matrix_data.empty()) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel volume :: No entity data in scene to export\n");
			vwDialogs::message_box("ERROR : Export voxel volume :: No entity data in scene to export");
			return false;
		}

		if (!open_file_stream(file_pathname)) {
			vwDialogs::message_box("export_node", "export point cloud object ERROR:\n Failed to open file\n%s\nto write to", file_pathname.c_str());
			std::string str = "export voxel volume ERROR:\n Failed to open file \n" + file_pathname + "\nto write to.";
			vwDialogs::message_box("export_node", str.c_str());
			return false;
		}

		if (!export_voxel_volume_data(voxel_object_data)) {
			close_file_stream();
			return false;
		}

		close_file_stream();

		std::string str = "Finished exporting voxel central point cloud data to file" + file_pathname;
		vwDialogs::message_box("export_node", str.c_str());

		return true;
	}

	bool hcp_voxel_editor_import_export_class::read_voxel_volume_data(std::vector<std::string> &lines, voxel_object_data_class &voxel_object_data, int &line_number) {
		if(!read_header(lines, voxel_object_data, line_number)) return false;
//std::cout << "hcp_voxel_editor_import_export_class : read_voxel_volume_data : 000000"<< std::endl;

		int line0 = line_number;
		if (line0 < 0) {
			std::cout << "line0 < 0\n"; 
			return false;
		}
		
		voxel_object_data.voxel_generator_parameters.resolution_step = voxel_object_data.voxel_size ;
		voxel_object_data.create_voxel_matrix();
		size_t matrix_size = voxel_object_data.voxel_matrix_data.size();
//std::cout << "hcp_voxel_editor_import_export_class : read_voxel_volume_data : 111111 " << voxel_object_data.voxel_size << std::endl;

		size_t number_lines = lines.size()-1;
		glm::ivec3 matrix_coord;
		glm::vec3  cart_coord;

		int index  = 0;
		int mindex = 0;
		for (int i = line_number; i < number_lines; i++) {
			index = i - line0;
			
			matrix_coord = voxel_object_data.get_matrix_coordinate(index);
			cart_coord   = voxel_object_data.get_voxel_cartesian_coordinate(matrix_coord) + voxel_object_data.matrix_origin;
//std::cout << "hcp_voxel_editor_import_export_class : read_voxel_volume_data : 222222A "<< cart_coord.x << ":" << cart_coord.y << ":" << cart_coord.z << std::endl;
			if (cart_coord.x >= voxel_object_data.voxel_generator_parameters.x_start && cart_coord.x <= voxel_object_data.voxel_generator_parameters.x_end &&
				cart_coord.y >= voxel_object_data.voxel_generator_parameters.y_start && cart_coord.y <= voxel_object_data.voxel_generator_parameters.y_end &&
				cart_coord.z >= voxel_object_data.voxel_generator_parameters.z_start && cart_coord.z <= voxel_object_data.voxel_generator_parameters.z_end) 
			{
//std::cout << "hcp_voxel_editor_import_export_class : read_voxel_volume_data : " << i << " : " << index << " : " << number_lines<< " : " <<lines.size() << " : " << mindex << " : " << voxel_object_data.voxel_matrix_data.size() << std::endl;
				if(mindex < matrix_size) voxel_object_data.voxel_matrix_data[mindex] = stoi(lines[i]);
				mindex++;
			}
//else{
//std::cout << "hcp_voxel_editor_import_export_class : read_voxel_volume_data : 222222B "<< cart_coord.x << ":" << cart_coord.y << ":" << cart_coord.z << std::endl;
//}
		}

//std::cout << "hcp_voxel_editor_import_export_class : read_voxel_volume_data : 3333333 "<< cart_coord.x << ":" << cart_coord.y << ":" << cart_coord.z << std::endl;

		return true;
	}
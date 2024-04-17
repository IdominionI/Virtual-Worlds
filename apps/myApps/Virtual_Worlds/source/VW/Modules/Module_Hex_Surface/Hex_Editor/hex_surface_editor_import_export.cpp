#include "hex_surface_editor_import_export_.h"

#include <Universal_FW/Tools/dialogs.h> // Seems cannot have or use this in a .h file. Get already has a body compile errors

//#include <Universal_FW/Kernal/FWstring_tools.h>

bool hex_surface_editor_import_export_class::export_hex_surface_volume(std::string file_pathname,hex_surface_object_data_class &hex_surface_object_data) {

		if (file_pathname.size() == 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex_surface volume :: Cannot export hex_surface volume : No file pathname defined\n");
			vwDialogs::display_error_message("Export hex surface volume", "ERROR :\n Cannot export hex_surface volume \n No file pathname defined\n");
			return false;
		}


		if (hex_surface_object_data.hex_surface_matrix_data.empty()) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex_surface volume :: No entity data in scene to export\n");
			vwDialogs::display_error_message("Export hex surface volume", "ERROR :\n No entity data in scene to export\n");
			return false;
		}

		if (!open_file_stream(file_pathname)) {
			std::string str = "export hex_surface volume ERROR:\n Failed to open file \n" + file_pathname + "\nto write to.";
			vwDialogs::display_error_message("export_node", str.c_str());
			return false;
		}

		if (!export_hex_surface_volume_data(hex_surface_object_data)) {
			close_file_stream();
			return false;
		}

		close_file_stream();

		std::string str = "Finished exporting hex_surface central point cloud data to file" + file_pathname;
		vwDialogs::message_box("export_node", str.c_str());

		return true;
	}

	bool hex_surface_editor_import_export_class::read_hex_surface_volume_data(std::vector<std::string> &lines, hex_surface_object_data_class &hex_surface_object_data, int &line_number) {
		if(!read_header(lines, hex_surface_object_data, line_number)) return false;
//std::cout << "hcp_hex_surface_editor_import_export_class : read_hex_surface_volume_data : 000000"<< std::endl;

		int line0 = line_number;
		if (line0 < 0) {
			std::cout << "line0 < 0\n";
			vwDialogs::display_error_message("Read hex volume data", "ERROR :\n Possible corrupted file: No data to read.\n");
			return false;
		}
		
		hex_surface_object_data.hex_surface_generator_parameters.resolution_step = hex_surface_object_data.hex_size ;
		hex_surface_object_data.create_hex_grid();
		size_t matrix_size = hex_surface_object_data.hex_surface_matrix_data.size();
//std::cout << "hcp_hex_surface_editor_import_export_class : read_hex_surface_volume_data : 111111 " << hex_surface_object_data.hex_surface_size << std::endl;

		size_t number_lines = lines.size()-1;
		glm::ivec3 matrix_coord;
		glm::vec3  cart_coord;

		int index  = 0;
		int mindex = 0;
		int error_code = 0;
		for (int i = line_number; i < number_lines; i++) {
			index = i - line0;
			
			matrix_coord = hex_surface_object_data.get_matrix_coordinate(index);
			cart_coord = hex_surface_object_data.get_hex_surface_cartesian_coordinate(matrix_coord) + glm::vec3{hex_surface_object_data.grid_origin, 0.0f};
//std::cout << "hcp_hex_surface_editor_import_export_class : read_hex_surface_volume_data : 222222A "<< cart_coord.x << ":" << cart_coord.y << ":" << cart_coord.z << std::endl;
			if (cart_coord.x >= hex_surface_object_data.hex_surface_generator_parameters.x_start && cart_coord.x <= hex_surface_object_data.hex_surface_generator_parameters.x_end &&
				cart_coord.y >= hex_surface_object_data.hex_surface_generator_parameters.y_start && cart_coord.y <= hex_surface_object_data.hex_surface_generator_parameters.y_end )
			{
//std::cout << "hcp_hex_surface_editor_import_export_class : read_hex_surface_volume_data : " << i << " : " << index << " : " << number_lines<< " : " <<lines.size() << " : " << mindex << " : " << hex_surface_object_data.hex_surface_matrix_data.size() << std::endl;
				//if(mindex < matrix_size) hex_surface_object_data.hex_surface_matrix_data[mindex] = stoi(lines[i]);
				if (mindex < matrix_size) { 
					//hex_surface_object_data.hex_surface_matrix_data[mindex] = stoi(lines[i]);
					if (!FW::stringtools::string_to_float(lines[i], &hex_surface_object_data.hex_surface_matrix_data[mindex], error_code)) return false;
				}
				mindex++;
			}
//else{
//std::cout << "hcp_hex_surface_editor_import_export_class : read_hex_surface_volume_data : 222222B "<< cart_coord.x << ":" << cart_coord.y << ":" << cart_coord.z << std::endl;
//}
		}

//std::cout << "hcp_hex_surface_editor_import_export_class : read_hex_surface_volume_data : 3333333 "<< cart_coord.x << ":" << cart_coord.y << ":" << cart_coord.z << std::endl;

		return true;
	}

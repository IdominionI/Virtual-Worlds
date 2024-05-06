#pragma once
#include <fstream>
#include <string>
#include <vector>

#include <FrameWork/Universal_FW/Tools/dialogs.h>

#include <VW/Editor/Import_Export/vw_import_export_parameters.h>
#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../Object/hex_surface_data_storage.h"

class hex_surface_editor_import_export_class {
public:
	std::string  filename_to_write;
	std::fstream stream;

	log_panel_class	*log_panel = NULL;

	int open_file_stream(std::string filename) {
		filename_to_write = filename;
		
		stream.open(filename, std::ios::out | std::ios::out);

		if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
			
		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	bool export_hex_surface_volume(std::string file_pathname, hex_surface_object_data_class& hex_surface_object_data);

	bool get_hex_surface_volume_limits(hex_surface_object_data_class &hex_surface_object_data) {
		if (hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to import data from \n Import Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream working_model_file(hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname, std::ios::in);

		if (!working_model_file) {
//QMessageBox::information(NULL, "Import hex_surface generation model", "Import hex_surface generation model ERROR : \n Could not find read Import hex_surface generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);
		
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		int line_number = 0;

		//return read_header(lines, hex_surface_object_data, line_number);
		if (!read_header(lines, hex_surface_object_data, line_number)) {
			std::string error_message = "ERROR ::\n Corrupted or out of sequence data at line\n";
			error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
            vwDialogs::display_error_message("Import hex surface volume", error_message);
			return false;
		}

		return true;
	}

	bool import_hex_surface_volume_data(hex_surface_object_data_class &hex_surface_object_data) {
		if (hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to import data from \n Import Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream working_model_file(hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname, std::ios::in);

		if (!working_model_file) {
//QMessageBox::information(NULL, "Import hex_surface generation model", "Import hex_surface generation model ERROR : \n Could not find read Import hex_surface generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);
		
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		int line_number = 0;

		//return read_hex_surface_volume_data(lines, hex_surface_object_data, line_number);
		if (!read_hex_surface_volume_data(lines, hex_surface_object_data, line_number)) {
			std::string error_message = "ERROR ::\n Corrupted or out of sequence data at line\n";
			error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
            vwDialogs::display_error_message("Import hex surface volume", error_message);
			return false;
		}

		return true;
	}

	// ---------------------------------------------------------------------------

	bool export_gradient(std::string file_pathname,std::string interpolation_mode,std::vector<std::pair<float,glm::vec4>> gradient_data) {
		
		if (file_pathname.size() == 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export Gradient :: Cannot export Gradiment : No file pathname defined\n");
			vwDialogs::display_error_message("Export Gradient", "ERROR : Export Gradient :: Cannot export Gradiment : No file pathname defined");
			return false;
		}

		if (gradient_data.empty()) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export Gradient :: No Gradient data export\n");
			vwDialogs::display_error_message("Export Gradient", "ERROR : Export Gradient :: No Gradient data export");
			return false;
		}

		if (!open_file_stream(file_pathname)) {
			vwDialogs::message_box("export Gradient", "export Gradient ERROR:\n Failed to open file\n%s\nto write to", file_pathname.c_str());
			std::string str = "export Gradient ERROR:\n Failed to open file \n" + file_pathname + "\nto write to.";
			vwDialogs::display_error_message("Export Gradient", str.c_str());
			return false;
		}

		export_gradient_data(interpolation_mode, gradient_data);

		close_file_stream();

		std::string str = "Finished exporting Gradient data to file" + file_pathname;
		vwDialogs::display_error_message("Export Gradient data", str.c_str());

		return true;
	}

	bool import_gradient(std::string file_pathname,std::string &interpolation_mode,std::vector<std::pair<float,glm::vec4>> &gradient_data) {
		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
			std::string str = "Import Gradient ERROR:\n Failed to open file \n" + file_pathname + "\nto write to.";
			vwDialogs::display_error_message("Export Gradient", str.c_str());
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);
		
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		if (lines.size() < 6) return false;

		int line_number = 0;

		if (!read_gradient_data(lines, interpolation_mode,gradient_data, line_number)) {
			std::string error_message = "ERROR :: Import Gradient failed\n corrupted or out of sequence data at line\n";
			error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
            vwDialogs::display_error_message("Import Gradient", error_message);
			return false;
		}

		return true;
	}

private:
	int error_code = 0;

	bool export_hex_surface_volume_data(hex_surface_object_data_class &hex_surface_object_data) {
		stream << hex_surface_object_data.hex_size << std::endl;
		stream << hex_surface_object_data.grid_origin.x << std::endl;
		stream << hex_surface_object_data.grid_origin.y << std::endl;
		stream << hex_surface_object_data.grid_dimension.x << std::endl;
		stream << hex_surface_object_data.grid_dimension.y << std::endl;
		stream << hex_surface_object_data.grid_dimension.z << std::endl;
	
		for (int hex_surface_value : hex_surface_object_data.hex_surface_matrix_data) {
			stream << hex_surface_value << std::endl;
		}

		return true;
	}

	bool read_header(std::vector<std::string>& lines, hex_surface_object_data_class& hex_surface_object_data, int& line_number){
		if (lines.size() < 7) return false;

		//hex_surface_object_data.hex_size          = stof(lines[line_number]);line_number++;
		//hex_surface_object_data.grid_origin.x 	  = stof(lines[line_number]);line_number++;
		//hex_surface_object_data.grid_origin.y 	  = stof(lines[line_number]);line_number++;
		//hex_surface_object_data.grid_dimension.x  = stoi(lines[line_number]);line_number++;
		//hex_surface_object_data.grid_dimension.y  = stoi(lines[line_number]);line_number++;
		//hex_surface_object_data.grid_dimension.z  = stoi(lines[line_number]);line_number++;

		if (!FW::stringtools::string_to_float(lines[line_number], &hex_surface_object_data.hex_size, error_code))      return false; line_number++;
		if (!FW::stringtools::string_to_float(lines[line_number], &hex_surface_object_data.grid_origin.x, error_code)) return false; line_number++;
		if (!FW::stringtools::string_to_float(lines[line_number], &hex_surface_object_data.grid_origin.y, error_code)) return false; line_number++;

		if (!FW::stringtools::string_to_int(lines[line_number], &hex_surface_object_data.grid_dimension.x, error_code)) return false; line_number++;
		if (!FW::stringtools::string_to_int(lines[line_number], &hex_surface_object_data.grid_dimension.y, error_code)) return false; line_number++;
		if (!FW::stringtools::string_to_int(lines[line_number], &hex_surface_object_data.grid_dimension.z, error_code)) return false; line_number++;

		return true;
	}

	bool read_hex_surface_volume_data(std::vector<std::string>& lines, hex_surface_object_data_class& hex_surface_object_data, int& line_number);

	void export_gradient_data(std::string interpolation_mode,std::vector<std::pair<float,glm::vec4>> gradient_data) {

		stream << interpolation_mode << std::endl;
		
		for (std::pair<float,glm::vec4> gradient_entry : gradient_data) {
			stream << gradient_entry.first    << std::endl;
			stream << gradient_entry.second.x << std::endl;
			stream << gradient_entry.second.y << std::endl;
			stream << gradient_entry.second.z << std::endl;
			stream << gradient_entry.second.w << std::endl;

//std::cout << "hcp_voxel_editor_editing_widget_class:: update_editor_shader_gradient " << pos << ":" << i << std::endl;

		}
	}

	bool read_gradient_data(std::vector<std::string> &lines, std::string &interpolation_mode,std::vector<std::pair<float,glm::vec4>> &gradient_data, int& line_number) {
		interpolation_mode = lines[0];

		line_number = 1;
		for (int i = 1; i < lines.size()-5; i = i + 5) {
			std::pair<float, glm::vec4> gradient_mark;
//std::cout << "hcp_voxel_editor_import_export_class:: read_gradient_data 000 " << i << " : " << line_number << std::endl;
			if (!FW::stringtools::string_to_float(lines[i],   &gradient_mark.first, error_code))    return false;line_number++;
			if (!FW::stringtools::string_to_float(lines[i+1], &gradient_mark.second.x, error_code)) return false;line_number++;
			if (!FW::stringtools::string_to_float(lines[i+2], &gradient_mark.second.y, error_code)) return false;line_number++;
			if (!FW::stringtools::string_to_float(lines[i+3], &gradient_mark.second.z, error_code)) return false;line_number++;
			if (!FW::stringtools::string_to_float(lines[i+4], &gradient_mark.second.w, error_code)) return false;line_number++;
			
			gradient_data.push_back(gradient_mark);
		}

//std::cout << "hcp_voxel_editor_import_export_class:: read_gradient_data 1111 "  << line_number << std::endl;
		return true;
	}
};
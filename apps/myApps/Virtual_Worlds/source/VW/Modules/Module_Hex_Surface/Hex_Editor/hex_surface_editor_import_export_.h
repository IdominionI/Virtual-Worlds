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

private:


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

	int error_code = 0;
};
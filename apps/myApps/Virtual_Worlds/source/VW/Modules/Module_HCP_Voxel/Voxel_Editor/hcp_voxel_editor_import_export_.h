#pragma once
#include <fstream>
#include <string>
#include <vector>

#include <VW/Editor/Import_Export/vw_import_export_parameters.h>
#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../Object/voxel_data_storage.h"

class hcp_voxel_editor_import_export_class {
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

	bool export_voxel_volume(std::string file_pathname, voxel_object_data_class& voxel_object_data);

	bool get_voxel_volume_limits(voxel_object_data_class &voxel_object_data) {
		if (voxel_object_data.voxel_generator_parameters.expression_file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to import data from \n Import Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream working_model_file(voxel_object_data.voxel_generator_parameters.expression_file_pathname, std::ios::in);

		if (!working_model_file) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find read Import voxel generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);
		
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		int line_number = 0;

		return read_header(lines, voxel_object_data, line_number);

	}

	bool import_voxel_volume_data(voxel_object_data_class &voxel_object_data) {
		if (voxel_object_data.voxel_generator_parameters.expression_file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to import data from \n Import Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream working_model_file(voxel_object_data.voxel_generator_parameters.expression_file_pathname, std::ios::in);

		if (!working_model_file) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find read Import voxel generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);
		
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		int line_number = 0;

		return read_voxel_volume_data(lines, voxel_object_data, line_number);
	}

private:
	bool export_voxel_volume_data(voxel_object_data_class &voxel_object_data) {
		stream << voxel_object_data.voxel_size << std::endl;
		stream << voxel_object_data.matrix_origin.x << std::endl;
		stream << voxel_object_data.matrix_origin.y << std::endl;
		stream << voxel_object_data.matrix_origin.z << std::endl;
		stream << voxel_object_data.matrix_dimension.x << std::endl;
		stream << voxel_object_data.matrix_dimension.y << std::endl;
		stream << voxel_object_data.matrix_dimension.z << std::endl;
	
		for (int voxel_value : voxel_object_data.voxel_matrix_data) {
			stream << voxel_value << std::endl;
		}

		return true;
	}

	bool read_header(std::vector<std::string>& lines, voxel_object_data_class& voxel_object_data, int& line_number){
		if (lines.size() < 7) return false;

		voxel_object_data.voxel_size          = stof(lines[line_number]);line_number++;
		voxel_object_data.matrix_origin.x 	  = stof(lines[line_number]);line_number++;
		voxel_object_data.matrix_origin.y 	  = stof(lines[line_number]);line_number++;
		voxel_object_data.matrix_origin.z 	  = stof(lines[line_number]);line_number++;
		voxel_object_data.matrix_dimension.x  = stoi(lines[line_number]);line_number++;
		voxel_object_data.matrix_dimension.y  = stoi(lines[line_number]);line_number++;
		voxel_object_data.matrix_dimension.z  = stoi(lines[line_number]);line_number++;

		return true;
	}

	bool read_voxel_volume_data(std::vector<std::string>& lines, voxel_object_data_class& voxel_object_data, int& line_number);

};
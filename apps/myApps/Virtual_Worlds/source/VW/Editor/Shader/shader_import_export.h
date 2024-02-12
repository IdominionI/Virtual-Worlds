#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <FrameWork/Universal_FW/Kernal/FWstring_tools.h>
#include <FrameWork/Universal_FW/Kernal/FWfile_tools.h>

#include <VW_framework/Shader/shader_components.h>

#define OVER_WRITE_SHADER_FILE    100
#define READ_SHADER_FILE          101

#define SHADER_BLOCK_START "["
#define SHADER_BLOCK_END   "]"

#define FLOAT_SHADER_BLOCK_START "<"
#define FLOAT_SHADER_BLOCK_END   ">"

#define INT_SHADER_BLOCK_START "{"
#define INT_SHADER_BLOCK_END   "}"

#define BOOL_SHADER_BLOCK_START "("
#define BOOL_SHADER_BLOCK_END   ")"

#define ENDL '\n'

/*
				SHADER PARAMETER IMPORT EXPORT

	Class that handles the import and export of shader uniform parameter data
	from and to an ASCII text file for all application data types.

	Shader data is defined by various data blocks for each data type, and
	for the entire shader parameter data itself. The shader variables data is
	of a given order and constant in the number of parameters per shader
	variable. Any change of this order and number of lines per variable type
	will result in a error.
*/

class voxel_texture_import_export_class {
public:
	std::fstream stream;

	std::string input_line;

	//shader_parameters_struct_type shader_parameters;
	material_struct_type shader_parameters;

	//int line_number = 0;

	int open_file_stream(std::string filename, int write_mode) {

		if (write_mode == OVER_WRITE_SHADER_FILE  ) {
			stream.open(filename, std::ios::out);

			if (!stream) return false;
		}
		else
			if (!stream.is_open()) return false;

		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	bool save_working_generated_function(std::string file_pathname) {
////QMessageBox::information(NULL, "", "in save_working_generated_function", //QMessageBox::Ok);
		if (file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save Static Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_SHADER_FILE  )) return false;

		write_shader_parameter_data(stream);

		close_file_stream();

		return true;
	}

	void write_shader_parameter_data(std::fstream &stream) {
		
		// ****** Shader files *******
		stream << SHADER_BLOCK_START << ENDL;

		stream << shader_parameters.vertex_shader_file_pathname.string()   << ENDL;
		//stream << shader_parameters.point_shader_file_pathname.string()    << ENDL;
		stream << shader_parameters.geometry_shader_file_pathname.string() << ENDL;
		stream << shader_parameters.fragment_shader_file_pathname.string() << ENDL;

		stream << FLOAT_SHADER_BLOCK_START << ENDL;

		for (shader_parameter_variable_struct_type variable : shader_parameters.variables) {
			stream << variable.active_variable        << ENDL;
			stream << variable.variable_name.c_str()  << ENDL;
			stream << variable.value				  << ENDL;
			stream << variable.variable_step          << ENDL;
			stream << variable.active_variable_step   << ENDL;
			stream << variable.slider_min             << ENDL;
			stream << variable.slider_max             << ENDL;
		}

		stream << FLOAT_SHADER_BLOCK_END << ENDL;

		stream << INT_SHADER_BLOCK_START << ENDL;

		for (shader_parameter_int_variable_struct_type variable : shader_parameters.int_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;
			stream << variable.value                 << ENDL;
			stream << variable.variable_step         << ENDL;
			stream << variable.active_variable_step  << ENDL;
			stream << variable.slider_min            << ENDL;
			stream << variable.slider_max            << ENDL;
		}

		stream << INT_SHADER_BLOCK_END    << ENDL;

		stream << BOOL_SHADER_BLOCK_START << ENDL;

		for (shader_parameter_bool_variable_struct_type variable : shader_parameters.bool_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;
			stream << variable.value                 << ENDL;
		}

		stream << BOOL_SHADER_BLOCK_END << ENDL;


		stream << SHADER_BLOCK_END << ENDL;// *****
	}

	bool import_voxel_genereated_function(material_struct_type *texture_parameters, std::string file_pathname) {
		if (file_pathname.size() == 0) {
			//QMessageBox::information(NULL, "", "No file name defined to import data from \n Import Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
			//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find read Import voxel generation model defined in file \n" +
			//																 file_pathname, //QMessageBox::Ok);
			return false;
		}

		// Read entire file working_model_file text contents into stringworking_model_string
		std::string working_model_string = FW::filetools::read_all(working_model_file);

		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		int line_number = 0;
		return read_shader_parameter_data(lines, texture_parameters, line_number);
	}

	bool read_shader_parameter_data(std::vector<std::string> lines, material_struct_type* texture_parameters, int& line_number) {
		std::string line;

		if (!FW::stringtools::contains(lines[line_number], SHADER_BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
			std::cout << "Import voxel  texture model ERROR : \n Could not find start block to import user shader files at line : " << line_number << " : " << lines[line_number] << std::endl;
			return false;
		}

		if (lines.size() < line_number + 5) {
			std::cout << "Import voxel  texture model ERROR : \n Corrupted file.";
			return false;
		}

		line_number++;

		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->vertex_shader_file_pathname = line; line_number++;
		//line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		//texture_parameters->point_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->geometry_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->fragment_shader_file_pathname = line; line_number++;

        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "voxel_texture_import_export_class: read_expression_into_shader_parameters 2222 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], FLOAT_SHADER_BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
            //error message
            return false;
        }

		line_number++;

		while (!FW::stringtools::contains(lines[line_number], FLOAT_SHADER_BLOCK_END)) {
			if (line_number + 8 > lines.size()) {
				return false;
			}

			shader_parameter_variable_struct_type variable;
			variable.active_variable = stof(lines[line_number]); line_number++;
			line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			variable.variable_name = line; line_number++;
			variable.value = stof(lines[line_number]); line_number++;
			variable.variable_step = stof(lines[line_number]); line_number++;
			variable.active_variable_step = stof(lines[line_number]); line_number++;
			variable.slider_min = stof(lines[line_number]);   line_number++;
			variable.slider_max = stof(lines[line_number]);   line_number++;

			texture_parameters->variables.push_back(variable);
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "voxel_texture_import_export_class: read_expression_into_shader_parameters 3333 : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], INT_SHADER_BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
			//error message
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], INT_SHADER_BLOCK_END)) {
			if (line_number + 8 > lines.size()) {
				return false;
			}

			shader_parameter_int_variable_struct_type variable;
			variable.active_variable = stoi(lines[line_number]); line_number++;
			line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			variable.variable_name = line; line_number++;
			variable.value = stoi(lines[line_number]); line_number++;
			variable.variable_step = stoi(lines[line_number]); line_number++;
			variable.active_variable_step = stoi(lines[line_number]); line_number++;
			variable.slider_min = stoi(lines[line_number]); line_number++;
			variable.slider_max = stoi(lines[line_number]); line_number++;

			texture_parameters->int_variables.push_back(variable);
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "voxel_texture_import_export_class: read_expression_into_shader_parameters 4444 : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], BOOL_SHADER_BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
			//error message
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], BOOL_SHADER_BLOCK_END)) {
			if (line_number + 4 > lines.size()) {
				return false;
			}

			shader_parameter_bool_variable_struct_type variable;
			variable.active_variable = stoi(lines[line_number]); line_number++;
			line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			variable.variable_name = line; line_number++;
			variable.value = stoi(lines[line_number]); line_number++;

			texture_parameters->bool_variables.push_back(variable);
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "voxel_texture_import_export_class: read_expression_into_shader_parameters 5555 : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], SHADER_BLOCK_END)) {
			//error message
			return false;
		}

		return true;
	}



};

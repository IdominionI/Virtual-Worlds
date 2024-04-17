#include "hex_surface_function_import_export.h"

#include <vector>

#include <Universal_FW/Kernal/FWstring_tools.h>
#include <Universal_FW/Kernal/FWfile_tools.h>
#include <Universal_FW/Tools/dialogs.h> // Seems cannot have or use this in a .h file. Get already has a body compile errors

#include <VW_FrameWork/Shader/shader_parameters.h>


int hex_surface_function_import_export_class::open_file_stream(std::string filename, int write_mode) {
	if (write_mode == OVER_WRITE_FUNCTION_FILE) {
		stream.open(filename, std::ios::out | std::ios::trunc);// open for output (std::ios::out) and overwrite (std::ios::trunc)

		if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
	}
	else
		if (!stream.is_open()) return false;

	return true;
}

void hex_surface_function_import_export_class::close_file_stream() {
	stream.close();
}

bool hex_surface_function_import_export_class::save_hex_surface_generated_function(std::string file_pathname) {
	////QMessageBox::information(NULL, "", "in save_working_generated_function", //QMessageBox::Ok);

	if (file_pathname.size() == 0) {
		vwDialogs::display_error_message("save hex surface function","ERROR :\nNo file name defined to save data to \n Save Static Generated Function aborted\n");
		return false;
	}

	if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) return false;

	write_hex_generation_function_paramater_data(stream);

	close_file_stream();

	return true;
}

void hex_surface_function_import_export_class::write_hex_generation_function_paramater_data(std::fstream& stream) {
	stream << BLOCK_START << ENDL;

	stream << hex_surface_generator_parameters.expression_file_pathname << ENDL;
	stream << hex_surface_generator_parameters.expression_file_name << ENDL;

	stream << hex_surface_generator_parameters.x_start << ENDL;
	stream << hex_surface_generator_parameters.x_end << ENDL;
	stream << hex_surface_generator_parameters.y_start << ENDL;
	stream << hex_surface_generator_parameters.y_end << ENDL;

	stream << hex_surface_generator_parameters.invocation << ENDL;

	stream << hex_surface_generator_parameters.resolution_step << ENDL;

	stream << hex_surface_generator_parameters.min_surface_value << ENDL;
	stream << hex_surface_generator_parameters.max_surface_value << ENDL;

	stream << FLOAT_BLOCK_START << ENDL;

	for (hex_surface_generator_parameter_variable_struct_type variable : hex_surface_generator_parameters.variables) {
		stream << variable.active_variable << ENDL;
		stream << variable.variable_name.c_str() << ENDL; // Need to export as a const char* type to avoid trailing nulls at end of std::string type
		stream << variable.value << ENDL;
		stream << variable.variable_step << ENDL;
		stream << variable.active_variable_step << ENDL;
	}

	stream << FLOAT_BLOCK_END << ENDL;

	stream << INT_BLOCK_START << ENDL;

	for (hex_surface_generator_parameter_int_variable_struct_type variable : hex_surface_generator_parameters.int_variables) {
		stream << variable.active_variable << ENDL;
		stream << variable.variable_name.c_str() << ENDL;// Need to export as a const char* type to avoid trailing nulls at end of std::string type
		stream << variable.value << ENDL;
		stream << variable.variable_step << ENDL;
		stream << variable.active_variable_step << ENDL;
	}

	stream << INT_BLOCK_END << ENDL;

	stream << BOOL_BLOCK_START << ENDL;

	for (hex_surface_generator_parameter_bool_variable_struct_type variable : hex_surface_generator_parameters.bool_variables) {
		stream << variable.active_variable << ENDL;
		stream << variable.variable_name.c_str() << ENDL;// Need to export as a const char* type to avoid trailing nulls at end of std::string type
		stream << variable.value << ENDL;
	}

	stream << BOOL_BLOCK_END << ENDL;

	stream << BLOCK_END << ENDL;
}


// -------------------------------------------------------------------

bool hex_surface_function_import_export_class::import_hex_surface_genereated_function(hex_surface_generator_parameters_struct_type& generator_parameters, std::string file_pathname) {
	if (file_pathname.size() == 0) {
		vwDialogs::display_error_message("Import hex surface generated function","ERROR :\nNo file name defined to import data from \n Import Generated Function aborted");
		return false;
	}

	std::fstream working_model_file(file_pathname, std::ios::in);

	if (!working_model_file) {
		std::string error_message = "ERROR :\nCould not find read Import voxel generation model defined in file \n";
		error_message += file_pathname + "\n";
		vwDialogs::display_error_message("Import hex surface generated function",error_message);
		return false;
	}

	std::string working_model_string = FW::filetools::read_all(working_model_file);

	std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
	int line_number = 0;
	//return read_expression_into_hex_surface_generator_parameters(lines, generator_parameters, line_number);
	if (!read_expression_into_hex_surface_generator_parameters(lines, generator_parameters, line_number)) {
		std::string error_message = "ERROR :: \n Corrupted or out of sequence data at line\n";
		error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
        vwDialogs::display_error_message("Import hex surface generated function", error_message);
		return false;
	}

	return true;
}

//bool hex_surface_function_import_export_class::read_expression_into_hex_surface_generator_parameters(std::string working_model_string, hex_surface_generator_parameters_struct_type& generator_parameters) {
bool hex_surface_function_import_export_class::read_expression_into_hex_surface_generator_parameters(std::vector<std::string> lines, hex_surface_generator_parameters_struct_type &generator_parameters, int &line_number) {
	//QStringList lines = working_model_string.split('\n');// Create a list of strings for each line in the expression code
	//std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');

	std::string line;
	//int line_number = 0;

	// Find the line or index number in the list of lines of the expression that has the expression variable list defined 
	//while (!FW::stringtools::contains(lines[line_number], BLOCK_START))
	//	line_number++;

	//// If have not found a defined expression return an error
	//if (line_number >= lines.size()) {
	//	//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find start block to import parameter data.", //QMessageBox::Ok);
	//	return false;
	//}

	//if (lines.size() < line_number + 13) {
	//	//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n missing parameter data for filename and matrix definitions.", //QMessageBox::Ok);
	//	return false;
	//}

	line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 1111 : " << line_number << ":" << lines[line_number] << std::endl;
	if (!FW::stringtools::contains(lines[line_number], BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
		vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nCould not find parameter data block start flag.\n");
		return false;
	}
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 1111AAA : " << line_number << ":" << lines[line_number] << std::endl;
	line_number++;
	line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
	generator_parameters.expression_file_pathname = line; line_number++;
std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 1111BBB : " << line_number << ":" << lines[line_number] << std::endl;
	line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
	generator_parameters.expression_file_name = line; line_number++;
	//generator_parameters.x_start = stof(lines[line_number]); line_number++;
	//generator_parameters.x_end   = stof(lines[line_number]); line_number++;
	//generator_parameters.y_start = stof(lines[line_number]); line_number++;
	//generator_parameters.y_end   = stof(lines[line_number]); line_number++;

	//generator_parameters.invocation = stoi(lines[line_number]); line_number++;

	//generator_parameters.resolution_step = stof(lines[line_number]); line_number++;

	//generator_parameters.min_surface_value = stof(lines[line_number]); line_number++;
	//generator_parameters.max_surface_value = stof(lines[line_number]); line_number++;

	if(!FW::stringtools::string_to_float(lines[line_number], &generator_parameters.x_start, error_code)) return false; line_number++;
	if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.x_end,error_code))     return false; line_number++;
	if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.y_start,error_code))   return false; line_number++;
	if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.y_end,error_code))     return false; line_number++;

	if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.invocation,error_code)) return false;	line_number++;

	if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.resolution_step,error_code)) return false; line_number++;
	if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.min_surface_value,error_code)) return false; line_number++;
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 1111CCC : " << line_number << ":" << lines[line_number] << std::endl;
	if (!FW::stringtools::string_to_float(lines[line_number], &generator_parameters.max_surface_value, error_code)) return false; line_number++;

//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 2222 : " << line_number << ":" << lines[line_number] << std::endl;
	if (!FW::stringtools::contains(lines[line_number], FLOAT_BLOCK_START)) {
		vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nCould not find parameter float variables data block start flag.\n");
		return false;
	}
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 3333 : " << line_number << ":" << lines[line_number] << std::endl;
	line_number++;
	while (!FW::stringtools::contains(lines[line_number], FLOAT_BLOCK_END)) {
		if (line_number + 4 > lines.size()) {
			//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Have function float variable data incorrectly defined.", //QMessageBox::Ok);
			////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
			vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nHave function float variable data incorrectly defined.\n");
			return false;
		}

		hex_surface_generator_parameter_variable_struct_type variable;

		//variable.active_variable = stoi(lines[line_number]); line_number++;
		//line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		//variable.variable_name = line; line_number++;
		//variable.value = stof(lines[line_number]); line_number++;
		//variable.variable_step = stof(lines[line_number]); line_number++;
		//variable.active_variable_step = stoi(lines[line_number]); line_number++;

		if(!FW::stringtools::string_to_bool(lines[line_number],&variable.active_variable,error_code)) return false;	line_number++;
		line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		variable.variable_name = line; line_number++;
		if(!FW::stringtools::string_to_float(lines[line_number],&variable.value,error_code)) return false;	line_number++;
		if(!FW::stringtools::string_to_float(lines[line_number],&variable.variable_step,error_code)) return false;	line_number++;
		if(!FW::stringtools::string_to_bool(lines[line_number],&variable.active_variable_step,error_code)) return false; line_number++;

		generator_parameters.variables.push_back(variable);
	}

	line_number++;

	if (!FW::stringtools::contains(lines[line_number], INT_BLOCK_START)) {
		vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nCould not find parameter integer variables data block start flag.\n");
		return false;
	}

	line_number++;
	while (!FW::stringtools::contains(lines[line_number], INT_BLOCK_END)) {
		if (line_number + 4 > lines.size()) {
			////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
			vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nHave function integer variable data incorrectly defined.\n");
			return false;
		}

		hex_surface_generator_parameter_int_variable_struct_type int_variable;

		//int_variable.active_variable = stoi(lines[line_number]); line_number++;
		//line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		//int_variable.variable_name = line; line_number++;
		//int_variable.value = stoi(lines[line_number]); line_number++;
		//int_variable.variable_step = stoi(lines[line_number]); line_number++;
		//int_variable.active_variable_step = stoi(lines[line_number]); line_number++;

		if(!FW::stringtools::string_to_bool(lines[line_number],&int_variable.active_variable,error_code)) return false;	line_number++;
		line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		int_variable.variable_name = line; line_number++;
		if(!FW::stringtools::string_to_int(lines[line_number],&int_variable.value,error_code)) return false;	line_number++;
		if(!FW::stringtools::string_to_int(lines[line_number],&int_variable.variable_step,error_code)) return false; line_number++;
		if(!FW::stringtools::string_to_bool(lines[line_number],&int_variable.active_variable_step,error_code)) return false;line_number++;

		generator_parameters.int_variables.push_back(int_variable);

	}

	line_number++;
	if (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_START)) {
		vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nCould not find parameter boolean variables data block start flag.\n");
		return false;
	}

	line_number++;
	while (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_END)) {
		if (line_number + 2 > lines.size()) {
			////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
			vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nHave function boolean variable data incorrectly defined.\n");
			return false;
		}

		hex_surface_generator_parameter_bool_variable_struct_type bool_variable;

		//bool_variable.active_variable = stoi(lines[line_number]); line_number++;
		//line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		//bool_variable.variable_name = line; line_number++;
		//bool_variable.value = stoi(lines[line_number]); line_number++;

		if(!FW::stringtools::string_to_bool(lines[line_number],&bool_variable.active_variable,error_code)) return false;line_number++;
		line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		bool_variable.variable_name = line; line_number++;
		if(!FW::stringtools::string_to_bool(lines[line_number],&bool_variable.value,error_code)) return false;	line_number++;

		generator_parameters.bool_variables.push_back(bool_variable);

	}

//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters AAAA : " << line_number << ":" << lines[line_number] << std::endl;
    if (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_END)) { // There msut allways be at least two nodes. The input and output link nodes
       vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nCould not find parameter boolean variables data block end flag.\n");
        return false;
    }

    line_number++;
    line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters BBBB : " << line_number << ":" << lines[line_number] << std::endl;
    if (!FW::stringtools::contains(lines[line_number], BLOCK_END)) { // There msut allways be at least two nodes. The input and output link nodes
        vwDialogs::display_error_message("Read hex generator parameters","ERROR :\nCould not find parameter data block end flag.\n");
        return false;
    }

	return true;
}

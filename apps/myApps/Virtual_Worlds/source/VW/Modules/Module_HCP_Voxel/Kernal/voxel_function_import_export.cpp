#include "voxel_function_import_export.h"

#include <vector>

#include <Universal_FW/Kernal/FWstring_tools.h>
#include <Universal_FW/Kernal/FWfile_tools.h>
#include <Universal_FW/tools/dialogs.h>

#include <VW_FrameWork/Shader/shader_parameters.h>


	int voxel_function_import_export_class::open_file_stream(std::string filename, int write_mode) {
		if (write_mode == OVER_WRITE_FUNCTION_FILE) {
			stream.open(filename, std::ios::out | std::ios::trunc);// open for output (std::ios::out) and overwrite (std::ios::trunc)

			if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
		}
		else
			if (!stream.is_open()) return false;

		return true;
	}

	void voxel_function_import_export_class::close_file_stream() {
		stream.close();
	}

    // ----------- Voxel function import/exort -----------------

	bool voxel_function_import_export_class::save_voxel_generated_function(std::string file_pathname) {
////QMessageBox::information(NULL, "", "in save_working_generated_function", //QMessageBox::Ok);

		if (file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save Static Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

        if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
            vwDialogs::display_error_message("Save voxel function parameter data", "ERROR : Could not open file stream to export voxel parameter data\n");
            return false;
        }

		write_voxel_generation_function_paramater_data(stream);

		close_file_stream();

		return true;
	}

	void voxel_function_import_export_class::write_voxel_generation_function_paramater_data(std::fstream &stream) {
		stream << BLOCK_START << ENDL;

		stream << voxel_generator_parameters.expression_file_pathname << ENDL;
		stream << voxel_generator_parameters.expression_file_name << ENDL;

		stream << voxel_generator_parameters.x_start << ENDL;
		stream << voxel_generator_parameters.x_end   << ENDL;
		stream << voxel_generator_parameters.y_start << ENDL;
		stream << voxel_generator_parameters.y_end   << ENDL;
		stream << voxel_generator_parameters.z_start << ENDL;
		stream << voxel_generator_parameters.z_end   << ENDL;

		stream << voxel_generator_parameters.invocation << ENDL;

		stream << voxel_generator_parameters.resolution_step      << ENDL;
		stream << voxel_generator_parameters.generation_threshold << ENDL;

		stream << voxel_generator_parameters.min_surface_value << ENDL;
		stream << voxel_generator_parameters.max_surface_value << ENDL;

		stream << FLOAT_BLOCK_START << ENDL;

		for (voxel_generator_parameter_variable_struct_type variable : voxel_generator_parameters.variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL; // Need to export as a const char* type to avoid trailing nulls at end of std::string type
			stream << variable.value                 << ENDL;
			stream << variable.variable_step         << ENDL;
			stream << variable.active_variable_step  << ENDL;
		}

		stream << FLOAT_BLOCK_END << ENDL;

		stream << INT_BLOCK_START << ENDL;

		for (voxel_generator_parameter_int_variable_struct_type variable : voxel_generator_parameters.int_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;// Need to export as a const char* type to avoid trailing nulls at end of std::string type
			stream << variable.value                 << ENDL;
			stream << variable.variable_step         << ENDL;
			stream << variable.active_variable_step  << ENDL;
		}

		stream << INT_BLOCK_END << ENDL;

		stream << BOOL_BLOCK_START << ENDL;

		for (voxel_generator_parameter_bool_variable_struct_type variable : voxel_generator_parameters.bool_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;// Need to export as a const char* type to avoid trailing nulls at end of std::string type
			stream << variable.value                 << ENDL;
		}

		stream << BOOL_BLOCK_END << ENDL;

		stream << BLOCK_END << ENDL;
	}

	// -------------------------------------------------------------------

	bool voxel_function_import_export_class::import_voxel_generated_function(voxel_generator_parameters_struct_type &generator_parameters, std::string file_pathname) {
		if (file_pathname.size() == 0) {
            vwDialogs::display_error_message("Import voxel generation model", "ERROR :\n No file name defined to import data from \n Import Generated Function aborted\n");
			return false;
		}

		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
            std::string error_message = "ERROR :\n Could not read Import voxel generation file \n" + file_pathname;
            vwDialogs::display_error_message("Import voxel generation model", error_message);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);
		
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		int line_number = 0;
        error_code = 0;
		//return read_expression_into_voxel_generator_parameters(lines, generator_parameters, line_number);
        if (!read_expression_into_voxel_generator_parameters(lines, generator_parameters, line_number)) {
			std::string error_message = "ERROR :: Import voxel generation funtion failed.\n Corrupted or out of sequence data at line\n";
			error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
            vwDialogs::display_error_message("Import voxel generation model", error_message);
            return false;
        }

        return true;
	}

	bool voxel_function_import_export_class::read_expression_into_voxel_generator_parameters(std::vector<std::string> lines, voxel_generator_parameters_struct_type  &generator_parameters, int &line_number) {
		std::string line;

       line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 1111 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find start data block flag\n");
            return false;
        }

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
        generator_parameters.expression_file_pathname = line;
        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
        generator_parameters.expression_file_name     = line;

        line_number++;
        //generator_parameters.x_start = stof(lines[line_number]);line_number++;
        //generator_parameters.x_end   = stof(lines[line_number]);line_number++;
        //generator_parameters.y_start = stof(lines[line_number]);line_number++;
        //generator_parameters.y_end   = stof(lines[line_number]);line_number++;
        //generator_parameters.z_start = stof(lines[line_number]);line_number++;
        //generator_parameters.z_end   = stof(lines[line_number]);line_number++;

        //generator_parameters.invocation = stof(lines[line_number]); line_number++;

        //generator_parameters.resolution_step      = stof(lines[line_number]); line_number++;
        //generator_parameters.generation_threshold = stof(lines[line_number]); line_number++;

        //generator_parameters.min_surface_value = stof(lines[line_number]); line_number++;
        //generator_parameters.max_surface_value = stof(lines[line_number]);

	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.x_start, error_code)) return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.x_end,error_code))    return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.y_start,error_code))  return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.y_end,error_code))    return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.z_start,error_code))  return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.z_end,error_code))    return false; line_number++;

	    if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.invocation,error_code)) return false;	line_number++;

	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.resolution_step,error_code)) return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.generation_threshold,error_code)) return false; line_number++;
	   
        if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.min_surface_value, error_code)) return false; line_number++;
	    if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.max_surface_value, error_code)) return false;

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 2222 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], FLOAT_BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find start of float variables data block flag\n");
            return false;
        }

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 3333 : " << line_number << ":" << lines[line_number] << std::endl;
        voxel_generator_parameter_variable_struct_type variable;
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 3333AAAA : " << line_number << ":" << lines[line_number] << std::endl;
        while (!FW::stringtools::contains(lines[line_number], FLOAT_BLOCK_END)) {// Have not reached end of node pin data block
            //variable.active_variable       = stoi(lines[line_number]);line_number++;
            //line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
            //variable.variable_name         = line; line_number++;// Need to export as a const char* type to avoid trailing nulls at end of std::string type
            //variable.value                 = stof(lines[line_number]);line_number++;
            //variable.variable_step         = stof(lines[line_number]);line_number++;
            //variable.active_variable_step  = stoi(lines[line_number]);line_number++;

		    if(!FW::stringtools::string_to_bool(lines[line_number],&variable.active_variable,error_code)) return false;	line_number++;
		    line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		    variable.variable_name = line; line_number++;
		    if(!FW::stringtools::string_to_float(lines[line_number],&variable.value,error_code)) return false;	line_number++;
		    if(!FW::stringtools::string_to_float(lines[line_number],&variable.variable_step,error_code)) return false;	line_number++;
		    if(!FW::stringtools::string_to_bool(lines[line_number],&variable.active_variable_step,error_code)) return false; line_number++;

            generator_parameters.variables.push_back(variable);
        }

//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 4444 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], FLOAT_BLOCK_END)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find end of float variables data block flag\n");
            return false;
        }

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 5555 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], INT_BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find start of integer variables data block flag\n");
            return false;
        }

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 6666 : " << line_number << ":" << lines[line_number] << std::endl;
        voxel_generator_parameter_int_variable_struct_type int_variable;
        while (!FW::stringtools::contains(lines[line_number], INT_BLOCK_END)) {// Have not reached end of node pin data block
            //int_variable.active_variable       = stoi(lines[line_number]);line_number++;
            //line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
            //int_variable.variable_name         = line; line_number++; // Need to export as a const char* type to avoid trailing nulls at end of std::string type
            //int_variable.value                 = stoi(lines[line_number]);line_number++;
            //int_variable.variable_step         = stoi(lines[line_number]);line_number++;
            //int_variable.active_variable_step  = stoi(lines[line_number]);line_number++;

		    if(!FW::stringtools::string_to_bool(lines[line_number],&int_variable.active_variable,error_code)) return false;	line_number++;
		    line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		    int_variable.variable_name = line; line_number++;
		    if(!FW::stringtools::string_to_int(lines[line_number],&int_variable.value,error_code)) return false;	line_number++;
		    if(!FW::stringtools::string_to_int(lines[line_number],&int_variable.variable_step,error_code)) return false; line_number++;
		    if(!FW::stringtools::string_to_bool(lines[line_number],&int_variable.active_variable_step,error_code)) return false;line_number++;

            generator_parameters.int_variables.push_back(int_variable);
        }

//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 7777 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], INT_BLOCK_END)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find end of integer variables data block flag\n");
            return false;
        }

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 8888 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find start of boolean variables data block flag\n");
            return false;
        }

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters 9999 : " << line_number << ":" << lines[line_number] << std::endl;
        voxel_generator_parameter_bool_variable_struct_type bool_variable;
        while (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_END)) {// Have not reached end of node pin data block
            //int_variable.active_variable       = stoi(lines[line_number]);line_number++;
            //line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
            //int_variable.variable_name         = line; line_number++; // Need to export as a const char* type to avoid trailing nulls at end of std::string type
            //int_variable.value                 = stoi(lines[line_number]);line_number++;

		    if(!FW::stringtools::string_to_bool(lines[line_number],&bool_variable.active_variable,error_code)) return false;line_number++;
		    line = lines[line_number];   line = FW::stringtools::truncate(line, line.size());
		    bool_variable.variable_name = line; line_number++;
		    if(!FW::stringtools::string_to_bool(lines[line_number],&bool_variable.value,error_code)) return false;	line_number++;

            generator_parameters.bool_variables.push_back(bool_variable);
        }

//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters AAAA : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_END)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find end of boolean variables data block flag\n");
            return false;
        }

        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "voxel_function_import_export_class: read_expression_into_voxel_generator_parameters BBBB : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], BLOCK_END)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read voxel generator parameters", "ERROR :\n Could not find end of voxel parameter data block flag\n");
            return false;
        }

        return true;
	}

    // ----------- Cart to HCP Voxel import/exort -----------------

    bool voxel_function_import_export_class::save_cart_to_hcp_voxel(std::string file_pathname) {
//QMessageBox::information(NULL, "", "in save_working_generated_function", //QMessageBox::Ok);

        if (file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save Static Generated Function aborted", //QMessageBox::Ok);
            return false;
        }

        if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) return false;

        write_cart_to_hcp_voxel_generation_function_paramater_data(stream);

        close_file_stream();

        return true;
    }

    void voxel_function_import_export_class::write_cart_to_hcp_voxel_generation_function_paramater_data(std::fstream& stream) {
        stream << BLOCK_START << ENDL;

        //stream << cart_to_hcp_voxel_generator_parameters.display_as_points << std::endl;
        //stream << cart_to_hcp_voxel_generator_parameters.voxel_scale       << std::endl;
        //stream << cart_to_hcp_voxel_generator_parameters.min_scale_value   << std::endl;
        //stream << cart_to_hcp_voxel_generator_parameters.max_scale_value   << std::endl;
        
        stream << cart_to_hcp_voxel_generator_parameters.ply_file_pathname << std::endl;
        stream << cart_to_hcp_voxel_generator_parameters.ply_file_name     << std::endl;
        
        stream << cart_to_hcp_voxel_generator_parameters.random_type       << std::endl;// ????

        stream << cart_to_hcp_voxel_generator_parameters.x_start << std::endl;
		stream << cart_to_hcp_voxel_generator_parameters.x_end   << std::endl;
		stream << cart_to_hcp_voxel_generator_parameters.y_start << std::endl;
		stream << cart_to_hcp_voxel_generator_parameters.y_end   << std::endl;
		stream << cart_to_hcp_voxel_generator_parameters.z_start << std::endl;
		stream << cart_to_hcp_voxel_generator_parameters.z_end   << std::endl;
		
        stream << cart_to_hcp_voxel_generator_parameters.generation_type << std::endl;// ????
        
        stream << cart_to_hcp_voxel_generator_parameters.number_of_points << std::endl;// ????
        stream << cart_to_hcp_voxel_generator_parameters.ctov_scale       << std::endl;// ????
        stream << cart_to_hcp_voxel_generator_parameters.voxel_size       << std::endl;// ????
        stream << cart_to_hcp_voxel_generator_parameters.min_data_value   << std::endl;// ????
        stream << cart_to_hcp_voxel_generator_parameters.max_data_value   << std::endl;// ????

        stream << BLOCK_END << ENDL;
    }


    bool voxel_function_import_export_class::import_cart_to_voxel_generated_function(cart_to_hcp_voxel_generator_parameters_struct_type &generator_parameters, std::string file_pathname) {
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

        std::string working_model_string = FW::filetools::read_all(working_model_file);

        std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
        int line_number = 0;
        error_code = 0;
        
        //return read_cart_to_voxel_parameter_data(lines, generator_parameters, line_number);
        if (!read_cart_to_voxel_parameter_data(lines, generator_parameters, line_number)) {
			std::string error_message = "ERROR :: Import cart to voxel generation failed\n corrupted or out of sequence data at line\n";
			error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
            vwDialogs::display_error_message("Import cart to voxel generation function", error_message);
            return false;
        }

        return true;
    }
    
    bool voxel_function_import_export_class::read_cart_to_voxel_parameter_data(std::vector<std::string> lines, cart_to_hcp_voxel_generator_parameters_struct_type &generator_parameters, int& line_number) {
 		std::string line;

       line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "voxel_function_import_export_class: read_cart_to_voxel_parameter_data 1111 : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], BLOCK_START)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read cart to voxel parameter data", "ERROR :\n Could not find start data block flag\n");
            return false;
        }

        line_number++;

        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
        generator_parameters.ply_file_pathname = line;
        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
        generator_parameters.ply_file_name = line;

        line_number++;
        //generator_parameters.random_type = stoi(lines[line_number]); line_number++;
        if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.random_type,error_code)) return false;	line_number++;

        //generator_parameters.x_start = stof(lines[line_number]);line_number++;
        //generator_parameters.x_end   = stof(lines[line_number]);line_number++;
        //generator_parameters.y_start = stof(lines[line_number]);line_number++;
        //generator_parameters.y_end   = stof(lines[line_number]);line_number++;
        //generator_parameters.z_start = stof(lines[line_number]);line_number++;
        //generator_parameters.z_end   = stof(lines[line_number]);line_number++;

        //generator_parameters.generation_type = stoi(lines[line_number]); line_number++;

        //generator_parameters.number_of_points = stoi(lines[line_number]); line_number++;
        //generator_parameters.ctov_scale       = stof(lines[line_number]); line_number++;
        //generator_parameters.voxel_size       = stof(lines[line_number]); line_number++;
        //generator_parameters.min_data_value   = stof(lines[line_number]); line_number++;
        //generator_parameters.max_data_value   = stof(lines[line_number]);

	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.x_start, error_code)) return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.x_end,error_code))    return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.y_start,error_code))  return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.y_end,error_code))    return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.z_start,error_code))  return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.z_end,error_code))    return false; line_number++;

	    if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.generation_type,error_code)) return false;	line_number++;

	    if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.number_of_points,error_code)) return false;	line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.ctov_scale,error_code)) return false; line_number++;
	    if(!FW::stringtools::string_to_float(lines[line_number],&generator_parameters.voxel_size,error_code)) return false; line_number++;
	    if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.min_data_value, error_code)) return false; line_number++;
	    if(!FW::stringtools::string_to_int(lines[line_number],&generator_parameters.max_data_value, error_code)) return false; line_number++;


        line_number++;
        line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "voxel_function_import_export_class: read_cart_to_voxel_parameter_data BBBB : " << line_number << ":" << lines[line_number] << std::endl;
        if (!FW::stringtools::contains(lines[line_number], BLOCK_END)) { // There msut allways be at least two nodes. The input and output link nodes
            vwDialogs::display_error_message("Read cart to voxel parameter data", "ERROR :\n Could not find end data block flag\n");
            return false;
        }

        return true;   
    }

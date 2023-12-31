#pragma once

#include <fstream>
#include <string>

#include "../Object/DataTypes/dt_voxel_generator.h"
#include "../Object/DataTypes/dt_cart_to_voxel_generator.h"

#define OVER_WRITE_FUNCTION_FILE    100
#define READ_FUNCTION_FILE          101

#define STATIC        1000
#define DYNAMIC       1001
#define WORKING_MODEL 1002

#define BLOCK_START "["
#define BLOCK_END   "]"

#define FLOAT_BLOCK_START "<"
#define FLOAT_BLOCK_END   ">"

#define INT_BLOCK_START "{"
#define INT_BLOCK_END   "}"

#define BOOL_BLOCK_START "("
#define BOOL_BLOCK_END   ")"

#define ENDL '\n' 

/*
	voxel_function_import_export_class

	C++ Class that handles the import and export of hcp voxel point cloud parameter data 
	from and to a formatted text file so as for the user to save and reuse paramaters without
	having to reenter the data from scratch.

	Method of file format to read/write this data is to flag each parameter witha <>_BLOCK_START flag
	on a line to indicate the fllowing lines up to a <>_BLOCK_END flag is a parameter. The parameter data
	is also in a particular order that dictates which line betweeen these flags corresponds to a 
	particular prarmeter value.

	In a similar design, the BLOCK_START flag gives an indication that all lines up to a
	BLOCK_END flag are a set of paramater data block that have individual parameter data
	values bounded by <>_BLOCK_START and <>_BLOCK_END flags.
*/


class voxel_function_import_export_class {
public:
	std::fstream stream;
	std::string  input_line;

	voxel_generator_parameters_struct_type voxel_generator_parameters;

	int  open_file_stream(std::string filename, int write_mode);
	void close_file_stream();
	
	// ----------- Voxel function import/exort -----------------

	bool save_voxel_generated_function(std::string file_pathname);
	void write_voxel_generation_function_paramater_data(std::fstream &stream);
	bool import_voxel_generated_function(voxel_generator_parameters_struct_type& generator_parameters, std::string file_pathname);
	bool read_expression_into_voxel_generator_parameters(std::vector<std::string> lines, voxel_generator_parameters_struct_type& generator_parameters, int& line_number);

	// ----------- Cart to HCP Voxel import/exort -----------------

	cart_to_hcp_voxel_generator_parameters_struct_type cart_to_hcp_voxel_generator_parameters;

	bool save_cart_to_hcp_voxel(std::string file_pathname);
	void write_cart_to_hcp_voxel_generation_function_paramater_data(std::fstream& stream);
	bool import_cart_to_voxel_generated_function(cart_to_hcp_voxel_generator_parameters_struct_type &generator_parameters, std::string file_pathname);
	bool read_cart_to_voxel_parameter_data(std::vector<std::string> lines, cart_to_hcp_voxel_generator_parameters_struct_type &generator_parameters, int& line_number);
};

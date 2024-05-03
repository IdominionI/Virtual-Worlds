#pragma once
#include <string>

// Voxel generator parameters to convert cloud point data in
// cartesian coordinates to hcp voxel data

struct cart_to_hcp_voxel_generator_parameters_struct_type {
	std::string ply_file_pathname = "";
	std::string ply_file_name = "...";

	int random_type = 0;// ????

	float	 x_start = -1.0f, x_end = 1.0f;
	float	 y_start = -1.0f, y_end = 1.0f;
	float	 z_start = -1.0f, z_end = 1.0f;

	int generation_type = 1;// ????

	int number_of_points = 1000;

	float ctov_scale = 10.0f; // testing only

	float voxel_size = 1.0f;

	int min_data_value = 1, max_data_value = 100;
};
#pragma once

#include "../Object/hex_surface_object.h"

// +++++++++++++++++ 3D Brush ++++++++++++++++++++++
enum class hex_brush_type_enum {cubic, ellipse};
enum class hex_brush_falloff_enum {none,step,linear}; // More added later ???? eg cubic

struct hex_brush_falloff_struct_type {
	float radius = 1.0f;
	float value  = 1.0f;
	hex_brush_falloff_enum falloff_type = hex_brush_falloff_enum::step;

	bool inside_brush_falloff(glm::vec2 coord_radius, glm::vec2 brush_radius) {
		glm::vec2 falloff_radius = radius*brush_radius;
		return (coord_radius.x <= falloff_radius.x && coord_radius.y <= falloff_radius.y);
		//return (coord_radius.x <= falloff_radius.x && coord_radius.y <= falloff_radius.y && coord_radius.z <= falloff_radius.z);
	}

	void set_falloff_radius(float f_radius) {
		if (f_radius < 0.0f) f_radius = 0.0f;
		if (f_radius > 1.0f) f_radius = 1.0f;

		radius = f_radius;
	}

	void set_falloff_value(float f_value) {
		if (f_value < 0.0f) f_value = 0.0f;
		if (f_value > 1.0f) f_value = 1.0f;

		value = f_value;
	}
};

struct hex_brush_struct_type {
	int         id;// Not sure is needed
	std::string name;
	hex_brush_type_enum type;

	//int brush_hex_value = 255;
	int brush_hex_value = 140;

	glm::vec2 radius_2d = { 1.0f,1.0f};

	std::vector<hex_brush_falloff_struct_type> falloff;// = { {0.0,0.0),(1.0,0.0)};

	//virtual void apply_brush_to_int_hex_value(glm::vec3 brush_center_coord, glm::vec3 voxel_coord, int &hex_value) {
	virtual void apply_brush_to_int_hex_value(glm::vec2 brush_center_coord, glm::vec2 voxel_coord, float &hex_value) {
		float radius = glm::distance(brush_center_coord, voxel_coord);

//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_hex_value AAAAA : " << std::endl;
		if (!inside_brush_volume(brush_center_coord, voxel_coord)) return;
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_hex_value BBBBB : " << std::endl;
		glm::vec2 voxel_brush_coord_radius = get_2d_coord_radius(brush_center_coord, voxel_coord);

		for(int i = 0; i<falloff.size(); i++){
			if (i + 1 < falloff.size()) { // linear falloff

				if (falloff[i+1].inside_brush_falloff(voxel_brush_coord_radius,radius_2d) && !falloff[i].inside_brush_falloff(voxel_brush_coord_radius,radius_2d)) {
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_hex_value 00000 : " << std::endl;
					if (falloff[i].falloff_type == hex_brush_falloff_enum::linear) {
						if (type == hex_brush_type_enum::cubic) {
							glm::vec2 voxel_fallout_coord_radius1 = get_2d_coord_radius(brush_center_coord, falloff[i].radius*radius_2d);
							glm::vec2 voxel_fallout_coord_radius2 = get_2d_coord_radius(brush_center_coord, falloff[i + 1].radius*radius_2d);

							float mx = (falloff[i + 1].value - falloff[i].value)/(voxel_fallout_coord_radius2.x - voxel_fallout_coord_radius1.x);
							float my = (falloff[i + 1].value - falloff[i].value)/(voxel_fallout_coord_radius2.y - voxel_fallout_coord_radius1.y);
							//float mz = (falloff[i + 1].value - falloff[i].value)/(voxel_fallout_coord_radius2.z - voxel_fallout_coord_radius1.z);

							glm::vec2 fallof_coord_radius = get_2d_coord_radius(brush_center_coord, voxel_coord);

							float frx = fallof_coord_radius.x - voxel_fallout_coord_radius1.x;
							float fry = fallof_coord_radius.y - voxel_fallout_coord_radius1.y;
							//float frz = fallof_coord_radius.z - voxel_fallout_coord_radius1.z;

							float fvx = (mx * frx + falloff[i].value) * brush_hex_value;
							float fvy = (my * fry + falloff[i].value) * brush_hex_value;
							//float fvz = (mz * frz + falloff[i].value) * brush_hex_value;

							hex_value = int((fvx+fvy)/2.0f);
						}
					} else { // step falloff
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_hex_value 11111 : " << hex_value << " : " << int(falloff[i].value * brush_hex_value) << std::endl;
						hex_value = int(falloff[i].value * brush_hex_value);
					}
					break;
				}

			} else{// no falloff
				if (falloff[i].inside_brush_falloff(voxel_brush_coord_radius,radius_2d)) {
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_hex_value 222222 : " << hex_value << " : " << int(falloff[i].value * brush_hex_value) << std::endl;
					hex_value = int(falloff[i].value * brush_hex_value);
					break;
				}
			}
		}
	}

	void define_brush_falloff_element(hex_brush_falloff_struct_type _falloff) {
		for (size_t i = 0; i < falloff.size(); i++) {
			if (falloff[i].radius == _falloff.radius) {
				falloff[i] = _falloff;
				return;
			}

			if (i + 1 < falloff.size()) {
				if (falloff[i].radius < _falloff.radius && _falloff.radius < falloff[i + 1].radius) {
					falloff.insert(falloff.begin() + i, _falloff);
					return;
				}
			}
		}

		falloff.push_back(_falloff);
	}

	bool inside_brush_volume(glm::vec2 brush_center_coord, glm::vec2 voxel_coord) {
		if (type == hex_brush_type_enum::cubic) 
			return inside_cubic_brush(brush_center_coord, voxel_coord);
		else
			return inside_ellipse_brush(brush_center_coord, voxel_coord);
	}

	bool inside_cubic_brush(glm::vec2 brush_center_coord, glm::vec2 voxel_coord) {
		glm::vec2 r = get_2d_coord_radius(brush_center_coord, voxel_coord);

//std::cout << "voxel_3d_brush_struct_type::inside_cubic_brush : " << r.x << ":"<< r.y << ":" << r.z << std::endl;

		return r.x <= radius_2d.x && r.y <= radius_2d.y;
	}

	bool inside_ellipse_brush(glm::vec2 brush_center_coord, glm::vec2 voxel_coord) {
		glm::vec2 r = get_2d_coord_radius(brush_center_coord, voxel_coord);

		float er = std::powf(r.x / radius_2d.x,2.0f) + std::powf(r.y / radius_2d.y,2.0f);
		if (er <= 1.0f)
			return true;
		else
			return false;
	}

	glm::vec2 get_2d_coord_radius(glm::vec2 center_coord, glm::vec2 r_coord) {
		float rx = abs(center_coord.x - r_coord.x);
		float ry = abs(center_coord.y - r_coord.y);
		//float rz = abs(center_coord.z - r_coord.z);

		return {rx,ry};
	}

	float voxel_brush_radius(glm::vec3 brush_center_coord, glm::vec3 voxel_coord) {
		return glm::distance(brush_center_coord, voxel_coord);
	}
};


struct hex_brush_volume_data_struct_type {
	int index = -1;
	glm::vec3 cart_coord = { 0.0f,0.0f,0.0f };
};

class hex_brush_class {
public:
	hex_brush_struct_type hex_brush;

	bool apply_brush(hex_surface_object_class *hex_surface_object_to_execute,int center_voxel_index) {
		if (!hex_surface_object_to_execute) return false;

		brush_voxels.clear();
		brush_voxels.shrink_to_fit();

		add_voxel_brush_volume_data(hex_surface_object_to_execute, center_voxel_index);

		size_t no_brush_voxels = brush_voxels.size();

//std::cout << "hcp_voxel_brush_class::apply_brush : " << no_brush_voxels << std::endl;

		for (size_t i = 0; i < no_brush_voxels; i++) {
			hex_surface_data_type &hex_value = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data[brush_voxels[i].index];
			hex_brush.apply_brush_to_int_hex_value(central_brush_voxel_cart_coord,brush_voxels[i].cart_coord,hex_value);

//std::cout << "hcp_voxel_brush_class::apply_brush : " << i << " : " << brush_voxels[i].index << " : " <<hex_value <<  " : "<<  hex_surface_object_to_execute->hex_surface_object_data.voxel_matrix_data[brush_voxels[i].index] << std::endl;
		}

		hex_surface_object_to_execute->define_geometry_data();

		return true;
	}

private:
	void add_voxel_brush_volume_data(hex_surface_object_class *hex_surface_object_to_execute,int center_voxel_index) {
		std::vector <hex_surface_data_type> &voxel_matrix_data = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data;

	central_brush_voxel_cart_coord = hex_surface_object_to_execute->hex_surface_object_data.get_hex_surface_world_cartesian_coordinate(center_voxel_index);
//std::cout << "hcp_voxel_brush_class::add_voxel_brush_volume_data : " << central_brush_voxel_cart_coord.x << ":"<< central_brush_voxel_cart_coord.y << ":" << central_brush_voxel_cart_coord.z << std::endl;
		// Create a voxel brush volume based upon the x,y,z brush radius inputs 

		float voxel_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size;
		float sqrt3 = sqrt(3.0f), z_mult = 2.0f * sqrt(6.0f) / 3.0f;

		// delta voxel index coordinates that define the index radius or range from the central brush voxel index coordinate
		int dx;
		int dy;

		//centrak brush voxel index coordinate
		glm::ivec3 voxel_matrix_coord = hex_surface_object_to_execute->hex_surface_object_data.get_matrix_coordinate(center_voxel_index);

		// Iterate around the central brush coordinate in the range of the index radial distances for each i,j,k coordinates.
		// Need to take into account the even/odd rows and levels that the vcentral brush voxel falls in.
		dy = int(hex_brush.radius_2d.y / (sqrt3*voxel_size))-1;
		for (int j = voxel_matrix_coord.y - dy; j <= voxel_matrix_coord.y + dy; j++) {
			if (j > -1 && j < hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.y) {

				if ((j) % 2 == 0) {//even column
					dx = int(hex_brush.radius_2d.x / (sqrt3*voxel_size))-1;
				} else{// odd column
					dx = int(hex_brush.radius_2d.y / (sqrt3*voxel_size));
				}

				for (int i = voxel_matrix_coord.x - dx; i <= voxel_matrix_coord.x + dx; i++) {
					if (i > -1 && i < hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.x) {
						index_data_type matrix_index = hex_surface_object_to_execute->hex_surface_object_data.get_hex_surface_matrix_data_index({ i,j});
						glm::vec3 voxel_cart_coord   = hex_surface_object_to_execute->hex_surface_object_data.get_hex_surface_world_cartesian_coordinate({ i,j,0 });

//std::cout << "hcp_voxel_brush_class::apply_brush 00001 : " << i << " : " << j << " : " << k << " : " << matrix_index << std::endl;

						if (hex_brush.inside_brush_volume(central_brush_voxel_cart_coord, voxel_cart_coord)) {
							add_voxel_brush_volume_voxel(matrix_index,voxel_cart_coord);
//std::cout << "hcp_voxel_brush_class::apply_brush 00002 : " << i << " : " << j << " : " << k << " : " << matrix_index << std::endl;
						}
					}
				}

			}	
		}
	}

	void add_voxel_brush_volume_voxel(int index,glm::vec3 voxel_cart_coord) {
		hex_brush_volume_data_struct_type hcp_voxel_brush_volume_data;

		hcp_voxel_brush_volume_data.index      = index;
		hcp_voxel_brush_volume_data.cart_coord = voxel_cart_coord;

		brush_voxels.push_back(hcp_voxel_brush_volume_data);
	}

	//void update_shader_brush_data() {
	// Not yet implemented
	//}

	glm::vec3 central_brush_voxel_cart_coord = { 0.0f,0.0f,0.0f };

	//hex_surface_object_data_class &hex_surface_object_data; // Cannot use as this creates a compile error in scene_node_editor_widget.cpp 

	std::vector<hex_brush_volume_data_struct_type> brush_voxels;
};


// ++++++++++++++++++++++++++++++++++++++++
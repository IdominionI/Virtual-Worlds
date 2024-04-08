#pragma once

#include "../Object/voxel_hcp_object.h"

// +++++++++++++++++ 3D Brush ++++++++++++++++++++++
enum class voxel_3d_brush_type_enum {cubic, ellipse};
enum class voxel_3d_brush_falloff_enum {none,step,linear}; // More added later ???? eg cubic

struct voxel_brush_falloff_struct_type {
	// 0.0 >= radius, value  <= 1.0
	float radius = 1.0f;
	float value  = 1.0f;
	voxel_3d_brush_falloff_enum falloff_type = voxel_3d_brush_falloff_enum::step;

	bool inside_brush_falloff(glm::vec3 coord_radius, glm::vec3 brush_radius) {
		glm::vec3 falloff_radius = radius*brush_radius;
		return (coord_radius.x <= falloff_radius.x && coord_radius.y <= falloff_radius.y && coord_radius.z <= falloff_radius.z);
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

struct voxel_3d_brush_struct_type {
	int         id;// Not sure is needed
	std::string name;
	voxel_3d_brush_type_enum type;

	//int brush_voxel_value = 255;
	int brush_voxel_value = 140;

	glm::vec3 radius_3d = { 1.0f,1.0f,1.0f };

	std::vector<voxel_brush_falloff_struct_type> falloff;// = { {0.0,0.0),(1.0,0.0)};

	virtual void apply_brush_to_int_voxel_value(glm::vec3 brush_center_coord, glm::vec3 voxel_coord, int &voxel_value) {
		float radius = glm::distance(brush_center_coord, voxel_coord);

//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_voxel_value AAAAA : " << std::endl;
		if (!inside_brush_volume(brush_center_coord, voxel_coord)) return;
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_voxel_value BBBBB : " << std::endl;
		glm::vec3 voxel_brush_coord_radius = get_3d_coord_radius(brush_center_coord, voxel_coord);

		for(int i = 0; i<falloff.size(); i++){
			if (i + 1 < falloff.size()) { // linear falloff

				if (falloff[i+1].inside_brush_falloff(voxel_brush_coord_radius,radius_3d) && !falloff[i].inside_brush_falloff(voxel_brush_coord_radius,radius_3d)) {
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_voxel_value 00000 : " << std::endl;
					if (falloff[i].falloff_type == voxel_3d_brush_falloff_enum::linear) {
						if (type == voxel_3d_brush_type_enum::cubic) {
							glm::vec3 voxel_fallout_coord_radius1 = get_3d_coord_radius(brush_center_coord, falloff[i].radius*radius_3d);
							glm::vec3 voxel_fallout_coord_radius2 = get_3d_coord_radius(brush_center_coord, falloff[i + 1].radius*radius_3d);

							float mx = (falloff[i + 1].value - falloff[i].value)/(voxel_fallout_coord_radius2.x - voxel_fallout_coord_radius1.x);
							float my = (falloff[i + 1].value - falloff[i].value)/(voxel_fallout_coord_radius2.y - voxel_fallout_coord_radius1.y);
							float mz = (falloff[i + 1].value - falloff[i].value)/(voxel_fallout_coord_radius2.z - voxel_fallout_coord_radius1.z);

							glm::vec3 fallof_coord_radius = get_3d_coord_radius(brush_center_coord, voxel_coord);

							float frx = fallof_coord_radius.x - voxel_fallout_coord_radius1.x;
							float fry = fallof_coord_radius.y - voxel_fallout_coord_radius1.y;
							float frz = fallof_coord_radius.z - voxel_fallout_coord_radius1.z;

							float fvx = (mx * frx + falloff[i].value) * brush_voxel_value;
							float fvy = (my * fry + falloff[i].value) * brush_voxel_value;
							float fvz = (mz * frz + falloff[i].value) * brush_voxel_value;

							voxel_value = int((fvx+fvy+fvz)/3.0f);
						}
					} else { // step falloff
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_voxel_value 11111 : " << voxel_value << " : " << int(falloff[i].value * brush_voxel_value) << std::endl;
						voxel_value = int(falloff[i].value * brush_voxel_value);
					}
					break;
				}

			} else{// no falloff
				if (falloff[i].inside_brush_falloff(voxel_brush_coord_radius,radius_3d)) {
//std::cout << "voxel_3d_brush_struct_type::apply_brush_to_int_voxel_value 222222 : " << voxel_value << " : " << int(falloff[i].value * brush_voxel_value) << std::endl;
					voxel_value = int(falloff[i].value * brush_voxel_value);
					break;
				}
			}
		}
	}

	void define_brush_falloff_element(voxel_brush_falloff_struct_type _falloff) {
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

	bool inside_brush_volume(glm::vec3 brush_center_coord, glm::vec3 voxel_coord) {
		if (type == voxel_3d_brush_type_enum::cubic) 
			return inside_cubic_brush(brush_center_coord, voxel_coord);
		else
			return inside_ellipse_brush(brush_center_coord, voxel_coord);
	}

	bool inside_cubic_brush(glm::vec3 brush_center_coord, glm::vec3 voxel_coord) {
		glm::vec3 r = get_3d_coord_radius(brush_center_coord, voxel_coord);

//std::cout << "voxel_3d_brush_struct_type::inside_cubic_brush : " << r.x << ":"<< r.y << ":" << r.z << std::endl;

		return r.x <= radius_3d.x && r.y <= radius_3d.y && r.z <= radius_3d.z;
	}

	bool inside_ellipse_brush(glm::vec3 brush_center_coord, glm::vec3 voxel_coord) {
		glm::vec3 r = get_3d_coord_radius(brush_center_coord, voxel_coord);

		float er = std::powf(r.x / radius_3d.x,2.0f) + std::powf(r.y / radius_3d.y,2.0f) + std::powf(r.z / radius_3d.z,2.0f);
		if (er <= 1.0f)
			return true;
		else
			return false;
	}

	glm::vec3 get_3d_coord_radius(glm::vec3 center_coord, glm::vec3 r_coord) {
		float rx = abs(center_coord.x - r_coord.x);
		float ry = abs(center_coord.y - r_coord.y);
		float rz = abs(center_coord.z - r_coord.z);

		return {rx,ry,rz};
	}

	float voxel_brush_radius(glm::vec3 brush_center_coord, glm::vec3 voxel_coord) {
		return glm::distance(brush_center_coord, voxel_coord);
	}
};


struct hcp_voxel_brush_volume_data_struct_type {
	int index = -1;
	glm::vec3 cart_coord = { 0.0f,0.0f,0.0f };
};

class hcp_voxel_brush_class {
public:
	voxel_3d_brush_struct_type hcp_voxel_3d_brush;

	bool apply_brush(voxel_hcp_object_class *voxel_hcp_object_to_execute,int center_voxel_index) {
		if (!voxel_hcp_object_to_execute) return false;

		brush_voxels.clear();
		brush_voxels.shrink_to_fit();

		add_voxel_brush_volume_data(voxel_hcp_object_to_execute, center_voxel_index);

		size_t no_brush_voxels = brush_voxels.size();

//std::cout << "hcp_voxel_brush_class::apply_brush : " << no_brush_voxels << std::endl;

		for (size_t i = 0; i < no_brush_voxels; i++) {
			voxel_data_type &voxel_value = voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data[brush_voxels[i].index];
			hcp_voxel_3d_brush.apply_brush_to_int_voxel_value(central_brush_voxel_cart_coord,brush_voxels[i].cart_coord,voxel_value);

//std::cout << "hcp_voxel_brush_class::apply_brush : " << i << " : " << brush_voxels[i].index << " : " <<voxel_value <<  " : "<<  voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data[brush_voxels[i].index] << std::endl;
		}

		voxel_hcp_object_to_execute->define_geometry_data();

		return true;
	}

private:
	void add_voxel_brush_volume_data(voxel_hcp_object_class *voxel_hcp_object_to_execute,int center_voxel_index) {
		std::vector <voxel_data_type> &voxel_matrix_data = voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data;


		central_brush_voxel_cart_coord = voxel_hcp_object_to_execute->voxel_object_data.get_voxel_world_cartesian_coordinate(center_voxel_index);
//std::cout << "hcp_voxel_brush_class::add_voxel_brush_volume_data : " << central_brush_voxel_cart_coord.x << ":"<< central_brush_voxel_cart_coord.y << ":" << central_brush_voxel_cart_coord.z << std::endl;
		
		// The following very inefficient and can be replaced with more complicated method yet to be derived
		// to directly get all indexed voxel matrix entries by index alone
//		size_t voxel_matrix_data_size =voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size();
//		for (int i = 0; i < voxel_matrix_data_size; i++) {
//			glm::vec3 voxel_cart_coord = voxel_hcp_object_to_execute->voxel_object_data.get_voxel_world_cartesian_coordinate(i);
////std::cout << "hcp_voxel_brush_class::add_voxel_brush_volume_data : " << voxel_cart_coord.x << ":"<< voxel_cart_coord.y << ":" << voxel_cart_coord.z << std::endl;
//
//			if (hcp_voxel_3d_brush.inside_brush_volume(central_brush_voxel_cart_coord, voxel_cart_coord)) {
//				add_voxel_brush_volume_voxel(i,voxel_cart_coord);
//			}
//		}

		// **************************
		// Create a voxel brush volume based upon the x,y,z brush radius inputs 

		float voxel_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_size;

		float sqrt3 = sqrt(3.0f), z_mult = 2.0f * sqrt(6.0f) / 3.0f;
		//float third = 1.0f / 3.0f, z_mult = 2.0f * sqrt(6.0f) / 3.0f;

		// delta voxel index coordinates that define the index radius or range from the central brush voxel index coordinate
		int dx;// = int(hcp_voxel_3d_brush.radius_3d.x / voxel_size);
		int dy;//= int(hcp_voxel_3d_brush.radius_3d.y / (sqrt3 * voxel_size));

		//centrak brush voxel index coordinate
		glm::ivec3 voxel_matrix_coord = voxel_hcp_object_to_execute->voxel_object_data.get_matrix_coordinate(center_voxel_index);

		// Iterate around the central brush coordinate in the range of the index radial distances for each i,j,k coordinates.
		// Need to take into account the even/odd rows and levels that the vcentral brush voxel falls in.
		int dz = int(hcp_voxel_3d_brush.radius_3d.z / (z_mult * voxel_size));
		for (int k = voxel_matrix_coord.z - dz; k <= voxel_matrix_coord.z + dz; k++) {
			if (k > -1 && k < voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension.z) {
				
				if (k % 2 == 0) {//even level
					dy = int(hcp_voxel_3d_brush.radius_3d.y / (sqrt3*voxel_size))-1;
				} else{// odd level
					dy = int(hcp_voxel_3d_brush.radius_3d.y / (sqrt3*voxel_size));
				}

				for (int j = voxel_matrix_coord.y - dy; j <= voxel_matrix_coord.y + dy; j++) {
					if (j > -1 && j < voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension.y) {

						if ((j+k) % 2 == 0) {//even column
							dx = int(hcp_voxel_3d_brush.radius_3d.x / (sqrt3*voxel_size))-1;
						} else{// odd column
							dx = int(hcp_voxel_3d_brush.radius_3d.y / (sqrt3*voxel_size));
						}

						for (int i = voxel_matrix_coord.x - dx; i <= voxel_matrix_coord.x + dx; i++) {
							if (i > -1 && i < voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension.x) {
								index_data_type matrix_index = voxel_hcp_object_to_execute->voxel_object_data.get_voxel_matrix_data_index({ i,j,k });
								glm::vec3 voxel_cart_coord   = voxel_hcp_object_to_execute->voxel_object_data.get_voxel_world_cartesian_coordinate({ i,j,k });

//std::cout << "hcp_voxel_brush_class::apply_brush 00001 : " << i << " : " << j << " : " << k << " : " << matrix_index << std::endl;

								if (hcp_voxel_3d_brush.inside_brush_volume(central_brush_voxel_cart_coord, voxel_cart_coord)) {
									add_voxel_brush_volume_voxel(matrix_index,voxel_cart_coord);
//std::cout << "hcp_voxel_brush_class::apply_brush 00002 : " << i << " : " << j << " : " << k << " : " << matrix_index << std::endl;
								}
							}
						}

					}	
				}

			}
		}


		// **************************

	}

	void add_voxel_brush_volume_voxel(int index,glm::vec3 voxel_cart_coord) {
		hcp_voxel_brush_volume_data_struct_type hcp_voxel_brush_volume_data;

		hcp_voxel_brush_volume_data.index      = index;
		hcp_voxel_brush_volume_data.cart_coord = voxel_cart_coord;

		brush_voxels.push_back(hcp_voxel_brush_volume_data);
	}

	//void update_shader_brush_data() {
	// Not yet implemented
	//}

	glm::vec3 central_brush_voxel_cart_coord = { 0.0f,0.0f,0.0f };

	//voxel_object_data_class &voxel_object_data; // Cannot use as this creates a compile error in scene_node_editor_widget.cpp 

	std::vector<hcp_voxel_brush_volume_data_struct_type> brush_voxels;
};


// ++++++++++++++++++++++++++++++++++++++++
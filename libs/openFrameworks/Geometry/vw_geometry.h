#pragma once

#include <variant>

#include "ofMesh.h"
#include "ofShader.h"

#include "vw_definitions.h"

/*
	Vetex class type T can be of vec2,vec3,or vec4
*/

enum class geometry_type_enum { undefined, null, points, mesh, triangles, lines };
enum class base_geometry_type_enum { undefined, null, D1, D2, D3, D4, line, plane, surface, surface_4D, hex_2D, hcp_3D, grid_2D, grid_3D, grid_4D, polar, spherical, point_cloud, field, tensor };
enum class base_geometry_attribute_data_type { undefined, attr_bool, attr_int, attr_float, attr_double, attr_vec2, attr_vec3, attr_vec4, attr_string };// Add more datatypes as required eg glm::ivec2 etc

typedef float      D1_data_type;

typedef glm::vec2  D2_data_type;
typedef glm::vec2  plane_data_type;
typedef glm::vec2  polar_data_type;
typedef glm::vec2  hex_2D_data_type;
typedef glm::vec2  grid_2D_data_type;

typedef glm::vec3  D3_data_type;
typedef glm::vec3  surface_data_type;
typedef glm::vec3  hcp_3D_data_type;
typedef glm::vec3  grid_3D_data_type;
typedef glm::vec3  spherical_data_type;
typedef glm::vec3  point_cloud_data_type;

//Following geometry data types can be used to define a scalar field at a vertex loctation
typedef glm::vec4  D4_data_type;
typedef glm::vec4  grid_4D_data_type;
typedef glm::vec4  surface_4D_data_type;
typedef glm::vec4  field_data_type;

//template<typename AT>
struct geometry_attribute_type_struct {
	GLint                   attribute_shader_location = INVALID_ID;
	std::string             attribute_name            = "";
	std::string             attribute_description     = "";
	base_geometry_attribute_data_type attribute_data_type = base_geometry_attribute_data_type::attr_float;

	//std::vector<std::variant<bool, int, float, double, glm::vec2, glm::vec3, glm::vec4>> attribute_values;// Add more datatypes as required eg glm::ivec2 etc.
	std::vector<float> attribute_values; // ofbuffer only permits float attribute data type. need to change this 

	bool set_attribute_shader_location(ofShader shader) {
		//cout << "In set_attribute_shader_location : " << shader.getProgram() << std::endl;

		attribute_shader_location = shader.getAttributeLocation(attribute_name);
		if (attribute_shader_location == GL_INVALID_OPERATION) {
			//cout << "In set_attribute_shader_location 01: attribute_shader_location == GL_INVALID_OPERATION " <<  std::endl;
			attribute_shader_location = INVALID_ID;
			return false;
		}
		else {
			//cout << "In set_attribute_shader_location 02: attribute_shader_location != GL_INVALID_OPERATION " << shader.getProgram() << " : "<< attribute_shader_location << std::endl;
			return true;
		}
	}

	bool shader_has_attribute_defined(ofShader shader) {
		if (shader.getAttributeLocation(attribute_name) == GL_INVALID_OPERATION)
			return false;
		else
			return true;
	}

	bool define_empty_data(int size, float value = 0.0f) {
		if (size < 1) return false;

		attribute_values.clear();
		for (int i = 0; i < size; i++) {
			attribute_values.push_back(value);
		}

		return true;
	}

	void set_attribute_value(int index, float value) {
		if (index < attribute_values.size() && index > -1)
			attribute_values[index] = value;
	}

	void set_attribute_values(std::vector<float> _attribute_values) {
		attribute_values.clear();
		attribute_values = _attribute_values;
	}

	//bool set_attribute_value(size_t index, std::variant<bool, int, float, double, glm::vec2, glm::vec3, glm::vec4> value) {
	//	if (index < 0 || index >= attribute_values.size()) return false;

	//	attribute_values[index] = value;

	//	return true;
	//}
};

class geometry_attribute_class {
public:
	GLint current_shader_program_to_use = INVALID_ID;

	std::vector<geometry_attribute_type_struct> attributes;

	size_t get_number_of_attributes() { return attributes.size(); }

	void add_attribute(geometry_attribute_type_struct attribute) {
		attributes.push_back(attribute);
	}

	bool delete_attribute(std::string attribute_name, base_geometry_attribute_data_type attribute_data_type) {
		for (size_t i = 0; i < attributes.size(); i++) {
			if (attributes[i].attribute_name == attribute_name && attributes[i].attribute_data_type == attribute_data_type) {
				attributes[i].attribute_values.clear();
				attributes.erase(attributes.begin() + i);
				return true;
			}
		}
		return false;
	}

	bool delete_attribute_data(std::string attribute_name, base_geometry_attribute_data_type attribute_data_type) {
		for (geometry_attribute_type_struct attribute : attributes) {
			if (attribute.attribute_name == attribute_name && attribute.attribute_data_type == attribute_data_type) {
				attribute.attribute_values.clear();
				return true;
			}
		}
		return false;
	}

	void delete_all_attribute_data(bool delete_attributes = false) {
		for (geometry_attribute_type_struct attribute : attributes) {
			attribute.attribute_values.clear();
		}

		if (delete_attributes)attributes.clear();
	}

	bool delete_all_attributes_with_vertex_index(size_t vertex_index) {
		if (attributes.size() < 1) return false;

		for (geometry_attribute_type_struct attribute : attributes) {
			if (vertex_index < attribute.attribute_values.size())
				attribute.attribute_values.erase(attribute.attribute_values.begin() + vertex_index);
		}

		return true;
	}

protected:

private:
};

// Much of what is required for this class is aleady defiuned in ofMesh class even if naming convention
// is not so good. datatype of vertex, normal, color and texture coordinates are defined in ofMesh

// Copy ofMesh code and rename, redefine of ofMesh code here if and when done with openFramwWorks
// as openFramwWorks vertices only deals with 3D vector data as ideally would like to have 2D and 4D
// geometry data defined here as a template of type glm::vec2 and glm::vec4 as well as glm::vec3
//template <typename VT = ofDefaultVertexType, typename NT = ofDefaultNormalType, typename CT = ofDefaultColorType, typename TT =  ofDefaultTexCoordType>// not yet used
//class base_geometry_class : public ofMesh_<VT,NT,CT,TT> {
class base_geometry_class : public ofMesh {
public:
	std::string             geometry_name = "";
	std::string             geometry_description = "";
	geometry_type_enum      geometry_type = geometry_type_enum::undefined;
	base_geometry_type_enum base_geometry_type = base_geometry_type_enum::D3;

	bool display_geometry = true;

	//std::vector<VT>                  vertices; // Vertex data of type T
	std::vector<glm::u64vec2>        edges;    // index of two vertex data points that make up a geometry edge
	std::vector<std::vector<size_t>> faces;    // index of vertex data points that make up a geometry face 

	geometry_attribute_class  vertex_attributes; // Attribute(s) that is associtaed with each vertex value of same index
	//std::vector<geometry_attribute_type_struct>  vertex_attributes; // Attribute(s) that is associtaed with each vertex value of same index
	//std::vector<AT>     attributes; // Attribute(s) that is associtaed with each vertex value
	//std::vector<EAT>     edge_attributes; // Attribute(s) that is associtaed with each edge value add in future ????
	//std::vector<FAT>     face_attributes; // Attribute(s) that is associtaed with each face value add in future ????

	size_t get_number_of_vertices() { return getNumVertices(); }// ofMesh method;
	size_t get_number_of_edges() { return edges.size(); }
	size_t get_number_of_faces() { return faces.size(); }

	//size_t get_number_of_attributes() { return vertex_attributes.size(); }


	// ++++++++++++++++++++++++++++
	//void add_vertex_attribute(geometry_attribute_type_struct vertex_attribute) {
	//	vertex_attributes.push_back(vertex_attribute);
	//}

	void add_edge(size_t vertex1_index, size_t vertex2_index) {
		edges.push_back(glm::u64vec2(vertex1_index, vertex2_index));
	}

	void add_triangle_face(glm::u64vec3 triangle) {
		std::vector<size_t> face;
		face.push_back(triangle.x);
		face.push_back(triangle.y);
		face.push_back(triangle.z);
		add_face(face);
	}

	void add_quad_face(glm::u64vec4 quad) {
		std::vector<size_t> face;
		face.push_back(quad.x);
		face.push_back(quad.y);
		face.push_back(quad.z);
		face.push_back(quad.w);
		add_face(face);
	}

	void add_ngon_face(std::vector<size_t> ngon) {
		std::vector<size_t> face;

		for (int i = 0; i < ngon.size(); i++) {
			face.push_back(ngon[i]);
		}

		add_face(face);
	}

	void add_face(std::vector<size_t> face) {
		faces.push_back(face);
	}

	// ++++++++++++++++++++++++++++

		// -------------------------------------
	// Delete geometry data
	// -------------------------------------

	void delete_geometry(bool delete_attributes = false) {
		//vertices.clear();
		//vertex_normals.clear();
		clearVertices();// ofMesh method;
		clearNormals();// ofMesh method;
		clearColors();// ofMesh method;
		clearTexCoords();// ofMesh method;

		edges.clear();
		faces.clear();
		vertex_attributes.delete_all_attribute_data(delete_attributes);
	}

	//bool delete_vertex(size_t vertex_index) {
	//	if (vertex_index >= vertices.size()) return false;

	//	vertices.erase(vertices.begin() + vertex_index);
	//	delete_vertex_normal(vertex_index);

	//	delete_all_edges_with_vertex_index(vertex_index);
	//	delete_all_faces_with_vertex_index(vertex_index);
	//	delete_all_attributes_with_vertex_index(vertex_index);

	//	return true;
	//}

	void delete_vertex(size_t vertex_index) {
		removeVertex(vertex_index); // ofMesh method;
	}

	bool delete_vertex_normal(size_t vertex_index) {
		removeNormal(vertex_index);// ofMesh method;
	}
	//bool delete_vertex_normal(size_t vertex_index) {
	//	if (vertex_index >= vertex_normals.size()) return false;

	//	vertex_normals.erase(vertex_normals.begin() + vertex_index);
	//	return true;
	//}

	bool delete_all_edges_with_vertex_index(size_t vertex_index) {
		if (vertex_index >= edges.size()) return false;

		for (size_t i = 0; i < edges.size(); i++) {
			if (edges[i].x == vertex_index || edges[i].y == vertex_index)
				edges.erase(edges.begin() + i);
		}

		return true;
	}

	bool delete_all_faces_with_vertex_index(size_t vertex_index) {
		if (vertex_index >= faces.size()) return false;

		for (size_t i = 0; i < faces.size(); i++) {
			for (size_t j = 0; j < faces[i].size(); j++) {
				if (faces[i][j] == vertex_index)
					faces[i].erase(faces[i].begin() + i);
			}

		}

		return true;
	}

protected:

private:

};

#pragma once

#include "vw_geometry.h"
#include "ofMaterial.h"
#include "ofNode.h"
//#include "vw_geometry.h"
//#include "../OpenGL/vertex_buffer.h"

#include "ofVbo.h"
#include "ofGraphics.h"

enum class geometry_render_texture_method_enum {undefined, shader, material};

// Much of what is required for this class is aleady defined in ofMesh class even if naming convention
// is not so good. datatype of vertex, normal, color and texture coordinates are defined for ofMesh
// Do not define datatype for entity_base_geometry_class if using a derived class of it as the datatype
// is then defined in the derived data class
//template <typename VT = ofDefaultVertexType, typename NT = ofDefaultNormalType, typename CT = ofDefaultColorType, typename TT = ofDefaultTexCoordType>// not yet used
//class entity_base_geometry_class : public base_geometry_class<VT,NT,CT,TT>{
class entity_base_geometry_class : public base_geometry_class {
public:
	entity_base_geometry_class() {
	//entity_base_geometry_class(ofNode *node = nullptr) {
		geometry_type = geometry_type_enum::undefined;
		//parent_node = node;
	}
	~entity_base_geometry_class() {
		delete_shader();
	}

	ofNode *node = nullptr;



	virtual bool init() {
		// Absolutely critical to have a setMode function to one of the openframeworks
		// primitive mode types or no points will be displayed when using a geometry shader.
		// This is an open frameworks thing that took two days to figure out since no
		// such setting is needed in openGL. Ridiculous !!!!
		// OF_PRIMITIVE_TRIANGLES,
		// OF_PRIMITIVE_TRIANGLE_STRIP,
		// OF_PRIMITIVE_TRIANGLE_FAN,
		// OF_PRIMITIVE_LINES,
		// OF_PRIMITIVE_LINE_STRIP,
		// OF_PRIMITIVE_LINE_LOOP,
		// OF_PRIMITIVE_POINTS,
		setMode(OF_PRIMITIVE_POINTS);//
		return true;
	}

	virtual bool load(const std::string& filepath, float x_off) { return false; }
	virtual bool save(const std::string& filepath) { return false; }

	virtual bool add_geometry_data() { return false; }
	virtual bool update_geometry() { return false; }

	// add_vertex function required but cannot be defined as a virtual function as the function paramter
	// needs to be spcified and thus cannot define this function with a different paramater data type
	// Using a template just unneccessarily complicates things as well and causes problems
	//virtual void add_vertex() = 0; // !!!!!!!!!!!! If define this as a pure virtual function just causes too many problems

	virtual bool create_buffers() { return false; };
	virtual bool delete_buffers() { return false; };

	// --------------------------------------------------
	// Need to define color scheme for each of these in shaders
	void drawVertices()  const {
		draw(OF_MESH_POINTS);
	}

	void drawWireframe()  const {
		draw(OF_MESH_WIREFRAME);
	}

	void drawFaces()  const {
		draw(OF_MESH_FILL);
	}

	void draw_normals(float length, bool bFaceNormals, glm::vec4 color_normals) const {
		//if (parent_node == nullptr) return;
		
		//parent_node->transformGL(ofGetCurrentRenderer().get());
		//node.transformGL(ofGetCurrentRenderer().get());

		if (usingNormals()) {
			const auto& normals = getNormals();
			const auto& vertices = getVertices();
			glm::vec3 normal;
			glm::vec3 vert;

			normalsMesh.setMode(OF_PRIMITIVE_LINES);
			normalsMesh.getVertices().resize(normals.size() * 2);

			if (bFaceNormals) {
				for (size_t i = 0; i < normals.size(); i++) {
					if (i % 3 == 0) {
						vert = (vertices[i] + vertices[i + 1] + vertices[i + 2]) / 3;
					}
					else if (i % 3 == 1) {
						vert = (vertices[i - 1] + vertices[i] + vertices[i + 1]) / 3;
					}
					else if (i % 3 == 2) {
						vert = (vertices[i - 2] + vertices[i - 1] + vertices[i]) / 3;
					}
					normalsMesh.setVertex(i * 2, vert);
					normal = glm::normalize(toGlm(normals[i]));
					normal *= length;
					normalsMesh.setVertex(i * 2 + 1, vert + normal);
				}
			}
			else {
				for (size_t i = 0; i < normals.size(); i++) {
					vert = vertices[i];
					normal = glm::normalize(toGlm(normals[i]));
					normalsMesh.setVertex(i * 2, vert);
					normal *= length;
					normalsMesh.setVertex(i * 2 + 1, vert + normal);
				}
			}

//std::cout << "Normal colors : 00" << normalsMesh.getNumColors() << std::endl;
			//for(int i=0;i< normalsMesh.getNumVertices();i++)
			for(int i=0;i< normalsMesh.getNumVertices();i++)
				normalsMesh.addColor({ color_normals.r,color_normals.g,color_normals.b,color_normals.a });
				//normalsMesh.addColor({255,0,255 });
//std::cout << "Normal colors : 01" << normalsMesh.getNumColors() << std::endl;

			normalsMesh.enableColors();
			normalsMesh.draw();
		}
		else {
			ofLogWarning("of3dPrimitive") << "drawNormals(): mesh normals are disabled";
		}


		//restoreTransformGL(ofGetCurrentRenderer().get());
		//restoreTransformGL(ofGetCurrentRenderer().get());
	}

	// ------------- SHADER FUNCTIONS -----------------

	ofShader* create_shader() {
		shader = new ofShader;
		internal_shader = true;
		return shader;
	}

	void delete_shader() {
		if (internal_shader && shader != NULL) { // delete internal shader
			shader->unload();
		}

		shader = NULL;  // if have external shader do not unload it but simply assign shader to use to NULL;
		internal_shader = false;
	}

	void define_geometry_shader(ofShader* _shader) {
		delete_shader();

		shader          = _shader;
		internal_shader = true;
	}

	//-----------------------------
	
	// ------------- MATERIAL FUNCTIONS -----------------

	ofMaterial* create_material() {
		material = new ofMaterial;
		internal_material = true;
		return material;
	}

	void delete_material() {
		if (internal_material && material != NULL) { // delete internal material
			delete material;
		}

		material          = NULL;
		internal_material = false;
	}

	void define_geometry_material(ofMaterial* _material) {
		delete_material();

		material          = _material;
		internal_material = true;
	}

	//-----------------------------

	// -------------- GEOMETRY ATTRIBUTE FUNCTIONS ----------------

	bool define_geometry_attributes_shader(ofShader* _shader = NULL) {
		if (_shader == NULL) {
			if (shader == NULL)
				return false;
		}
		else
			define_geometry_shader(_shader);

		shader->begin();
		for (size_t i = 0; i < vertex_attributes.attributes.size(); i++) {
			vertex_attributes.attributes[i].set_attribute_shader_location(*shader);
			//cout << " define_geometry_attribiutes : " << shader.getProgram() << " : " << ertex_attributes.attributes[i].attribute_shader_location << std::endl;
		}
		shader->end();

		return true;
	}

	bool set_geometry_attribiute_data() {
		if (vertex_attributes.attributes.size() == 0) return false;

		// +++++++++++++++++++++++++++++++
		for (size_t i = 0; i < vertex_attributes.attributes.size(); i++) {
			//cout << "point_cloud_data_class :: create_buffers 1111 @@@@ : " << i << " : " << vertex_attributes.attributes[i].attribute_values.size() << std::endl;
			if (vertex_attributes.attributes[i].attribute_values.size() == getNumVertices()) {
				//cout << "point_cloud_data_class :: create_buffers 1111AAA @@@@ : " << vertex_attributes.attributes.size() << std::endl;
				for (geometry_attribute_type_struct vertex_attribute : vertex_attributes.attributes) {

					//cout << "point_cloud_data_class :: create_buffers 2222 @@@@ : " << std::endl;
					if (vertex_attribute.attribute_shader_location != INVALID_ID) {

						// ofbuffer only permits float attribute data type.
						// setAttributeData only accepts flaot data type.  need to change this and use os switch becomes relevant
						//switch (vertex_attribute.attribute_data_type) {
						//    case base_geometry_attribute_data_type::attr_float: {
						//        float *data = (float*)vertex_attributes.attributes[0].attribute_values.data();
						//        vertex_buffer.setAttributeData(vertex_attribute.attribute_shader_location, data, 1, vertex_attributes.attributes.size(), GL_STATIC_DRAW);
						//        break;
						//    }

						//    case base_geometry_attribute_data_type::attr_int: {
						//        int *data = (int*)vertex_attributes.attributes[0].attribute_values.data();   break;
						//        vertex_buffer.setAttributeData(vertex_attribute.attribute_shader_location, data, 1, vertex_attributes.attributes.size(), GL_STATIC_DRAW);
						//        break;
						//    }
						//
						//}
//cout << "point_cloud_data_class :: create_buffers 3333 @@@@ : " << vertex_attribute.attribute_shader_location << " : " << vertex_attribute.attribute_values.size() << std::endl;
						//float *data = (float*)vertex_attributes.attributes[i].attribute_values.data();
						//vertex_buffer.setAttributeData(vertex_attribute.attribute_shader_location, data, 1, vertex_attributes.attributes[i].attribute_values.size(), GL_DYNAMIC_DRAW);
//cout << "point_cloud_data_class :: create_buffers 4444 @@@@ : " << vertex_attribute.attribute_values[0] << std::endl;

						vertex_buffer.setAttributeData(vertex_attribute.attribute_shader_location, vertex_attribute.attribute_values.data(), 1, vertex_attribute.attribute_values.size(), GL_DYNAMIC_DRAW);
					}
				}
			}
		}

		// Other attributes to be placed here
		return true;
	}

	//-----------------------------------------------------------

	void define_geometry_render_texture_method(geometry_render_texture_method_enum _geometry_render_texture_method) {
		geometry_render_texture_method = _geometry_render_texture_method;
	}


	virtual void render() {
		if (geometry_render_texture_method == geometry_render_texture_method_enum::shader)
			shader_render();
		else
			if (geometry_render_texture_method == geometry_render_texture_method_enum::material)
				material_render();
			else {
				// render to externaly defined shader
				ofPushMatrix();// This needs to be tested : needed for Shadows 
				render_geometry();
				ofPopMatrix();// This needs to be tested : needed for Shadows 
			}
	};

	virtual void shader_render() {
		if (shader != NULL) { // Have internal shader to render to
			shader->begin();
			ofPushMatrix();// This needs to be tested : needed for Shadows 
			render_geometry();
			ofPopMatrix();// This needs to be tested : needed for Shadows 
			shader->end();
		}
	}

	virtual void material_render() {
		if (material != NULL) { // Have internal material to render to
			material->begin();
			ofPushMatrix();// This needs to be tested : needed for Shadows 
			render_geometry();
			ofPopMatrix();// This needs to be tested : needed for Shadows 
			material->end();
		}
	}

	bool has_internal_shader()   { return internal_shader; }
	bool has_internal_material() { return internal_material; }

	geometry_render_texture_method_enum geometry_render_texture_method = geometry_render_texture_method_enum::undefined;

	//virtual void bind()   { vertex_buffer->bind(); }
	//virtual void unbind() { vertex_buffer->unbind(); }

//protected:
	// Buffers manager
	//std::unique_ptr<openGL_vertex_Index_buffer_class> vertex_buffer;

	ofVbo       vertex_buffer;
	ofShader   *shader   = nullptr;
	ofMaterial *material = nullptr;

	bool        display_faces      = true;
	bool        display_wireframe  = false;
	bool        display_normals    = false;
	bool        display_vertices   = false;

	mutable ofMesh normalsMesh;

protected:
	//void transformGL(ofBaseRenderer* renderer) const {
	//	if (renderer == nullptr) {
	//		renderer = ofGetCurrentRenderer().get();
	//	}
	//	renderer->pushMatrix();
	//	renderer->multMatrix(getGlobalTransformMatrix());
	//}

	//void restoreTransformGL(ofBaseRenderer* renderer) const {
	//	if (renderer == nullptr) {
	//		renderer = ofGetCurrentRenderer().get();
	//	}
	//	renderer->popMatrix();
	//}

	//glm::mat4 getGlobalTransformMatrix() const {
	//	//if (parent) return parent->getGlobalTransformMatrix() * getLocalTransformMatrix();
	//	//else return getLocalTransformMatrix();
	//	return getLocalTransformMatrix();
	//}

//private:
	virtual void render_geometry() {}
	bool         internal_shader   = false;
	bool         internal_material = false;

	//ofNode         node;
	//mutable ofMesh normalsMesh;
};
#pragma once

#include <Geometry/vw_entity_object_geometry.h>

/*
                MESH GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point cloud object data type with
    point vertex location data only.
*/

class icosphere_geometry_class : public entity_base_geometry_class {
public:
	icosphere_geometry_class() = default;

	virtual ~icosphere_geometry_class() {
		//if (buffers_created) delete_buffers(); // without this test, will have a crash 
		mode              = OF_PRIMITIVE_TRIANGLES;
		bVertsChanged     = false;
		bColorsChanged    = false;
		bNormalsChanged   = false;
		bTexCoordsChanged = false;
		bIndicesChanged   = false;
		bFacesDirty       = false;
		useColors         = true;
		useTextures       = true;
		useNormals        = true;
		useIndices        = true;
	}

	bool init(ofNode *_node, float radius = 1.0, std::size_t iterations = 5) {
		geometry_type = geometry_type_enum::mesh;
		//vertex_buffer = std::make_unique<openGL_vertex_Index_buffer_class>();
		node = _node;

		return generate_geometry(radius, iterations);
	}

	bool generate_geometry(float radius = 1.0, std::size_t iterations = 5) {
		delete_geometry();

		/// Step 1 : Generate icosahedron
		const float sqrt5 = sqrt(5.0f);
		const float phi = (1.0f + sqrt5) * 0.5f;
		const float invnorm = 1 / sqrt(phi * phi + 1);

		addVertex(invnorm * glm::vec3(-1, phi, 0));//0
		addVertex(invnorm * glm::vec3(1, phi, 0));//1
		addVertex(invnorm * glm::vec3(0, 1, -phi));//2
		addVertex(invnorm * glm::vec3(0, 1, phi));//3
		addVertex(invnorm * glm::vec3(-phi, 0, -1));//4
		addVertex(invnorm * glm::vec3(-phi, 0, 1));//5
		addVertex(invnorm * glm::vec3(phi, 0, -1));//6
		addVertex(invnorm * glm::vec3(phi, 0, 1));//7
		addVertex(invnorm * glm::vec3(0, -1, -phi));//8
		addVertex(invnorm * glm::vec3(0, -1, phi));//9
		addVertex(invnorm * glm::vec3(-1, -phi, 0));//10
		addVertex(invnorm * glm::vec3(1, -phi, 0));//11

		ofIndexType firstFaces[] = {
		0,1,2,
		0,3,1,
		0,4,5,
		1,7,6,
		1,6,2,
		1,3,7,
		0,2,4,
		0,5,3,
		2,6,8,
		2,8,4,
		3,5,9,
		3,9,7,
		11,6,7,
		10,5,4,
		10,4,8,
		10,9,5,
		11,8,6,
		11,7,9,
		10,8,11,
		10,11,9
		};

		for (ofIndexType i = 0; i < 60; i += 3) {
			addTriangle(firstFaces[i], firstFaces[i + 1], firstFaces[i + 2]);
		}

		auto& vertices = getVertices();
		auto& faces = getIndices();

		ofIndexType size = faces.size();

		/// Step 2 : tessellate
		for (ofIndexType iteration = 0; iteration < iterations; iteration++)
		{
			size *= 4;
			std::vector<ofIndexType> newFaces;
			for (ofIndexType i = 0; i < size / 12; i++)
			{
				auto i1 = faces[i * 3];
				auto i2 = faces[i * 3 + 1];
				auto i3 = faces[i * 3 + 2];
				auto i12 = vertices.size();
				auto i23 = i12 + 1;
				auto i13 = i12 + 2;
				auto v1 = vertices[i1];
				auto v2 = vertices[i2];
				auto v3 = vertices[i3];
				//make 1 vertice at the center of each edge and project it onto the sphere
				vertices.push_back(glm::normalize(toGlm(v1 + v2)));
				vertices.push_back(glm::normalize(toGlm(v2 + v3)));
				vertices.push_back(glm::normalize(toGlm(v1 + v3)));
				//now recreate indices
				newFaces.push_back(i1);
				newFaces.push_back(i12);
				newFaces.push_back(i13);
				newFaces.push_back(i2);
				newFaces.push_back(i23);
				newFaces.push_back(i12);
				newFaces.push_back(i3);
				newFaces.push_back(i13);
				newFaces.push_back(i23);
				newFaces.push_back(i12);
				newFaces.push_back(i23);
				newFaces.push_back(i13);
			}
			faces.swap(newFaces);
		}

		/// Step 3 : generate texcoords
		std::vector<glm::vec2> texCoords;
		for (ofIndexType i = 0; i < vertices.size(); i++)
		{
			const auto& vec = vertices[i];
			float u, v;
			float r0 = sqrtf(vec.x * vec.x + vec.z * vec.z);
			float alpha;
			alpha = atan2f(vec.z, vec.x);
			u = alpha / glm::two_pi<float>() + .5f;
			v = atan2f(vec.y, r0) / glm::pi<float>() + .5f;
			// reverse the u coord, so the default is texture mapped left to
			// right on the outside of a sphere 
			// reverse the v coord, so that texture origin is at top left
			texCoords.push_back(glm::vec2(1.0 - u, 1.f - v));
		}

		/// Step 4 : fix texcoords
		// find vertices to split
		std::vector<ofIndexType> indexToSplit;

		for (ofIndexType i = 0; i < faces.size() / 3; i++)
		{
			glm::vec2& t0 = texCoords[faces[i * 3 + 0]];
			glm::vec2& t1 = texCoords[faces[i * 3 + 1]];
			glm::vec2& t2 = texCoords[faces[i * 3 + 2]];

			if (std::abs(t2.x - t0.x) > 0.5)
			{
				if (t0.x < 0.5)
					indexToSplit.push_back(faces[i * 3]);
				else
					indexToSplit.push_back(faces[i * 3 + 2]);
			}
			if (std::abs(t1.x - t0.x) > 0.5)
			{
				if (t0.x < 0.5)
					indexToSplit.push_back(faces[i * 3]);
				else
					indexToSplit.push_back(faces[i * 3 + 1]);
			}
			if (std::abs(t2.x - t1.x) > 0.5)
			{
				if (t1.x < 0.5)
					indexToSplit.push_back(faces[i * 3 + 1]);
				else
					indexToSplit.push_back(faces[i * 3 + 2]);
			}
		}

		//split vertices
		for (ofIndexType i = 0; i < indexToSplit.size(); i++)
		{
			ofIndexType index = indexToSplit[i];
			//duplicate vertex
			glm::vec3 v = vertices[index];
			glm::vec2 t = texCoords[index] + glm::vec2(1.f, 0.f);
			vertices.push_back(v);
			texCoords.push_back(t);
			ofIndexType newIndex = vertices.size() - 1;
			//reassign indices
			for (ofIndexType j = 0; j < faces.size(); j++)
			{
				if (faces[j] == index)
				{
					ofIndexType index1 = faces[(j + 1) % 3 + (j / 3) * 3];
					ofIndexType index2 = faces[(j + 2) % 3 + (j / 3) * 3];
					if ((texCoords[index1].x > 0.5) || (texCoords[index2].x > 0.5))
					{
						faces[j] = newIndex;
					}
				}
			}
		}

		// tig: flip face(=triangle) winding order, so that we are consistent with all other ofPrimitives.
		// i wish there was a more elegant way to do this, but anything happening before "split vertices"
		// makes things very, very complicated.

		for (ofIndexType i = 0; i < faces.size(); i += 3) {
			std::swap(faces[i + 1], faces[i + 2]);
		}

		addNormals(vertices);
		addTexCoords(texCoords);

		for (ofIndexType i = 0; i < vertices.size(); i++) {
			vertices[i] *= radius;
		}

		//return update_geometry();
		return true;

	}

/*
	// Update geometry data in the buffers with the current 
	// vertex data content.
	bool update_geometry() {
		//cout << "point_cloud_data_class :: update_geometry : 0000 || " << buffers_created << std::endl;
		if (buffers_created) delete_buffers();
		//cout << "point_cloud_data_class :: update_geometry : 1111" <<  std::endl;
		if (!create_buffers()) return false;
		//cout << "point_cloud_data_class :: update_geometry : 2222" <<  std::endl;
		return true;
	}

	// Need buffers_created to avoid app crashing if no VAO created by defining vertices
	// Seems OpneGl has no way to test if a VAO exists or not so need this
	// NOTE : Early stage of development so this may change 
	bool buffers_created = false;

	// add_vertex function required but cannot be defined as a virtual function as the function paramter
	// needs to be spcified and thus cannot define this function with a different paramater data type
	// Using a template just unneccessarily complicates things as well and causes problems
	//void add_vertex(const point_vertex_class& vertex) {
	void add_vertex(const glm::vec3& vertex) {
		addVertex(vertex);
	}

	void add_normal(const glm::vec3& normal) {
		addNormal(normal);
	}

	bool create_buffers() {
		if (get_number_of_vertices() < 1) return false;
		cout << "point_cloud_data_class :: create_buffers 0000 " << getNumVertices() << " : " << vertex_attributes.attributes.size() << std::endl;
		cout << "point_cloud_data_class :: create_buffers 0001 " << getNumColors() << " : " << vertex_attributes.attributes.size() << std::endl;
		cout << "point_cloud_data_class :: create_buffers 0002 " << getNumNormals() << " : " << vertex_attributes.attributes.size() << std::endl;
		cout << "point_cloud_data_class :: create_buffers 0003 " << getNumTexCoords() << " : " << vertex_attributes.attributes.size() << std::endl;
		cout << "point_cloud_data_class :: create_buffers 0004 " << getIndices().size() << " : " << vertex_attributes.attributes.size() << std::endl;
		cout << "point_cloud_data_class :: create_buffers 0005 " << getUniqueFaces().size() << " : " << vertex_attributes.attributes.size() << std::endl;

		vertex_buffer.setVertexData(getVerticesPointer(), getNumVertices(), GL_STATIC_DRAW);
		vertex_buffer.setColorData(getColorsPointer(), getNumColors(), GL_STATIC_DRAW);
		vertex_buffer.setNormalData(getNormalsPointer(), getNumNormals(), GL_STATIC_DRAW);
		vertex_buffer.setTexCoordData(getTexCoordsPointer(), getNumTexCoords(), GL_STATIC_DRAW);
		// Add normal, color, texture here

		set_geometry_attribiute_data();

		buffers_created = true;

		cout << "point_cloud_data_class :: create_buffers 11111 @@@@ : " << buffers_created << std::endl;

		return buffers_created;
	}

	bool delete_buffers() {
		//printf("point_cloud_data_class :: delete_buffers : 0000 \n"); 
		if (!buffers_created) return false;
		//printf("point_cloud_data_class :: delete_buffers : 1111 \n"); 
		vertex_buffer.clear();

		//printf("point_cloud_data_class :: delete_buffers : 2222 \n"); 

		buffers_created = false;
		return true;
	}
*/
	//void render() {
	//	if (shader != NULL) { // Have internal shader to render to
	//		//cout << "shader != NULL\n";
	//		shader->begin();
	//		render_geometry();
	//		shader->end();
	//	}
	//	else { // render to externaly defined shader
	//		//cout << "shader == NULL\n";
	//		render_geometry();
	//	}
	//}


private:
	void render_geometry() {
//cout << "in     void render_geometry() \n";

		if (display_geometry) {
//cout << "in     void render_geometry()002 " << faces.size() << std::endl;

			if (getNumVertices() == 0) return;

			if (display_faces) {
				if (!display_wireframe && !display_vertices) // comment out when display_wireframe && display_faces solved
				drawFaces();
			}

			if (display_wireframe) {
				// need color parameter here
				drawWireframe();
			}

			if (display_vertices) {
				// need color parameter here
				drawVertices();// does not work
			}
/*
			if (display_wireframe && display_faces) { // Not yet solved 
				if (node == nullptr) return;
				ofNoFill();
				ofSetColor(0, 0, 0);// does not work
//std::cout << "HERE : display_wireframe && display_faces\n";
				//node->set_local_scale(2.0f, true,false);// does not work
				node->setScale(1.01);// // does notwork
				drawWireframe();
				//node->set_local_scale(0.5f, true, false);// does not work
				node->setScale(1.00);// // does not work
			}

			if (display_vertices && display_faces) { // Not yet solved 
				if (node == nullptr) return;
				//ofNoFill();
				//ofSetColor(0, 0, 0);
				//setScale(1.01f);
				drawVertices();
				//setScale(1.0f);
			}
*/
		}
	}

};

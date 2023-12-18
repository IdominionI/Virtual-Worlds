#pragma once

#include <Geometry/vw_entity_object_geometry.h>

/*
                MESH GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point cloud object data type with
    point vertex location data only.
*/


class mesh_geometry_base_class : public entity_base_geometry_class {
public:
	mesh_geometry_base_class() {
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
    ~mesh_geometry_base_class() {}

	bool init() {
		geometry_type = geometry_type_enum::mesh;
		return generate_geometry();
	}

	virtual bool generate_geometry() {
		return false;
	}

	// Add whatever is needed 

protected:

private:
	void render_geometry() {
//cout << "in     void render_geometry() \n";
		if (display_geometry) {
//cout << "in     void render_geometry()002 " << faces.size() << std::endl;
			if (getNumVertices() == 0) return;

			if (display_faces) {
				draw(OF_MESH_FILL);
			}
			//if (display_faces)     drawFaces();
			//if(display_wireframe)  drawWireframe();
			
			if(display_wireframe && !display_faces)
				drawWireframe();

			if (display_vertices && !display_faces)
				drawVertices();

			if (display_wireframe && display_faces){ // Not yet solved 
				//ofNoFill();
				//ofSetColor(0, 0, 0);
				//setScale(1.01f);
				drawWireframe();
				//setScale(1.0f);
			}

			if (display_vertices && display_faces) { // Not yet solved 
				//ofNoFill();
				//ofSetColor(0, 0, 0);
				//setScale(1.01f);
				drawVertices();
				//setScale(1.0f);
			}
			
			//if (display_normals) {
			//	ofSetColor(225, 0, 255);
			//	//drawNormals(float length, bool bFaceNormals)
			//	drawNormals(20,true);
			//}
			
		}
	}
};
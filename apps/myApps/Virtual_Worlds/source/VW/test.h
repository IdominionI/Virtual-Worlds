#pragma once

#include "ofMain.h"

#include <ImGuiEx/ImGui_utils.h>

#include "Basis_classes/Entity/entity_object_template.h"

#include "Basis_classes/Geometry/point_cloud.h"

class test_class : public ofBaseApp {

	~test_class() {
		ImGui_ui_context.ImGui_end();
	}

public:
	void setup() {
		mainGLFW_ptr = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();

		ImGui_ui_context.ImGui_init(mainGLFW_ptr);


		entity_object.geometry = new entity_base_geometry_class;
		entity_object2.geometry = new entity_base_geometry_class;

		pc = new point_cloud3D_class;
		pc2 = new point_cloud3D_class;
		entity_object.geometry = pc;
		entity_object2.geometry = pc2;

		if (!pc->generate_test_data()) {
			cout << " point_cloud3D_class : geometry not updated !!!!" << std::endl;
		} else {

			cout << " vertices Num entity : " << entity_object.geometry->getNumVertices() << std::endl;

			// shader files must exist in directory called data. Need to change this
			shaders_loaded = shader.load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");

			if (!shaders_loaded) {
				cout << " Shaders not loaded !!!!! : " << std::endl;
			}
			else
				cout << " Shaders loaded ^^^^^ : " << std::endl;



			//vbo.setVertexData(entity_object.geometry->getVerticesPointer(), entity_object.geometry->getNumVertices(), GL_STATIC_DRAW);
			//cout << " VBO vertices Num  : " << vbo.getNumVertices() << std::endl;
			cout << " VBO vertices Num  : " << pc->vertex_buffer.getNumVertices() << std::endl;


			//light.setup();
			//light.setPosition(-100, 200, 0);
			//ofEnableDepthTest();
		}

		if (!pc2->generate_test_data()) {
			cout << " point_cloud3D_class : geometry not updated 2 !!!!" << std::endl;
		}
		else {

			cout << " vertices Num entity 2: " << entity_object2.geometry->getNumVertices() << std::endl;

			// shader files must exist in directory called data. Need to change this
			shaders_loaded = shader2.load("Shaders/passthru.vert", "Shaders/grid_fragment_shader2.glsl");

			if (!shaders_loaded) {
				cout << " Shaders not loaded 2 !!!!! : " << std::endl;
			}
			else
				cout << " Shaders loaded ^^^^^ 2 : " << std::endl;



			//vbo.setVertexData(entity_object.geometry->getVerticesPointer(), entity_object.geometry->getNumVertices(), GL_STATIC_DRAW);
			//cout << " VBO vertices Num  : " << vbo.getNumVertices() << std::endl;
			cout << " VBO vertices Num  2 : " << pc2->vertex_buffer.getNumVertices() << std::endl;


			//light.setup();
			//light.setPosition(-100, 200, 0);
			//ofEnableDepthTest();
		}
		//boxMaterial.setDiffuseColor(ofFloatColor::red);
		//boxMaterial.setShininess(0.02);
		ofEnableDepthTest();
		glEnable(GL_DEPTH_TEST);// !!!!!
	}



	void update(){}
	void draw(){
		if (!shaders_loaded) return;

		cam.begin();
		//ofEnableDepthTest();

		glPointSize(1.0f);

		shader.begin();
		ofScale(200.0, 200.0, 200.0);
		pc->render();
		//ofDrawRectangle (0.0, 0.0, 100, 100);
		shader.end();

		glPointSize(2.0f);
		shader2.begin();
		ofScale(.5, .5, .5);
		pc2->render();
		shader2.end();

		//boxMaterial.begin();
		//box.draw();
		//boxMaterial.end();

		//ofDisableDepthTest();
		cam.end();
	}

	void keyPressed(int key){}
	void keyReleased(int key){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
	void mouseEntered(int x, int y){}
	void mouseExited(int x, int y){}
	void windowResized(int w, int h){}
	void dragEvent(ofDragInfo dragInfo){}
	void gotMessage(ofMessage msg){}

	shared_ptr<ofAppBaseWindow> main_window = NULL;

	// ImGui
	GLFWwindow            *mainGLFW_ptr;
	ImGui_ui_context_class ImGui_ui_context;

	//Testing data
	entity_object_basis_class<glm::vec3> entity_object;// strange need to define <glm::vec3> here but not in setup
	entity_object_basis_class<glm::vec3> entity_object2;// strange need to define <glm::vec3> here but not in setup
	point_cloud3D_class *pc, *pc2;

	ofShader shader,shader2;
	bool shaders_loaded = false;

	ofEasyCam cam;
	//ofLight   light;

	//ofBoxPrimitive box;
	//ofMaterial boxMaterial;

	//ofVbo vbo;

};
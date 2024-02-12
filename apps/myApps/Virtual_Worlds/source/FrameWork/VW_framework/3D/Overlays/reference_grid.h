#pragma once

#include <memory>
//#include <iostream>

#include <glm/glm.hpp>

#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofCairoRenderer.h"

#define GRID_ENTITY_ID 1000000000

class reference_grid_class {
public:
	glm::vec3 camera_loc = { 0.0f,0.0f,0.0f };

	float  stepSize = .5f;
    size_t number_grid_cells = 20;
    bool   labels = false;

	bool display_xy_plane = true,  display_xz_plane = false, display_yz_plane = false;
	bool relative_xy_grid = false, relative_xz_grid = false, relative_yz_grid = false;
	bool relative_xy_grid_absolute = false, relative_xz_grid_absolute = false, relative_yz_grid_absolute = false;
	
	float relative_xy_grid_dist = -3.0f, relative_xz_grid_dist = 3.0f, relative_yz_grid_dist = 3.0f;
	glm::vec3 xy_grid_origin = { 0.0f,0.0f,0.0f }, xz_grid_origin = { 0.0f,0.0f,0.0f }, yz_grid_origin = { 0.0f,0.0f,0.0f };

	//glm::vec4 xy_grid_color = { 0.7843,0.5882,0.5882,1 }, xz_grid_color = { 0.5882,0.7843,0.5882,1 }, yz_grid_color = { 0.1961,0.1961,0.7843,1 };
	ofColor ofxy_grid_color = { 0.7843f * 255.0f,0.5882f * 255.0f,0.5882f * 255.0f,1.0f * 255.0f };
	ofColor ofxz_grid_color = { 0.5882f * 255.0f,0.7843f * 255.0f,0.5882f * 255.0f,1.0f * 255.0f };
	ofColor ofyz_grid_color = { 0.1961f*255.0f,0.1961f*255.0f,0.7843f*255.0f,1.0f*255.0f };

	// This crashes the app if not placed here rather than as a private variable.
	// Shared pointers cause more problems than they solve
	std::shared_ptr<ofBaseRenderer> renderer = ofGetCurrentRenderer();
	ofColor prevColor = renderer->getStyle().color;


	// Seems that the default plane in openframeworks that its inbuilt plane and gris liws on the y-z plane along the x axis
	// and that all rotations and translations performed by rotate are performed in this local coordinates according to this
	// y-z plane and the depth axis being the x axis. This is counter to most 3D models where the x-y plane is the view coordinates
	// and the z axis is defiend as the depth. Confuses everything. 

    void display_grid() {
		define_grid_intervals();

		// ****************** X-Y PLANE *******************
		if (display_xy_plane) {		

			if (!relative_xy_grid && !relative_xy_grid_absolute) {
				xy_dist = 0.0f;
			}else {
				if (relative_xy_grid && relative_xy_grid_absolute) {
					xy_dist = camera_loc.z + relative_xy_grid_dist;
					relative_xy_grid = false;
				} else {
					if (relative_xy_grid && !relative_xy_grid_absolute) {
						xy_dist = camera_loc.z + relative_xy_grid_dist;
					}
				}
			}
			
			renderer->setColor(ofxy_grid_color);
		    glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0,1,0));//rotate(mat<4, 4, T, Q> const& m, T angle, vec<3, T, Q> const& v)
			glm::mat4 t = glm::translate(m, glm::vec3(xy_dist,0.0f,0.0f)); // appears matrix mult to perform translate along z axis has has z translation in the x coord
		    renderer->pushMatrix();
			renderer->multMatrix(t);
		    renderer->get3dGraphics().drawGridPlane(x_grid_interval, number_grid_cells, labels);
			renderer->popMatrix();
		}
		// ****************** Y-Z PLANE *******************
		if(display_yz_plane){
			// if have a selection for allways in front of where camera is looking, need a
			// routine to check camera lookat at vector and see which world view x dir it 
			// is looking at and if it is in the +x dir 
			//		yz_dist = camera_loc.x + relative_yz_grid_dist
			// otherwise
			//      yz_dist = camera_loc.x - relative_yz_grid_dist
			//if (relative_yz_grid)

			if (!relative_yz_grid && !relative_yz_grid_absolute) {
				yz_dist = 0.0f;
			}else {
				if (relative_yz_grid && relative_yz_grid_absolute) {
					yz_dist = camera_loc.x + relative_yz_grid_dist;
					relative_yz_grid = false;
				} else {
					if (relative_yz_grid && !relative_yz_grid_absolute) {
						yz_dist = camera_loc.x + relative_yz_grid_dist;
					}
				}
			}

			renderer->setColor(ofyz_grid_color);
			glm::mat4 t = glm::translate(glm::mat4(1.0), glm::vec3(yz_dist,0.0f,0.0f));//translate(mat<4, 4, T, Q> const& m, vec<3, T, Q> const& v) +++++++++
		    renderer->pushMatrix();
		    renderer->multMatrix(t);
		    renderer->get3dGraphics().drawGridPlane(y_grid_interval, number_grid_cells, labels);
			renderer->popMatrix();
		}
		// ****************** X-Z PLANE *******************
		if(display_xz_plane !=0){
			if (!relative_xz_grid && !relative_xz_grid_absolute) {
				xz_dist = 0.0f;
			}else {
				if (relative_xz_grid && relative_xz_grid_absolute) {
					xz_dist = camera_loc.y + relative_xz_grid_dist;
					relative_xz_grid = false;
				} else {
					if (relative_xz_grid && !relative_xz_grid_absolute) {
						xz_dist = camera_loc.y + relative_xz_grid_dist;
					}
				}
			}
	
			renderer->setColor(ofxz_grid_color);
		    glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0,0,-1));
			glm::mat4 t = glm::translate(m, glm::vec3(xz_dist,0,0));
		    renderer->pushMatrix();
		    //renderer->multMatrix(m);
		    renderer->multMatrix(t);
		    renderer->get3dGraphics().drawGridPlane(z_grid_interval, number_grid_cells, labels);
		    renderer->popMatrix();
		}

	    renderer->setColor(prevColor);

    }

	void define_grid_intervals() {
		int cx_level = int(log_10(abs(camera_loc.x- relative_yz_grid_dist)));
		int cy_level = int(log_10(abs(camera_loc.y- relative_xz_grid_dist)));
		int cz_level = int(log_10(abs(camera_loc.z- relative_xy_grid_dist)));
	
		int max_level= max(cx_level,max(cy_level,cz_level));
	
		x_grid_interval = pow(10,max_level)*stepSize;
		y_grid_interval = pow(10,max_level)*stepSize;
		z_grid_interval = pow(10,max_level)*stepSize;
	
		//x_grid_interval = pow(10,cx_level);
		//y_grid_interval = pow(10,cy_level);
		//z_grid_interval = pow(10,cz_level);
	}

private:
	int   x_level=0, y_level=0, z_level=0;
	float x_grid_interval=1.0f,y_grid_interval=1.0f,z_grid_interval=1.0f;

	float xy_dist = 0.0f,yz_dist =0.0f,xz_dist=0.0f;

	float cost_log10 = 1/log(10);

	float log_10(float x){ // OpenGl does not have this math function !!!!
		return cost_log10*log(x);
	}
	

};

/*
    void display_grid() {
	    if (display_yz_plane) {
		    //c.setHsb(0.0f, 200.0f, 255.0f);
		    //renderer->setColor(0,255,0,255);
		    renderer->setColor(yz_grid_color);
		    renderer->get3dGraphics().drawGridPlane(stepSize, number_grid_cells, labels);
	    }
	    if (display_xz_plane) {
		    //c.setHsb(255.0f / 3.0f, 200.0f, 255.0f);
		    //renderer->setColor(0,0,255,255);
		    renderer->setColor(xz_grid_color);
		    glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0,0,-1));
		    renderer->pushMatrix();
		    renderer->multMatrix(m);
		    renderer->get3dGraphics().drawGridPlane(stepSize, number_grid_cells, labels);
		    renderer->popMatrix();
	    }
	    if (display_xy_plane) {
		    //c.setHsb(255.0f * 2.0f / 3.0f, 200.0f, 255.0f);
		    //renderer->setColor(255, 0, 0, 255);
		    renderer->setColor(xy_grid_color);
		    glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0,1,0));
		    renderer->pushMatrix();
		    renderer->multMatrix(m);
		    renderer->get3dGraphics().drawGridPlane(stepSize, number_grid_cells, labels);
		    renderer->popMatrix();
	    }

	    if (labels) {
		    ofDrawBitmapMode mode = renderer->getStyle().drawBitmapMode;
		    renderer->setColor(255, 255, 255);
		    float labelPos = stepSize * (number_grid_cells + 0.5);
		    renderer->setBitmapTextMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		    renderer->drawString("x", labelPos, 0, 0);
		    renderer->drawString("y", 0, labelPos, 0);
		    renderer->drawString("z", 0, 0, labelPos);
		    renderer->setBitmapTextMode(mode);
	    }
	    renderer->setColor(prevColor);

    }
*/
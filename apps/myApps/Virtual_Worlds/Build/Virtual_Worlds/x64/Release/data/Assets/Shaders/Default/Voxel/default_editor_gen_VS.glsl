//#version 450
#version 450 core
//#version 330 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/Universal_reserved_uniforms.glsl"

#include "shader_basis_code/vs_reserved_uniforms.glsl"


// -------------- User Defined Uniforms ----------------------

in float voxel_values; // shader attribute data
//in int voxel_values; // shader attribute data
in vec4 color;

out vec3 positionFromVertex;

uniform int display_as_points;
vec4 raw_color;

#define max_number_gradient_marks 100

uniform int   number_gradient_marks;
uniform int   gradient_interpolation;
uniform float gradient_mark_location[max_number_gradient_marks];
uniform vec4  gradient_mark_color[max_number_gradient_marks];

uniform int   enable_range;
uniform int   min_range_value;
uniform int   max_range_value;

uniform int   enable_value;
uniform int   selected_value;

uniform vec3  min_matrix_coord;
uniform vec3  max_matrix_coord;

uniform vec3 brush_radius;

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as application defined uniforms are placed here
// Must exist in every glsl code unless user wishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved functions ----------------

// -------------- User Defined Functions -------------------

vec4 int_gradient_color_linear(int value){
	if(value < 0.0f) return vec4(0.0,0.0,0.0,0.0);
	
	vec4 gcolor = {0.0,0.0,0.0,1.0};
	float color_value = float(value)/255.0f;
	
	for(int i=0; i<number_gradient_marks; i++){
		if(i+1 < number_gradient_marks){
			float lower_gradient_loc = gradient_mark_location[i];
			float upper_gradient_loc = gradient_mark_location[i+1];
			
			if(color_value >= lower_gradient_loc && color_value <upper_gradient_loc){ // Inside a gradient mark range
				vec4  lower_color = gradient_mark_color[i];
				vec4  upper_color = gradient_mark_color[i+1];
				
				vec4 color_grad = (upper_color-lower_color)/(upper_gradient_loc-lower_gradient_loc);
				
				gcolor = lower_color+color_grad*(color_value-lower_gradient_loc);
				break;
			}
		} else {
			float gradient_loc0 = gradient_mark_location[i];
			float gradient_locN = gradient_mark_location[number_gradient_marks-1];
			if(color_value < gradient_loc0) {
				gcolor = gradient_mark_color[0];
				break;
			}
			else
				if(color_value > gradient_locN){
					gcolor = gradient_mark_color[number_gradient_marks-1];
					break;
				}
		}
	}

	return gcolor;
}

vec4 int_gradient_color_step(int value){
	if(value < 0.0f) return vec4(0.0,0.0,0.0,0.0);
	
	vec4 gcolor = {0.0,0.0,0.0,1.0};
	float color_value = float(value)/255.0f;
	
	for(int i=0; i<number_gradient_marks; i++){
		if(i+1 < number_gradient_marks){
			float lower_gradient_loc = gradient_mark_location[i];
			float upper_gradient_loc = gradient_mark_location[i+1];
			
			if(color_value >= lower_gradient_loc && color_value <upper_gradient_loc){ // Inside a gradient mark range
				gcolor = gradient_mark_color[i+1];
				break;
			}
		} else {
			float gradient_loc0 = gradient_mark_location[i];
			float gradient_locN = gradient_mark_location[number_gradient_marks-1];
			if(color_value < gradient_loc0) {
				gcolor = gradient_mark_color[0];
				break;
			}
			else
				if(color_value > gradient_locN){
					gcolor = gradient_mark_color[number_gradient_marks-1];
					break;
				}
		}
	}

	return gcolor;
}

bool inside_matrix_display(){
	if((position.x >= min_matrix_coord.x && position.x <= max_matrix_coord.x) &&
	   (position.y >= min_matrix_coord.y && position.y <= max_matrix_coord.y) &&
	   (position.z >= min_matrix_coord.z && position.z <= max_matrix_coord.z)) return true;
	
	return false;
}

void main(){
	//int voxel_value = int(position.w);              // required as a min
	int voxel_value = int(voxel_values);              // required as a min
	//int voxel_value = int(color.w);              // required as a min
	int matrix_value = -1;
	
	// following required as a min
	if(display_as_points == 0){ // do not display as points
		gl_Position = position; // required as a min
	}else{// diaplay as points
		gl_Position = modelViewProjectionMatrix*position; // required as a min
	}
	
	raw_color = vec4(0.0,1.0,0.0,1.0);
	
	//raw_color = int_gradient_color_linear(int(position.w));
	
	//voxel_value = 150; // testing only : comment out delete when finished
	
	if(inside_matrix_display()){
	
		if(enable_range==1){
			if(voxel_value > min_range_value && voxel_value < max_range_value){
				matrix_value = voxel_value;
			}
		} else {
			matrix_value = voxel_value;
		}
		
		if(enable_value == 1){
			if(voxel_value == selected_value)
				matrix_value = voxel_value;
		}
		

		if(gradient_interpolation==1)
			raw_color = int_gradient_color_linear(matrix_value);
		else
			raw_color = int_gradient_color_step(matrix_value);
	
	} else
		raw_color = vec4(0.0,0.0,0.0,0.0);
	
	if(voxel_value == 0) raw_color = vec4(1.0,0.0,0.0,1.0);
	//if(voxel_values == 0) raw_color = vec4(0.0,1.0,0.0,1.0);
	
	vs_out.varyingColor = raw_color;

}

#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/universal_reserved_uniforms.glsl"

#include "shader_basis_code/vs_reserved_uniforms.glsl"

// -------------- User Defined Uniforms ----------------------
//uniform float height_map_axis;
uniform float min_height;
uniform float max_height;

//float min_height      = -5.0;
//float max_height      = 5.0;
vec3 vertex;
vec4 light_color     = vec4(1.0,0.0,0.0,1.0);
//vec4 raw_color       = vec4(1.0,0.0,0.0,1.0);
vec4 raw_color;

uniform int display_as_points;

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------

/*
vec4 use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){
     vec3 diffuse;
     float NdotL;
     vec3 viewDir;
     vec3 reflectDir;
     vec3 light_intensity;
	 vec3 camera_light_intensity;
	 vec3 light_direction_vector;
	 
     NdotL   = max(dot(normalize(vertex_normal), normalize(-lighting_direction)), 0.0);
	 //NdotL   = max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0);
     diffuse = NdotL * light_color.xyz;

     viewDir    = normalize(camera_loc - vec3(vertex.xyz));
     reflectDir = reflect(normalize(-lighting_direction), vertex_normal);
	 //reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

     //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
     vec3 specular = specular_strength * spec * light_color.xyz;

	 //light_intensity = light_intensity + (ambient_light + diffuse + specular);
	 light_intensity = (ambience + diffuse + specular)*lighting_intensity*0.5;//*.25

	 camera_light_intensity = vec3(0.0,0.0,0.0);
	 if(use_camera_lighting){
		 if(camera_lighting_type == 0){
			 vec3 light_camera_relative_x = camera_right_vector *lighting_camera_offset.x;
			 vec3 light_camera_relative_y = camera_up_vector    *lighting_camera_offset.y;
			 vec3 light_camera_relative_z = camera_front_vector *lighting_camera_offset.z;
			 
			 vec3 light_loc =  camera_loc  + light_camera_relative_x + light_camera_relative_y + light_camera_relative_z;
			 
			 light_direction_vector = -(light_loc - vertex.xyz);// spot light from camera pointing dir of camera
		 } else
			 light_direction_vector = (camera_front_vector);//  global light pointing in direction of camera : not good
		 
		 NdotL   = max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0);
		 diffuse = NdotL * light_color.xyz;

		 viewDir    = normalize(camera_loc - vec3(vertex.xyz));
		 reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

		 //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
		 vec3 specular = specular_strength * spec * light_color.xyz;
		 
		 camera_light_intensity = (diffuse + specular)*lighting_intensity; //*.5
	 }
	 
     //gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
     return vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
}
*/

float x;
float y;
float z;

float r;
float g;
float b;

float height;

vec4  map_height_color(){
   height = 2.0;

   if(vertex.z >= min_height && vertex.z <= max_height)
	   height = (vertex.z - min_height)/(max_height-min_height);


// RGB color cube color space along cube edges 
  if(height <= 0.1666667){
      r = (0.1666667-height)/0.1666667;
      g = 0;
      b = 1.0;
  } else {
      if(height > 0.1666667 && height <= 0.3333333){
          r = 0.0;
          g = (height-0.1666667)/0.1666667;
          b = 1.0;
       } else {
          if(height > 0.3333333 && height <= 0.5){
              b = (0.5-height)/0.1666667;
              r = 0.0;
              g = 1.0;
           } else {
               if(height > 0.5 && height <= 0.6666667){
                   b = 0;
                   r = (height-0.5)/0.1666667;
                   g = 1.0;
               } else {
                    if(height > 0.6666667 && height <= 0.8333333){
                        b = 0;
                        r = 1.0;
                        g = (0.8333333-height)/0.1666667;
                    } else {
                         if(height <= 1.0){
                             r = 1.0;
                             b = (height-0.8333333)/0.1666667;
                             g = (height-0.8333333)/0.1666667;
                } else {
                  r = 0.5;
                  g = 0.5;
                  b = 0.5;
              }
              }
              }
         }
         }
     }
     return vec4(r,g,b,1.0);
//     varyingColor = vec4(r,g,b,1.0);
}

void main(){

	// following required as a min
	if(display_as_points == 0){ // do not display as points
		gl_Position = position; // required as a min
		//gl_Position = vec4(position.x,position.y,position.z,1.0); // required as a min
		//vs_out.varyingColor = vec4(1.0,0.0,0.0,1.0);
	}else{// diaplay as points
		gl_Position = modelViewProjectionMatrix*position; // required as a min
		//vs_out.varyingColor = vec4(0.0,1.0,0.0,1.0);
	}
	
	vertex = position.xyz;
	raw_color = map_height_color();// required as a min not working properly for unknown reason need to fix up
	
	
	vs_out.varyingColor = raw_color;
}
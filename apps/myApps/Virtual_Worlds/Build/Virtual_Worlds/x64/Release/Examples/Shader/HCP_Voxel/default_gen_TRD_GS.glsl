//#version 450
//#version 450 core
#version 450 core

// -------------- Shader Reserved Uniforms -------------------

layout(points) in;
layout(triangle_strip, max_vertices = 48) out;
//layout(points, max_vertices = 1) out;// Testing only

#include "shader_basis_code/gs_reserved_uniforms.glsl"



// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#


// -------------- Shader Reserved funtions ----------------

#include "shader_basis_code/gs_defined_functions.glsl"

// -------------- User Defined Functions -------------------

void use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){

     vec4 diffuse;
     float NdotL;
     vec3 viewDir;
     vec3 reflectDir;
     vec4 light_intensity;
	 vec4 camera_light_intensity;
	 vec3 light_direction_vector;
	 
	 vec3 camera_loc = uCameraPos;
	 light_color = global_light_color;
	 lighting_intensity = global_light_intensity;
	 lighting_direction = global_light_direction;
 
	 if(uCamera_forward.x == 0.0f && uCamera_forward.y == 0.0f && uCamera_forward.z == 0.0f){
	 //if(uCameraPos.x == 0.0f && uCameraPos.y == 0.0f && uCameraPos.z == 0.0f){
		gs_out.varyingColor = vec4(1.0,1.0,0.0,1.0);
	 } else{
	 
     NdotL   = max(dot(normalize(vertex_normal), normalize(-lighting_direction)), 0.0);
     diffuse = NdotL * light_color;

     viewDir    = normalize(camera_loc - vec3(vertex.xyz));
     reflectDir = reflect(normalize(-lighting_direction), vertex_normal);

     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
     vec4 specular = mat_specular * spec * light_color;

	 light_intensity = (mat_ambient + diffuse + specular)*lighting_intensity*0.5;//*.25

	 camera_light_intensity = vec4(0.5,0.5,0.5,1.0);
	 
	 // Test code
//	 if(global_light_location.x == 100.1 && global_light_location.y == 900 && global_light_location.z == 1300){
//		 gs_out.varyingColor = raw_color;
//	 } else {
		 if(use_camera_lighting!=0){
			 if(camera_lighting_type == 0){
				 vec3 light_camera_relative_x = uCamera_right   *lighting_camera_offset.x;
				 vec3 light_camera_relative_y = uCamera_up      *lighting_camera_offset.y;
				 vec3 light_camera_relative_z = uCamera_forward *lighting_camera_offset.z;
				 
				 vec3 light_loc =  uCameraPos  + light_camera_relative_x + light_camera_relative_y + light_camera_relative_z;
				 
				 light_direction_vector = -(light_loc - vertex.xyz);// spot light from camera pointing dir of camera
			 } else
				 light_direction_vector = (uCamera_forward);//  global light pointing in direction of camera : not good
			 
			 NdotL   = max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0);
			 diffuse = NdotL * light_color;

			 viewDir    = normalize(uCameraPos - vec3(vertex.xyz));
			 reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

			 //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
			 vec4 specular = specular_strength * spec * light_color;
			 
			 camera_light_intensity = (diffuse + specular)*lighting_intensity; //*.5
		 }
     //gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
     gs_out.varyingColor = vec4((light_intensity+camera_light_intensity))* raw_color;
	 }
//	 }

//test
//	gs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);
}

float sqrt_3 = sqrt(3.0);
float sqrt_6 = sqrt(6.0);

float sf     = 20.0f;//1.0f/0.48f *10.0f;

vec4 point_0 = vec4(0.0,0.0,(1.0/sqrt_6+sqrt_6/3.0),0.0)* voxSize*sf;
vec4 point_1 = vec4(0.0,2.0/sqrt_3,sqrt_6/3.0,0.0)* voxSize*sf;
vec4 point_2 = vec4(1.0,1.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize*sf;
vec4 point_3 = vec4(1.0,-1.0/sqrt_3,sqrt_6/3.0,0.0)* voxSize*sf;
vec4 point_4 = vec4(0.0,-2.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize*sf;
vec4 point_5 = vec4(-1.0,-1.0/sqrt_3,(sqrt_6/3.0),0.0)* voxSize*sf;
vec4 point_6 = vec4(-1.0,1.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize*sf;
/*
vec4 point_0 = vec4(0.0,0.0,(1.0/sqrt_6+sqrt_6/3.0),0.0)* voxSize;
vec4 point_1 = vec4(0.0,2.0/sqrt_3,sqrt_6/3.0,0.0)* voxSize;
vec4 point_2 = vec4(1.0,1.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize;
vec4 point_3 = vec4(1.0,-1.0/sqrt_3,sqrt_6/3.0,0.0)* voxSize;
vec4 point_4 = vec4(0.0,-2.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize;
vec4 point_5 = vec4(-1.0,-1.0/sqrt_3,(sqrt_6/3.0),0.0)* voxSize;
vec4 point_6 = vec4(-1.0,1.0/sqrt_3,(sqrt_6/3.0-1.0/sqrt_6),0.0)* voxSize;
*/

void add_top_bottom(vec4 center, int top_bottom, int sector){
	
	// glsl has rounding and accuracy problems with converting float to int and need to add 0.5 so any values
	// very close to an integer value but just below it are not floored
	//int hcp_layer = int ((center.z-voxel_origin.z)/voxel_hcp_z_increment+0.5);
	
	//float layer = (center.z-voxel_origin.z*sf)/(voxel_hcp_z_increment*sf);
	
	int hcp_layer = int (abs(center.z-voxel_origin.z*sf)/(voxel_hcp_z_increment*sf)+0.5); 
	
	if(top_bottom == 1){
		if(sector == 0 ){
			if(mod(hcp_layer,2) == 0){
				vec3 vertex_normal = -vec3(-0.5,-0.2886751346,-0.8164965809);
			
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_2.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
				EmitVertex();
			
			} else{
				vec3 vertex_normal = -vec3(0.1889822365,0.3273268354,0.9258200998);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_2.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
				EmitVertex();
			}
			
			EndPrimitive();
	
		}
	
		if(sector == 1){
			vec4 color = vec4(0.0,1.0,1.0,1.0);
			if(mod(hcp_layer,2) == 0){
				vec3 vertex_normal = -vec3(0.0,0.5773502692,-0.8164965809);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_4.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
					
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
				EmitVertex();
			} else{
				vec3 vertex_normal = -vec3(-0.1889822365,0.3273268354,0.9258200998);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
				EmitVertex();
				
									
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_4.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);
				gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
				EmitVertex();

			}

			EndPrimitive();
		}
		
		if(sector == 2){
			vec4 color = vec4(1.0,1.0,0.0,1.0);
			if(mod(hcp_layer,2) == 0){
				vec3 vertex_normal = -vec3(0.5,-0.2886751346,-0.8164965809);

				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_6.z,1.0));
				EmitVertex();
					
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
				EmitVertex();

			} else{
				vec3 vertex_normal = -vec3(0.0,-0.5773502692,-0.8164965809);
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
				EmitVertex();
				
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_6.z,1.0));
				EmitVertex();				
					
				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,-point_0.z,1.0));
				EmitVertex();

				use_lighting(center, vertex_normal,gs_in[0].varyingColor);

				gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
				EmitVertex();
	
			}
			
			EndPrimitive();
		}
	}
	
		if(top_bottom == -1){
			if(sector == 0 ){
					if(mod(hcp_layer,2) == 0){
						vec3 vertex_normal = -vec3(0.5,0.2886751346,-0.8164965809);
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_2.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
						EmitVertex();

					} else {
						vec3 vertex_normal = vec3(-0.1889822365,-0.3273268354,0.9258200998);
					
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_5.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_4.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
						EmitVertex();
						
						use_lighting(center, vertex_normal,gs_in[0].varyingColor);
						gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_3.z,1.0));
						EmitVertex();

					}
				EndPrimitive();
			}
			
			if(sector == 1){
				if(mod(hcp_layer,2) == 0){
					vec3 vertex_normal =-vec3(0.5,0.2886751346,-0.8164965809);
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_4.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
					EmitVertex();
	
				} else{
					vec3 vertex_normal = vec3(0.1889822365,-0.3273268354,0.9258200998);

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_1.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_6.z,1.0));
					EmitVertex();

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_5.z,1.0));
					EmitVertex();
				}
				EndPrimitive();
			}
			
			if(sector == 2){
				if(mod(hcp_layer,2) == 0){
					vec3 vertex_normal = -vec3(-0.5,0.2886751346,-0.8164965809);

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_6.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
					EmitVertex();
				} else {
					vec3 vertex_normal = -vec3(0.0,0.5773502692,-0.8164965809);

					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_3.z,1.0));
					EmitVertex();					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);

					gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_2.z,1.0));
					EmitVertex();
						
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_0.x,point_0.y,point_0.z,1.0));
					EmitVertex();
					
					use_lighting(center, vertex_normal,gs_in[0].varyingColor);
					gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_1.z,1.0));
					EmitVertex();
					
				}
				EndPrimitive();
			}
	}

}

void add_side(vec4 center,int side){

	//int hcp_layer = int ((center.z-voxel_origin.z)/voxel_hcp_z_increment+0.5);
	int hcp_layer = int (abs(center.z-voxel_origin.z*sf)/(voxel_hcp_z_increment*sf)+0.5); 

	if(side == 0){
		vec3 vertex_normal = -vec3(-0.5,-0.8660254038,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);

		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_1.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_6.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_6.z,1.0));
		EmitVertex();

		EndPrimitive();
	}
	
	if(side == 1){
		vec3 vertex_normal =-vec3(-1,0,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_2.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_2.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,point_1.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_2.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_2.x,point_2.y,-point_1.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}
	
	if(side == 2){
		vec3 vertex_normal = -vec3(0.8660254038,0.5,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_3.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,point_2.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_3.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_3.x,point_3.y,-point_2.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

	if(side == 3){
		vec3 vertex_normal = -vec3(-0.8660254038,0.5,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_4.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_4.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,point_3.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_4.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_4.x,point_4.y,-point_3.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

	if(side == 4){
		vec3 vertex_normal = -vec3(1,0,0.0);

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_5.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,point_4.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_5.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_5.x,point_5.y,-point_4.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

	if(side == 5){
		vec3 vertex_normal = -vec3(0.5,-0.8660254038,0.0);

		use_lighting(center, -vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,point_6.z,1.0));
		EmitVertex();
		
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_1.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_1.x,point_1.y,-point_6.z,1.0));
		EmitVertex();

		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,point_5.z,1.0));
		EmitVertex();
	
		use_lighting(center, vertex_normal,gs_in[0].varyingColor);
		if(mod(hcp_layer,2) == 0)
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_6.z,1.0));
		else
			gl_Position = mvpMatrix*(center + vec4(point_6.x,point_6.y,-point_5.z,1.0));
		EmitVertex();
		
		EndPrimitive();
	}

}



void main(){
	mvpMatrix = modelViewProjectionMatrix;
	
    vec4 center = gl_in[0].gl_Position*sf;
    

	  for(int sector = 0; sector<3;sector++){
		  add_top_bottom(center,1,sector); // top surface elelment
	  }
	
	 for(int sector = 0; sector<3;sector++){
		  add_top_bottom(center,-1,sector); // bottom surface elelment
	 }
	
	 for(int side = 0; side<6;side++){
		 add_side(center,side); // side surface elelment
	 }


	//gl_Position = modelViewProjectionMatrix * center;
	// gl_Position = mvpMatrix * center;
	// gs_out.varyingColor = gs_in[0].varyingColor;
	// EmitVertex();
	// EndPrimitive();


/*
	gl_Position = modelViewProjectionMatrix * vec4(center.x+1.0,center.y,center.z, 1.0);
	gs_out.varyingColor = gs_in[0].varyingColor;
	EmitVertex();
	
	gl_Position = modelViewProjectionMatrix * vec4(center.x+1.0,center.y+1.0,center.z, 1.0);
	gs_out.varyingColor = gs_in[0].varyingColor;
	EmitVertex();
	
	gl_Position = modelViewProjectionMatrix * vec4(center.x,center.y+1.0,center.z, 1.0);
	gs_out.varyingColor = gs_in[0].varyingColor;
	EmitVertex();

	gl_Position = modelViewProjectionMatrix * vec4(center.x,center.y,center.z, 1.0);
	gs_out.varyingColor = gs_in[0].varyingColor;
	EmitVertex();
	
	EndPrimitive();
*/
}

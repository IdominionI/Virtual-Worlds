#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/universal_reserved_uniforms.glsl"

layout(points) in;

#include "Shader_basis_code/pgs_reserved_uniforms.glsl"

// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

#include "Shader_basis_code/pgs_defined_functions.glsl"

// -------------- User Defined Functions -------------------
// A layout for the output must be defined for whatever geometry shader functions are performed
layout(triangle_strip, max_vertices = 72) out; // The output needs to be defined by the user in the function definitions
//layout(triangle_strip, max_vertices = 3) out; // The output needs to be defined by the user in the function definitions
//layout(points, max_vertices = 1) out; // The output needs to be defined by the user in the function definitions


void use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){

     vec4 diffuse;
     float NdotL;
     vec3 viewDir;
     vec3 reflectDir;
     vec4 light_intensity;
	 vec4 camera_light_intensity;
	 vec3 light_direction_vector;
	 
	 vec3 camera_loc = uCameraPos;
 
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

//test
//	gs_out.varyingColor = vec4(1.0,1.0,1.0,1.0);
}

/*  
	void use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){
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
     vec3 specular = mat_specular * spec * light_color.xyz;

	 light_intensity = (mat_ambient + diffuse + specular)*lighting_intensity*0.5;//*.25

	 camera_light_intensity = vec3(0.0,0.0,0.0);
	 if(use_camera_lighting!=0){
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
		 vec3 specular = mat_specular * spec * light_color.xyz;
		 
		 camera_light_intensity = (diffuse + specular)*lighting_intensity; //*.5
	 }
	 
     //gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
     gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* gs_in[0].varyingColor;
     //gs_out.varyingColor = gs_in[0].varyingColor;
*/	 

//111
//	 gs_out.varyingColor = raw_color;
//}

/*
void use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){

     vec4 diffuse;
     float NdotL;
     vec3 viewDir;
     vec3 reflectDir;
     vec4 light_intensity;
	 vec4 camera_light_intensity;
	 vec3 light_direction_vector;
	 
 
     NdotL   = max(dot(normalize(vertex_normal), normalize(-lighting_direction)), 0.0);
     diffuse = NdotL * light_color;

     viewDir    = normalize(camera_loc - vec3(vertex.xyz));
	 
	 vec3 result = vec3(0.0,0.0,0.0);
	 
	 // phase 1: directional lighting
	 for(int i = 0; i < number_directional_lights; i++)
	     result += CalcDirLight(DirLight[i], vertex_normal, viewDir, material);

	 // phase 2: point lights
	mat4 model = mat4( 1.0f );
	vec3 FragPos = vec3(model * vertex);
    for(int i = 0; i < number_point_lights; i++)// !!!!!!!!
        result += CalcPointLight(PointLights[i], vertex_normal, FragPos, viewDir, material);

	 // phase 3: spot light
	 for(int i = 0; i < number_spot_lights; i++)
		 result += CalcSpotLight(SpotLight[i], vertex_normal, FragPos, viewDir, material);

//gs_out.varyingColor = vec4(result, 1.0)* raw_color; 	 


//gs_out.varyingColor = vec4((vec4(result, 1.0)+camera_light_intensity))* raw_color;
	 
     reflectDir = reflect(normalize(-lighting_direction), vertex_normal);

     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
     vec4 specular = specular_strength * spec * light_color;

	 light_intensity = (ambience + diffuse + specular)*lighting_intensity*0.5;//*.25

	 camera_light_intensity = vec4(0.0,0.0,0.0,1.0);
	 if(use_camera_lighting!=0){
		 if(camera_lighting_type == 0){
			 vec3 light_camera_relative_x = camera_right_vector *lighting_camera_offset.x;
			 vec3 light_camera_relative_y = camera_up_vector    *lighting_camera_offset.y;
			 vec3 light_camera_relative_z = camera_front_vector *lighting_camera_offset.z;
			 
			 vec3 light_loc =  camera_loc  + light_camera_relative_x + light_camera_relative_y + light_camera_relative_z;
			 
			 light_direction_vector = -(light_loc - vertex.xyz);// spot light from camera pointing dir of camera
		 } else
			 light_direction_vector = (camera_front_vector);//  global light pointing in direction of camera : not good
		 
		 NdotL   = max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0);
		 diffuse = NdotL * light_color;

		 viewDir    = normalize(camera_loc - vec3(vertex.xyz));
		 reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

		 //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
		 vec4 specular = specular_strength * spec * light_color;
		 
		 camera_light_intensity = (diffuse + specular)*lighting_intensity; //*.5
		 
		 gs_out.varyingColor = vec4((vec4(result, 1.0)+camera_light_intensity))* raw_color;
	 } else {
		gs_out.varyingColor = vec4((vec4(result, 1.0)+light_intensity))* raw_color;
	 }
     //gs_out.varyingColor = vec4((vec4(0.5f,0.5f,0.5f, 1.0)+camera_light_intensity))* raw_color;
}
*/
// -------------------------------------------------------------------------------

float xp = 0.866025f;
float sf = 2.0f; // Scaling factor : if not of value around 2.0 have point and hex columns not equating with each other. 

vec4 point_0 = vec4(0.0,0.0,0.0,1.0) ;
// vec4 point_1 = vec4(0.0,1.0* hexSize,0.0,1.0);
// vec4 point_2 = vec4(xp* hexSize,0.5* hexSize,0.0,1.0)  ;
// vec4 point_3 = vec4(xp* hexSize,-0.5* hexSize,0.0,1.0) ;
// vec4 point_4 = vec4(0.0,-1.0* hexSize,0.0,1.0);
// vec4 point_5 = vec4(-xp* hexSize,-0.5* hexSize,0.0,1.0);
// vec4 point_6 = vec4(-xp* hexSize,0.5* hexSize,0.0,1.0) ;

vec4 point_1 = vec4(0.0,sf* hexSize,0.0,1.0);
vec4 point_2 = vec4(xp* hexSize*sf,0.5* hexSize*sf,0.0,1.0)  ;
vec4 point_3 = vec4(xp* hexSize*2.0,-0.5* hexSize*sf,0.0,1.0) ;
vec4 point_4 = vec4(0.0,-sf* hexSize,0.0,1.0);
vec4 point_5 = vec4(-xp* hexSize*sf,-0.5* hexSize*sf,0.0,1.0);
vec4 point_6 = vec4(-xp* hexSize*sf,0.5* hexSize*sf,0.0,1.0) ;

void add_top(vec4 center,bool top){
	vec3 vertex_normal = vec3(0.0,0.0,1.0);
	
	if(!top)
		vertex_normal.z = -1;
			
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();

	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_1);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_2);
	EmitVertex();

	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_2);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_3);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_3);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_4);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_4);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_5);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_5);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_6);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_6);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(center + point_1);
	EmitVertex();
	
	EndPrimitive();

}


void add_sides(vec4 center){
	vec3 vertex_normal;
	vec4 top    = center;
	vec4 bottom = center;
	bottom.z = 0.0;
	
		// Side 1
	vertex_normal= vec3(0.25,0.75,0.0);
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_1);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_1);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_2);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_1);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_2);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_2);
	EmitVertex();
	
	EndPrimitive();
	
	// Side 2
	vertex_normal= vec3(1.0,0.0,0.0);
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_2);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_2);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_3);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_2);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_3);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_3);
	EmitVertex();
	
	EndPrimitive();
	
	// Side 3
	vertex_normal= vec3(0.25,-0.75,0.0);
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_3);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_3);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_4);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_3);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_4);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_4);
	EmitVertex();
	
	EndPrimitive();
	
	// Side 4
	vertex_normal= vec3(-0.25,-0.75,0.0);
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_4);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_4);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_5);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_4);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_5);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_5);
	EmitVertex();
	
	EndPrimitive();
	
	// Side 5
	vertex_normal= vec3(-1.0,0.0,0.0);
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_5);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_5);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_6);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_5);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_6);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_6);
	EmitVertex();
	
	EndPrimitive();
	
	// Side 6
	vertex_normal= vec3(-0.25,0.75,0.0);
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_6);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_6);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_1);
	EmitVertex();
	
	EndPrimitive();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_6);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(top + point_1);
	EmitVertex();
	
	use_lighting(center, vertex_normal,gs_in[0].varyingColor);
	gl_Position = mvpMatrix*(bottom + point_1);
	EmitVertex();
	
	EndPrimitive();
}


void main (){
	mvpMatrix = modelViewProjectionMatrix;

	vec4 center = gl_in[0].gl_Position;
	
	center.x *= sf;
	center.y *= sf;
	center.z *= sf;

	// following do not work : need to fix as probably die to uniforms 
	// mat_specular and others not being set
	add_top(center,true); // top surface element
	add_sides(center);    // side surface element 

	add_top(vec4(center.x,center.y,0.0,1.0),false); // bottom surface element

	// following for testing only : delete when finished

	////gl_Position = modelViewProjectionMatrix * center;
	 // gl_Position = mvpMatrix * center;
	 // if(hexSize != 0.0)
		// gs_out.varyingColor = gs_in[0].varyingColor;
	 // else 
		// gs_out.varyingColor = vec4(0.0,0.0,1.0,1.0);
	 // EmitVertex();
	 // EndPrimitive();

	//gs_out.varyingColor = gs_in[0].varyingColor;

}
#version 450 core

// ---------- Universal shader uniforms ------------
uniform mat4 matVP;
uniform mat4 matGeo;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 modelViewProjectionMatrix;//model-view-projection matrix
//uniform mat4 mvpMatrix; //model-view-projection matrix
mat4 mvpMatrix; //model-view-projection matrix

// ---------- End Universal shader uniforms ----------

// ========== PGS reserved shader uniforms ===========
//layout(points) in;
layout(triangles) in;// This is a ShaderED thing that loads an imported file as triangles

//uniform float hexSize;
float hexSize = .01;

//hex surface display data
//uniform float hex_min_surface_display_value;
//uniform float hex_max_surface_display_value;


//uniform vec2  grid_origin;
//uniform float voxel_hcp_z_increment;
vec2  grid_origin = vec2(0.0,0.0);
float voxel_hcp_z_increment = .01f;

in VertexData
{
 // float value;
  vec4 varyingColor;
} gs_in[]; // Must have as an array otherwise will not work as one for each input vertex,

out VertexData
{
//  float value;
  vec4 varyingColor;
} gs_out;


// ========== End PGS shader uniforms =============

// -------------- User Defined Functions -------------------
// A layout for the output must be defined for whatever geometry shader functions are performed
//layout(triangle_strip, max_vertices = 72) out; // The output needs to be defined by the user in the function definitions
//layout(triangle_strip, max_vertices = 1) out; // The output needs to be defined by the user in the function definitions
layout(points, max_vertices = 1) out; // The output needs to be defined by the user in the function definitions


// -------------------------------------------------------------------------------
void use_lighting(vec4 vertex, vec3 vertex_normal, vec4 raw_color){
/*     vec3 diffuse;
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
	 gs_out.varyingColor = raw_color;
}

// -------------------------------------------------
float xp = 0.866025;

vec4 point_0 = vec4(0.0,0.0,0.0,1.0) ;
vec4 point_1 = vec4(0.0,1.0* hexSize,0.0,1.0);
vec4 point_2 = vec4(xp* hexSize,0.5* hexSize,0.0,1.0)  ;
vec4 point_3 = vec4(xp* hexSize,-0.5* hexSize,0.0,1.0) ;
vec4 point_4 = vec4(0.0,-1.0* hexSize,0.0,1.0);
vec4 point_5 = vec4(-xp* hexSize,-0.5* hexSize,0.0,1.0);
vec4 point_6 = vec4(-xp* hexSize,0.5* hexSize,0.0,1.0) ;

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


// !!!!!!!!!!!!!!!!!!!!!!  GEOMETRY SHADER CODE !!!!!!!!!!!!!!!!!!!!!!

void main (){
	
	//mvpMatrix = modelViewProjectionMatrix;
	mvpMatrix = matVP*matGeo;//modelViewProjectionMatrix;
	//mvpMatrix = projection*view*model;

	vec4 center = gl_in[0].gl_Position;

	// following do not work : need to fix as probably die to uniforms 
	// mat_specular and others not being set
	add_top(center,true); // top surface element
	add_sides(center);    // side surface element 
	add_top(vec4(center.x,center.y,0.0,1.0),false); // bottom surface element
	
	// following for testing only : delete when finished
	gs_out.varyingColor = gs_in[0].varyingColor;
	//gs_out.varyingColor = vec4(1.0,0.0,0.0,1.0);
	
	
	//gl_Position = center;
	gl_Position = mvpMatrix*center;
	//gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	EndPrimitive();

	
	
}
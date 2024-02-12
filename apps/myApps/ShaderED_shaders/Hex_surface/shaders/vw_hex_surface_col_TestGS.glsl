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

// !!!!!!!!!!!!!!!!!!!!!!  GEOMETRY SHADER CODE !!!!!!!!!!!!!!!!!!!!!!

void main (){
	
	//mvpMatrix = modelViewProjectionMatrix;
	mvpMatrix = matVP*matGeo;//modelViewProjectionMatrix;
	//mvpMatrix = projection*view*model;

	vec4 center = gl_in[0].gl_Position;

	// following do not work : need to fix as probably die to uniforms 
	// mat_specular and others not being set
	//add_top(center,true); // top surface element
	//add_sides(center);    // side surface element 


	//add_top(vec4(center.x,center.y,0.0,1.0),false); // bottom surface element
	
	// following for testing only : delete when finished
	gs_out.varyingColor = gs_in[0].varyingColor;
	//gs_out.varyingColor = vec4(1.0,0.0,0.0,1.0);
	
	
	//gl_Position = center;
	gl_Position = mvpMatrix*center;
	//gl_Position = mvpMatrix*(center + point_0);
	EmitVertex();
	EndPrimitive();

	
	
}
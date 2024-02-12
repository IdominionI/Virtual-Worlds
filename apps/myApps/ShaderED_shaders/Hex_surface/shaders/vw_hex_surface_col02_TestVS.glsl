#version 450 core

// ---------- Universal shader uniforms ------------

// ### uniform mat4 modelViewProjectionMatrix;//model-view-projection matrix
uniform mat4 matVP;
uniform mat4 matGeo;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// ---------- End Universal shader uniforms ----------


// +++++++++++ VS reserved shader uniforms +++++++++++++
in vec4 position; // Vertex position 


// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following output block is to allow  varyingColor to be passed to either 
// a geometry shader to be psssed on to the fragment shader, or to the fragemt alone
out VertexData
{
//  float  value;
  vec4 varyingColor;
} vs_out;

// ++++++++++++ End VS shader uniforms ++++++++++++++


// -------------- User Defined Uniforms ----------------------
//uniform float min_height;
//uniform float max_height;

float min_height      = -.30;
float max_height      = .40;

vec3 vertex;
vec4 light_color     = vec4(1.0,0.0,0.0,1.0);
//vec4 raw_color       = vec4(1.0,0.0,0.0,1.0);
vec4 raw_color;

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------


// !!!!!!!!!!!!!!!!!!!!!!  VERTEX SHADER CODE !!!!!!!!!!!!!!!!!!!!!!
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

void main() {
  // gl_Position = matVP * matGeo * position;
   
    vertex = position.xyz;
	gl_Position = position;

	//raw_color = vec4(1.0,0.0,1.0,1.0); // required as a min testing
	raw_color =  map_height_color();; // required as a min testing
	
	vs_out.varyingColor = raw_color;

}
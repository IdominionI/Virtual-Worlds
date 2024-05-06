#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
//in vec2 texcoord;

//out vec2 vTexCoord;

in float test_attr_data;

//test_error

void main() {
    //vTexCoord = texcoord;
    
   //gl_Position = modelViewProjectionMatrix * position;
	
	//if(test_attr_data >= 199.0f)
      gl_Position = modelViewProjectionMatrix * position + test_attr_data;
	//else
	//  gl_Position = vec4(0.0,0.0,0.0,0.0);
}

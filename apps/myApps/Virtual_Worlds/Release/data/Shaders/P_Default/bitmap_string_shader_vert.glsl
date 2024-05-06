Vertex Shader bitmapStringShader
#version 450

#define IN in
#define OUT out
#define TEXTURE texture

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

IN vec4 position;
IN vec4 color;
IN vec2 texcoord;

OUT vec2 texCoordVarying;

void main() { 
	texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}

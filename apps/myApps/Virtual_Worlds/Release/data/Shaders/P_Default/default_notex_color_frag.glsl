Fragment SHADER  : defaultNoTexColor
-----------------------
#version 450

#define IN in
#define OUT out
#define TEXTURE texture
#define FRAG_COLOR fragColor

out vec4 fragColor;
uniform float usingTexture;
uniform float usingColors;
uniform vec4 globalColor;

IN float depth;
IN vec4 colorVarying;
IN vec2 texCoordVarying;

void main(){ 
	FRAG_COLOR = colorVarying;
}

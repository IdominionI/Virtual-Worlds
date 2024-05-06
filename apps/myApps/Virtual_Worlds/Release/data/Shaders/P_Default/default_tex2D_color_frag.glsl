Fragment SHADER  : defaultTex2DColor
-----------------------
#version 450

#define IN in
#define OUT out
#define TEXTURE texture
#define FRAG_COLOR fragColor

out vec4 fragColor;

uniform sampler2D src_tex_unit0;
uniform float usingTexture;
uniform float usingColors;
uniform vec4 globalColor;

IN float depth;
IN vec4 colorVarying;
IN vec2 texCoordVarying;

void main(){ 
	FRAG_COLOR = TEXTURE(src_tex_unit0, texCoordVarying) * colorVarying;
}



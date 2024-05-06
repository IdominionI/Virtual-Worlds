Fragment SHADER  : alphaMask2DShader
-----------------------
#version 450

#define IN in
#define OUT out
#define TEXTURE texture
#define FRAG_COLOR fragColor

out vec4 fragColor;

uniform sampler2D src_tex_unit0;
uniform sampler2D src_tex_unit1;
uniform float usingTexture;
uniform float usingColors;
uniform vec4 globalColor;

IN float depth;
IN vec4 colorVarying;
IN vec2 texCoordVarying;

void main(){ 
	FRAG_COLOR = vec4(TEXTURE(src_tex_unit0, texCoordVarying).rgb, TEXTURE(src_tex_unit1, texCoordVarying).r)* globalColor;
}

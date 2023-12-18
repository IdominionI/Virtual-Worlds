Fragment SHADER  : bitmapStringShader
-----------------------
#version 450

#define IN in
#define OUT out
#define TEXTURE texture
#define FRAG_COLOR fragColor

out vec4 fragColor;

uniform sampler2D src_tex_unit0;
uniform vec4 globalColor;

IN vec2 texCoordVarying;

void main() { 
	vec4 tex = TEXTURE(src_tex_unit0, texCoordVarying);
	if (tex.a < 0.5) discard;
	FRAG_COLOR = globalColor * tex;
}


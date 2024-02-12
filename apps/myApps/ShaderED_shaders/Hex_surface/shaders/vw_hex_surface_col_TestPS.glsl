#version 450 core

// ---------- FRAGMENT SHADER UNIFORMS -------------------
//layout(location = 0) out vec4 frag_color;// out put color

// Cannot use same variable name for in and out variables
// use Input and output interface blocks to solve this
// The following input block is to allow  varyingColor to be
// either a vertex or geometry shader

in VertexData
{
//  float value;
  vec4 varyingColor;
} fs_in;


out vec4 frag_color;
// ---------- END FRAGMENT SHADER UNIFORMS -------------------

void main()
{
    frag_color = fs_in.varyingColor;
    //frag_color = vec4(1.0, 1.0, 0.0, 1.0);
}

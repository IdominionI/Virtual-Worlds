
//Fragment Shader;

layout(location = 0) out vec4 frag_color;// out put color 
//out vec4 FragColor; // output color
//layout(location = 0) out vec4 FragColor;// out put color

// Cannot use same variable name for in and out variables
// use Input and output interface blocks to solve this
// The following input block is to allow  varyingColor to be
// either a vertex or geometry shader

in VertexData
{
  vec4 varyingColor;
} fs_in;

//Fragment Shader;

//layout(location = 0) out vec4 frag_color;// out put color
out vec4 frag_color;// out put color

// Cannot use same variable name for in and out variables
// use Input and output interface blocks to solve this
// The following input block is to allow  varyingColor to be
// either a vertex or geometry shader

in VertexData
{
//  float value;
  vec4 varyingColor;
} fs_in;

//point_cloud_fragment_shader
//in varying vec4 varyingColor; // color variable to accept from the vertex buffer to renmder;
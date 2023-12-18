#pragma once

#include <typeinfo>

#include "opengl_defines.h"

#include "../Geometry/geometry.h"

/*
                        OpenGL Vetex Buffer Objects Class

    This code is an adaptation of the original source code of
    the JGL openGL demo from which the Virtual Worlds application
    used as a basis to evolve from.

    This is code that sets up and initialises and performs the
    various basic OpenGl vertex buffer object function calls
    required to be performed to store data that is used in a glsl
    shader program, and delete the VBO when no longer needed.

    This class also manages the display of the VBO by the specified
    glsl shader program id number.
*/

class openGL_vertex_Index_buffer_class //: public vertex_index_buffer_class
{
public:
    openGL_vertex_Index_buffer_class() : mVBO{ 0 }, mVAO{ 0 }, mIBO{ 0 } {}

    /*
        Buffer Binding Target 	            Purpose

        GL_ARRAY_BUFFER 	            Vertex attributes
        GL_ATOMIC_COUNTER_BUFFER 	    Atomic counter storage
        GL_COPY_READ_BUFFER 	        Buffer copy source
        GL_COPY_WRITE_BUFFER 	        Buffer copy destination
        GL_DISPATCH_INDIRECT_BUFFER     Indirect compute dispatch commands
        GL_DRAW_INDIRECT_BUFFER 	    Indirect command arguments
        GL_ELEMENT_ARRAY_BUFFER 	    Vertex array indices
        GL_PIXEL_PACK_BUFFER 	        Pixel read target
        GL_PIXEL_UNPACK_BUFFER 	        Texture data source
        GL_QUERY_BUFFER 	            Query result buffer
        GL_SHADER_STORAGE_BUFFER 	    Read-write storage for shaders
        GL_TEXTURE_BUFFER 	            Texture data buffer
        GL_TRANSFORM_FEEDBACK_BUFFER 	Transform feedback buffer
        GL_UNIFORM_BUFFER 	            Uniform block storage

        void glBufferData( 	GLenum target, GLsizeiptr size,const void * data, GLenum usage);
        void glNamedBufferData( GLuint buffer, GLsizeiptr size,const void *data, GLenum usage);

        void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
        void glNamedBufferSubData( 	GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
    */

    // Amazing that this function works without having to specify data types from calling function !!! 
    // Seems defining data types in the derived class requires no definitions in calling this function

    // Dynamic creation of vertex buffer data for vertex geometry of 2D, 3D or 4D with additional vertex data
    // of color or normals that may or may not be included.
    // VT1 datatype is the datatype of the vertex geometry data point

    template <typename VT1>
    bool create_vertex_buffer(base_geometry_class<VT1> &vertices) {
cout << "VBO : create_vertex_buffer 0000 #### " << vertices.getNumVertices() << std::endl;
cout << "VBO : create_vertex_buffer 1111 #### " << vertices.getNumNormals() << std::endl;
        //const size_t verticesSizeBytes = vertices.vertices.size() * sizeof(VT1);
       // const size_t normalsSizeBytes = vertices.vertex_normals.size() * sizeof(VT1);
       const size_t verticesSizeBytes = vertices.getNumVertices() * sizeof(VT1);// ofmesh method
       const size_t normalsSizeBytes  = vertices.getNumNormals() * sizeof(VT1);// ofmesh method

cout << "VBO : create_vertex_buffer 2222 #### " << std::endl;
        if (verticesSizeBytes < 1) return false;// Must have vertex location data

        size_t totalSizeBytes = verticesSizeBytes + normalsSizeBytes;// +texCoords0SizeBytes + texCoords1SizeBytes;
cout << "VBO : create_vertex_buffer 2222A #### " << totalSizeBytes << std::endl;
if(&mVAO == NULL) cout << "VBO : create_vertex_buffer 2222A &mVAO == NULL #### " << totalSizeBytes << std::endl;
        //!!!!!!! openframeworks crashes here : seems there is a problem not using openframeworks ofVBO and other opengl code !!!!!!!
        glGenVertexArrays(1, &mVAO); //generate vertex array of n object names stored in index referenced by &mVAO: Not sure need this
cout << "VBO : create_vertex_buffer 2222AA #### " << std::endl;
        glGenBuffers(1, &mVBO);
cout << "VBO : create_vertex_buffer 2222AAA #### " << std::endl;
        glBindVertexArray(mVAO); // binds the vertex array object with name mVAO : Not sure need this
cout << "VBO : create_vertex_buffer 2222AAAA #### " << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);// Define buffer type to be used for buffer of specified id

cout << "VBO : create_vertex_buffer 2222B #### " << std::endl;
        // ################################################################################################
         // The folowing method of generating dynamic vertex data for opengl and glsl programs should be able to be applied to any
         // graphic or other data to be stored in a openGL vertex buffer object. What needs to be taken note of is what data is 
         // stored and the data type used to store that data in the buffer, and in what order is the data. 
         // Here it is point vertex location, color and normal data. 

         // vertex data is mandatory, but color and normal data is optional and if all three are used the glsl code needs to have the code
         //  layout(location = 0) in vec4 vertex;
         //  layout(location = 1) in vec4 color;
         //  layout(location = 2) in vec4 normal;
         // defined so as to have this data transfered and used in the glsl program
         // If normal data is defined, but not color, then this code becomes
         //  layout(location = 0) in vec4 vertex;
         //  layout(location = 1) in vec4 normal;

         //  All other vertex buffer data can follow this same method and code template to utilise any kind of data to be transfered to
         // a glsl program.


        glBufferData(GL_ARRAY_BUFFER, totalSizeBytes, NULL, GL_STATIC_DRAW);// Define buffer storage type, size, data to store and use

cout << "VBO : create_vertex_buffer 2222C #### " << std::endl;

        GLintptr offset = 0;
        GLuint   attribute_array_index = 0;
        GLint    number_of_VT1_components = 3;
        //GLint    number_of_VT2_components = 3;
        bool     valid_VT1_data_type = false;
        //bool     valid_VT2_data_type = false;

        if (typeid(VT1) == typeid(glm::vec2)) { number_of_VT1_components = 2; valid_VT1_data_type = true; } // Have a vertex of 2D
        if (typeid(VT1) == typeid(glm::vec3)) { number_of_VT1_components = 3; valid_VT1_data_type = true; } // Have a vertex of 3D
        if (typeid(VT1) == typeid(glm::vec4)) { number_of_VT1_components = 4; valid_VT1_data_type = true; } // Have a vertex of 4D
        // add other datatypes here
cout << "VBO : create_vertex_buffer 3333 #### " << std::endl;
        if (!valid_VT1_data_type) {
            // Error message here !!!
//printf("openGL_vertex_Index_buffer_class :: create_vertex3DD_buffer : Error : invalid data type to create vertex buffer\n");
cout << "VBO : openGL_vertex_Index_buffer_class :: create_vertex3DD_buffer : Error : invalid data type to create vertex buffer" << std::endl;
            return false;
        }

        // upload positions
        //glBufferSubData(GL_ARRAY_BUFFER, offset, verticesSizeBytes, &vertices.vertices[0]);
        glBufferSubData(GL_ARRAY_BUFFER, offset, verticesSizeBytes, &vertices.getVertices());// ofmesh method
        glEnableVertexAttribArray(attribute_array_index); //use currently bound vertex array object for the operation of index 0
        //glVertexAttribPointer(attribute_array_index, 4, GL_FLOAT, GL_FALSE, sizeof(VT1), (void*)0); //define an array of generic vertex location data :: see parameter list at bottom
        glVertexAttribPointer(attribute_array_index, number_of_VT1_components, GL_FLOAT, GL_FALSE, sizeof(VT1), (void*)0); //define an array of generic vertex location data :: see parameter list at bottom
        offset += verticesSizeBytes;
        attribute_array_index += 1;

cout << "VBO : create_vertex_buffer 44444 #### " << std::endl;
        // upload normals
        //if (!vertices.vertex_normals.empty()) {
        if (!vertices.getNormals().empty()) {// ofmesh method
            //glBufferSubData(GL_ARRAY_BUFFER, offset, normalsSizeBytes, &vertices.vertex_normals[0]);
            glBufferSubData(GL_ARRAY_BUFFER, offset, normalsSizeBytes, &vertices.getNormals());// ofmesh method
            glEnableVertexAttribArray(attribute_array_index); //use currently bound vertex array object for the operation of index 0
            //glVertexAttribPointer(attribute_array_index, 4, GL_FLOAT, GL_FALSE, sizeof(VT2), (void*)offset); //define an array of generic vertex location data :: see parameter list at bottom
            glVertexAttribPointer(attribute_array_index, number_of_VT1_components, GL_FLOAT, GL_FALSE, sizeof(VT1), (void*)offset); //define an array of generic vertex location data :: see parameter list at bottom

            offset += normalsSizeBytes;
            attribute_array_index += 1;
        }
cout << "VBO : create_vertex_buffer 5555 #### " << std::endl;
        // Need to have other data to transfer to shaders here

        // If ever to use texture coordinates define here

        // ################################################################################################
        glBindVertexArray(0); //: Not sure need this

        return true;

    };

    void delete_vertex_buffer() {
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }

 
// -------------------------------------------------------------------------------------------

    virtual void bind() {
        glBindVertexArray(mVAO);
    }

    virtual void unbind() {
        glBindVertexArray(0);
    }

    virtual void draw_triangles(int index_count) {
        bind();

        // the vertices as line loop
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);

        unbind();
    }

    virtual void draw_triangles_array(int index_count, int shader_program_id) {
        bind();

        // the vertices as line loop
//printf("draw_triangles_array %i\n", index_count);

        glDrawArrays(GL_TRIANGLES, 0, index_count);

        unbind();
    }

    virtual void draw_points(size_t index_count) {
        bind();
        //printf("draw_points %i\n", index_count);
        glDrawArrays(GL_POINTS, 0, index_count);
        // glDrawElements(GL_POINTS, index_count, GL_UNSIGNED_INT, nullptr); // this causes a crash

        unbind();
    }

    virtual void draw_lines(int index_count) {
        bind();
        //printf("draw_lines %i\n", index_count);
        glDrawArrays(GL_LINES, 0, index_count);

        unbind();
    }

protected:
    GLuint mVBO;// Storage id of vertex buffer object that stores geometry vertex data
    GLuint mVAO;// Storage id of vertex array object that stores geometry vertex data
    GLuint mIBO;// Storage id of vertex index buffer object that stores geometry vertex indices data for drawing triangles
    //GLuint mDBO;// Storage id of vertex data buffer object that stores data for each vertex point // *****
};


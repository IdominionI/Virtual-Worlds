#pragma once

#include <Geometry/vw_entity_object_geometry.h>

/*
                POINT CLOUD GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point cloud object data type with
    point vertex location data only.
*/

/*
                POINT CLOUD VERTEX GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point data cloud object data type
    with point vertex location and a normal vector value
    corresponding to that vertex location.
*/

//class point_cloud3D_class : public base_geometry_class<glm::vec3> {
class point_cloud3D_class : public entity_base_geometry_class {
public:
    point_cloud3D_class() = default;

    virtual ~point_cloud3D_class() {}

    ofMaterialSettings material_settings;

    bool init() {
        geometry_type = geometry_type_enum::points;
        generate_test_data();
        // Absolutely critical to have a setMode function to one of the openframeworks
        // primitive mode types or no points will be displayed when using a geometry shader->
        // This is an open frameworks thing that took two days to figure out since no
        // such setting is needed in openGL. Ridiculous !!!!
        // OF_PRIMITIVE_TRIANGLES,
        // OF_PRIMITIVE_TRIANGLE_STRIP,
        // OF_PRIMITIVE_TRIANGLE_FAN,
        // OF_PRIMITIVE_LINES,
        // OF_PRIMITIVE_LINE_STRIP,
        // OF_PRIMITIVE_LINE_LOOP,
        // OF_PRIMITIVE_POINTS,
        setMode(OF_PRIMITIVE_POINTS);
        return true;
    }

    bool generate_test_data(float scale = 1.0f,float att_test = 0.0f) {
        delete_geometry();

        glm::vec3 vertex;
        glm::vec3 normal;

        geometry_attribute_type_struct vertex_attribute_data;
        vertex_attribute_data.attribute_name = "test_attr_data";
        vertex_attribute_data.attribute_description = "test attribute data";

        //for (float i = -0.5f; i < 0.5f; i += 0.1f) {
        //    for (float j = -0.5f; j < 0.5f; j += 0.1f) {
        //        for (float k = -0.5f; k < 0.5f; k += 0.1f) {
        //            vertex = { i,j,k };
        //            normal = { 0.0,0.0,0.0 };

        //            add_vertex(vertex);
        //            add_normal(normal);
        //        }
        //    }
        //}

        for (float i = -100.0f; i < 100.0f; i += 10.0f) {
            for (float j = -100.0f; j < 100.0f; j += 10.0f) {
                for (float k = -100.0f; k < 100.0f; k += 10.0f) {
                    vertex = { i* scale,j * scale,k * scale };
                    normal = { 0.0,0.0,0.0 };

                    add_vertex(vertex);
                    add_normal(normal);
                    vertex_attribute_data.attribute_values.push_back(att_test);
                }
            }
        }

        vertex_attributes.attributes.push_back(vertex_attribute_data);

        //return update_geometry();
        return true;
    }

    // Update geometry data in the buffers with the current 
    // vertex data content.
//    bool update_geometry() {

//        return true;
//    }

    // add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    //void add_vertex(const point_vertex_class& vertex) {
    void add_vertex(const glm::vec3& vertex) {
        addVertex(vertex);
    }

    void add_normal(const glm::vec3& normal) {
        addNormal(normal);
    }

    //void render() {
    //    if (shader != NULL) { // Have internal shader to render to
    //        shader->begin();
    //            render_geometry();
    //        shader->end();
    //    } else { // render to externaly defined shader
    //        render_geometry();
    //    }
    //}

    // This is for testing only at the moment 
    //void update_material(ofGLProgrammableRenderer& renderer) const {
    void update_material() {
        ofShader* currentRenderShader = currentRenderShader = shader;

        //shader->setUniform4fv("mat_emissive", &material_settings.emissive.r);
        //shader->setUniform2f("mat_texcoord_scale", material_settings.texCoordScale);
/*
        if (isPBR()) {
            shader->setUniform3f("uCameraPos", renderer.getCurrentEyePosition());
            shader->setUniform4fv("mat_diffuse", &material_settings.diffuse.r);
            shader->setUniform1f("mat_roughness", material_settings.roughness);
            shader->setUniform1f("mat_metallic", material_settings.metallic);
            shader->setUniform1f("mat_reflectance", material_settings.reflectance);
            if (isClearCoatEnabled()) {
                shader->setUniform2f("mat_clearcoat", material_settings.clearCoatStrength, material_settings.clearCoatRoughness);
            }

            if (hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT)) {
                shader->setUniform1f("mat_displacement_strength", material_settings.displacementStrength);
                shader->setUniform1f("mat_displacement_normals_strength", material_settings.displacementNormalsStrength);
            }
            if (hasTexture(OF_MATERIAL_TEXTURE_NORMAL) || hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT)) {
                shader->setUniform1f("mat_normal_mix", material_settings.normalGeomToNormalMapMix);
            }

            std::shared_ptr<ofCubeMap::material_settings> cubeMapmaterial_settings = ofCubeMap::getActivematerial_settings();
            if (cubeMapmaterial_settings) {
                shader->setUniform1f("mat_ibl_exposure", cubeMapmaterial_settings->exposure);
                shader->setUniform1f("uCubeMapEnabled", 1.0f);
                shader->setUniform1f("uEnvMapMaxMips", cubeMapmaterial_settings->maxMipLevels);
            }
            else {
                shader->setUniform1f("mat_ibl_exposure", 1.0f);
                shader->setUniform1f("uCubeMapEnabled", 0.0f);
                shader->setUniform1f("uEnvMapMaxMips", 1.0f);
            }

        }
        else {*/
            shader->setUniform4fv("mat_ambient", &material_settings.ambient.r);
            shader->setUniform4fv("mat_diffuse", &material_settings.diffuse.r);
            shader->setUniform4fv("mat_specular", &material_settings.specular.r);
            //shader->setUniform4fv("global_ambient", &ofGetGlobalAmbientColor().r);
            shader->setUniform1f("mat_shininess", material_settings.shininess);
        //}
    }

private:
    void render_geometry() {
        if (internal_shader) {
            if (shader){
                update_material();
                shader->begin();
                if (display_geometry)
                    // Only use this function if displaying vertices only as
                    // any geometry shader triangles will not be displayed as a mesh
                    // Have no idea how this is so, and took over two days to figure
                    // out 
                    //drawVertices();

                    draw(); //Mesh draw 
                    //vertex_buffer.draw(GL_POINTS, 0, getNumVertices());
                shader->end();
            } 
            else
std::cout << "point_cloud3D_class :: render_geometry : shader == NULL\n";
        } 
        else {
std::cout << "point_cloud3D_class :: render_geometry : !internal_shader\n";
              if (display_geometry)
                  drawVertices();
        }

    }

};



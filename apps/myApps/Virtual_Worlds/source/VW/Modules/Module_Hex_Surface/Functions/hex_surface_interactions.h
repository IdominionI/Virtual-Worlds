#pragma once
#include "../Object/hex_surface_object.h"
#include "../Object/hex_surface_parameters.h"

class hex_surface_interactions_class {
public:
    vw_scene_class   *scene_manager = nullptr;
    log_panel_class  *log_panel     = nullptr;

    static bool voxel_intersection(hex_surface_object_class *hex_surface_object_A, hex_surface_object_class *hex_surface_object_B) {
        bool x_intersection = false, y_intersection = false, z_intersection = false;

        float a_xmin = hex_surface_object_A->hex_surface_object_data.grid_origin.x;
        float a_xmax = hex_surface_object_A->hex_surface_object_data.grid_origin.x + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_start);
        float b_xmin = hex_surface_object_B->hex_surface_object_data.grid_origin.x;
        float b_xmax = hex_surface_object_B->hex_surface_object_data.grid_origin.x + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_start);

        float a_ymin = hex_surface_object_A->hex_surface_object_data.grid_origin.y;
        float a_ymax = hex_surface_object_A->hex_surface_object_data.grid_origin.y + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_start);
        float b_ymin = hex_surface_object_B->hex_surface_object_data.grid_origin.y;
        float b_ymax = hex_surface_object_B->hex_surface_object_data.grid_origin.y + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_start);


        if (((a_xmin >= b_xmin) && (a_xmin <= b_xmax)) || ((a_xmax <= b_xmax) && (a_xmax >= b_xmin))) x_intersection = true;
        if (((b_xmin >= a_xmin) && (b_xmin <= a_xmax)) || ((b_xmax <= a_xmax) && (b_xmax >= a_xmin))) x_intersection = true;
        if (((a_ymin >= b_ymin) && (a_ymin <= b_ymax)) || ((a_ymax <= b_ymax) && (a_ymax >= b_ymin))) y_intersection = true;
        if (((b_ymin >= a_ymin) && (b_ymin <= a_ymax)) || ((b_ymax <= a_ymax) && (b_ymax >= a_ymin))) y_intersection = true;

        if (x_intersection && y_intersection)
            return true;
        else
            return false;

    }

    static bool define_intersection_boundary(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, glm::vec3 &boundary_min, glm::vec3 &boundary_max) {
        bool x_intersection = false, y_intersection = false, z_intersection = false;

        float a_xmin = hex_surface_object_A->hex_surface_object_data.grid_origin.x;
        float a_xmax = hex_surface_object_A->hex_surface_object_data.grid_origin.x + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.x_start);
        float b_xmin = hex_surface_object_B->hex_surface_object_data.grid_origin.x;
        float b_xmax = hex_surface_object_B->hex_surface_object_data.grid_origin.x + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.x_start);

        float a_ymin = hex_surface_object_A->hex_surface_object_data.grid_origin.y;
        float a_ymax = hex_surface_object_A->hex_surface_object_data.grid_origin.y + (hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_A->hex_surface_object_data.hex_surface_generator_parameters.y_start);
        float b_ymin = hex_surface_object_B->hex_surface_object_data.grid_origin.y;
        float b_ymax = hex_surface_object_B->hex_surface_object_data.grid_origin.y + (hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_end - hex_surface_object_B->hex_surface_object_data.hex_surface_generator_parameters.y_start);

        if ((a_xmin >= b_xmin) && (a_xmin <= b_xmax)) {
            boundary_min.x = a_xmin;
            x_intersection = true;
        } else
            if ((b_xmin >= a_xmin) && (b_xmin <= a_xmax)) {
                boundary_min.x = b_xmin;
                x_intersection = true;
            }

        if ((a_ymin >= b_ymin) && (a_ymin <= b_ymax)){
            boundary_min.y = a_ymin;
            y_intersection = true;
        } else
            if ((b_ymin >= a_ymin) && (b_ymin <= a_ymax)) {
                boundary_min.y = b_ymin;
                y_intersection = true;
            }

        if ((a_xmax <= b_xmax) && (a_xmax >= b_xmin)){
            boundary_max.x = a_xmax;
            x_intersection = true;
        } else
            if ((b_xmax <= a_xmax) && (b_xmax >= a_xmin)) {
                boundary_max.x = b_xmax;
                x_intersection = true;
            }

        if ((a_ymax <= b_ymax) && (a_ymax >= b_ymin)){
            boundary_max.y = a_ymax;
            y_intersection = true;
        } else
            if ((b_ymax <= a_ymax) && (b_ymax >= a_ymin)) {
                boundary_max.y = b_ymax;
                z_intersection = true;
            }

        if (x_intersection && y_intersection)
            return true;
        else
            return false;
    }


   bool align_voxel_matricies(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, bool update_hcp_object_A_vertices = true) {
//printf("hcp_interaction_node_class:align_voxel_matricies 0000 %\n");
        if (hex_surface_object_A->hex_surface_object_data.hex_size != hex_surface_object_B->hex_surface_object_data.hex_size) return false;

        float hex_size = hex_surface_object_A->hex_surface_object_data.hex_size;

        glm::vec2 a_origin = hex_surface_object_A->hex_surface_object_data.grid_origin;
        glm::vec2 b_origin = hex_surface_object_B->hex_surface_object_data.grid_origin;

        glm::vec2 origin_diff = b_origin - a_origin;

        glm::vec2 translation;

        translation.x = fmod(origin_diff.x, hex_size);
        translation.y = fmod(origin_diff.y, hex_size);

        hex_surface_object_A->hex_surface_object_data.grid_origin += translation * hex_size;

        if (update_hcp_object_A_vertices) update_voxel_verticies(hex_surface_object_A);

        return true;
    }

        // !!!!!!!! FOLLOWING UPDATE CODE NEEDS TO BE PUT INTO A GENERAL UPDATE VOXEL VERTICES FUNCTION !!!!!!!!!!!!!!
    void update_voxel_verticies(hex_surface_object_class *hex_surface_object) {
//printf("hcp_voxel_interactions_class:update_voxel_verticies 0000\n");        
         hex_surface_object->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display
    }

    hex_surface_object_class *create_interaction_object(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B) {
        glm::vec3 boundary_min, boundary_max;
       
        if (scene_manager == NULL) return NULL;

//printf("hcp_interaction_node_class:create_interaction_object 0000 %\n");
        if (!align_voxel_matricies(hex_surface_object_A, hex_surface_object_B), false) return NULL;
//printf("hcp_interaction_node_class:create_interaction_object 11111  %\n");
        if (!define_intersection_boundary(hex_surface_object_A, hex_surface_object_B, boundary_min, boundary_max)) return NULL;
//printf("hcp_interaction_node_class:create_interaction_object 2222 min : %f : %f \n", boundary_min.x, boundary_min.y);
//printf("hcp_interaction_node_class:create_interaction_object 3333 max : %f : %f\n", boundary_max.x, boundary_max.y);

        //+++++++++++++++++
        hex_surface_object_class *interaction_object = new hex_surface_object_class;

        int object_category_id = scene_manager->scene_entities_manager.get_objects_category_index(SCENE_CATEGORY_HEX_SURFACE);
        if (object_category_id == -1) {
//std::cout << "hcp_interaction_node_class:create_interaction_object 4444 :  object_category_id == -1  111111111111 \n";
             interaction_object->object_category_id = scene_manager->scene_entities_manager.define_new_entity_category(SCENE_CATEGORY_HEX_SURFACE);
        }
        else
            interaction_object->object_category_id = object_category_id;

        interaction_object->gizmo_display = node_gizmo_display_enum::none;
        interaction_object->axis_size = 100.0;

//std::cout << "hcp_interaction_node_class:create_interaction_object 55555 \n";
        scene_manager->scene_entities_manager.add_object(interaction_object,interaction_object->object_category_id);
        
        if (interaction_object == NULL) return NULL;

        interaction_object->object_type_id = ENTITY_TYPE_OBJECT;

        interaction_object->define_initial_shader_program();

        if (interaction_object->geometry->shader) {
            if (ofIsGLProgrammableRenderer()) {
//std::cout << "hex_surface_interactions_class::create_interaction_object: ofIsGLProgrammableRenderer() : " << std::endl;
            }
            else {
//std::cout << "hex_surface_interactions_class::create_interaction_object:!!!!ofIsGLProgrammableRenderer() : " << std::endl;
                int i = scene_manager->scene_entities_manager.get_objects_category_index("Hex_Surface");// SCENE_CATEGORY_HEX_SURFACE does not work ?????
                if (i > -1) {
                    scene_manager->scene_entities_manager.delete_object(interaction_object->id, scene_manager->scene_entities_manager.scene_objects[i].objects_category_id);
                }
                return false;
            }

            if (!interaction_object->geometry->shader->shader_compile_successful) {
//std::cout << "hcp_interaction_hex_surface_interactions_classnode_class::create_interaction_object: hcp_voxel Shaders not loaded !!!!! : " << std::endl;
//std::string s = "jjjj\n";
//cout << s << std::endl;
//std::cout << interaction_object->geometry->shader->compile_log << std::endl;
//std::cout << "hex_surface_interactions_class::create_interaction_object: hcp_voxel Shaders not loaded !!!!! END : " << std::endl;
            }
            else {
//cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
                std::cout << "hex_surface_interactions_class::create_interaction_object: hcp_voxel Shaders loaded ^^^^^ : " << std::endl;
//cout << entity_object03->geometry->shader->compile_log << std::endl;
            }
        }
        else
//std::cout << "hex_surface_interactions_class::create_interaction_object: hcp_voxel Shader not created : " << std::endl;
         //+++++++++++++++++
 
         if (interaction_object == NULL) return NULL;
//std::cout << "hcp_interaction_node_class:create_interaction_object 6666 \n";
        interaction_object->hex_surface_object_data.grid_origin = boundary_min;
        interaction_object->hex_surface_object_data.hex_size = hex_surface_object_A->hex_surface_object_data.hex_size;

        // ########### CREATE EMPTY HEX SURFACE CLOUD MATRIX #################
        float x_size = boundary_max.x - boundary_min.x;
        float y_size = boundary_max.y - boundary_min.y;

        float x_res_step = interaction_object->hex_surface_object_data.hex_size * 2.0f;
        float y_res_step = interaction_object->hex_surface_object_data.hex_size * (3.0f / sqrt(3.0f));

        int data_set_x_size, data_set_y_size;

        if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0f)
            data_set_x_size = (int)(x_size / x_res_step) + 1;
        else
            data_set_x_size = (int)(x_size / x_res_step);

        if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0f)
            data_set_y_size = (int)(y_size / y_res_step) + 1;
        else
            data_set_y_size = (int)(y_size / y_res_step);

        interaction_object->hex_surface_object_data.grid_dimension = { data_set_x_size,data_set_y_size,0};
//printf("hcp_interaction_node_class:create_interaction_object 7777 dim :  %i : %i : %i \n", interaction_object->hex_surface_object_data.grid_dimension.x, interaction_object->hex_surface_object_data.grid_dimension.y);
        interaction_object->hex_surface_object_data.create_empty_surface_cubic(interaction_object->hex_surface_object_data.grid_dimension.x,
                                                                               interaction_object->hex_surface_object_data.grid_dimension.y);

//printf("hcp_interaction_node_class:create_interaction_object 8888 size : %i \n", interaction_object->hex_surface_object_data.hex_surface_matrix_data.size());
        // ########### END CREATE EMPTY VOXEL CLOUD MATRIX #################

        return interaction_object;
    }

    void deterime_interaction_matrix_start_values(hex_surface_object_class *hex_surface_object_A,
                                                  hex_surface_object_class *hex_surface_object_B, 
                                                  hex_surface_object_class *interaction_object,
                                                  glm::ivec2 &a_start, glm::ivec2 &b_start)
    {
        float xs, ys;// , xe, ye, ze;
        float hex_size = interaction_object->hex_surface_object_data.hex_size;

        //start indexes for voxel matrix A
        xs = ((interaction_object->hex_surface_object_data.grid_origin.x - hex_surface_object_A->hex_surface_object_data.grid_origin.x) / hex_size) + 0.5f;
        ys = ((interaction_object->hex_surface_object_data.grid_origin.y - hex_surface_object_A->hex_surface_object_data.grid_origin.y) / hex_size) + 0.5f;

        if (xs <= 1.0f) a_start.x = 0; else a_start.x = int(floor(xs)*hex_size);
        if (ys <= 1.0f) a_start.y = 0; else a_start.y = int(floor(ys)*hex_size);

        //start indexes for voxel matrix B
        xs = ((interaction_object->hex_surface_object_data.grid_origin.x - hex_surface_object_B->hex_surface_object_data.grid_origin.x) / hex_size) + 0.5f;
        ys = ((interaction_object->hex_surface_object_data.grid_origin.y - hex_surface_object_B->hex_surface_object_data.grid_origin.y) / hex_size) + 0.5f;

        if (xs <= 1.0f) b_start.x = 0; else b_start.x = int(floor(xs)*hex_size);
        if (ys <= 1.0f) b_start.y = 0; else b_start.y = int(floor(ys)*hex_size);

        // Calculate coordinates at end of entity matrices :: Is this needed ???????
        // Included here as comment to uncomment and put in seperate method if needed
/*        glm::vec3 a_origin = hex_surface_object_A->hex_surface_object_data.grid_origin;
        glm::vec3 b_origin = hex_surface_object_B->hex_surface_object_data.grid_origin;
        glm::vec3 i_origin = interaction_object->hex_surface_object_data.grid_origin;
        glm::vec3 a_endf, b_endf,i_endf;

        a_endf.x = (a_origin.x + hex_surface_object_A->hex_surface_object_data.matrix_dimension.x*hex_size);
        a_endf.y = (a_origin.y + hex_surface_object_A->hex_surface_object_data.matrix_dimension.y*hex_size);
        a_endf.z = (a_origin.z + hex_surface_object_A->hex_surface_object_data.matrix_dimension.z*hex_size);

        b_endf.x = (b_origin.x + hex_surface_object_B->hex_surface_object_data.matrix_dimension.x*hex_size);
        b_endf.y = (b_origin.y + hex_surface_object_B->hex_surface_object_data.matrix_dimension.y*hex_size);
        b_endf.z = (b_origin.z + hex_surface_object_B->hex_surface_object_data.matrix_dimension.z*hex_size);

        i_endf.x = (i_origin.x + interaction_object->hex_surface_object_data.matrix_dimension.x * hex_size);
        i_endf.y = (i_origin.y + interaction_object->hex_surface_object_data.matrix_dimension.y * hex_size);
        i_endf.z = (i_origin.z + interaction_object->hex_surface_object_data.matrix_dimension.z * hex_size);

        // End indexes for voxel matrix A
        xe = ((i_endf.x - a_endf.x) / hex_size) + 0.5;
        ye = ((i_endf.y - a_endf.y) / hex_size) + 0.5;
        ze = ((i_endf.z - a_endf.z) / hex_size) + 0.5;

        if (xe <= 0.0) a_end.x = a_start.x + (int)(floor(xe)); else a_end.x = hex_surface_object_A->hex_surface_object_data.matrix_dimension.x;
        if (ye <= 0.0) a_end.y = a_start.y + (int)(floor(ye)); else a_end.y = hex_surface_object_A->hex_surface_object_data.matrix_dimension.y;
        if (ze <= 0.0) a_end.z = a_start.z + (int)(floor(ze)); else a_end.z = hex_surface_object_A->hex_surface_object_data.matrix_dimension.z;

        // End indexes for voxel matrix B
        xe = ((i_endf.x - b_endf.x) / hex_size) + 0.5;
        ye = ((i_endf.y - b_endf.y) / hex_size) + 0.5;
        ze = ((i_endf.z - b_endf.z) / hex_size) + 0.5;

        if (xe <= 0.0) b_end.x = b_start.x + (int)(floor(xe)); else b_end.x = hex_surface_object_A->hex_surface_object_data.matrix_dimension.x;
        if (ye <= 0.0) b_end.y = b_start.y + (int)(floor(ye)); else b_end.y = hex_surface_object_A->hex_surface_object_data.matrix_dimension.y;
        if (ze <= 0.0) b_end.z = b_start.z + (int)(floor(ze)); else b_end.z = hex_surface_object_A->hex_surface_object_data.matrix_dimension.z;

        // END Calculate coordinates at end of entity matrices :: Is this needed ???????
*/
    }


private:
    hex_surface_render_class hex_surface_render;
};

#pragma once

#include "ofMain.h"

#include "../Interface/theme.h"
#include "../Interface/IconsFontAwesome.h"

#include <VW_framework/App/global.h> // Must be placed here above all other includes

#include <VW_framework/Scene/scene_entities_manager.h>

#include <ImGuiEx/ImGui_utils.h>

#include "Panels/log_panel.h"
#include "Panels/parameter_panel.h"

#include "Panels/node_editor_panel.h"

#include <VW_framework/Scene/vw_scene.h>
// !!!!!!!!!!!!!! testing !!!!!!!!!!!!!!!!!!!!


#include <FrameWork/VW_framework/Geometry/vw_point_cloud.h>

#include <FrameWork/VW_framework/Geometry/vw_mesh_geometry.h>
#include <FrameWork/VW_framework/Geometry/vw_mesh_geometry_primitives.h>

#include <FrameWork/VW_framework/GL/vw_shader_Constructor.h>

#include <VW_framework/Shader/shader.h>

//#include <Universal_FW/im-neo-sequencer/imgui_neo_sequencer.h>

// ++++++++++++++++++++++++++++++++++

#include <VW/Modules/Module_HCP_Voxel/Object/voxel_hcp_object.h>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class main_window_class : public ofBaseApp {
public:
    //main_window_class() : mIsRunning(true), GLFW_window_ptr(nullptr) {
    main_window_class() : GLFW_window_ptr(nullptr) {
       // ui_context = std::make_unique<UI_context_class>();
       // render_context = new openGL_context_class;
    }

    ~main_window_class() {
        //ImNodes::DestroyContext();
        ImGui_ui_context.ImGui_end();
        //render_context->end();
        //node_editor_panel.NodeEditorShutdown();
    }

    shared_ptr<ofAppBaseWindow> main_window = NULL;

    void setup() {
//cout << "setup here : 000" << std::endl;
        //ofDisableArbTex();

        GLFW_window_ptr = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();

        if (GLFW_window_ptr == NULL) {
            cout << "Appplication main window not defined : Application aborted" << std::endl;
            return;
        }

        ImGui_ui_context.ImGui_init(GLFW_window_ptr);
        
        //ImNodes::CreateContext();

        log_panel = new log_panel_class();
        if (log_panel == NULL) {
            cout << "CRITCAL ERROR :: No Applicaton Logger Defined : Cannot perform application \n";
            return;
        }
//cout << "setup here : " << std::endl;
        //scene_manager = new scene_entities_manager_class;
        vw_scene = new vw_scene_class;
        scene_manager = &vw_scene->scene_entities_manager;

        //scene_manager = new scene_manager_class;
        //scene_manager->log_panel = log_panel;

        //parameter_panel.scene_manager = scene_manager;
        parameter_panel.log_panel     = log_panel;
        parameter_panel.scene_manager = scene_manager;

        scene_node_editor = new scene_node_editor_class;
        node_editor_panel.log_panel     = log_panel;
        node_editor_panel.vw_scene      = vw_scene; // May need to crete this as a new vw_scene_class
        //node_editor_panel.scene_manager = scene_manager;
        //node_editor_panel.scene_node_editor = scene_node_editor;
        //node_editor_panel.scene_node_editors.push_back(scene_node_editor);
        scene_node_editors.push_back(scene_node_editor);

        //scene_node_editor1 = new scene_node_editor_class;
        //node_editor_panel.log_panel = log_panel;
        //node_editor_panel.scene_manager = scene_manager;
        //node_editor_panel.scene_node_editor1 = scene_node_editor1;

        setup_test_data();

        camer = scene_manager->scene_cameras.cameras[0];// to Test camera parameter display panel : delete when done
        camer->setNearClip(.001);
        camer->setFarClip(10000);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // Set up the fonts file to use to display text 
        // and the awesome icon font file to display icons since ImGui is not designed
        // to have icon images imported and displayed as part of the GUI !!!!!!!
        ImFontConfig cfg;
        cfg.OversampleH = 3;
        cfg.OversampleV = 1;
        cfg.PixelSnapH = true;
        cfg.GlyphExtraSpacing.x = 0.0f;
        cfg.GlyphExtraSpacing.y = 0.0f;
        cfg.RasterizerMultiply = 1.25f;

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear();
        ImFont* mainFont = io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 13.0f, &cfg);

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.GlyphOffset = ImVec2(0, 1);

        ImFont* iconFont = io.Fonts->AddFontFromFileTTF("Fonts/fontawesome_6_solid.otf", 13.0f + 1.0, &icons_config, icons_ranges);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    }

    void update() {
        //entity_object->setPosition(-200.0, 0.0, 0.0);// this works
        //entity_object02->setPosition(200.0, 0.0, 0.0);// this does not work


        if (cs == 0) {
            entity_object02->set_local_orientation( -45.0,0.0,0.0 , true, true);

            //entity_object02->set_global_location( -200.0,0.0,0.0 , true,true);
            //entity_object02->set_local_location({ -200.0,-200.0,0.0 }, true, true);
             entity_object02->set_local_location({ 0.0,-200.0,00.0 }, true, true);
 
            //entity_object02->set_local_orientation( 0.0,45.0,0.0 , true, true);
            //ico_entity->set_local_orientation( 0.0,45.0,0.0 , true, true);
            //entity_object02->set_global_orientation( 0.0,45.0,0.0 , true, true);

            //entity_object02->set_local_scale(1.75,1.0,1.0 , true,true);
            entity_object02->set_global_scale(.5, .5, .5, true, true);
           
            
            //entity_object02->translate_local_location({ 0.0,100.0,0.0 }, true, true);
            //entity_object02->translate_global_location({ 0.0,00.0,200.0 }, true, true);
            //entity_object02->translate_local_radius(100, true, true);


            //entity_object02->set_local_scale(1.75,1.0,1.0 , true,false);
            //ico_entity->set_local_scale(1.75,1.0,1.0 , true, false);

            //entity_object02->global_translation(0,0,100, true, true);
            //ico_entity->global_translation(0,0,100, true, false);

            //entity_object02->pitch(-45,true,true);
            //entity_object02->yaw(45,true,true);
            entity_object02->roll(45,true,false);


            //entity_object->tiltDeg(-45.0);
            //entity_object->rollDeg(45.0);
            //entity_object->change_geometry_orientation();
            //entity_object02->rollDeg(45.0);
//cout << "child change orientation " << std::endl;
            //entity_object02->change_geometry_orientation();
            //entity_object->setScale(0.5);
            //entity_object->change_geometry_scale();

        }

        cs++;
    }
    
    
    void draw() {
        const ImGuiIO &io = ImGui::GetIO();// This crashes app and causes problems if it a is private variable

        glfwMakeContextCurrent(GLFW_window_ptr);
        ImGui_ui_context.ImGui_pre_render();

        // Define interface style parameters each render pass so as
        // to be able to change interface style or style components
        tron_style();

        //// Log panel to display application logging messages and shader code compile error messages
        log_panel->display_application_log();
        log_panel->display_code_log();

        if (camer != NULL) {
//cout << "setup here : camer != NULL" << std::endl;
            //globalc::set_current_selected_entity_id(camer->id);
            //globalc::set_current_selected_entity_type_id(ENTITY_TYPE_CAMERA);

            //current_selected_entity_id = camer->id;
            //current_selected_entity_type_id = ENTITY_TYPE_CAMERA;
        }

//        if (hcp_voxel != NULL) {
////cout << "setup here : hcp_voxel != NULL" << std::endl;
//            globalc::set_current_selected_entity_id(hcp_voxel->id);
//            globalc::set_current_selected_entity_category_type_id(hcp_voxel->object_category_id);
//            globalc::set_current_selected_entity_type_id(ENTITY_TYPE_OBJECT);
//        }

        parameter_panel.show();
        node_editor_panel.show();

        // **** Timeline test ****
/*
        if (ImGui::BeginNeoSequencer("Sequencer", &currentFrame, &startFrame, &endFrame, { 0, 0 },
            ImGuiNeoSequencerFlags_EnableSelection |
            ImGuiNeoSequencerFlags_Selection_EnableDragging |
            ImGuiNeoSequencerFlags_Selection_EnableDeletion))
        {
            if (ImGui::BeginNeoGroup("Transform", &transformOpen))
            {

                if (ImGui::BeginNeoTimelineEx("Position"))
                {
                    for (auto&& v : keys)
                    {
                        ImGui::NeoKeyframe(&v);
                        // Per keyframe code here
                    }


                    if (doDelete)
                    {
                        uint32_t count = ImGui::GetNeoKeyframeSelectionSize();

                        ImGui::FrameIndexType* toRemove = new ImGui::FrameIndexType[count];

                        ImGui::GetNeoKeyframeSelection(toRemove);

                        //Delete keyframes from your structure
                    }
                    ImGui::EndNeoTimeLine();
                }
                ImGui::EndNeoGroup();
            }

            ImGui::EndNeoSequencer();
        }
*/
        // *****************

        //parameter_panel.show(selected_node);

        // Following uncommented when investigating ImGui widget examples
        //ImGui::Begin;
        //bool show_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);
        //ImGui::End;
        // ----------------------------------
        // !!!!!!! TESTING !!!!!!!!!!!!!!!!
        if (!shaders_loaded) return;

        //ofBackground(0.0,1.0,1.0,1.0);

        ofEnableDepthTest();// +++++++++++++
        
        //shader_contructor.begin();
        //// ofScale(120);
        //shader_contructor.setUniform4f("globalColor", 0 / 255., 0 / 255., 100 / 255., 255 / 255.);

        //// folowing line needs resolving
        ////bool usingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
        //shader_contructor.setUniform1f("usingTexture", false);
        //shader_contructor.setUniform1f("usingColors", true);

        //shader_contructor.material_shader.updateMaterial(shader_contructor, *ofGLPRenderer);
        //shader_contructor.material_shader.updateLights(shader_contructor, *ofGLPRenderer);
        //shader_contructor.material_shader.updateShadows(shader_contructor, *ofGLPRenderer);
        //shader_contructor.material_shader.updateEnvironmentMaps(shader_contructor, *ofGLPRenderer);

        //vw_scene.render_scene();

        //shader_contructor.end();
        // !!!!!!!!!!!!!!!! BEGIN to be done in vw_scene.render_scene() !!!!!!!!!!!!!!!!!!!


        scene_lights_manager.scene_lights_render_setup();
        scene_cameras_manager.scene_cameras_render_setup();

        // If Mouse or keyboard inputs are outside the 2D graphics viewport
        // disable all mouse and keyboard events that the scene cameras capture
        // and use to alter or perform tasks in the viewport window.
        if (io.WantCaptureMouse && io.WantCaptureKeyboard)
            scene_cameras_manager.disable_render_mouse_input();
        else
            scene_cameras_manager.enable_render_mouse_input();

        if (light->shouldRenderShadowDepthPass()) {
            int numShadowPasses = light->getNumShadowDepthPasses();
//cout << "light->shouldRenderShadowDepthPass() : " << numShadowPasses << std::endl;

            for (int j = 0; j < numShadowPasses; j++) {
                if (!light->beginShadowDepthPass(j)) {
                    cout << "!light->beginShadowDepthPass(j) : " << j << std::endl;
                    break;
                }
                //else
                //    cout << "light->beginShadowDepthPass(j) : OKKK " << j << std::endl;
                render_scene();

                light->endShadowDepthPass(j);
            }
        }
        //else
        //    cout << "! light->shouldRenderShadowDepthPass() : " << std::endl;

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (!ofIsGLProgrammableRenderer()) {
            ofEnableLighting();
        }

        // CULL the back faces of the geometry for rendering
        //glEnable(GL_CULL_FACE);
       // glFrontFace(GL_CW);
       // glCullFace(GL_BACK);
        render_scene();

        //glDisable(GL_CULL_FACE);

        if (!ofIsGLProgrammableRenderer()) {
            ofDisableLighting();
        }


        //for (int i = 0; i < lights.size(); i++) {
        //    auto& light = lights[i];

        //    ofSetColor(light->getDiffuseColor());
        //    if (light->getType() == OF_LIGHT_POINT) {
        //        ofDrawSphere(light->getPosition(), 12);
        //    }
        //    else {
        //        light->draw();
        //    }
        //    if (light->getIsEnabled() && light->getShadow().getIsEnabled() && bDrawFrustums) {
        //        light->getShadow().drawFrustum();
        //    }
        //}
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        scene_cameras_manager.scene_cameras_render_exit();
        scene_lights_manager.scene_lights_render_exit();

        ofDisableDepthTest(); // +++++++++++++
        // !!!!!!!!!!!!!!!! END to be done in vw_scene.render_scene() !!!!!!!!!!!!!!!!!!!


        ImGui_ui_context.ImGui_post_render();
    }

    void render_scene() {

        vw_scene->scene_entities_manager.render_objects();

        //matSphere.begin();
//if(shad != NULL){
        //shad->begin();
       // ofPushMatrix();
        //vw_icosphere->render();
        //vw_icosphere->sphere.draw();
        //ofPopMatrix();
        //shad->end();
//}
        //matSphere.end();

        matSphere.begin();
        ofPushMatrix();
        //ofScale(50);
        meshPlySphere.draw();
        ofPopMatrix();
        matSphere.end();

        mat_plane.begin();
        ofPushMatrix();
        //ofScale(100);
        //ofTranslate(0, 0, -2.0);
        mesh_plane.draw();
        ofPopMatrix();
        mat_plane.end();
    }

    void setup_test_data() {
        // !!!!!!!!!!!!!!!!!!! testing !!!!!!!!!!!!!!!!!!!!!!

// ++++++++++++++++++++++++++++++++++++++++++++++
        entity_catedgory_id = vw_scene->scene_entities_manager.define_new_entity_category("point_cloud");
        //cout << "setup entity_catedgory_id @@@@ : " << entity_catedgory_id << std::endl;

                // ++++++++++++++++++++++++++++++++++++++++++++++
        entity_object = new vw_object_base_class;
        entity_object02 = new vw_object_base_class;
        entity_object03 = new vw_object_base_class;

        entity_object->object_category_id = entity_catedgory_id;
        entity_object02->object_category_id = entity_catedgory_id;
        entity_object03->object_category_id = entity_catedgory_id;

        entity_object->geometry = new entity_base_geometry_class;
        entity_object02->geometry = new entity_base_geometry_class;
        entity_object03->geometry = new entity_base_geometry_class;

        pc = new point_cloud3D_class;
        pc2 = new point_cloud3D_class;
        pc3 = new point_cloud3D_class;
        entity_object->geometry = pc;
        entity_object02->geometry = pc2;
        entity_object03->geometry = pc3;
        entity_object03->id = 100;

        entity_object->gizmo_display = node_gizmo_display_enum::axis;
        entity_object02->gizmo_display = node_gizmo_display_enum::axis;
        entity_object03->gizmo_display = node_gizmo_display_enum::axis;

        pc->define_geometry_render_texture_method(geometry_render_texture_method_enum::shader);
        pc2->define_geometry_render_texture_method(geometry_render_texture_method_enum::shader);
        pc3->define_geometry_render_texture_method(geometry_render_texture_method_enum::shader);

        if (!pc->generate_test_data(1.0, 200.0)) {
            cout << " point_cloud3D_class : geometry not updated !!!!" << std::endl;
        }
        else {

            cout << " vertices Num entity : " << entity_object->geometry->getNumVertices() << std::endl;

            // shader files must exist in directory called data. Need to change this
            if (entity_object->geometry->create_shader() == NULL) {
                cout << " entity_object->geometry->create_shader()== NULL " << std::endl;
            }
            else {
                shaders_loaded = entity_object->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");

                //if (!shaders_loaded) {
                if (!entity_object->geometry->shader->shader_compile_successful) {
                    cout << " Shaders not loaded !!!!! : " << std::endl;
                    //std::string s = "jjjj\n";
                    //cout << s << std::endl;
                    cout << entity_object->geometry->shader->compile_log << std::endl;
                    cout << " Shaders not loaded !!!!! END : " << std::endl;
                }
                else {
                    //cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
                    cout << " Shaders loaded ^^^^^ : " << std::endl;
                    //cout << entity_object03->geometry->shader->compile_log << std::endl;
                }

                cout << " VBO vertices Num  : " << pc->vertex_buffer.getNumVertices() << std::endl;
            }
        }

        if (!pc2->generate_test_data(0.5, -100.0)) {
            cout << " point_cloud3D_class : geometry not updated 2 !!!!" << std::endl;
        }
        else {

            cout << " vertices Num entity 2: " << entity_object02->geometry->getNumVertices() << std::endl;

            // shader files must exist in directory called data. Need to change this
            if (entity_object02->geometry->create_shader() == NULL) {
                cout << " entity_object02->geometry->create_shader()== NULL " << std::endl;
            }
            else {
                shaders_loaded = entity_object02->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader2.glsl");

                //if (!shaders_loaded) {
                if (!entity_object02->geometry->shader->shader_compile_successful) {
                    cout << " Shaders not loaded 2 !!!!! : " << std::endl;
                    cout << entity_object02->geometry->shader->compile_log << std::endl;
                    cout << " Shaders not loaded 2 !!!!! END : " << std::endl;
                }
                else
                    cout << " Shaders loaded ^^^^^ 2 : " << std::endl;

                cout << " VBO vertices Num  2 : " << pc2->vertex_buffer.getNumVertices() << std::endl;

            }
        }

        if (!pc3->generate_test_data(0.75, 400.0)) {
            cout << " point_cloud3D_class : geometry not updated 2 !!!!" << std::endl;
        }
        else {

            cout << " vertices Num entity 3: " << entity_object03->geometry->getNumVertices() << std::endl;

            if (entity_object03->geometry->create_shader() == NULL) {
                cout << " entity_object03->geometry->create_shader()== NULL " << std::endl;
            }
            else {
                // shader files must exist in directory called data. Need to change this
                shaders_loaded = entity_object03->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader2.glsl");

                //if (!shaders_loaded) {
                if (!entity_object03->geometry->shader->shader_compile_successful) {
                    cout << " Shaders not loaded 3 !!!!! : " << std::endl;
                    cout << entity_object03->geometry->shader->compile_log << std::endl;
                    cout << " Shaders not loaded 3 !!!!! END : " << std::endl;
                }
                else
                    cout << " Shaders loaded ^^^^^ 3 : " << std::endl;

                cout << " VBO vertices Num  3 : " << pc3->vertex_buffer.getNumVertices() << std::endl;
            }
        }
        cout << "Entities 00 " << std::endl;
        //entity_category_id = vw_scene.scene_entities_manager.define_new_entity_category("mesh");
        ico_entity = new vw_object_base_class;
        ico_entity->object_category_id = vw_scene->scene_entities_manager.define_new_entity_category("mesh");
        //ico_entity->geometry = new entity_base_geometry_class(ico_entity);
        ico_entity->geometry = new entity_base_geometry_class();
        vw_icosphere = new icosphere_geometry_class;
        ico_entity->geometry = vw_icosphere;
        vw_icosphere->init(ico_entity,50.0, 1.0);
        vw_icosphere->create_material();
        ico_entity->gizmo_display = node_gizmo_display_enum::axis;
        ico_entity->axis_size = 100.0;
        ico_entity->display_normals = true;

        // Test setting existing attribute data to a given shader program
        //if (!entity_object->geometry->define_geometry_attributes_shader()) {
        //    cout << " !entity_object->geometry->define_geometry_attributes_shader()" << std::endl;
        //}

        //if (!entity_object02->geometry->define_geometry_attributes_shader()) {
        //    cout << " !entity_object02->geometry->define_geometry_attributes_shader()" << std::endl;
        //}

        //if (!entity_object03->geometry->define_geometry_attributes_shader()) {
        //    cout << " !entity_object03->geometry->define_geometry_attributes_shader()" << std::endl;
        //}

        // -------------------------------------
 //cout << "Entities 01 "  << std::endl;
/*        entity_object02->setParent(*entity_object); // setParent may need to be changed
        entity_object03->setParent(*entity_object02);// setParent may need to be changed
        entity_object->add_child(entity_object02);
        entity_object02->add_child(entity_object03);

        ico_entity->setParent(*entity_object03);
        entity_object03->add_child(ico_entity);
        //cout << "Entities 02 "  << std::endl;
        ico_entity->set_local_location(-200.0, 0.0, 0.0, true, true);
        entity_object03->set_local_location(0.0, -200.0, 0.0, true, true);
        entity_object02->set_local_location(0.0, -200.0, 0.0, true, true);
        entity_object->set_local_location(0.0, 100.0, 0.0, true, true);
        //cout << "Entities 03 "  << std::endl;
        vw_scene.scene_entities_manager.add_object(entity_object, entity_catedgory_id);
        vw_scene.scene_entities_manager.add_object(entity_object02, entity_catedgory_id);
        vw_scene.scene_entities_manager.add_object(entity_object03, entity_catedgory_id);



        cout << "ico_entity colors number : " << vw_icosphere->getNumColors() << std::endl;
        for (int i = 0; i < vw_icosphere->getNumVertices(); i++) {
            vw_icosphere->addColor({ 1.0,1.0,0.0 });
        }
        cout << "ico_entity colors number after add : " << vw_icosphere->getNumColors() << std::endl;

        vw_scene.scene_entities_manager.add_object(ico_entity, ico_entity->object_category_id);
        ico_entity->geometry->display_wireframe = true;
        ico_entity->geometry->display_faces     = true;
        //ico_entity->geometry->display_vertices  = true;
*/
        vw_icosphere->material->setPBR(true);
        vw_icosphere->material->setDiffuseColor(ofFloatColor(1.0, 1.0));
        vw_icosphere->material->setMetallic(1.0);
        vw_icosphere->material->setAmbientColor(1.0);
        ////matSphere.setRoughness(0.05);
        vw_icosphere->material->setRoughness(.75);
        vw_icosphere->define_geometry_render_texture_method(geometry_render_texture_method_enum::material);
        // -------------------------------------
 //cout << " Lights 00 "  << std::endl;
        //light = new ofLight;
        light = scene_lights_manager.add_light();
        //light = scene_lights_manager.selected_light;
        //light = new vw_light_class;
 //cout << " Lights 01 "  << std::endl;

        //light->enable();
        //light->setPointLight();
        //light->setSpotlight(60, 20);
        //light->setup();
        light->setDirectional();


        //light->setPosition(100.1, 400, 600);
        light->setPosition(100.1, 900, 1300);
        light->lookAt(glm::vec3(0, 0, 0));

        light->getShadow().setEnabled(true);// +++++
        light->getShadow().setGlCullingEnabled(true);// +++++
        light->getShadow().setDirectionalBounds(2000, 1000);// +++++
        light->getShadow().setNearClip(200);// +++++
        light->getShadow().setFarClip(10000);// +++++
        light->getShadow().setStrength(1.0);// +++++
        light->getShadow().setShadowType(OF_SHADOW_TYPE_PCF_LOW);// +++++

        // increase alpha value to increase strength of light
        light->setDiffuseColor(ofFloatColor(0.5, 2.0));
        light->display_shadow_frustrum = true;

        // light->disable();
  //cout << " Lights 02 "  << std::endl;
         //light2 = new ofLight;
         //light2 = new vw_light_class;
 /*        light2 = scene_lights_manager.add_light();
 // cout << " Lights 03 "  << std::endl;
         //light2->setup();
         //light2->setDirectional();
         light2->setPointLight();
         light2->enable();
         //light2->setPosition(-100.1,-400,-600);
         light2->set_position(-100.1,-400,-600);
         //light2->lookAt(glm::vec3(0, 0, 0));
         //light2->getShadow().setEnabled(true);
         //light2->getShadow().setGlCullingEnabled(true);
         //light2->getShadow().setDirectionalBounds(2000, 1000);
         //light2->getShadow().setNearClip(200);
         //light2->getShadow().setFarClip(2000);
         //light2->getShadow().setStrength(1.0);
         // increase alpha value to increase strength of light2
         light2->setDiffuseColor(ofFloatColor(0.5, 2.0));
         //light2->getShadow().setShadowType(OF_SHADOW_TYPE_PCF_LOW);
         light2->disable();
  //cout << " Lights 04 "  << std::endl;
  */
        vw_scene->set_scene_shadow_render_settings();
        /*
                 // shadows are disabled by default
                 // call this function to enable all of them that are attached to lights
                 ofShadow::enableAllShadows();// +++++++++++
                 // shadow bias is the margin of error the shadow depth
                 // increasing the bias helps reduce shadow acne, but can cause light leaking
                 // try to find a good balance that fits your needs
                 // bias default is 0.005
                 ofShadow::setAllShadowBias(0.007);// +++++++++++
                 // normal bias default is 0
                 // moves the bias along the normal of the mesh, helps reduce shadow acne
                 ofShadow::setAllShadowNormalBias(-4.f);// +++++++++++
                #ifndef TARGET_OPENGLES
                 ofShadow::setAllShadowDepthResolutions(1024, 1024);// +++++++++++
                #endif
                 ofShadowType shadowType = OF_SHADOW_TYPE_PCF_LOW;// +++++++++++
                 ofShadow::setAllShadowTypes(shadowType);// +++++++++++
        */


        // higher resolution sphere
        meshPlySphere = ofMesh::icosphere(1.0, 5);

        //meshPlySphere.clearColors();
        cout << "mesh colors number : " << meshPlySphere.getNumColors() << std::endl;
        for (int i = 0; i < meshPlySphere.getNumVertices(); i++) {
            meshPlySphere.addColor({ 1.0,0.0,0.0 });
            glm::vec3 v = meshPlySphere.getVertex(i);
            meshPlySphere.setVertex(i, { v.x * 120.0f + 200.0f, v.y * 120.0f, v.z * 120.0f });
        }
        cout << "mesh colors number after add : " << meshPlySphere.getNumColors() << std::endl;

        //cubeMap.load("kloppenheim_06_puresky_1k.exr", 512);

        matSphere.setPBR(true);
        matSphere.setDiffuseColor(ofFloatColor(1.0, 1.0));
        matSphere.setMetallic(1.0);
        matSphere.setAmbientColor(1.0);
        ////matSphere.setRoughness(0.05);
        matSphere.setRoughness(.25);

        mesh_plane = ofMesh::plane(100, 100, 10, 10, OF_PRIMITIVE_TRIANGLES);
        cout << "mesh plane colors number : " << mesh_plane.getNumColors() << std::endl;
        for (int i = 0; i < mesh_plane.getNumVertices(); i++) {
            mesh_plane.addColor({ 1.0,1.0,0.0 });
            glm::vec3 v = mesh_plane.getVertex(i);
            mesh_plane.setVertex(i, { v.x * 100.0f, v.y * 100.0f, (v.z - 2.0f) * 100.0f });
        }
        cout << "mesh plane colors number after add : " << mesh_plane.getNumColors() << std::endl;

        //mat_plane.setPBR(true);
        mat_plane.setDiffuseColor(ofFloatColor(1.0, 1.0));
        mat_plane.setMetallic(0.25);
        mat_plane.setAmbientColor(1.0);
        //////mat_plane.setRoughness(0.05);
        mat_plane.setRoughness(.75);

        mat_plane.setPBR(true);
        mat_plane.loadTexture(OF_MATERIAL_TEXTURE_DIFFUSE, "plywood/plywood_diff_2k.jpg");
        mat_plane.loadTexture(OF_MATERIAL_TEXTURE_NORMAL, "plywood/plywood_nor_gl_2k.png");
        mat_plane.loadTexture(OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, "plywood/plywood_arm_2k.jpg");
        mat_plane.setTexCoordScale(1000.0, 1000.0);
        mat_plane.setClearCoatEnabled(true);
        mat_plane.setClearCoatStrength(1.0);
        mat_plane.setClearCoatRoughness(0.0);

        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        ofGLPRenderer = dynamic_cast<ofGLProgrammableRenderer*>(ofGetCurrentRenderer().get());
        //ofGLBRenderer = ofGetGLRenderer().get();

        if (ofGLPRenderer == NULL) {
            cout << "ofGLPRenderer == NULL\n";
        }
        else {
            cout << "ofGLPRenderer != NULL\n";
        }

        shader_contructor.ofGLPRenderer = ofGLPRenderer;
        shader_contructor.define_basic_default_shader(basic_shader_function_type_enum::color, ofGLPRenderer, true);

        if (shader_contructor.is_PBR_shader_material()) cout << "IS PBR SHADER #####\n";

        shader_contructor.material_shader.setDiffuseColor(ofFloatColor(1.0, 1.0));
        shader_contructor.material_shader.setMetallic(.5);// only works for PBR set to true otherwise if used will cause shader not to display object
        shader_contructor.material_shader.setAmbientColor(.6);
        shader_contructor.material_shader.setRoughness(.25);// only works for PBR set to true otherwise if used will cause shader not to display object



        //shad = shader_contructor.material_shader.getShader(OF_NO_TEXTURE, false, *ofGLPRenderer);

        //shad = matSphere.getShader(OF_NO_TEXTURE, true, *ofGLPRenderer);// Does not work
//        shad = matSphere.get_shader(OF_NO_TEXTURE, true, *ofGLPRenderer); // Does not work
//        
//if (shad == NULL)
//cout << "shad == NULL\n";
//else 
//cout << "shad != NULL\n";


        //vw_icosphere->shader = new ofShader;
        //vw_icosphere->shader = shad;

        //shader_contructor.define_basic_vertex_source_code(basic_shader_function_type_enum::color,ofGLPRenderer, true);
        //geometry_shader_definition_struct_type geometry_shader_definition;
        //shader_contructor.define_basic_geometry_source_code(geometry_shader_definition,ofGLPRenderer);

        // +++++++++++++++++++++++++++++++++++++++++++
        hcp_voxel = new voxel_hcp_object_class;
        hcp_voxel->object_category_id = vw_scene->scene_entities_manager.define_new_entity_category("HCP_Voxel");
        //hcp_voxel->geometry = new point_cloud3D_class;
        hcp_voxel->gizmo_display = node_gizmo_display_enum::none;
        hcp_voxel->axis_size = 100.0;

        //voxel_hcp_parameters_widget_class *pw = new voxel_hcp_parameters_widget_class;
        //hcp_voxel->parameter_widget = pw;

        //hcp_voxel->parameter_widget->log_panel = log_panel; // ---------------
        //hcp_voxel->define_initial_shader_program();

        if (vw_scene->scene_entities_manager.add_object(hcp_voxel, hcp_voxel->object_category_id)) {
cout << "vw_scene->scene_entities_manager.add_object : " << hcp_voxel->id << " : " << hcp_voxel->object_category_id << std::endl;

            hcp_voxel->define_initial_shader_program();

            //hcp_voxel->geometry->create_shader();
            //hcp_voxel->geometry->init();
            if(hcp_voxel->geometry->shader){
                if (ofIsGLProgrammableRenderer()) {
                    cout << " ofIsGLProgrammableRenderer() : " << std::endl;
                    //shaders_loaded = hcp_voxel->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");
                    //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl");
                    //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_PGS.glsl");
                    //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_GS.glsl");
                } else{
                    cout << "!!!!ofIsGLProgrammableRenderer() : " << std::endl;
                    return;
                }

            //if (!shaders_loaded) {
            if (!hcp_voxel->geometry->shader->shader_compile_successful) {
                cout << " hcp_voxel Shaders not loaded !!!!! : " << std::endl;
//std::string s = "jjjj\n";
//cout << s << std::endl;
                cout << hcp_voxel->geometry->shader->compile_log << std::endl;
                cout << "hcp_voxel Shaders not loaded !!!!! END : " << std::endl;
            }
            else {
//cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
                cout << "hcp_voxel Shaders loaded ^^^^^ : " << std::endl;
//cout << entity_object03->geometry->shader->compile_log << std::endl;
             }
            } else
                cout << "hcp_voxel Shader not created : " << std::endl;

        }

        if (hcp_voxel != NULL) {
            cout << "setup here : hcp_voxel != NULL" << std::endl;
            globalc::set_current_selected_entity_id(hcp_voxel->id);
            globalc::set_current_selected_entity_category_type_id(hcp_voxel->object_category_id);
            globalc::set_current_selected_entity_type_id(ENTITY_TYPE_OBJECT);

            //current_selected_entity_id = hcp_voxel->id;
            //current_selected_entity_category_type_id = hcp_voxel->object_category_id;
            //current_selected_entity_type_id = ENTITY_TYPE_OBJECT;
        }

        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    }


    void keyPressed(int key) {}
    void keyReleased(int key) {}
    void mouseMoved(int x, int y) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void mouseEntered(int x, int y) {}
    void mouseExited(int x, int y) {}
    void windowResized(int w, int h) {}
    void dragEvent(ofDragInfo dragInfo) {}
    void gotMessage(ofMessage msg) {}


    // !!!!!!!!!!!!!!!! Testing !!!!!!!!!!!!!!!!
    vw_object_base_class *entity_object, *entity_object02, *entity_object03,* ico_entity;;
    point_cloud3D_class *pc, *pc2, *pc3;
   // ofShader shader, shader2;shader3;
    bool shaders_loaded = false;

    int cs = 0;

    //ofCubeMap cubeMap;
    ofLight *light, *light2;
    //ofLight _light; // This crashes the application
    ofVboMesh meshPlySphere, mesh_plane;

    ofMaterial matSphere,mat_plane;
    shader_contruction_class shader_contructor;

    ofGLProgrammableRenderer *ofGLPRenderer;
    ofBaseGLRenderer *ofGLBRenderer;

    //vw_camera_class vw_camera;
    scene_cameras_manager_class scene_cameras_manager;
    scene_lights_manager_class scene_lights_manager;

    vw_scene_objects_category_class vw_scene_objects_category;

    vw_scene_class *vw_scene;
    int entity_catedgory_id;

    icosphere_geometry_class *vw_icosphere;

    vw_camera_class* camer = NULL;

    // +++++++++++++++++++++++

    voxel_hcp_object_class *hcp_voxel;

    scene_node_editor_class *scene_node_editor, *scene_node_editor1;

    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    
    GLFWwindow                 *GLFW_window_ptr; // glfw 


    ImGui_ui_context_class ImGui_ui_context;


    scene_entities_manager_class *scene_manager = NULL;

    // UI components
    log_panel_class       *log_panel;
    parameter_panel_class  parameter_panel;

    node_editor_panel_class node_editor_panel;

    // Timeline vars
    //int32_t currentFrame = 0;
    //int32_t startFrame = -10;
    //int32_t endFrame = 64;
    //bool transformOpen = true;
    //std::vector<ImGui::FrameIndexType> keys = { 0, 10, 24 };
    //bool doDelete = true;
};
#pragma once

#include "ofMain.h"

#include <VW_framework/App/global.h> // Must be placed here above all other includes

#include <VW_framework/Scene/scene_entities_manager.h>

#include <ImGuiEx/ImGui_utils.h>

#include "Panels/log_panel.h"
#include "Panels/property_panel.h"

// !!!!!!!!!!!!!! testing !!!!!!!!!!!!!!!!!!!!


#include <FrameWork/VW_framework/Geometry/vw_point_cloud.h>

//#include <FrameWork/VW_framework/GL/vw_material.h>

#include <FrameWork/VW_framework/GL/vw_shader_Constructor.h>

#include <VW_framework/Scene/vw_scene.h>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class main_window_class : public ofBaseApp {
public:
    //main_window_class() : mIsRunning(true), GLFW_window_ptr(nullptr) {
    main_window_class() : GLFW_window_ptr(nullptr) {
        // ui_context = std::make_unique<UI_context_class>();
        // render_context = new openGL_context_class;
    }

    ~main_window_class() {
        //  ImNodes::DestroyContext();
        ImGui_ui_context.ImGui_end();
        //render_context->end();
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
        scene_manager = new scene_entities_manager_class;
        //scene_manager = new scene_manager_class;
        //scene_manager->log_panel = log_panel;

        //parameter_panel.scene_manager = scene_manager;
        parameter_panel.log_panel = log_panel;
        parameter_panel.scene_manager = scene_manager;


        // !!!!!!!!!!!!!!!!!!! testing !!!!!!!!!!!!!!!!!!!!!!

        // ++++++++++++++++++++++++++++++++++++++++++++++
        entity_catedgory_id = vw_scene.scene_entities_manager.define_new_entity_category("point_cloud");
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

                if (!shaders_loaded) {
                    cout << " Shaders not loaded !!!!! : " << std::endl;
                }
                else {
                    //cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
                    cout << " Shaders loaded ^^^^^ : " << entity_object->geometry->shader->getProgram() << " : " << std::endl;

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

                if (!shaders_loaded) {
                    cout << " Shaders not loaded 2 !!!!! : " << std::endl;
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

                if (!shaders_loaded) {
                    cout << " Shaders not loaded 3 !!!!! : " << std::endl;
                }
                else
                    cout << " Shaders loaded ^^^^^ 3 : " << std::endl;

                cout << " VBO vertices Num  3 : " << pc3->vertex_buffer.getNumVertices() << std::endl;
            }
        }

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

        entity_object02->setParent(*entity_object); // setParent may need to be changed
        entity_object03->setParent(*entity_object02);// setParent may need to be changed
        entity_object->add_child(entity_object02);
        entity_object02->add_child(entity_object03);

        entity_object03->set_position(0.0, -50.0, 0.0, true);
        entity_object02->set_position(0.0, 50.0, 0.0, true);
        entity_object->set_position(50.0, 0.0, 0.0, true);

        vw_scene.scene_entities_manager.add_object(entity_object, entity_catedgory_id);
        vw_scene.scene_entities_manager.add_object(entity_object02, entity_catedgory_id);
        vw_scene.scene_entities_manager.add_object(entity_object03, entity_catedgory_id);

        // -------------------------------------
 //cout << " Lights 00 "  << std::endl;
        //light = new ofLight;
        light = scene_lights_manager.add_light();
        //light = scene_lights_manager.selected_light;
        //light = new vw_light_class;
 //cout << " Lights 01 "  << std::endl;

        //light->enable();
        light->setPointLight();
        //light->setSpotlight(60, 20);
        //light->setup();
        //light->setDirectional();   


        //light->setPosition(100.1, 400, 600);
        light->set_position(100.1, 900, 1300);
        //light->lookAt(glm::vec3(0, 0, 0));

        light->getShadow().setEnabled(true);// +++++
        light->getShadow().setGlCullingEnabled(true);// +++++
        light->getShadow().setDirectionalBounds(2000, 1000);// +++++
        light->getShadow().setNearClip(200);// +++++
        light->getShadow().setFarClip(10000);// +++++
        light->getShadow().setStrength(1.0);// +++++

        // increase alpha value to increase strength of light
        light->setDiffuseColor(ofFloatColor(0.5, 2.0));
        light->getShadow().setShadowType(OF_SHADOW_TYPE_PCF_LOW);// +++++
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



        // higher resolution sphere
        meshPlySphere = ofMesh::icosphere(1.0, 5);

        //meshPlySphere.clearColors();
        cout << "mesh colors number : " << meshPlySphere.getNumColors() << std::endl;
        for (int i = 0; i < meshPlySphere.getNumVertices(); i++) {
            meshPlySphere.addColor({ 1.0,0.0,0.0 });
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
        }
        cout << "mesh plane colors number after add : " << mesh_plane.getNumColors() << std::endl;

        //mat_plane.setPBR(true);
        //mat_plane.setDiffuseColor(ofFloatColor(1.0, 1.0));
        //mat_plane.setMetallic(0.25);
        //mat_plane.setAmbientColor(1.0);
        //////mat_plane.setRoughness(0.05);
        //mat_plane.setRoughness(.75);

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




        //shader_contructor.define_basic_vertex_source_code(basic_shader_function_type_enum::color,ofGLPRenderer, true);
        geometry_shader_definition_struct_type geometry_shader_definition;
        shader_contructor.define_basic_geometry_source_code(geometry_shader_definition, ofGLPRenderer);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    }

    void update() {
        //entity_object->setPosition(50.0, 0.0, 0.0);// this works
        //entity_object02->setPosition(200.0, 0.0, 0.0);// this does not work


        if (cs == 0) {
            //            entity_object->setScale(0.75);
            //            entity_object->change_geometry_scale();
            //            //entity_object->tiltDeg(45.0);
            //            entity_object->rollDeg(45.0);
            //            entity_object->change_geometry_orientation();
            //            entity_object02->rollDeg(45.0);
            ////cout << "child change orientation " << std::endl;
            //            entity_object02->change_geometry_orientation();
            //            entity_object->setScale(0.5);
            //            entity_object->change_geometry_scale();

        }

        cs++;
    }


    void draw() {
        glfwMakeContextCurrent(GLFW_window_ptr);
        ImGui_ui_context.ImGui_pre_render();

        //// Log panel to display application logging messages and shader code compile error messages
        log_panel->display_application_log();
        log_panel->display_code_log();

        parameter_panel.show();
        //parameter_panel.show(selected_node);

        // Following uncommented when investigating ImGui widget examples
        ImGui::Begin;
        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);
        ImGui::End;
        // ----------------------------------
        // !!!!!!! TESTING !!!!!!!!!!!!!!!!
        if (!shaders_loaded) return;

        //ofBackground(.5);

        ofEnableDepthTest();// +++++++++++++

        vw_scene.render_scene();

        scene_lights_manager.scene_lights_render_setup();
        scene_cameras_manager.scene_cameras_render_setup();

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



/*
        shader_contructor.begin();
        ofScale(120);
        shader_contructor.setUniform4f("globalColor", 100 / 255., 255 / 255., 100 / 255., 255 / 255.);

        // folowing line needs resolving
        //bool usingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
        shader_contructor.setUniform1f("usingTexture", false);
        shader_contructor.setUniform1f("usingColors", true);

        shader_contructor.material_shader.updateMaterial(shader_contructor, *ofGLPRenderer);
        shader_contructor.material_shader.updateLights(shader_contructor, *ofGLPRenderer);
        shader_contructor.material_shader.updateShadows(shader_contructor, *ofGLPRenderer);
        shader_contructor.material_shader.updateEnvironmentMaps(shader_contructor, *ofGLPRenderer);
*/
// render_scene();

//        shader_contructor.end();
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
        light->getShadow().drawFrustum();
        //    }
        //}
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


        scene_cameras_manager.scene_cameras_render_exit();
        scene_lights_manager.scene_lights_render_exit();

        ofDisableDepthTest(); // +++++++++++++
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


        ImGui_ui_context.ImGui_post_render();
    }

    void render_scene() {

        matSphere.begin();
        ofPushMatrix();
        ofScale(50);
        meshPlySphere.draw();
        ofPopMatrix();
        matSphere.end();

        mat_plane.begin();
        ofPushMatrix();
        ofScale(100);
        ofTranslate(0, 0, -2.0);
        mesh_plane.draw();
        ofPopMatrix();
        mat_plane.end();
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
    vw_object_base_class* entity_object, * entity_object02, * entity_object03;
    point_cloud3D_class* pc, * pc2, * pc3;
    ofShader shader, shader2, shader3;
    bool shaders_loaded = false;

    int cs = 0;

    //ofCubeMap cubeMap;
    ofLight* light, * light2;
    //ofLight _light; // This crashes the application
    ofVboMesh meshPlySphere, mesh_plane;

    ofMaterial matSphere, mat_plane;
    shader_contruction_class shader_contructor;

    ofGLProgrammableRenderer* ofGLPRenderer;
    ofBaseGLRenderer* ofGLBRenderer;

    //vw_camera_class vw_camera;
    scene_cameras_manager_class scene_cameras_manager;
    scene_lights_manager_class scene_lights_manager;

    vw_scene_objects_category_class vw_scene_objects_category;

    vw_scene_class vw_scene;
    int entity_catedgory_id;
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:


    GLFWwindow* GLFW_window_ptr; // glfw 


    ImGui_ui_context_class ImGui_ui_context;


    scene_entities_manager_class* scene_manager = NULL;

    // UI components
    log_panel_class* log_panel;
    parameter_panel_class  parameter_panel;




};
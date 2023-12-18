#pragma once

#include "ofMain.h"

#include <VW_framework/App/global.h> // Must be placed here above all other includes

//#include <VW_framework/Scene/scene_manager.h>
#include <VW_framework/Scene/scene_entities_manager.h>

#include <ImGuiEx/ImGui_utils.h>

#include "Panels/log_panel.h"
#include "Panels/property_panel.h"

// !!!!!!!!!!!!!! testing !!!!!!!!!!!!!!!!!!!!


#include <FrameWork/VW_framework/Geometry/vw_point_cloud.h>

#include <FrameWork/VW_framework/GL/vw_material.h>

#include <FrameWork/VW_framework/GL/vw_shader_Constructor.h>

//#include <VW_framework/3D/vw_camera.h>
//#include <VW_framework/Scene/scene_cameras.h>
//#include <VW_framework/Scene/scene_lights.h>
#include <VW_framework/Scene/vw_scene.h>

#include <VW_framework/3D/vw_light.h>
//#include "ofLight.h"
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

        scene_manager = new scene_entities_manager_class;
        //scene_manager = new scene_manager_class;
        //scene_manager->log_panel = log_panel;

        //parameter_panel.scene_manager = scene_manager;
        parameter_panel.log_panel = log_panel;
        parameter_panel.scene_manager = scene_manager;


        // !!!!!!!!!!!!!!!!!!! testing !!!!!!!!!!!!!!!!!!!!!!

        // ++++++++++++++++++++++++++++++++++++++++++++++
        entity_catedgory_id = vw_scene.scene_entities_manager.define_new_entity_category("point_cloud");
        cout << "setup entity_catedgory_id @@@@ : " << entity_catedgory_id << std::endl;

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
            //shaders_loaded = shader.load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");
            if (entity_object->geometry->create_shader() == NULL) {
                cout << " entity_object->geometry->create_shader()== NULL " << std::endl;
            }
            else {

                //shaders_loaded = shader.load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");
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

            //light->setup();
            //light->setPosition(-100, 200, 0);
            //ofEnableDepthTest();
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
                // shaders_loaded = shader2.load("Shaders/passthru.vert", "Shaders/grid_fragment_shader2.glsl");
                shaders_loaded = entity_object02->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader2.glsl");

                if (!shaders_loaded) {
                    cout << " Shaders not loaded 2 !!!!! : " << std::endl;
                }
                else
                    cout << " Shaders loaded ^^^^^ 2 : " << std::endl;

                cout << " VBO vertices Num  2 : " << pc2->vertex_buffer.getNumVertices() << std::endl;

            }
            //light->setup();
            //light->setPosition(-100, 200, 0);
            //ofEnableDepthTest();
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
                //shaders_loaded = shader3.load("Shaders/passthru.vert", "Shaders/grid_fragment_shader2.glsl");
                shaders_loaded = entity_object03->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader2.glsl");

                if (!shaders_loaded) {
                    cout << " Shaders not loaded 3 !!!!! : " << std::endl;
                }
                else
                    cout << " Shaders loaded ^^^^^ 3 : " << std::endl;

                cout << " VBO vertices Num  3 : " << pc3->vertex_buffer.getNumVertices() << std::endl;
            }

            //light->setup();
            //light->setPosition(-100, 200, 0);
            //ofEnableDepthTest();
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

        entity_object03->setPosition(0.0, -50.0, 0.0, true);
        entity_object02->setPosition(0.0, 50.0, 0.0, true);
        entity_object->setPosition(50.0, 0.0, 0.0, true);

        vw_scene.scene_entities_manager.add_new_entity(entity_object, entity_catedgory_id);
        vw_scene.scene_entities_manager.add_new_entity(entity_object02, entity_catedgory_id);
        vw_scene.scene_entities_manager.add_new_entity(entity_object03, entity_catedgory_id);

        // -------------------------------------

        //light = new ofLight;
        light = scene_lights_manager.selected_light;
        //light = new vw_light_class;

        //light->setup();
        light->setDirectional();
        //light->setPointLight();
        light->enable();
        light->setPosition(100.1, 400, 600);
        light->lookAt(glm::vec3(0, 0, 0));
        //light->getShadow().setEnabled(true);
        //light->getShadow().setGlCullingEnabled(true);
        //light->getShadow().setDirectionalBounds(2000, 1000);
        //light->getShadow().setNearClip(200);
        //light->getShadow().setFarClip(2000);
        //light->getShadow().setStrength(1.0);
        // increase alpha value to increase strength of light
        light->setDiffuseColor(ofFloatColor(0.5, 2.0));
        //light->getShadow().setShadowType(OF_SHADOW_TYPE_PCF_LOW);
        light->disable();

        //light2 = new ofLight;
        //light2 = new vw_light_class;
        light2 = scene_lights_manager.add_light();

        //light2->setup();
        //light2->setDirectional();
        light2->setPointLight();
        light2->enable();
        light2->setPosition(-100.1, -400, -600);
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



        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        ofGLPRenderer = dynamic_cast<ofGLProgrammableRenderer*>(ofGetCurrentRenderer().get());
        //ofGLBRenderer = ofGetGLRenderer().get();

        if (ofGLPRenderer == NULL) {
            cout << "ofGLPRenderer == NULL\n";
        }
        else {
            cout << "ofGLPRenderer != NULL\n";
        }

        //shader.load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");
        //shader.load("Shaders/default/color_vertex.glsl", "Shaders/default/color_frag.glsl");
        //shader_contructor.load("Shaders/default/color_vertex.glsl", "Shaders/default/color_frag.glsl");

        shader_contructor.ofGLPRenderer = ofGLPRenderer;
        shader_contructor.define_basic_default_shader(basic_shader_function_type_enum::color, ofGLPRenderer, true);

        //shader_contructor.material_shader.currentRenderShader = &shader;


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

        //light->enable();
        //light2->enable();
        //shader_contructor.material_shader.updateLights(shader_contructor, *ofGLPRenderer);
        //light->disable();
        //light2->disable();
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

        //ofEnableDepthTest();
        //ofEnableLighting();
        //light->enable();
        //light2->enable();
        scene_lights_manager.scene_lights_render_setup();

        if (light->shouldRenderShadowDepthPass()) {
            int numShadowPasses = light->getNumShadowDepthPasses();
            for (int j = 0; j < numShadowPasses; j++) {
                light->beginShadowDepthPass(j);
                matSphere.begin();
                ofPushMatrix();
                //ofTranslate(700.0 * cos(angle - PI), -20, sin(angle - PI) * 300.0 - 100);
                ofScale(120);
                meshPlySphere.draw();
                ofPopMatrix();
                matSphere.end();
                light->endShadowDepthPass(j);
            }
        }

        //ofcam.begin();
        //cam.begin();
        //vw_camera.setPosition(0, 0, 1000);
        //vw_camera.lookAt({ 0, 0, 0 });
        //vw_camera.begin();
        //scene_cameras_manager.selected_camera->begin();
        scene_cameras_manager.scene_cameras_render_setup();
        //ofEnableDepthTest();

        glPointSize(1.0f);

        vw_scene.scene_entities_manager.render_objects();

        // shader.begin();
         //ofScale(200.0, 200.0, 200.0);
         //ofRotateXDeg(45.0);

         //entity_object->geometry->render();

         //pc->render();
         //ofDrawRectangle (0.0, 0.0, 100, 100);
        // shader.end();

        glPointSize(2.0f);
        //shader2.begin();
        //ofScale(.5, .5, .5);

        //ofRotateXDeg(45.0);

        //entity_object02->geometry->render();
        //entity_object03->geometry->render();

        //entity_object->parent
        //pc2->render();
       // shader2.end();

        //boxMaterial.begin();
        //box.draw();
        //boxMaterial.end();

        //matSphere.begin();
        //ofPushMatrix();
        ////ofTranslate(700.0 * cos(angle - PI), -20, sin(angle - PI) * 300.0 - 100);
        //ofScale(120);
        //meshPlySphere.draw();
        //ofPopMatrix();
        //matSphere.end();

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

        // folowing lines needs resolving
       // ofShadow currentShadow = light->getShadow();
       // if (currentShadow.getIsEnabled()) {
 // cout << "currentShadow.getIsEnabled()\n";
            //if (currentShadow.isMultiCubeFacePass()) {
            //    currentShadow.updateDepth(shader, shadowCubeFace, *ofGLPRenderer);
           // }
            //else {
         //      currentShadow.updateDepth(shader, *ofGLPRenderer);
           // }
       // }

        //if (currentShadow) {
            //if (currentShadow->isMultiCubeFacePass()) {
            //    currentShadow->updateDepth(*currentShader, shadowCubeFace, *this);
            //}
            //else {
              // currentShadow->updateDepth(*currentShader, *this);
            //}
        //}

        meshPlySphere.draw();


        //ofPopMatrix();
        //shader_contructor.material_shader.end();
        shader_contructor.end();
        //shader.end();

        //light->getShadow().drawFrustum();

        //if (cubeMap.hasPrefilteredMap()) {
        //    cubeMap.drawPrefilteredCube(0.2f);
        //}

        //ofDrawBox(100.0);

        //ofDisableDepthTest();
        //ofcam.end();
        //cam.end();
        //vw_camera.end();
        //scene_cameras_manager.selected_camera->end();
        scene_cameras_manager.scene_cameras_render_exit();
        //light->disable();
        //light2->disable();
        //ofDisableLighting();
        //ofDisableDepthTest();

        scene_lights_manager.scene_lights_render_exit();
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


        ImGui_ui_context.ImGui_post_render();
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
   // entity_object_basis_class entity_object, entity_object02, entity_object03;
    //vw_node_class entity_object, entity_object02, entity_object03;
    vw_object_base_class* entity_object, * entity_object02, * entity_object03;
    point_cloud3D_class* pc, * pc2, * pc3;
    ofShader shader, shader2, shader3;
    bool shaders_loaded = false;

    ofEasyCam cam;
    //ofCamera ofcam;

    int cs = 0;

    //ofCubeMap cubeMap;
    //ofLight *light, *light2;
    vw_light_class* light, * light2;
    //ofLight _light; // This crashes the application
    ofVboMesh meshPlySphere;
    material_shader_class matSphere;
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
    //bool mIsRunning;


    void updateLights(const ofShader* shader, ofGLProgrammableRenderer& renderer, bool isPBR = false) const {
        //std::cout << "updateLights 00 : " << ofLightsData().size() << std::endl;

        for (size_t i = 0; i < ofLightsData().size(); i++) {
            //std::cout << "updateLights 01 : " <<std::endl;
            string idx = ofToString(i);
            shared_ptr<ofLight::Data> light = ofLightsData()[i].lock();
            //std::cout << "updateLights 01A : " <<std::endl;
            //if(!light)
            //std::cout << "updateLights 01B : !light" << std::endl;
            //if (!light->isEnabled)
            //std::cout << "updateLights 01C : !light->isEnabled" << std::endl;

            if (!light || !light->isEnabled) {
                shader->setUniform1f("lights[" + idx + "].enabled", 0);
                continue;
            }
            //std::cout << "updateLights 02 : " <<std::endl;
            glm::vec4 lightEyePosition = light->position;
            // pbr uses global positions
            if (!isPBR) {
                if (light->lightType == OF_LIGHT_DIRECTIONAL) {
                    // support for reversed phong lighting setup
                    lightEyePosition = renderer.getCurrentViewMatrix() * -light->position;
                    //std::cout << "updateLights !isPBR()  : light->lightType == OF_LIGHT_DIRECTIONAL " <<std::endl;
                }
                else {
                    lightEyePosition = renderer.getCurrentViewMatrix() * light->position;
                    //std::cout << "updateLights !isPBR()  : light->lightType != OF_LIGHT_DIRECTIONAL " <<std::endl;
                }
            }

            if (isPBR) {
                if (light->lightType == OF_LIGHT_DIRECTIONAL) {
                    lightEyePosition = glm::vec4(-light->direction, lightEyePosition.w);
                }
                if (light->lightType != OF_LIGHT_POINT) {
                    shader->setUniform3f("lights[" + idx + "].direction", light->direction);
                }
            }
            //std::cout << "updateLights 03 : " << lightEyePosition.x << " : " << lightEyePosition.y << " : " << lightEyePosition.z << " : " <<lightEyePosition.z << std::endl;
            //lightEyePosition = { 100.1, 400, 600 ,600};


            shader->setUniform1f("lights[" + idx + "].enabled", 1);
            shader->setUniform1f("lights[" + idx + "].type", light->lightType);
            shader->setUniform4f("lights[" + idx + "].position", lightEyePosition);
            if (!isPBR) {
                shader->setUniform4f("lights[" + idx + "].ambient", light->ambientColor);
                shader->setUniform4f("lights[" + idx + "].specular", light->specularColor);
            }
            shader->setUniform4f("lights[" + idx + "].diffuse", light->diffuseColor);

            if (light->lightType != OF_LIGHT_DIRECTIONAL) {
                // TODO: add in light radius if pbr?
                //shader->setUniform1f("lights[" + idx + "].radius", 0.0f);
                shader->setUniform1f("lights[" + idx + "].constantAttenuation", light->attenuation_constant);
                shader->setUniform1f("lights[" + idx + "].linearAttenuation", light->attenuation_linear);
                shader->setUniform1f("lights[" + idx + "].quadraticAttenuation", light->attenuation_quadratic);
            }
            //std::cout << "updateLights 04 : " <<std::endl;
            if (light->lightType == OF_LIGHT_SPOT) {
                // PBR light calcs are in world space
                glm::vec3 direction = light->direction;
                if (!isPBR) {
                    direction = glm::vec3(light->position) + light->direction;
                    glm::vec4 direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction, 1.0);
                    direction = glm::vec3(direction4) / direction4.w;
                    direction = direction - glm::vec3(lightEyePosition);
                    shader->setUniform3f("lights[" + idx + "].spotDirection", glm::normalize(direction));
                }
                //shader->setUniform3f("lights["+idx+"].spotDirection", glm::normalize(direction));
                shader->setUniform1f("lights[" + idx + "].spotExponent", light->exponent);
                shader->setUniform1f("lights[" + idx + "].spotCutoff", light->spotCutOff);
                shader->setUniform1f("lights[" + idx + "].spotCosCutoff", cos(ofDegToRad(light->spotCutOff)));
            }
            else if (light->lightType == OF_LIGHT_DIRECTIONAL) {
                if (!isPBR) {
                    glm::vec3 halfVector(glm::normalize(glm::vec4(0.f, 0.f, 1.f, 0.f) + lightEyePosition));
                    shader->setUniform3f("lights[" + idx + "].halfVector", halfVector);
                }
            }
            else if (light->lightType == OF_LIGHT_AREA) {
                shader->setUniform1f("lights[" + idx + "].width", light->width);
                shader->setUniform1f("lights[" + idx + "].height", light->height);
                glm::vec3 direction = light->direction;
                if (!isPBR) {
                    direction = glm::vec3(light->position) + light->direction;
                    glm::vec4 direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction, 1.0);
                    direction = glm::vec3(direction4) / direction4.w;
                    direction = direction - glm::vec3(lightEyePosition);
                    shader->setUniform3f("lights[" + idx + "].spotDirection", glm::normalize(direction));
                }

                auto right = light->right;
                auto up = light->up;
                if (!isPBR) {
                    right = glm::vec3(light->position) + light->right;
                    glm::vec4 right4 = renderer.getCurrentViewMatrix() * glm::vec4(right, 1.0);
                    right = glm::vec3(right4) / right4.w;
                    right = right - glm::vec3(lightEyePosition);
                    up = glm::cross(right, direction);
                }
                shader->setUniform3f("lights[" + idx + "].right", glm::normalize(toGlm(right)));
                shader->setUniform3f("lights[" + idx + "].up", glm::normalize(up));
            }
        }
    }
    /*
    void ofGLProgrammableRenderer::setDefaultUniforms() {
        if (!currentShader) return;
        currentShader->setUniform4f(COLOR_UNIFORM, currentStyle.color.r / 255., currentStyle.color.g / 255., currentStyle.color.b / 255., currentStyle.color.a / 255.);
        bool usingTexture = texCoordsEnabled & (currentTextureTarget != OF_NO_TEXTURE);
        currentShader->setUniform1f(USE_TEXTURE_UNIFORM, usingTexture);
        currentShader->setUniform1f(USE_COLORS_UNIFORM, colorsEnabled);
        if (currentMaterial) {
            currentMaterial->updateMaterial(*currentShader, *this);
            currentMaterial->updateLights(*currentShader, *this);
            currentMaterial->updateShadows(*currentShader, *this);
            currentMaterial->updateEnvironmentMaps(*currentShader, *this);
        }
        if (currentShadow) {
            if (currentShadow->isMultiCubeFacePass()) {
                currentShadow->updateDepth(*currentShader, shadowCubeFace, *this);
            }
            else {
                currentShadow->updateDepth(*currentShader, *this);
            }
        }
    }
    */
};
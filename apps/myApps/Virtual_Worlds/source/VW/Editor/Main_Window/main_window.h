#pragma once

#include "ofMain.h"

#include "../Interface/theme.h"
#include <Universal_FW/Interface/IconsFontAwesome.h>

#include <VW_framework/App/global.h> // Must be placed here above all other includes
#include <VW_framework/Scene/scene_entities_manager.h>

#include <ImGuiEx/ImGui_utils.h>

#include "Panels/log_panel.h"
#include "Panels/parameter_panel.h"
#include "Panels/node_editor_panel.h"
#include "Panels/animation_timeline_panel.h"

#include <VW_framework/Scene/vw_scene.h>

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

        // Delete all pointers here ?????
        //delete reference_grid;
    }

    shared_ptr<ofAppBaseWindow> main_window = NULL;

    bool display_grid        = true;
    bool display_info        = true;
    bool display_gimbal      = true;
    bool display_crosshairs  = true;
    //glm::vec4 xhair_color = glm::vec4{ 255.0f,255.0f,255.0f,255.0f };
    ofColor xhair_color = { 255,255,255,255 };

    void setup() {
        cout << "setup here : 000" << std::endl;
        //ofDisableArbTex();

        GLFW_window_ptr = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();

        if (GLFW_window_ptr == NULL) {
            cout << "Appplication main window not defined : Application aborted" << std::endl;
            return;
        }

        ImGui_ui_context.ImGui_init(GLFW_window_ptr);

        log_panel = new log_panel_class();
        if (log_panel == NULL) {
            cout << "CRITCAL ERROR :: No Applicaton Logger Defined : Cannot perform application \n";
            return;
        }
        cout << "setup here 11111: " << std::endl;

        vw_scene = new vw_scene_class;
        scene_manager = &vw_scene->scene_entities_manager;

        parameter_panel.log_panel = log_panel;
        parameter_panel.scene_manager = scene_manager;


        parameter_panel.viewport_properties_widget.display_camera_info = &display_info;// ++++++
        parameter_panel.viewport_properties_widget.display_gimbal      = &display_gimbal;// ++++++
        parameter_panel.viewport_properties_widget.display_crosshairs  = &display_crosshairs;// ++++++

        parameter_panel.viewport_properties_widget.xhair_color_viewport = &xhair_color;// ++++++
        // ++++++++++++++
        // Define a global pointer to the time line tracks widget whch is part of the timeline widget
        // that can be referenced by any object to perform animations with 
        timeline_class* animation_tracks = dynamic_cast<timeline_class*> (&animation_timeline_panel);
        animation_timeline_tracks_widget = &animation_tracks->timeline_tracks_widget;
        // ++++++++++++++

        scene_node_editor = new scene_node_editor_class;
        node_editor_panel.log_panel = log_panel;
        node_editor_panel.vw_scene = vw_scene; // May need to crete this as a new vw_scene_class

        scene_node_editors.push_back(scene_node_editor);

        // ############################# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // default Openframeworks requires at least one light to be defined otherwise app will crash
        // This is fine as need a global light anyway to display image and for shaders to use.
        
        if (!scene_manager->scene_lights.define_global_light()) {
             cout << "setup here : !scene_lights_manager.define_global_light()" << std::endl; // +++++++++++
        } else{
            parameter_panel.global_light_parameters_widget.global_light = scene_manager->scene_lights.scene_global_light;
        }

//cout << "setup here : scene_manager->scene_cameras.cameras size : " << scene_manager->scene_cameras.cameras.size() << std::endl; // +++++++++++

        current_camera = scene_manager->scene_cameras.selected_camera;// to Test camera parameter display panel : delete when done
        current_camera->setNearClip(.001);
        current_camera->setFarClip(10000);
//cout << "setup here : scene_manager->scene_cameras.cameras id : " << current_camera->id << std::endl; // +++++++++++
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
cout << "setup here ZZZZZZ: " << std::endl;

        reference_grid = new reference_grid_class;
        parameter_panel.viewport_properties_widget.reference_grid = reference_grid;
    }

    void update() {

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

        if (current_camera != NULL) {
//cout << "setup here : current_camera != NULL" << std::endl;
            //globalc::set_current_selected_entity_id(current_camera->id);
            //globalc::set_current_selected_entity_type_id(ENTITY_TYPE_CAMERA);

            //current_selected_entity_id = current_camera->id;
            //current_selected_entity_type_id = ENTITY_TYPE_CAMERA;
        }


        parameter_panel.camera_poperties_widget.camera = current_camera;// move to update

        parameter_panel.show();
        node_editor_panel.show();
        animation_timeline_panel.show();


        // Following uncommented when investigating ImGui widget examples
        //ImGui::Begin;
        //bool show_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);
        //ImGui::End;
        // ----------------------------------
 

        ofEnableDepthTest();// +++++++++++++
 
        // !!!!!!!!!!!!!!!! BEGIN to be done in vw_scene.render_scene() !!!!!!!!!!!!!!!!!!!


        scene_lights_manager.scene_lights_render_setup();
        scene_manager->scene_cameras.scene_cameras_render_setup();

        // If Mouse or keyboard inputs are outside the 2D graphics viewport
        // disable all mouse and keyboard events that the scene cameras capture
        // and use to alter or perform tasks in the viewport window.
        if (io.WantCaptureMouse && io.WantCaptureKeyboard){
            scene_manager->scene_cameras.disable_camera_mouse_input();
            scene_manager->scene_cameras.disable_camera_keyboard_input();
        }else{
            scene_manager->scene_cameras.enable_camera_mouse_input();
            scene_manager->scene_cameras.enable_camera_keyboard_input();
        }

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (!ofIsGLProgrammableRenderer()) {
            ofEnableLighting();
        }

        render_scene();


        if (!ofIsGLProgrammableRenderer()) {
            ofDisableLighting();
        }


        display_gimbal_overlay();
        reference_grid->camera_loc = current_camera->getPosition();
        reference_grid->display_grid();

        scene_manager->scene_cameras.scene_cameras_render_exit();
        scene_lights_manager.scene_lights_render_exit();

        ofDisableDepthTest();
        // !!!!!!!!!!!!!!!! END to be done in vw_scene.render_scene() !!!!!!!!!!!!!!!!!!!
       
        display_camera_crosshairs();
        display_info_overlay();

        ImGui_ui_context.ImGui_post_render();
    }

    void render_scene() {

        vw_scene->render_scene();

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

    void display_camera_crosshairs() {
  //cout << "display_viewport_crosshairs 0000"  << std::endl;
        if (display_crosshairs) {// Draw crosshairs at center of viewport
 // cout << "display_viewport_crosshairs 1111"  << std::endl;
            
            ofRectangle view_dim = current_camera->get_camera_viewport_rect();

//cout << "display_viewport_crosshairs 1111"  << view_dim.width << ":" << view_dim.height << std::endl;
            // Center of viewport
            glm::vec2 p1 = { view_dim.width / 2 - 10, view_dim.height / 2 };
            glm::vec2 p2 = { view_dim.width / 2 + 10, view_dim.height / 2 };
            glm::vec2 p3 = { view_dim.width / 2,      view_dim.height / 2 - 10 };
            glm::vec2 p4 = { view_dim.width / 2,      view_dim.height / 2 + 10 };
            
            ofSetColor(xhair_color);
            //ofDrawLine(100.0f,100.0f,100.0f,-90.0f);
            ofDrawLine(p1,p2);
            ofDrawLine(p3,p4);
        }
    }

    void display_info_overlay() {
        if (display_info) {
            glm::vec2 info_location = { 10.0f,50.0f };
            float y_spacing = 20.0f;
            std::string s;

            glm::vec3 pos = current_camera->getPosition();
            glm::vec3 orient = current_camera->getOrientationEulerDeg();
            float round_r = 4.0;

            ofSetColor(xhair_color,20);
            ofDrawRectRounded(info_location,180,150,round_r,round_r,round_r,round_r);

            ofSetColor(xhair_color,255);
            info_location.x += 5.0; info_location.y += 23.0f;

            s = "X Coord : " + std::to_string(pos.x);
            ofDrawBitmapString(s, info_location.x, info_location.y);
            info_location.y += y_spacing;
            s = "Y Coord : " + std::to_string(pos.y);
            ofDrawBitmapString(s, info_location.x, info_location.y);
            info_location.y += y_spacing;
            s = "Z Coord : " + std::to_string(pos.z);
            ofDrawBitmapString(s, info_location.x, info_location.y);
            info_location.y += y_spacing + 5;
            s = "Pitch : " + std::to_string(orient.x);
            ofDrawBitmapString(s, info_location.x, info_location.y);
            info_location.y += y_spacing;
            s = "Yaw   : " + std::to_string(orient.y);
            ofDrawBitmapString(s, info_location.x, info_location.y);
            info_location.y += y_spacing;
            s = "Roll  : " + std::to_string(orient.z);
            ofDrawBitmapString(s, info_location.x, info_location.y);
        }
    }


    // This is a workaround to display the cameras global orientation
    // and needs to be ofset from the center of the screen in a fixed
    // location. Needs an axis display that is draw to the viewport
    // as an overlay after all 3d rendering complete. This solution is
    // not ideal
    void display_gimbal_overlay() {
        if (display_gimbal) {
            //ofDrawAxis(100.0f);
            glm::vec3 cp      = current_camera->getPosition();
            glm::vec3 cam_dir = current_camera->getLookAtDir();
            glm::vec3 offset  = glm::vec3{0.0,0.0,cam_dir.z - 1.0f};

            float axis_size  = 0.5f;
            float arrow_size = 0.05f;

            glm::vec3 p0 = cp + cam_dir*5.0f;
            glm::vec3 px = glm::vec3{ p0.x+axis_size,p0.y,p0.z };
            glm::vec3 py = glm::vec3{ p0.x,p0.y+axis_size,p0.z };
            glm::vec3 pz = glm::vec3{ p0.x,p0.y,p0.z+axis_size };

            shared_ptr<ofBaseRenderer> renderer = ofGetCurrentRenderer();
            ofColor prevColor = renderer->getStyle().color;

            ofSetColor(255,0,0,255);
            ofDrawArrow(p0, px,arrow_size);
            ofSetColor(0,255,0,255);
            ofDrawArrow(p0, py,arrow_size);
            ofSetColor(0,0,255,255);
            ofDrawArrow(p0, pz,arrow_size);

            renderer->setColor(prevColor);
        }
    }

    void display_grid_planes() {
        float stepSize = 10.0f;
        size_t numberOfSteps = 20;
        bool labels = false;
        bool x = true; bool y = true; bool z= true;

        shared_ptr<ofBaseRenderer> renderer = ofGetCurrentRenderer();

	    ofColor prevColor = renderer->getStyle().color;

	    if (y) {
		    renderer->setColor(0,255,0,255);
		    renderer->get3dGraphics().drawGridPlane(stepSize, numberOfSteps, labels);
	    }
	    if (z) {
		    renderer->setColor(0,0,255,255);
		    glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0,0,-1));
		    renderer->pushMatrix();
		    renderer->multMatrix(m);
		    renderer->get3dGraphics().drawGridPlane(stepSize, numberOfSteps, labels);
		    renderer->popMatrix();
	    }
	    if (x) {
		    renderer->setColor(255, 0, 0, 255);
		    glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0,1,0));
		    renderer->pushMatrix();
		    renderer->multMatrix(m);
		    renderer->get3dGraphics().drawGridPlane(stepSize, numberOfSteps, labels);
		    renderer->popMatrix();
	    }

	    if (labels) {
		    ofDrawBitmapMode mode = renderer->getStyle().drawBitmapMode;
		    renderer->setColor(255, 255, 255);
		    float labelPos = stepSize * (numberOfSteps + 0.5);
		    renderer->setBitmapTextMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		    renderer->drawString("x", labelPos, 0, 0);
		    renderer->drawString("y", 0, labelPos, 0);
		    renderer->drawString("z", 0, 0, labelPos);
		    renderer->setBitmapTextMode(mode);
	    }
	    renderer->setColor(prevColor);
    }

private:

    vw_scene_class  *vw_scene;
    vw_camera_class *current_camera = NULL;
    
    GLFWwindow                 *GLFW_window_ptr; // glfw 

    ImGui_ui_context_class ImGui_ui_context;


    scene_entities_manager_class *scene_manager = NULL;

    scene_lights_manager_class scene_lights_manager;

    // UI components
    log_panel_class               *log_panel;
    parameter_panel_class          parameter_panel;
    node_editor_panel_class        node_editor_panel;
    animation_timeline_panel_class animation_timeline_panel;
    scene_node_editor_class       *scene_node_editor;


    // Overlays
    // Because ofFrameworks use shared pointers or due to a contructor  
    // in one of it classes, if this is not a pointer the app crashes 
    // upon openingcause. It seems that use of shared ponters cause more
    // problems than they solve
    reference_grid_class *reference_grid = NULL;
};
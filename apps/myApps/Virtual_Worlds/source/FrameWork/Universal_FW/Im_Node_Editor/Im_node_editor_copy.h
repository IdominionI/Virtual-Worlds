#pragma once

#include <vector>

#include <OpenFrameWorks/Source/ofMain.h>

#include <ImGuiEx/ImGui_utils.h>

#include "ImNe_window.h"
#include "ImNe_app.h"


//#include "../../Node_editor/ImNodes/Kernal/node_editor.h"
#include <Universal_FW/Im_Node_Editor/ImNodes/Kernal/node_editor.h>

class ImNe_node_editor_class : public ofBaseApp , public node_editor_basis_class{
public:
	ImNe_node_editor_class(){}

	~ImNe_node_editor_class() {}

	ImNe_Window_class *root_ImNe_window             = NULL;
	ImNe_Window_class *current_selected_ImNe_Window = NULL;


public:
	void setup(){
		mainGLFW_ptr = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();

		ImGui_ui_context.ImGui_init(mainGLFW_ptr);

        ImNodes::CreateContext();
/*
		//point cloud test
		// ----------------------------------
		ofSetVerticalSync(true);

		// load an image from disk
		img.load("linzer.png");

		// we're going to load a ton of points into an ofMesh
		mesh.setMode(OF_PRIMITIVE_POINTS);

		// loop through the image in the x and y axes
		int skip = 4; // load a subset of the points
		for (int y = 0; y < img.getHeight(); y += skip) {
			for (int x = 0; x < img.getWidth(); x += skip) {
				ofColor cur = img.getColor(x, y);
				if (cur.a > 0) {
					// the alpha value encodes depth, let's remap it to a good depth range
					float z = ofMap(cur.a, 0, 255, -300, 300);
					cur.a = 255;
					mesh.addColor(cur);
					glm::vec3 pos(x, y, z);
					mesh.addVertex(pos);
				}
			}
		}

		ofEnableDepthTest();
		glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
		glPointSize(3); // make the points bigger

		// ----------------------------------
		// Create a second application window testing
		//ImNe_Window_class ImNe_Window;

		//ImNe_Window_class *ImNe_Window = new ImNe_Window_class;
		ImNe_Window = new ImNe_Window_class;
		if (ImNe_Window == NULL) {
			std::cout << "ImNe_window_pointer == NULL" << std::endl;
			return;
		}
		ImNe_Window->create_new_ImNE_window(main_window);

		std::cout << "ImNe_window_pointer != NULL" << std::endl;
		shared_ptr<ImNe_App_class> guiApp(new ImNe_App_class);
		ImNe_App = guiApp;

		ImNe_App->ImNe_window = ImNe_Window->app_base_window;

		ofRunApp(ImNe_Window->app_base_window, ImNe_App);

		// 2222222222222222222222222222222222222222

		ImNe_Window2 = new ImNe_Window_class;
		if (ImNe_Window2 == NULL) {
			std::cout << "ImNe_window_pointer2 == NULL" << std::endl;
			return;
		}
		ImNe_Window2->create_new_ImNE_window(ImNe_Window->app_base_window, ImNe_Window);

		std::cout << "ImNe_window_pointer2 != NULL" << std::endl;
		shared_ptr<ImNe_App_class> guiApp2(new ImNe_App_class);
		ImNe_App = guiApp2;

		ImNe_App->ImNe_window = ImNe_Window2->app_base_window;
		ImNe_Window2->ImNe_App = ImNe_App;

		ofRunApp(ImNe_Window2->app_base_window, ImNe_App);

		// 3333333333333333333333333333333333333333
		ImNe_Window3 = new ImNe_Window_class;
		if (ImNe_Window3 == NULL) {
			std::cout << "ImNe_window_pointer3 == NULL" << std::endl;
			return;
		}
		ImNe_Window3->create_new_ImNE_window(ImNe_Window->app_base_window, ImNe_Window2);

		std::cout << "ImNe_window_pointer3 != NULL" << std::endl;
		shared_ptr<ImNe_App_class> guiApp3(new ImNe_App_class);
		ImNe_App = guiApp3;

		ImNe_App->ImNe_window = ImNe_Window3->app_base_window;
		ImNe_Window3->ImNe_App = ImNe_App;

		ofRunApp(ImNe_Window3->app_base_window, ImNe_App);


		// 3111111111111111111111111111111
		ImNe_Window31 = new ImNe_Window_class;
		if (ImNe_Window31 == NULL) {
			std::cout << "ImNe_window_pointer31 == NULL" << std::endl;
			return;
		}
		ImNe_Window31->create_new_ImNE_window(ImNe_Window->app_base_window, ImNe_Window3);

		std::cout << "ImNe_window_pointer31 != NULL" << std::endl;
		shared_ptr<ImNe_App_class> guiApp31(new ImNe_App_class);
		ImNe_App = guiApp31;

		ImNe_App->ImNe_window = ImNe_Window31->app_base_window;
		ImNe_Window31->ImNe_App = ImNe_App;

		ofRunApp(ImNe_Window31->app_base_window, ImNe_App);
*/	
	}


	void update(){}

	void draw(){
		//point cloud test
		// ----------------------------------
		// !!!! ofBackgroundGradient does not work here for multiple windows!!!!!!
		// !!!! wasted whole afternoon thinking problem originated !!!!
		// !!!! from the opengl window surface not being assigned correctly !!!
		//ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);
		//ofBackgroundGradient(ofColor(64), ofColor(0));
/*
		ofSetBackgroundColor(100, 50, 150);

		// even points can overlap with each other, let's avoid that
		cam.begin();
		ofScale(2, -2, 2); // flip the y axis and zoom in a bit
		ofRotateYDeg(90);
		ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
		mesh.draw();

		//glPointSize(10);
		//glLineWidth(2.5);
		//glColor3f(1.0, 0.0, 0.0);
		//glBegin(GL_LINES);
		//glVertex3f(0.0, 0.0, 0.0);
		//glVertex3f(5.0, 5.0, 0.0);
		//glEnd();

		cam.end();
*/

		glfwMakeContextCurrent(mainGLFW_ptr);
		ImGui_ui_context.ImGui_pre_render();

		// Following uncommented when investigating ImGui widget examples
		ImGui::Begin;
		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::End;


		// The node editor window
		auto flags = ImGuiWindowFlags_MenuBar;
        nodes_context = ImNodes::GetCurrentContext();

		ImGui::Begin("Scene Node Editor", NULL, flags);
		//scene_node_editor.log_panel = log_panel;
		//scene_node_editor.scene_manager = scene_manager;
		//scene_node_editor.nodes_context = nodes_context;
		//scene_node_editor.display();
		display();
		//selected_node = scene_node_editor.selected_node;
		ImGui::End();

		// ----------------------------------

		ImGui_ui_context.ImGui_post_render();
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++
    void display() {
//printf("scene_node_editor_class:: display : TEST : %i : %i \n", global_sclass::get_current_selected_entity_id(), global_sclass::get_current_selected_entity_type_id());

        if (nodes_context == NULL) {
            std::cout << "node editor nodes context == NULL" << std::endl;
            //if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Nodes context not defined\n");
            return;
        }

        //if (scene_manager == NULL) {
        //    if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: No scene manager defined to create node in node editor\n");
        //    return;
        //}
//std::cout << "display() 0001" << std::endl;
        ImNodes::BeginNodeEditor();
//std::cout << "display() 0002" << std::endl;
        // Right-click on blank space
        // Create a menu to perform actions if nothing is selected and right mouse pressed
        if (no_node_element_is_hovered_over && selected_node == NULL && selected_link.link_id == INVALID_NODE_ID) { // This menu is for when no node or link is hovered over //*****

            //if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::BeginPopupContextWindow("menu", ImGuiPopupFlags_MouseButtonRight)) {
//std::cout << "display() 0003" << std::endl;
                ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();
                menu_selection = true;
//std::cout << "display() 0003A" << std::endl;
                if (ImGui::BeginMenu("HCP Voxel ...", true)) {
                    {
//std::cout << "display() 0003AA" << std::endl;
                        if (ImGui::MenuItem("Create node for currently selected voxel")) { std::cout << "Create node for currently selected voxel" << std::endl; }
                           // create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_ENTITY, FUNCTION_CATEGORY_GENERATOR, click_pos);
                        if (ImGui::MenuItem("Translation")) { std::cout << "Create node for currently selected voxel Translation" << std::endl; }
                           // create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_TRANSLATION, FUNCTION_CATEGORY_OPERATOR, click_pos);
                        if (ImGui::MenuItem("Boolean")) { std::cout << "Create node for currently selected voxel Boolean" << std::endl; }
                           // create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_INTERACTION, FUNCTION_CATEGORY_MODIFIER, click_pos);

                        ImGui::EndMenu();
                    }
                };
//std::cout << "display() 0003B" << std::endl;
                if (ImGui::BeginMenu("Hex Surface ...", true)) {
                    {
//std::cout << "display() 0003BB" << std::endl;
                        if (ImGui::MenuItem("Create node for currently selected hex_surface")) { std::cout << "Create node for currently selected hex" << std::endl; }
                            //create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_ENTITY, FUNCTION_CATEGORY_GENERATOR, click_pos);
                        if (ImGui::MenuItem("Translation")) { std::cout << "Create node for currently selected hex Translation" << std::endl; }
                            //create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_TRANSLATION, FUNCTION_CATEGORY_OPERATOR, click_pos);
                        if (ImGui::MenuItem("Boolean")) { std::cout << "Create node for currently selected hex Translation" << std::endl; }
                            //create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_INTERACTION, FUNCTION_CATEGORY_MODIFIER, click_pos);
                        ImGui::EndMenu();
                    }
                };
//std::cout << "display() 0003C" << std::endl;
                ImGui::EndPopup();
                nodes_context->LeftMouseClicked = false;
                menu_selection = false;
//std::cout << "display() 0003D" << std::endl;
            }
        }
//std::cout << "display() 0004" << std::endl;
        // Right click on a selected node to display a menu selection of actions to perform on that node
        if (selected_node != NULL) {

            //if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::BeginPopupContextWindow(0, 1)) {
                //ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();
                menu_selection = true;

                if (ImGui::BeginMenu("Node ...")) {
                    {
                        if (ImGui::MenuItem("Delete node")) {
                            if (current_selected_node_id > -1) graph.delete_node(current_selected_node_id);
                        }
                        if (ImGui::MenuItem("delete node links")) {
                            if (current_selected_node_id > -1) graph.delete_node_links(current_selected_node_id);
                        }

                        ImGui::EndMenu();
                    }
                };

                // If selected node is an entity node, then these options are available to be performed
                if (selected_node->entity_id > -1) {
                    if (ImGui::BeginMenu("Entity ...")) {
                        {
                            if (ImGui::MenuItem("save node tree")) {
                                printf("scene_node_editor_class : display : save entity selected\n");
                                // Not yet implemented    
                            }

                            if (ImGui::MenuItem("select node tree")) {
                                printf("scene_node_editor_class : display : select node tree\n");
                                // Not yet implemented    
                            }

                            if (ImGui::MenuItem("delete node tree")) {
                                //node_tree.delete_node_tree(&graph, selected_node);
                            }

                            ImGui::EndMenu();
                        }
                    };
                }

                ImGui::EndPopup();
                nodes_context->LeftMouseClicked = false;
                menu_selection = false;
            }
        }
//std::cout << "display() 0005" << std::endl;
        if (selected_link.link_id != INVALID_NODE_ID) {

            //if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::BeginPopupContextWindow(0, 1)) {
                menu_selection = true;

                if (ImGui::BeginMenu("Link ...")) {
                    {
                        if (ImGui::MenuItem("Delete link")) {
                            if (selected_link.link_id > -1) graph.delete_link(selected_link.link_id);
                        }
                        ImGui::EndMenu();
                    }
                };

                ImGui::EndPopup();
                nodes_context->LeftMouseClicked = false;
                menu_selection = false;
            }
        }
//std::cout << "display() 0006" << std::endl;
        //************************

//printf("scene_node_editor_class::display:: 00000\n");
        // Draw nodes
        for (node_basis_class* node : graph.nodes.nodes) {
//printf("scene_node_editor_class::draw:: 000 %i \n", node->node_id);
            node->draw_node(graph.pins);
        }

//std::cout << "display() 0007" << std::endl;

        // Display existing link lines
        for (link_struct_type link : graph.links.links)
        {
            // If edge doesn't start at value, then it's an internal edge, i.e.
            // an edge which links a node's operation to its input. We don't
            // want to render node internals with visible links.
            //if (graph_.node(edge.from).node_type.function != VALUE)
            //    continue;

            ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(0, 139, 139, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkHovered, IM_COL32(0, 175, 175, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkSelected, IM_COL32(100, 255, 255, 255));
            ImNodes::Link(link.link_id, link.from, link.to);
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }

//std::cout << "display() 0008" << std::endl;

        ImNodes::EndNodeEditor();

        // ---------------------------------------------------------------------------------
        // These are driven by Imnodes, so we place the code after EndNodeEditor().
//std::cout << "display() 0009" << std::endl;
        // Handle links
        handle_new_links();
        handle_deleted_links();

//std::cout << "display() 0010" << std::endl;
        no_node_element_is_hovered_over = no_element_hovered_over();

        if (ImNodes::IsNodeHovered(&hover_node_id))
        {
            //node_hovered = node_id;
//printf("node hovered over id : %i\n", hover_node_id);
        }

        //****
        //if (!inside_editor_canvas(nodes_context->MousePos)) {
        //    globalc::set_current_selected_node_id(INVALID_NODE_ID);//*****
       // }
        //****
//std::cout << "display() 0011" << std::endl;
        //if (nodes_context != NULL)
        //{
            if (nodes_context->LeftMouseClicked && inside_editor_canvas(nodes_context->MousePos) && !menu_selection) {
//printf("Left mouse clicked inside canvas 00 %f : %f \n", nodes_context->MousePos.x, nodes_context->MousePos.y);
//printf("Left mouse clicked 11 %f : %f \n", nodes_context->CanvasOriginScreenSpace.x, nodes_context->CanvasOriginScreenSpace.y);
//printf("Left mouse clicked 22 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.x, nodes_context->CanvasRectScreenSpace.Max.x);
//printf("Left mouse clicked 33 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.y, nodes_context->CanvasRectScreenSpace.Max.y);
                //if (globalc::get_current_selected_node_id() == -1) {
                if (current_selected_node_id == -1) {
                    //globalc::set_current_selected_entity_type_id(INVALID_NODE_ID);
                    //globalc::set_current_selected_entity_id(INVALID_NODE_ID);
                    //globalc::set_outliner_selection(false);
                }
            }
        //}
//std::cout << "display() 0012" << std::endl;
        num_selected_nodes = ImNodes::NumSelectedNodes();
        //if (num_selected_nodes > 0  && !globalc::get_outliner_selection())
        if (num_selected_nodes > 0 )
        {
            static std::vector<int> selected_nodes;
            selected_nodes.resize(static_cast<size_t>(num_selected_nodes));
            ImNodes::GetSelectedNodes(selected_nodes.data());
            for (const int node_id : selected_nodes)
            {
                //if (node_id < 0) ImNodes::ClearNodeSelection(); break; // use if selected nodes not cleared and have invalid node id
                current_selected_node_id = selected_nodes[selected_nodes.size() - 1];
//printf("selected node id : %i : %i\n", node_id, current_selected_node_id);
                //if (log_panel != NULL) log_panel->application_log.AddLog("selected node id : %i: %i\n", node_id, current_selected_node_id);

            }
        }
        else {
            current_selected_node_id = -1;
            selected_node = NULL;
//printf("current node id : %i \n", current_selected_node_id);
        }
//std::cout << "display() 0013" << std::endl;
        if (current_selected_node_id > -1) {
//if (globalc::get_current_selected_node_id() > -1) {
//printf("Found current node of id : %i  \n", current_selected_node_id);
            node_basis_class* node;
            node = graph.nodes.get_node(current_selected_node_id);

            if (node != NULL) {
//printf("Found current node of id : %i : %i : %i  \n", node->node_id, node->ui_node_type.category, node->entity_id);
                //globalc::set_current_selected_entity_type_id(node->ui_node_type.category);
               // globalc::set_current_selected_entity_id(node->entity_id);

                selected_node = node;
            }
        }
//std::cout << "display() 0014" << std::endl;
        //*****
        num_selected_links = ImNodes::NumSelectedLinks();
        if (num_selected_links > 0)
        {
            //printf("num_selected_links : %i \n", num_selected_links);
            static std::vector<int> selected_links;
            selected_links.resize(static_cast<size_t>(num_selected_links));
            ImNodes::GetSelectedLinks(selected_links.data());
            for (const int link_id : selected_links)
            {
                //if (node_id < 0) ImNodes::ClearNodeSelection(); break; // use if selected nodes not cleared and have invalid node id
                current_selected_link_id = selected_links[selected_links.size() - 1];
                //printf("selected link id : %i : %i\n", link_id, current_selected_link_id);
                //if (log_panel != NULL) log_panel->application_log.AddLog("selected node id : %i: %i\n", node_id, current_selected_node_id);

            }
        }
        else {
            current_selected_link_id = -1;
            selected_link.link_id = INVALID_NODE_ID;
            //printf("current node id : %i \n", current_selected_node_id);
        }
//std::cout << "display() 0015" << std::endl;
        if (current_selected_link_id > -1) {
            //if (globalc::get_current_selected_node_id() > -1) {
            //printf("current_selected_link_id : %i  \n", current_selected_link_id);
            link_struct_type link;
            link = graph.links.find_link(current_selected_link_id);

            if (link.link_id != INVALID_NODE_ID) {
                //printf("Found current link of id : %i  \n", link.link_id);
                selected_link = link;
            }
        }
//std::cout << "display() 0016" << std::endl;
        //*****
    }


    //----------------------------------------------

    void create_node(int category, int context, int function, int function_category, ImVec2 click_pos) {

        //select existing voxel entity to reference and get entity id
//printf("scene_node_editor_class::create_entity_node 0000\n");
        node_type_struct editor_node_definition;
        editor_node_definition.category = category;
        editor_node_definition.context = context;
        editor_node_definition.function = function;
        editor_node_definition.function_category = function_category;

//printf("scene_node_editor_class::create_entity_node 1111\n");
        //node_editor_category_manager->perform_editor_node_creation(editor_node_definition, &graph, scene_manager, log_panel, click_pos);
//printf("scene_node_editor_class::create_entity_node 22222\n");
    }

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    void handle_new_links() {
        int start_attr, end_attr;
        // This does not create a link, only returns the pin ids of the the two elected pin links
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            //printf("handle_new_links 0000 :: IsLinkCreated");


            pin_struct_type start_pin = graph.pins.find_pin(start_attr);
            pin_struct_type end_pin = graph.pins.find_pin(end_attr);

            //printf("handle_new_links 0000 %i : %i\n", start_pin.pin_id, end_pin.pin_id);

            if (start_pin.pin_value_type.category == end_pin.pin_value_type.category &&
                start_pin.pin_value_type.context == end_pin.pin_value_type.context &&
                start_pin.pin_value_type.function_category == end_pin.pin_value_type.function_category &&
                start_pin.pin_value_type.value_type == end_pin.pin_value_type.value_type)
            {
                // Make sure do not create a link if have pin already assigned to a link if have a single link only permitted
                // Future enhancement do auto delete existing link ?????
                if ((start_pin.pin_type == pin_type_enum::single_output && !graph.links.start_pin_has_link(start_pin.pin_id)) ||
                    start_pin.pin_type == pin_type_enum::multi_output)
                {
                    if ((end_pin.pin_type == pin_type_enum::single_input && !graph.links.end_pin_has_link(end_pin.pin_id)) ||
                        end_pin.pin_type == pin_type_enum::multi_input)
                    {
                        graph.links.insert(start_pin.pin_id, end_pin.pin_id);
                    }
                }



                // Ensure the edge is always directed from the value to
                // whatever produces the value
               // if (start_pin.pin_type != VALUE)
               // {
               //     std::swap(start_attr, end_attr);
               // }
               // graph_.insert_edge(start_attr, end_attr);

               // graph.links.insert(start_pin.pin_id,end_pin.pin_id);
            }

        }

    }


    void handle_deleted_links() {
        // Delete link edges that define existing link input-output pairs
        {
            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                //graph_.erase_edge(link_id);
                graph.links.delete_link(link_id);
            }
        }

        // Delete link definitions
        {
            const int num_selected_links = ImNodes::NumSelectedLinks();
            if (num_selected_links > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_links;
                selected_links.resize(static_cast<size_t>(num_selected_links));
                ImNodes::GetSelectedLinks(selected_links.data());

                for (int link_id : selected_links) {
                    graph.links.delete_link(link_id);
                }

                ImNodes::ClearLinkSelection();// or will still have selected links 
                //still defined that do not exist and will cause a crash
            }
        }

        // Delete node definitions
        {
            const int num_selected_links = ImNodes::NumSelectedNodes();
            if (num_selected_links > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<size_t>(num_selected_links));
                ImNodes::GetSelectedNodes(selected_nodes.data());
                for (int node_id : selected_nodes) {
                    //printf("scene_node_editor_class::handle_deleted_links:: 0000 %i \n", node_id);                   
                    graph.delete_node(node_id);
                }

                ImNodes::ClearNodeSelection();// or will still have selected nodes 
                // still defined that do not exist and will cause a crash 
            }

        }
    }

    bool menu_selection = false;
	
    node_basis_class    *selected_node = NULL;
    link_struct_type     selected_link;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++



	void keyPressed(int key){
		//std::cout << "keyPressed 0001" << std::endl;
		//if (ImNe_Window != NULL) ImNe_Window->delete_ImNE_window_children();
		//std::cout << "keyPressed 0002" << std::endl;
	}

	void keyReleased(int key){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
	void mouseEntered(int x, int y){}
	void mouseExited(int x, int y){}
	void windowResized(int w, int h){}
	void dragEvent(ofDragInfo dragInfo){}
	void gotMessage(ofMessage msg){}

	// point cloud example test
	ofEasyCam cam;
	ofMesh mesh;
	ofImage img;

	// for multiple appd
	shared_ptr<ofAppBaseWindow> main_window = NULL;
	//ImNe_Window_class ImNe_Window;
	GLFWwindow* ImNe_window_pointer;
	shared_ptr <ImNe_App_class>    ImNe_App;

	// ImGui
	GLFWwindow* mainGLFW_ptr;
	ImGui_ui_context_class ImGui_ui_context;

	// test ImNe_Window_class
	ImNe_Window_class* ImNe_Window;
	ImNe_Window_class* ImNe_Window2, * ImNe_Window3, * ImNe_Window31;

private:

};
#pragma once

#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

#include <app/ofAppBaseWindow.h>
#include <app/ofAppGLFWWindow.h>
#include <app/ofAppRunner.h>

#include "ImNe_app.h"

class ImNe_Window_class {
public:

	ImNe_Window_class() {
	}

	~ImNe_Window_class() {
		//close_ImNE_window();
		//delete_ImNE_window_children();
		//remove_window_reference_from_parent();
	}


	int                         window_id       = -1;
	std::string                 window_name     = "";
	GLFWwindow                 *window_pointer  = NULL;
	shared_ptr<ofAppBaseWindow> app_base_window = NULL;
	shared_ptr <ImNe_App_class> ImNe_App        = NULL;

	ImNe_Window_class                *window_parent = NULL;
	std::vector<ImNe_Window_class *>  window_children;

	ofGLFWWindowSettings settings;

	// **** Change and add as needed here ***
	void define_default_settings() {
		settings.setSize(600, 600);
		settings.setPosition(glm::vec2(300, 500));
		settings.resizable  = true;
		settings.windowMode = ofWindowMode::OF_WINDOW;
	}

	// +++++++++++ Creation +++++++++++
	GLFWwindow *create_new_ImNE_window(shared_ptr<ofAppBaseWindow> main_window,ImNe_Window_class *parent = NULL) {
		if (parent != NULL) {
			settings = parent->settings;
			settings.setPosition(parent->settings.getPosition() + glm::vec2(100, 100));
		} else{
			define_default_settings();
		}
		settings.shareContextWith = main_window;

		std::shared_ptr<ofAppBaseWindow> new_Window = ofCreateWindow(settings);

		if (new_Window == NULL) return NULL;

		window_pointer = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();
		//window_pointer  = dynamic_cast<GLFWwindow*>(new_Window.get());

		if (window_pointer == NULL) return NULL;

		new_Window->setVerticalSync(false);// Not sure what this is for or if neccessary, but is in multiple windows on app example

		window_parent = parent;
		if(parent!= NULL) parent->window_children.push_back(this);
		app_base_window = new_Window;

		return window_pointer;
	}

	// Not sure need this one !!!!
	ImNe_Window_class *create_ImNE_window(shared_ptr<ofAppBaseWindow> main_window,ImNe_Window_class *parent = NULL) {
		if (create_new_ImNE_window(main_window, parent) == NULL) return NULL;

		return this;
	}

	// -------------- Deletion ------------------
	void close_ImNE_window() {
		if (app_base_window == NULL) {
std::cout << "app_base_window == NULL " << std::endl;
			return;
		}
std::cout << "app_base_window == !NULL " << std::endl;

		std::shared_ptr<ofMainLoop> main_loop = ofGetMainLoop();

		main_loop->delete_window(app_base_window);

		app_base_window = NULL;
		window_pointer  = NULL;
		ImNe_App        = NULL;
		// !!!!!!! Will need function to set all child app_base_window values to NULL ??????
	}

	void remove_window_reference_from_parent() {
		if (window_parent != NULL) {
			int index = window_parent->window_parent->get_child_index(this);

			if (index > -1)
				window_parent->window_parent->window_children.erase(window_parent->window_parent->window_children.begin() + index);
		}
	}

	//void delete_ImNE_window(ImNe_Window_class *ImNe_Window) {
	//	if (ImNe_Window == NULL) return;
	//	ImNe_Window->delete_ImNE_window_children();
	//	ImNe_Window->remove_window_reference_from_parent();
	//	delete ImNe_Window;
	//}

	// Postorder recursive routine to delete all ImNE child window
	// Seems to work
	void delete_ImNE_window_children() {
//std::cout << "delete_ImNE_window_children 001" << std::endl;
		for(ImNe_Window_class *child_window : window_children) {
//std::cout << "delete_ImNE_window_children 002" << std::endl;
			if(child_window != NULL){
//std::cout << "delete_ImNE_window_children 003" << std::endl;
				child_window->delete_ImNE_window_children();
			}
//std::cout << "delete_ImNE_window_children 004" << std::endl;		
			child_window->close_ImNE_window();
			delete child_window;
		}

//std::cout << "delete_ImNE_window_children 005 :: " << window_children.size() << std::endl;
		window_children.clear();
//std::cout << "delete_ImNE_window_children 006 :: " << window_children.size() << std::endl;
	}

	void delete_ImNE_window_child(ImNe_Window_class *ImNe_Window_child =  NULL) {
		if (ImNe_Window_child == NULL) return;

		ImNe_Window_child->delete_ImNE_window_children();
		
		if (ImNe_Window_child->window_parent != NULL) {
			int index = ImNe_Window_child->window_parent->get_child_index(ImNe_Window_child);

			if (index > -1)
				ImNe_Window_child->window_parent->window_children.erase(ImNe_Window_child->window_parent->window_children.begin() + index);
		}

		delete ImNe_Window_child;
	}

	// *************** Find functions ****************

	int get_child_index(ImNe_Window_class *ImNe_Window_child) {
		for (size_t i = 0; i < window_children.size();i++){
			if (window_children[i] == ImNe_Window_child) return i;
		}
		return -1;
	}

	int get_child_index(int child_window_id) {
		for (size_t i = 0; i < window_children.size(); i++) {
			if (window_children[i]->window_id == child_window_id) return i;
		}
		return -1;
	}

	int get_child_index(std::string child_window_name) {
		for (size_t i = 0; i < window_children.size(); i++) {
			if (window_children[i]->window_name == child_window_name) return i;
		}
		return -1;
	}
	
	// Postorder recursive routine to find ImNE child window of window_id
	// Needs testing

	ImNe_Window_class *Find_ImNe_Window_child_of_id(int window_id,bool &child_not_found) {
		for (ImNe_Window_class *child_window : window_children) {
			if (child_window != NULL && child_not_found)
				if (child_window->window_id == window_id) {
					child_not_found = false;
					return child_window;
				}

			ImNe_Window_class *cw = child_window->Find_ImNe_Window_child_of_id(window_id, child_not_found);
			if (cw != NULL)
				return cw;
		}

		return NULL;
	}

	// Postorder recursive routine to find ImNE child window of name
	// Needs testing

	ImNe_Window_class *Find_ImNe_Window_child_of_id(std::string window_name, bool &child_not_found) {
		for (ImNe_Window_class* child_window : window_children) {
			if (child_window != NULL && child_not_found)
				if (child_window->window_name == window_name) {
					child_not_found = false;
					//child_found_ptr = child_window;
					return child_window;
				}

			ImNe_Window_class *cw = child_window->Find_ImNe_Window_child_of_id(window_name, child_not_found);
			if (cw != NULL)
				return cw;
		}

		return NULL;
	}



private:


};

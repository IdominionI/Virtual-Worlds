#pragma once

#include <vector>
#include <string>
//#include <utility> // Used to access std::pair : remove if not needed
#include <iostream>// for testing only. comment out when not in use or delete
#include <fstream>

#include <imgui.h>
//#include <imgui_internal.h>// +++++

#include <glm/glm.hpp>

#include "node_parameters.h"
#include "node_editor_parameters.h"
#include <Universal_FW/Kernal/id_key_manager.h>


#include "links.h"

// function category
#define GENERAL    -2
#define EVALUATOR  0
#define MODIFIER   1
#define GENERATOR  2
#define OPERATOR   3

struct node_type_struct {
    int category          = INVALID_ID;
    //int context           = INVALID_ID; // Not sure these will be used
    //int function_category = INVALID_ID; // Not sure these will be used
    //int function          = INVALID_ID; // Not sure these will be used
    int node_data_context = INVALID_ID; // Not sure these will be used
    int node_type         = INVALID_ID;
};

// This structure created for possible future feature for user to be able to 
// change node appearance as a part to customise application
struct node_draw_data_struct_type {
    ImColor node_header_color          = ImColor(0, 139, 139, 255);
    ImColor node_header_hovered_color  = ImColor(0, 175, 175, 255);
    ImColor node_header_selected_color = ImColor(0, 255, 255, 255);
    ImU32   node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS
    
    ImColor node_body_color            = ImColor(75, 75, 75, 255);
    ImColor node_body_hovered_color    = ImColor(100, 100, 100, 255);
    ImColor node_body_selected_color   = ImColor(150, 150, 150, 255);
    ImColor node_outline_color         = ImColor(175, 175,175, 255);
};

#define MIN_NODE_WIDTH  30  
#define MIN_NODE_HEIGHT 20

class graph_class; // Forward declaration
class node_basis_class {
public:
    //explicit node_basis_class(graph_class *graph_) : graph(graph_) {} This creates a compile error

    bool visible   = true;
    //bool evaluated = false;// Flag to give to give indication that the node evaluation has occured or not

    graph_class *graph = NULL;

    node_type_struct ui_node_type;

    // The identifying id of the ui node. For add, multiply, sine, and time
    // this is the "operation" node id. The additional input nodes are
    // stored in the structs.
    node_id_type node_id       = INVALID_ID;
    node_id_type node_group_id = INVALID_ID;

    // These are for the editor to use to display node entity parameter UI
    node_id_type node_entity_id          = INVALID_ID;
    node_id_type node_entity_category_id = INVALID_ID;
    node_id_type node_entity_type_id     = INVALID_ID;

    virtual void* get_node_data();// { return nullptr; }// Node data to be accessed by other nodes// +++++++++++++++++++

    std::vector<int> inputs;
    std::vector<int> outputs;

    std::string label = "node";

    //ImRect node_box      ;
    ImVec2 node_dimensions      = { 100.0f,5.0f };
    ImVec2 node_draw_dimensions = { 100.0f,5.0f };
    ImVec2 node_location        = { 0.0f,0.0f };


    node_draw_data_struct_type node_draw_data;

    void change_node_draw_data(node_draw_data_struct_type _node_draw_data) {
        node_draw_data = _node_draw_data;
    }

    // ****** VW NEXT VERSION ******
    ImVec2 node_graph_coordinate;

    bool has_an_input_link() {
        if (inputs.size() == 0) 
            return false;
        else
            return true;
    }

    bool has_an_output_link() {
        if (outputs.size() == 0)
            return false;
        else
            return true;
    }

    bool has_input_link(int input_link_pin_id) {
        for (int pin_id : inputs) {
            if (pin_id == input_link_pin_id) {
                return true;
            }
        }

        return false;
    }

    bool has_output_link(int output_link_pin_id) {
        for (int pin_id : outputs) {
            if (pin_id == output_link_pin_id) {
                return true;
            }
        }

        return false;
    }

    //UI_Node_struct_type ui_node; // This does not work as the way ImNodes is implemented
                                   // it seems that creating a vector of classes to store
                                   // the node ui_node data crashes the program.

    virtual bool define_node(ImVec2 click_pos, node_id_type  entity_id_) { return false; }
    virtual bool define_import_node(ImVec2 click_pos, node_id_type  entity_id_,void* node_misc_data = NULL) { return false; }
    virtual bool define_inputs(pins_class  &pins) { return false; }
    virtual bool define_outputs(pins_class &pins) { return false; }

    virtual void delete_node_entity() {};// since destructor creates compilation and other problems, this will need to be included

    // !!!!!!!!!!!!!!!! NEED A FUNCTION OR LINK TO A FUNCTION HERE TO DISPLAY AND EDIT NODE PARAMETER VALUES HERE !!!!!!!!!!!!!!!!!!!!
    virtual bool define_ui() { return false; }
    virtual void display_ui(node_id_type current_selected_object_id = -1) {}
    virtual void editor_menu_options() {}
    virtual void evaluate() {}// funtion that processes the inputs to perform an action or to create an ouput value(s) to be passed on to the ouput 
    virtual void evaluate(std::vector<pin_struct_type> &input_pins) {}// funtion that processes the inputs to perform an action or to create an ouput value(s) to be passed on to the ouput 

    virtual void update() {}
    virtual void on_link_connection() {}
    virtual void on_link_disconnection() {}
    virtual void export_node(std::fstream &stream) {}

    virtual bool import_node(std::vector<std::string> lines, int &line_number) { return true; }// *****

    virtual bool evaluated();

    bool draw_node(pins_class pins, float scale = 1.0);

    void delete_node_links(links_class &links) {
        for (int pin_id : inputs) {
            for (link_struct_type link : links.links) {
                if (link.to == pin_id) {
                    links.delete_link(link.link_id);
                }
            }
        }

        for (int pin_id : outputs) {
            for (link_struct_type link : links.links) {
                if (link.from == pin_id) {
                    links.delete_link(link.link_id);
                }
            }
        }
    }

    void  delete_node_pins(pins_class &pins) {
//std::cout << "node_basis_class::delete_node_pins:: 000 : inputs : " << inputs.size()<< std::endl;
        for (int pin_id : inputs) {
            pins.delete_input_pin(pin_id);
        }
//std::cout << "node_basis_class::delete_node_pins::111 : outputs : " << outputs.size()<< std::endl;
        for (int pin_id : outputs) {
            pins.delete_output_pin(pin_id);
        }
//std::cout << "node_basis_class::delete_node_pins:: 222 \n";

        inputs.clear();
        outputs.clear();
    }

// !!!!!!!!!!!!!!!!!!! DEFINE NODE INPUTS FUNCTIONS !!!!!!!!!!!!!!!!!!!!!!!!!
    int add_node_input_pin(pin_struct_type pin, pins_class& pins){
        pin.node_id = node_id;
        int pin_id  = pins.add_input_pin(pin);

        int i = int(pins.input_pins.size()) - 1;
//std::cout << "node_basis_class :: add_node_input_pin 0000 : " << i << " : "<< pins.input_pins.size() << ":" << pin.node_id <<std::endl;
        if (i > INVALID_ID) {
            inputs.push_back(pins.input_pins[i].pin_id);
//std::cout << "node_basis_class :: add_node_input_pin 1111 : " << i << " : " << pins.input_pins.size() << std::endl;
            return pin_id;
        }

        return INVALID_ID;
//std::cout << "node_basis_class :: add_node_input_pin 2222 : " << i << " : " << pins.input_pins.size();
    }

    bool remove_input_pin(pin_struct_type pin, pins_class &pins) {
        return remove_input_pin(pin.pin_id, pins);
    }

    bool remove_input_pin(int pin_id, pins_class &pins) {
        for (int i = 0; i < pins.input_pins.size(); i++) {
            if (pins.input_pins[i].pin_id == pin_id) {
                pins.input_pins.erase(pins.input_pins.begin() + i);
                remove_node_input(pin_id);
                return true;
            }
        }
        return false;
    }

    int add_node_output_pin(pin_struct_type pin, pins_class &pins){
        pin.node_id = node_id;
        int pin_id = pins.add_output_pin(pin);

        int i = int(pins.output_pins.size()) - 1;
//std::cout <<"node_basis_class :: add_node_output_pin OOOO : "<<i <<" : " << pins.output_pins.size() << std::endl;
        if (i > INVALID_ID) {
            outputs.push_back(pins.output_pins[i].pin_id);
//std::cout <<"node_basis_class :: add_node_output_pin IIII : "<<i <<" : " << pins.output_pins.size() << std::endl;
            return pin_id;
        }

        return INVALID_ID;
//printf("node_basis_class :: define_node_output_value_of_type 0000 %i : % i\n",i, pins.input_pins.size());
    }

    bool remove_output_pin(pin_struct_type pin, pins_class& pins) {
        return remove_output_pin(pin.pin_id, pins);
    }

    bool remove_output_pin(int pin_id, pins_class& pins) {
        for (int i = 0; i < pins.output_pins.size(); i++) {
            if (pins.output_pins[i].pin_id == pin_id) {
                pins.output_pins.erase(pins.output_pins.begin() + i);
                remove_node_output(pin_id);
                return true;
            }
        }
        return false;
    }

protected:

    void remove_node_input(int pin_id) {
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i] == pin_id)
                inputs.erase(inputs.begin() + i);
        }
    }

    void remove_node_output(int pin_id) {
        for (int i = 0; i < outputs.size(); i++) {
            if (outputs[i] == pin_id)
                outputs.erase(outputs.begin() + i);
        }
    }

    // ^^^^^^^^^^^^^^^^^ END DEFINE NODE INPUTS FUNCTIONS ^^^^^^^^^^^^^^^^^

    // !!!!!!!!!!!!!!!!!!! DRAW NODE FUNCTIONS !!!!!!!!!!!!!!!!!!!
    bool draw_body(const ImVec2& size, float scale);

    void draw_node_header(std::string header_label, float scale = 1.0f);
    void draw_node_header(float scale = 1.0f);

    void draw_node_input_link(int input_pin_id,
                              pins_class &pins);

    void draw_node_output_link(int output_pin_id,
                               pins_class &pins);

    void draw_node_input_links() {
    }

    void draw_node_output_links() {

    }

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ END DRAW NODE FUNCTIONS ^^^^^^^^^^^^^^^^^^^^^^^^^^
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    void export_mandatory_node_data(std::fstream& stream);
    void export_node_pin_data(std::fstream& stream);
    void export_null_node_pin_data(std::fstream& stream);
};

class nodes_class {
public:
    std::vector<node_basis_class*> nodes;

    void insert(node_basis_class* node) {
        nodes.push_back(node);
    }

    int get_node_index(int node_id) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->node_id == node_id) {
                return i;
                break;
            }
        }

        return INVALID_ID;
    }

    node_basis_class* get_node(int node_id) {
        int index = get_node_index(node_id);
        if (index == INVALID_ID) return NULL;

        return nodes[index];
    }

    bool delete_node(int node_id) {
        int index = get_node_index(node_id);
//std::cout <<"nodes_class::delete_node:: NNN000 : " << index << std::endl;
        if (index == INVALID_ID) return false;
//std::cout <<"nodes_class::delete_node:: NNN111 : " << index << " : " << nodes.size()<< " : "<< nodes[index]->node_id << std::endl;
        nodes[index]->delete_node_entity();
//std::cout <<"nodes_class::delete_node:: NNN222 : " << index << " : " << nodes.size()<< std::endl;
        nodes.erase(nodes.begin() + index);

        return true;
    }

    node_basis_class* find_node_with_output_pin(int pin_id) {
        for (node_basis_class *node : nodes) {
            if (node->has_output_link(pin_id))
                return node;
        }
        return nullptr;
    }

    node_basis_class* find_node_with_intput_pin(int pin_id) {
        for (node_basis_class* node : nodes) {
            if (node->has_input_link(pin_id))
                return node;
        }
        return nullptr;
    }

    // BEWARE AND NOTE : find_node_of_category_and_type will return the first node it finds
    //                   and thus should only be used if it is known only one node exists
    //                   of the requested category type and node id
    node_basis_class *find_node_of_category_and_type(int category_id, int node_type_id) {
        for (node_basis_class* node : nodes) {
            if (node->ui_node_type.category == category_id && node->ui_node_type.node_type == node_type_id)
                return node;
        }
        return nullptr;
    }

protected:

private:
};
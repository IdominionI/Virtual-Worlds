#pragma once

#include <Universal_FW/ImNodes/Kernal/node_group.h>
#include <Universal_FW/ImNodes/Kernal/node_link.h>
#include <Universal_FW/ImNodes/Kernal/node_editor.h>
#include <VW_framework/Scene/scene_entities_manager.h>
#include <VW_framework/Node_editor/node_editor_category_manager.h>

#include <VW_framework/Scene/vw_scene.h>

class scene_node_editor_class;
inline std::vector<scene_node_editor_class*> scene_node_editors;// This needed because ImGui::begin ImGui::end canot be nested to display node editors 

inline node_id_type current_active_editor = INVALID_ID;

//inline graph_class master_graph; // A master graph which stores the graph of the entire scene
// A master graph which stores the graph of the entire scene for evaluation or other purposes
// This is called to add delete or modify nodes, pins, links or to evaluate pin variable values or perform
// procesess as part of the nodes funtion.
//inline scene_graph_class master_graph;

class scene_node_editor_class : public node_editor_basis_class {
public:
    bool         display_on = true;
    node_id_type editor_id  = INVALID_ID;// except for root editor, editor id is the same as the group id that the editor is associated with

    node_editor_category_manager_class    *node_editor_category_manager;

    scene_node_editor_class               *parent_group = nullptr;
    std::vector<scene_node_editor_class *> child_groups;

    group_link_node_class *input_link_node  = nullptr;
    group_link_node_class *output_link_node = nullptr;

    vw_scene_class *vw_scene = NULL;
    log_panel_class              *log_panel = NULL;

    int selected_hover_pin_id = INVALID_ID;
    node_basis_class    *selected_node = NULL;
    link_struct_type    selected_link;
    pin_struct_type     selected_pin;

    void display();

    scene_node_editor_class *get_root_editor(scene_node_editor_class *scene_node_editor){
        if (!scene_node_editor) return nullptr;
        scene_node_editor_class *root_scene_node_editor = scene_node_editor;
        while (root_scene_node_editor->parent_group != nullptr) {
            root_scene_node_editor = root_scene_node_editor->parent_group;
//std::cout << "get_root_editor editor : " << root_scene_node_editor->editor_id << std::endl;
        }
        return root_scene_node_editor;
    }

    scene_node_editor_class *get_node_editor(node_id_type editor_id_to_find) {
        for(scene_node_editor_class *node_editor : scene_node_editors){
            if (node_editor->editor_id == editor_id_to_find) return node_editor;
        }
        return nullptr;
    }

    void display_graph_status(scene_node_editor_class* scene_node_editor) {

        if (!scene_node_editor) {
            std::cout << "scene_node_editor_class : display_graph_status  !scene_node_editor \n";
            return;
        }
        scene_node_editor_class* root_scene_node_editor = get_root_editor(scene_node_editor);
        if (!root_scene_node_editor) {
            std::cout << "scene_node_editor_class : display_graph_status  !root_scene_node_editor \n";
            return;
        }
        int neditors = 0, nnodes = 0, ninput_pins = 0, noutput_pins = 0, nlinks = 0;

        get_graph_content_count(root_scene_node_editor, neditors, nnodes, ninput_pins, noutput_pins, nlinks);

        std::cout << "scene_node_editor_class : Total scene graph status : editors : " << neditors << " nodes : " << nnodes << " : input pins " << ninput_pins << " : output pins " << noutput_pins << " links : " << nlinks << std::endl;

    }

private:
    bool menu_selection = false;

    // !!!!! NODE GROUP FUNCTIONS !!!!!!!!
    void get_graph_content_count(scene_node_editor_class *node_editor,int &neditors,int &nnodes,int &ninput_pins ,int &noutput_pins,int &nlinks) {
        if (!node_editor) return;
        for (scene_node_editor_class* child_editor : node_editor->child_groups) {
            get_graph_content_count(child_editor, neditors, nnodes, ninput_pins, noutput_pins, nlinks);
            std::cout << "scene_node_editor_class : scene graph status for editor of ID " << child_editor->editor_id << " nodes : " << child_editor->graph.nodes.nodes.size() << " : input pins " << child_editor->graph.pins.input_pins.size() << " : output pins " << child_editor->graph.pins.output_pins.size() << " links : " << child_editor->graph.links.links.size() << std::endl;
        }

        neditors     += node_editor->child_groups.size();
        nnodes       += node_editor->graph.nodes.nodes.size();
        ninput_pins  += node_editor->graph.pins.input_pins.size();
        noutput_pins += node_editor->graph.pins.output_pins.size();
        nlinks       += node_editor->graph.links.links.size();
    }

    void toggle_group_node_visibility(node_id_type node_id) {
        for (scene_node_editor_class* node_group : child_groups) {
            if (node_group->editor_id == current_selected_node_id) {
                node_group->display_on = !node_group->display_on;
            }
        }
    }

    void expand_group_node(node_id_type node_id) {
//std::cout << "scene_node_editor_class : expand_node_group 000: " << node_id << std::endl;
        for (scene_node_editor_class *node_group : child_groups) {
//std::cout << "scene_node_editor_class : expand_node_group 111: " << node_group->editor_id << std::endl;
            if (node_group->editor_id == node_id) {
//std::cout << "scene_node_editor_class : expand_node_group 222: " << node_group->editor_id << std::endl;
                node_group->display_on = true;
            }
        }
    }

    void collapse_group_node(node_id_type node_id) {
//std::cout << "scene_node_editor_class : expand_node_group 000: " << node_id << std::endl;
        for (scene_node_editor_class *node_group : child_groups) {
//std::cout << "scene_node_editor_class : expand_node_group 111: " << node_group->editor_id << std::endl;
            if (node_group->editor_id == node_id) {
//std::cout << "scene_node_editor_class : expand_node_group 222: " << node_group->editor_id << std::endl;
                node_group->display_on = false;
            }
        }
    }
public:
    //bool create_group_node(ImVec2 click_pos);
    node_group_class *create_group_node(ImVec2 click_pos, bool create_import_group_node = false);

    void delete_group_node(node_basis_class *selected_node) {
    //void delete_group_node(node_group_class *selected_node) {
//std::cout << "scene_node_editor_class : delete_group_node 000: " << selected_node->node_id << std::endl;
        for (int i = 0; i < child_groups.size();i++) {
//std::cout << "scene_node_editor_class : delete_group_node 111: " << child_groups[i]->editor_id << std::endl;
            if (child_groups[i]->editor_id == selected_node->node_id) {
//std::cout << "scene_node_editor_class : delete_group_node 222 : " << selected_node->node_id << ":" << child_groups[i]->graph.nodes.nodes.size() <<  std::endl;
                child_groups[i]->delete_child_group_nodes();
                child_groups[i]->delete_group_links();
                child_groups[i]->delete_group_node_graph();// !!!!!

//std::cout << "scene_node_editor_class : delete_group_node 222AAA : " << selected_node->node_id << std::endl;
                delete_group_links();// ++++++
//std::cout << "scene_node_editor_class : delete_group_node 222BBBB : " << selected_node->node_id << std::endl;
                graph.delete_node(selected_node->node_id);
                delete_node_group_editor(child_groups[i]->editor_id);
                child_groups.erase(child_groups.begin() + i);
//std::cout << "scene_node_editor_class : delete_group_node 333 : " << selected_node->node_id << std::endl;
                //master_graph.delete_node(selected_node->node_id);// !!!!!
                display_graph_status(this);// testing only
                return;
            }
        }
    }
private:
    // Recursive Post order deleteion of groups
    void delete_child_group_nodes(){
        for (scene_node_editor_class *node_group : child_groups) {
//std::cout << "scene_node_editor_class : delete_child_group_nodes 000: " << node_group->editor_id << std::endl;
             node_group->delete_child_group_nodes();
             node_group->delete_group_links();
             node_group->delete_group_node_graph();
             delete_node_group_editor(node_group->editor_id);
        }
    }

    void delete_group_links() {
        delete_group_input_links();
        delete_group_output_links();
    }

    void delete_group_input_links() {
        if (!parent_group || !input_link_node) return;

        for (int pin_id : input_link_node->outputs) {
            pin_struct_type input_link_pin = graph.pins.find_pin(pin_id);
            if (input_link_pin.pin_id == pin_id) // As graph.pins.find_pin(pin_id) may have failed
                delete_group_node_input_pin(input_link_pin);
        }
    }

    void delete_group_output_links() {
        if (!parent_group || !output_link_node) return;

        for (int pin_id : output_link_node->inputs) {
            pin_struct_type output_link_pin = graph.pins.find_pin(pin_id);
            if (output_link_pin.pin_id == pin_id)// As graph.pins.find_pin(pin_id) may have failed
                delete_group_node_output_pin(output_link_pin);
        }
    }

    void delete_group_node_graph() {
//std::cout << "scene_node_editor_class : delete_group_node_graph 000: " <<  std::endl;
        // need to delete all nodes from a master graph that holds a complete graph of all nodes for proccessing ??????
        // or does a master graph be constructed from all root and all group graphs each time a process is performed ???
        //master_graph.delete_sub_graph_nodes(graph);// !!!!!
//std::cout << "scene_node_editor_class : delete_group_node_graph 111: " <<graph.nodes.nodes.size() <<  std::endl;
        graph.clear_graph();
    }

    void delete_node_group_editor(node_id_type editor_id) {
        for (int i = 0; i < scene_node_editors.size();i++) {
//std::cout << "scene_node_editor_class : expand_node_group 111: " << node_group->editor_id << std::endl;
            if (scene_node_editors[i]->editor_id == editor_id) {
//std::cout << "scene_node_editor_class : expand_node_group 222: " << node_group->editor_id << std::endl;
                scene_node_editors.erase(scene_node_editors.begin() + i);
                return;
            }
        }
    }

    void add_group_node_input_pin(pin_struct_type pin) {
//std::cout << "Scene node editor : add_group_node_input_pin 1111 : " << std::endl;
        if (parent_group == nullptr) return;// Root scene node editor
//std::cout << "Scene node editor : add_group_node_input_pin 2222 : " << editor_id << std::endl;
        // Find group node to add a pin to in the parent scene node editor
        // Group node and scene editor linked to group node have the same id
        node_basis_class *group_basis_node = parent_group->graph.nodes.get_node(editor_id);
        if (group_basis_node == nullptr) return;
//std::cout << "Scene node editor : add_group_node_input_pin 3333 : " << std::endl;
        node_group_class *group_node = dynamic_cast<node_group_class*>(group_basis_node);
        if(group_node == nullptr || input_link_node == nullptr) return;

//std::cout << "Scene node editor : add_group_node_input_pin 4444 : " << std::endl;
        // Create node pins in parent scene node editor group node and current editor input link node
        int group_input_pin_id = group_node->add_node_input_pin(pin, parent_group->graph.pins);//!!!!
        if (group_input_pin_id == INVALID_ID) return;
  
        // Need to define group link input node ouput lins as output pins
        pin_struct_type tpin = pin;
        if(pin.pin_type == pin_type_enum::single_input)
            tpin.pin_type = pin_type_enum::single_output;
        else // Need to dissable multi_inputs
            if (pin.pin_type == pin_type_enum::multi_input)
                tpin.pin_type = pin_type_enum::multi_output;

//std::cout << "Scene node editor : add_group_node_input_pin 5555 : "<< input_link_node->node_id << std::endl;
        int input_link_pin_id  = input_link_node->add_node_output_pin(tpin, graph.pins);
        if (input_link_pin_id == INVALID_ID) {
            group_node->remove_input_pin(pin.pin_id, parent_group->graph.pins);
            group_node->remove_output_pin(tpin.pin_id, graph.pins);
            return;
        }

//std::cout << "Scene node editor : add_group_node_input_pin 6666 : " << std::endl;
        std::pair<int, int> group_input_link = { group_input_pin_id,input_link_pin_id };

        group_node->group_input_links.push_back(group_input_link);

        // Create a link between the input pin that is to be linked to the parent editor group node and the nominated
        // input pin of the node within the curent editor and the output pin of the input link node editor of this editor
        // The input link between this editor and its parent editor is a hidden link that is not to be drawn.

        graph.links.insert(input_link_pin_id,pin.pin_id);

        input_link_node->input_links = group_node->group_input_links;// ++++++++

//std::cout << "Scene node editor : add_group_node_input_pin : 6666 : " << group_node->group_input_links.size() << " : " << master_group_node->group_input_links.size() << std::endl;
        display_graph_status(this);// testing only
    }

    // To complete and to check :: not ready
    void delete_group_node_input_pin(pin_struct_type pin) {
//std::cout << "Scene node editor : delete_group_node_input_pin 1111 : " << std::endl;
        if (parent_group == nullptr) return;// Root scene node editor
//std::cout << "Scene node editor : delete_group_node_input_pin 2222 : " << editor_id << std::endl;
        // Find group node to add a pin to in the parent scene node editor
        // Group node and scene editor linked to group node have the same id
        node_basis_class *group_basis_node = parent_group->graph.nodes.get_node(editor_id);
        if (group_basis_node == nullptr) return;
//std::cout << "Scene node editor : delete_group_node_input_pin 3333 : " << std::endl;
        node_group_class *group_node = dynamic_cast<node_group_class*>(group_basis_node);
        if(group_node == nullptr || input_link_node == nullptr) return;
//std::cout << "Scene node editor : delete_group_node_input_pin 3333AAA : " << pin.pin_id << ":" << group_node->group_input_links.size()  <<std::endl;

        // Find node pin data to delete
        int group_input_pin_link_index = group_node->find_link_output_index(pin.pin_id);
        if (group_input_pin_link_index == INVALID_ID) {
            // error message
            return;
        }
        std::pair<int, int> group_input_pin_link = group_node->group_input_links[group_input_pin_link_index];
//std::cout << "Scene node editor : delete_group_node_input_pin 44444 : " << std::endl;

        // If have any links to group nodes in the parent editor where the goup node exits delete the links
        if (parent_group->graph.links.end_pin_has_link(group_input_pin_link.first)) {
            link_struct_type group_node_input_link = parent_group->graph.links.find_link_with_end_pin(group_input_pin_link.first);
            parent_group->graph.links.delete_link(group_node_input_link.link_id);
        }

        // find input link data in the current group node editor to delete
        link_struct_type group_link_node_input_link        = graph.links.find_link_with_start_pin(pin.pin_id);

        graph.delete_link(group_link_node_input_link.link_id);

        // delete node pins
        group_node->remove_input_pin(group_input_pin_link.first, parent_group->graph.pins);
        group_node->remove_output_pin(pin.pin_id, graph.pins);

        //delete group node internal pin link from parent node editor to current group node editor
        group_node->delete_group_input_link(group_input_pin_link_index);

        input_link_node->input_links = group_node->group_input_links;// ++++++++
//std::cout << "Scene node editor : delete_group_node_input_pin : 6666 : " << group_node->group_input_links.size() << " : " << std::endl;
        display_graph_status(this);// testing only
    }

    void add_group_node_output_pin(pin_struct_type pin) { // This will be the eventual function
//std::cout << "Scene node editor : add_group_node_output_pin 1111 : " << std::endl;
        if (parent_group == nullptr) return;// Root scene node editor
//std::cout << "Scene node editor : add_group_node_output_pin 2222 : " << editor_id << std::endl;
        // Find group node to add a pin to in the parent scene node editor
        // Group node and scene editor linked to group node have the same id
        node_basis_class *group_basis_node = parent_group->graph.nodes.get_node(editor_id);
        if (group_basis_node == nullptr) return;
//std::cout << "Scene node editor : add_group_node_output_pin 3333 : " << std::endl;
        node_group_class *group_node = dynamic_cast<node_group_class*>(group_basis_node);
        if(group_node == nullptr || output_link_node == nullptr) return;

//std::cout << "Scene node editor : add_group_node_output_pin 4444 : " << std::endl;
        // Create node pins in parent scene node editor group node and current editor output link node
        int group_output_pin_id = group_node->add_node_output_pin(pin, parent_group->graph.pins);//!!!!
        if (group_output_pin_id == INVALID_ID) return;
  
        // Need to define group link output node ouput lins as output pins
        pin_struct_type tpin = pin;
        if(pin.pin_type == pin_type_enum::single_output)
            tpin.pin_type = pin_type_enum::single_input;
        else // Need to dissable multi_outputs
            if (pin.pin_type == pin_type_enum::multi_output)
                tpin.pin_type = pin_type_enum::multi_input;

//std::cout << "Scene node editor : add_group_node_output_pin 5555 : "<< output_link_node->node_id << std::endl;
        int output_link_pin_id  = output_link_node->add_node_input_pin(tpin, graph.pins);
        if (output_link_pin_id == INVALID_ID) {
            group_node->remove_output_pin(pin.pin_id, parent_group->graph.pins);
            group_node->remove_input_pin(tpin.pin_id, graph.pins);
            return;
        }

//std::cout << "Scene node editor : add_group_node_output_pin 6666 : " << std::endl;
        std::pair<int, int> group_output_link = { output_link_pin_id,group_output_pin_id };

        group_node->group_output_links.push_back(group_output_link);

        // Create a link between the output pin that is to be linked to the parent editor group node and the nominated
        // output pin of the node within the curent editor and the output pin of the output link node editor of this editor
        // The output link between this editor and its parent editor is a hidden link that is not to be drawn.
        graph.links.insert(pin.pin_id, output_link_pin_id);

        output_link_node->output_links = group_node->group_output_links;// ++++++++
//std::cout << "Scene node editor : add_group_node_output_pin : 6666 : " << group_node->group_output_links.size() << " : " << master_group_node->group_output_links.size() << std::endl;
        display_graph_status(this);// testing only
    }

    void delete_group_node_output_pin(pin_struct_type pin) {
//std::cout << "Scene node editor : delete_group_node_output_pin 1111 : " << std::endl;
        if (parent_group == nullptr) return;// Root scene node editor
//std::cout << "Scene node editor : delete_group_node_output_pin 2222 : " << editor_id << std::endl;
        // Find group node to add a pin to in the parent scene node editor
        // Group node and scene editor linked to group node have the same id
        node_basis_class *group_basis_node = parent_group->graph.nodes.get_node(editor_id);
        if (group_basis_node == nullptr) return;
//std::cout << "Scene node editor : delete_group_node_output_pin 3333 : " << std::endl;
        node_group_class *group_node = dynamic_cast<node_group_class*>(group_basis_node);
        if(group_node == nullptr || output_link_node == nullptr) return;
//std::cout << "Scene node editor : delete_group_node_output_pin 3333AAA : " << std::endl;

        // Find node pin data to delete
        int group_output_pin_link_index = group_node->find_link_input_index(pin.pin_id);
        if (group_output_pin_link_index == INVALID_ID) {
            // error message
            return;
        }

//std::cout << "Scene node editor : delete_group_node_output_pin 44444 : "<< group_output_pin_link_index << std::endl;
        std::pair<int, int> group_output_pin_link = group_node->group_output_links[group_output_pin_link_index];
//std::cout << "Scene node editor : delete_group_node_output_pin 44444AAAA : "<< group_output_pin_link.first << " : " << group_output_pin_link.second << std::endl;

        // If have any links to group nodes in the parent editor where the goup node exits delete the links
        if (parent_group->graph.links.start_pin_has_link(group_output_pin_link.second)) {// !!!!!!!!!!!!!
            link_struct_type group_node_output_link = parent_group->graph.links.find_link_with_start_pin(group_output_pin_link.second);// !!!!!!!!!!!!!
            parent_group->graph.links.delete_link(group_node_output_link.link_id);// !!!!!!!!!!!!!
        }

        // find output link data in the current group node editor to delete
        link_struct_type group_link_node_output_link = graph.links.find_link_with_end_pin(pin.pin_id);
        graph.delete_link(group_link_node_output_link.link_id);

        // delete node pins
        group_node->remove_output_pin(group_output_pin_link.second, parent_group->graph.pins);
        group_node->remove_input_pin(pin.pin_id, graph.pins);

        //delete group node internal pin link from parent node editor to current group node editor
        group_node->delete_group_output_link(group_output_pin_link_index);

        output_link_node->output_links = group_node->group_output_links;
//std::cout << "Scene node editor : delete_group_node_output_pin : 6666 : " << group_node->group_output_links.size() << " : " << std::endl;
        display_graph_status(this);// testing only
    }

    scene_node_editor_class* get_node_group_editor(node_id_type node_group_id) {
        for (scene_node_editor_class* node_group : child_groups) {
//std::cout << "scene_node_editor_class : expand_node_group 111: " << node_group->editor_id << std::endl;
            if (node_group->editor_id == node_group_id) {
                return node_group;
            }
        }

        return nullptr;
    }

    void save_group(node_basis_class *group_node);
    void import_group(scene_node_editor_class *node_group_editor, ImVec2 click_pos);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    template <class T>
    void create_node(ImVec2 click_pos);
    void delete_node(node_basis_class *selected_node);
    void delete_node_group_links(node_basis_class *selected_node);
    void delete_link(int link_id);

    void display_node_ui(node_basis_class *node);

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    void handle_new_links();
    void handle_deleted_links();

    id_key_manager_class<unsigned int> node_id_key;
};

#pragma once

#include <vector>
#include <fstream>

#include <Universal_FW/Kernal/id_key_manager.h>

#include "node_parameters.h"

struct pin_value_type_struct {
	int category   = INVALID_ID;
    //int context           = INVALID_ID;
	//int function_category = INVALID_ID;
	int value_type = INVALID_ID;
};

enum class pin_type_enum { undefined, single_input, single_output, multi_input, multi_output, parent, children };

struct pin_struct_type {
    std::string label = "Pin";

    pin_type_enum pin_type  = pin_type_enum::undefined;
	pin_value_type_struct     pin_value_type;
    int           context   = INVALID_ID;
    int           data_type = DATA_VALUE_TYPE_NULL;
    void          *value    = nullptr;
    bool          evaluated = false;

	int            pin_id   = INVALID_ID;
	int            node_id  = INVALID_ID;

	//int link_id = INVALID_ID; not a good idea !!!! may have more than one link per pin

    // Pin Drawing attributes
    ImColor          pin_color          = IM_COL32(120, 30, 30, 255);
    ImColor          pin_hovered_color  = IM_COL32(200, 85, 85, 255);
    ImColor          pin_selected_color = IM_COL32(200, 120, 120, 255);
    ImNodesPinShape_ pin_shape = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

    void export_pin_data(std::fstream &stream) {
        stream << GRAPH_PIN_START_DATA_FLAG << std::endl;
        stream << label << std::endl;
        stream << static_cast<int>(pin_type) << std::endl;
        stream << pin_value_type.category << std::endl;
        stream << pin_value_type.value_type << std::endl;
        stream << context << std::endl;
        stream << data_type << std::endl;
        stream << pin_id << std::endl;
        stream << node_id << std::endl;

        // Pin Drawing attributes 
        stream << pin_color.Value.x << ":" << pin_color.Value.y << ":" << pin_color.Value.z << ":" <<pin_color.Value.w << std::endl;
        stream << pin_hovered_color.Value.x << ":" << pin_hovered_color.Value.y << ":" << pin_hovered_color.Value.z << ":" << pin_hovered_color.Value.w << std::endl;
        stream << pin_selected_color.Value.x << ":" << pin_selected_color.Value.y << ":" << pin_selected_color.Value.z << ":" << pin_selected_color.Value.w << std::endl;
        stream << static_cast<int>(pin_shape) << std::endl;
        // these wiil need to be saved 

        //ImColor          pin_color = IM_COL32(120, 30, 30, 255);
        //ImColor          pin_hovered_color = IM_COL32(200, 85, 85, 255);
        //ImColor          pin_selected_color = IM_COL32(200, 120, 120, 255);
        //ImNodesPinShape_ pin_shape = ImNodesPinShape_::ImNodesPinShape_QuadFilled;
        stream << GRAPH_PIN_END_DATA_FLAG << std::endl;
    }
};

inline id_key_manager_class<unsigned int> pin_id_key;// This needed as global so all pins in all editors have a unique pin id

class pins_class {
public:
	std::vector<pin_struct_type> input_pins;
	std::vector<pin_struct_type> output_pins;

    int add_input_pin(pin_struct_type pin) {
        pin.pin_id  = pin_id_key.get_available_id_key();
        input_pins.push_back(pin);

        return pin.pin_id;
    }

    int add_output_pin(pin_struct_type pin) {
        pin.pin_id = pin_id_key.get_available_id_key();
        output_pins.push_back(pin);

        return pin.pin_id;
    }

    int get_input_pin_index(int input_pin_id) {
        for (int i = 0; i < input_pins.size();i++) {
            if (input_pins[i].pin_id == input_pin_id)
                return i;
        }
        return INVALID_ID;
    }

    int get_output_pin_index(int output_pin_id) {
        for (int i = 0; i < output_pins.size(); i++) {
            if (output_pins[i].pin_id == output_pin_id)
                return i;
        }
        return INVALID_ID;
    }

    pin_struct_type &find_pin(int pin_id) {
        int i = get_input_pin_index(pin_id);
        if (i != INVALID_ID) return input_pins[i];

        i = get_output_pin_index(pin_id);
        if (i != INVALID_ID) return output_pins[i];
       
        pin_struct_type pin;
        return pin;
    }

    bool delete_input_pin(int input_pin_id) {
        int i = get_input_pin_index(input_pin_id);
        if (i == INVALID_ID) return false;

        pin_id_key.assign_free_id_key(input_pins[i].pin_id);
        input_pins.erase(input_pins.begin() + i);
        return true;
    }

    bool delete_output_pin(int output_pin_id) {
        int i = get_output_pin_index(output_pin_id);
        if (i == INVALID_ID) return false;

        pin_id_key.assign_free_id_key(output_pins[i].pin_id);
        output_pins.erase(output_pins.begin() + i);
        return true;
    }

protected:

private:
    //id_key_manager_class<unsigned int> pin_id_key;
};
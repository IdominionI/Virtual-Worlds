#pragma once

#include "pins.h"

struct link_struct_type {
    int	link_id = INVALID_ID, from = -1, to = -1;

    // link Drawing attributes
    // not sure needed as specifying individual link between specific
    // pin types need to be defined in the editor which may be impractical or not efficient
    ImColor link_color                    = IM_COL32(0, 139, 139, 255);
    ImColor link_hovered_color            = IM_COL32(0, 175, 175, 255);
    ImColor link_selected_color           = IM_COL32(100, 255, 255, 255);
    float   link_thickness                = 1.0;
    float   link_line_segments_per_length = 1.0;

    std::pair<int, int> link_pins;
    void swap() {
        int i = from;
        from = to;
        to = i;
    }

    void export_link_data(std::fstream& stream) {
        stream << GRAPH_LINK_START_DATA_FLAG << std::endl;

        stream << link_id << std::endl;
        stream << from    << std::endl;
        stream << to      << std::endl;

        // link Drawing attributes
        // not sure needed as drawing of links defined by the editor
        //ImColor link_color = IM_COL32(0, 139, 139, 255);
        //ImColor link_hovered_color = IM_COL32(0, 175, 175, 255);
        //ImColor link_selected_color = IM_COL32(100, 255, 255, 255);
        //float   link_thickness = 1.0;
        //float   link_line_segments_per_length = 1.0;

        // This can be reconstructed when loading above from and to values
        //std::pair<int, int> link_pins;
        stream << GRAPH_LINK_END_DATA_FLAG << std::endl;
    }
};

inline id_key_manager_class<unsigned int> link_id_key;// This needed as global so all links in all editors have a unique link id

class links_class {
public:

    std::vector<link_struct_type > links;

    void insert(int start_pin_id, int end_pin_id) {
        link_struct_type link;
        link.link_id = link_id_key.get_available_id_key();
        link.from = start_pin_id;
        link.to   = end_pin_id;

        links.push_back(link);
    }

    bool delete_link(int link_id) {
        for (int i = 0; i < links.size(); i++) {
            if (links[i].link_id == link_id) {
                link_id_key.assign_free_id_key(links[i].link_id);
                links.erase(links.begin() + i);
                return true;
                break;
            }
        }
        return false;
    }

    link_struct_type find_link(int link_id) {
        link_struct_type link_;
        for (link_struct_type link : links) {
            if (link.link_id == link_id) {
                return link;
                break;
            }
        }
        return link_;
    }

    std::vector<link_struct_type> get_all_links_with_start_pin(int pin_id) {
        std::vector<link_struct_type> link_list;

        for (link_struct_type link : links) {
            if (link.from == pin_id) {
                link_list.push_back(link);
            }
        }
        return link_list;
    }

    std::vector<link_struct_type> get_all_links_with_end_pin(int pin_id) {
        std::vector<link_struct_type> link_list;

        for (link_struct_type link : links) {
            if (link.to == pin_id) {
                link_list.push_back(link);
            }
        }
        return link_list;
    }

    link_struct_type find_link_with_start_pin(int pin_id) {
        link_struct_type link_;
        for (link_struct_type link : links) {
            if (link.from == pin_id) {
                return link;
                break;
            }
        }
        return link_;
    }

    link_struct_type find_link_with_end_pin(int pin_id) {
        link_struct_type link_;
        for (link_struct_type link : links) {
            if (link.to == pin_id) {
                return link;
                break;
            }
        }
        return link_;
    }

    bool start_pin_has_link(int start_pin_id) {
        for (link_struct_type link : links) {
            if (link.from == start_pin_id) {
                return true;
                break;
            }
        }

        return false;
    }

    bool end_pin_has_link(int end_pin_id) {
        for (link_struct_type link : links) {
            if (link.to == end_pin_id) {
                return true;
                break;
            }
        }

        return false;
    }

protected:

private:
   // id_key_manager_class<unsigned int> link_id_key;
};
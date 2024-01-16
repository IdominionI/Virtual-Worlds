#pragma once
#include <vector>
#include <iostream> // For cout only: delete/comment out when finished

#include <Universal_FW/Kernal/id_key_manager.h>

#include "timeline_interval.h"

inline id_key_manager_class<unsigned int> group_track_id_key;

class timeline_track_group_class {
public:
   // ~timeline_track_group_class() {
        //delete data_ptr;
    //    delete group_active_ptr;
   // }

    //void *data_ptr = nullptr;
    animation_object_basis_class *animation_object = nullptr;
    int  data_type_id = -1;

    int   track_group_id = -1;
    bool *group_active_ptr = nullptr;// Aparantly, ImGui checkbox will not work in a for loop within a Imgui child window unless have a bool variable as a pointer : Wiered !!!!!!!!
    
    std::string label = "track_group";

    std::vector<timeline_track_basis_class *> timeline_tracks;

    bool group_active() { 
        if (group_active_ptr != nullptr)
            return *group_active_ptr;
        else
            return false;
    }

    void add_track(timeline_track_basis_class *timeline_track){
        timeline_tracks.push_back(timeline_track);
    }

    // Needs testing
    bool delete_track(int track_id){
        for (int i = 0; i < timeline_tracks.size(); i++) {
            if (timeline_tracks[i]->track_id == track_id) {
                timeline_tracks.erase(timeline_tracks.begin() + i);
                return true;
            }
        }

        return false;
    }

    // Needs testing
    timeline_track_basis_class *get_track(int track_id) {
        for (timeline_track_basis_class *timeline_track : timeline_tracks) {
            if (timeline_track->track_id == track_id) {
                return timeline_track;
            }
        }

        return nullptr;
    }

    // Needs testing
    void move_track_up(int track_id) {
        for (int i = 0; i < timeline_tracks.size(); i++) {
            if (timeline_tracks[i]->track_id == track_id && i > 0) {
                timeline_track_basis_class *timeline_track_temp = timeline_tracks[i - 1];
                timeline_tracks[i - 1] = timeline_tracks[i];
                timeline_tracks[i]     = timeline_track_temp;
            }
        }
    }
    
    // Needs testing
    void move_track_down(int track_id) {
        for (int i = 0; i < timeline_tracks.size(); i++) {
            if (timeline_tracks[i]->track_id == track_id && i < timeline_tracks.size()-1) {
                timeline_track_basis_class* timeline_track_temp = timeline_tracks[i +1];
                timeline_tracks[i + 1] = timeline_tracks[i];
                timeline_tracks[i] = timeline_track_temp;
            }
        }
    }

    void clear_all_tracks() { 
        timeline_tracks.clear();
    }


private:
};

class timeline_tracks_class {
public:
    std::vector<timeline_track_group_class> timeline_track_groups;

    //bool test_tracks_added = false;//testing only
    
    void display_tracks(ImVec2 widget_size, float track_width,  int_timeline_parameter_data_struct_type timeline_data) {
        int i = 2;
        int child_id = ImGui::GetID((void*)(intptr_t)i);

        // ----------------------------------------
        //Test data : delete when not needed any more
/*        
        if (!test_tracks_added) { // make sure this is only called once 
            //test_track00 = new timeline_int_interval_class;
            //test_track00->track_id = 0;
            //test_track00->button_id_add = 0;
            //timeline_track_group00.group_active_ptr = new bool;
            //timeline_track_group00.add_track(test_track00);
            //timeline_track_groups.push_back(timeline_track_group00);



            test_track11 = new timeline_int_interval_class;
            test_track11->track_id = 1;
            test_track11->button_id_add = 1;
            test_track11->track_name = "test_track11";

            test_track112 = new timeline_int_interval_class;
            test_track112->track_id = 12;
            test_track112->button_id_add = 12;
            test_track112->track_name = "test_track112";

            timeline_track_group11.group_active_ptr = new bool;
            timeline_track_group11.label = "group11";
            timeline_track_group11.add_track(test_track11);
            timeline_track_group11.add_track(test_track112);
            timeline_track_groups.push_back(timeline_track_group11);

            test_track22 = new timeline_int_interval_class;
            test_track22->track_id = 2;
            test_track22->button_id_add = 2;
            test_track22->track_name = "test_track22";
            timeline_track_group22.group_active_ptr = new bool;
            timeline_track_group22.label = "group22";
            timeline_track_group22.add_track(test_track22);
            timeline_track_groups.push_back(timeline_track_group22);

            test_tracks_added = true;
        }
*/
        // ----------------------------------------

        ImGui::BeginChild(child_id, widget_size, true);
//std::cout << "animation_timeline_panel_class:: before screen pos : " << p0.x << ":" << p0.y << ":" << window->Pos.y << std::endl;

            for (timeline_track_group_class timeline_track_group : timeline_track_groups) {
                if (group_CollapsingHeader(timeline_track_group.label.c_str(), timeline_track_group.group_active_ptr, ImGuiTreeNodeFlags_None)) {
                    for (timeline_track_basis_class *timeline_track : timeline_track_group.timeline_tracks) {
                        timeline_track->display_track(track_width, timeline_data);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
                    }
                }
            }

        ImGui::EndChild();
	}

    // This is a copy of the original ImGui CollapsingHeader function with modifications to display
    // a checkbox in place of the close button as for this purpose the only way to close a 
    // group collapsing header is to delete the group in the editor.
    bool group_CollapsingHeader(const char* label, bool *group_active_ptr, ImGuiTreeNodeFlags flags)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiID id = window->GetID(label);
        flags |= ImGuiTreeNodeFlags_CollapsingHeader;

        flags |= ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_ClipLabelForTrailingButton;
        bool is_open = ImGui::TreeNodeBehavior(id, flags, label);

        // Create a small overlapping check box
        // FIXME: We can evolve this into user accessible helpers to add extra buttons on title bars, headers, etc.
        // FIXME: CloseButton can overlap into text, need find a way to clip the text somehow.

        ImVec2 pos = ImGui::GetCursorPos();
        ImGui::SameLine();

        ImGui::SetCursorPosX(window->Size.x - 46);
        std::string s1 = "###"; std::string s2 = label;
        std::string s = s1 + s2;
        ImGui::Checkbox(s.c_str(), group_active_ptr);

        ImGui::SetCursorPos(pos);

        return is_open;
    }

    // Track Group functions

    bool timeline_track_group_exists(int track_group_id) {
        for (timeline_track_group_class timeline_track_group : timeline_track_groups) {
            if (timeline_track_group.track_group_id == track_group_id)
                return true;
        }
        return false;
    }

    //bool timeline_track_group_exists(std::string label) {
    //    for (timeline_track_group_class timeline_track_group : timeline_track_groups) {
    //        if (timeline_track_group.label == label)
    //            return true;
    //    }
    //    return false;
    //}

    int add_timeline_group(std::string label, int track_group_id) {
        if (timeline_track_group_exists(track_group_id))
            return -1;

        timeline_track_group_class timeline_track_group;
        timeline_track_group.label            = label;
        timeline_track_group.track_group_id   = track_group_id;
        timeline_track_group.group_active_ptr = new bool;

        timeline_track_groups.push_back(timeline_track_group);
        return timeline_track_groups.size()-1;
    }

    // Needs testing
    bool delete_timeline_group(int track_group_id) {
        for (int i=0; i < timeline_track_groups.size();i++) {
            if (timeline_track_groups[i].track_group_id == track_group_id) {
                //timeline_track_groups[i].clear_all_tracks();
                for (timeline_track_basis_class *timeline_track : timeline_track_groups[i].timeline_tracks) {
                    group_track_id_key.assign_free_id_key(timeline_track->track_id);
                    timeline_track_groups[i].delete_track(timeline_track->track_id);
                }
                delete timeline_track_groups[i].group_active_ptr;
                timeline_track_groups.erase(timeline_track_groups.begin() + i);
                return true;
            }
        }

        return false;
    }

    // Flowing does not work and crashes app when adding a track
    // DO NOT USE !!!!!!!!!!!!!!
    //timeline_track_group_class *get_track_group(int track_group_id) {
    //    for (timeline_track_group_class timeline_track_group : timeline_track_groups) {
    //        if (timeline_track_group.track_group_id == track_group_id) {
    //            return &timeline_track_group;
    //        }
    //    }
    //    return nullptr;
    //}

    timeline_track_basis_class *get_group_track(int track_group_id, int track_id) {
        for (timeline_track_group_class timeline_track_group : timeline_track_groups) {
            if (timeline_track_group.track_group_id == track_group_id) {
                return timeline_track_group.get_track(track_id);
            }
        }
        return nullptr;
    }

    // For unique track ids this function must be executed and the returned id key assigned to the
    // created group track for the timeline track to function correctly.
    int get_timeline_group_track_id() {
        return group_track_id_key.get_available_id_key();
    }

    // Needs testing
    bool add_timeline_group_track(int track_group_id, timeline_track_basis_class *timeline_group_track) {
        for (timeline_track_group_class timeline_track_group : timeline_track_groups) {
            if (timeline_track_group.track_group_id == track_group_id) { // Found group to add track to
                for (timeline_track_basis_class* group_track : timeline_track_group.timeline_tracks) {
                    if (group_track->track_id == timeline_group_track->track_id) // Track already exits
                        return false;
                }
                timeline_track_group.add_track(timeline_group_track);
                return true;
            }
        }
        return false;
    }

    //works
    bool delete_timeline_group_track(int track_group_id, int track_id) {
        for (int i=0; i < timeline_track_groups.size(); i++) {
            if (timeline_track_groups[i].track_group_id == track_group_id) {
                timeline_track_groups[i].delete_track(track_id);
                group_track_id_key.assign_free_id_key(track_id);
                return true;
            }
        }

        return false;
    }

    //timeline_int_interval_class* test_track00, * test_track11, * test_track22, * test_track112;// testing only : delete/comment out when done
    //timeline_track_group_class timeline_track_group00, timeline_track_group11, timeline_track_group22;// testing only : delete/comment out when done

protected:
private:
  //  id_key_manager_class<int> group_track_id_key;
};

// ++++++++++
inline timeline_tracks_class *animation_timeline_tracks_widget;
// ++++++++++
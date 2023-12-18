#pragma once

#include <string>
#include <vector>
//#include <map>

#include <ofNode.h>
#include <ofShader.h>

#include <FrameWork/VW_framework/Types/vw_type_definitions.h>

#include <VW_framework/Geometry/vw_entity_object_geometry.h>
#include "entity_object_gui.h"
#include "entity_object_node_editor.h"

enum entity_object_translation_operation_enum {parent, all_children, parent_child, immediate_children};

//template <class dataclass, class guiclass, class nodesclass>
//template <typename VT = ofDefaultVertexType, typename NT = ofDefaultNormalType, typename CT = ofDefaultColorType, typename TT = ofDefaultTexCoordType>// not yet used
class entity_object_basis_class;
class entity_associations_class {
public:
	~entity_associations_class() {
		clear_entity_associations();
	}

	struct entity_associations_struct_type {
		entity_object_basis_class *entity;
		int                        association_type;
	};

	std::vector<entity_associations_struct_type> entity_associations;

	bool  add_entity_association(entity_object_basis_class *entity,int association_type) {
		if (entity == NULL) return false;

		entity_associations_struct_type entity_association;
		entity_association.association_type = association_type;
		entity_association.entity           = entity;

		entity_associations.push_back(entity_association);
		return true;
	}

	void clear_entity_associations() {
		entity_associations.clear();
	}

	bool remove_all_entity_associations_for_entity(entity_object_basis_class *entity) {
		if (entity == NULL) return false;
		for (size_t i = entity_associations.size() - 1; i >= 0; i--) {
			if (entity_associations[i].entity == entity) {
				remove_entity_association(i);
			}
		}
		return true;
	}

	void remove_entity_association(size_t index) {
		if (index < entity_associations.size() && index >= 0) {
			entity_associations.erase(entity_associations.begin() + index);
		}
	}

	


private:
};

//template <typename VT = glm::vec3>// not yet used
// ofNode assumes a node object exists only in 3D space
// May and probably need to change this assumption
class entity_object_basis_class : public ofNode {
public:
	id_type                     entity_id          = INVALID_ID;
	std::string                 entity_name        = "";
	std::string                 entity_description = "";
	std::map<int ,std::string>  entity_context;

	bool display_object = true;
	//bool use_material   = false;
	entity_object_basis_class() {
		origin_location   = glm::vec3{ 0.0,0.0,0.0 };
		previous_position = origin_location;
		current_position  = previous_position;

		setScale(1.0f,1.0f,1.0f);

		setOrientation({0.0,0.0,0.0});
		previous_orientation_quat  = getOrientationQuat();
		current_orientation_quat   = previous_orientation_quat;
		previous_orientation_euler = getOrientationEuler();
		current_orientation_euler  = previous_orientation_euler;
		
	}
	/// \brief Classes extending ofNode can override this method to get
	///        notified when the position changed.
	//void onPositionChanged() {}

	// Object children defined in ofNode as a std::set but not directly used or have
	// functions to reference or create them. A listener function
	// allocates a child to a ofNode when creating a parent, but no function
	// exists to allocate a child to given node or to retrieve a child node.
	// Why on earth open frameworks offers no child access or funtionality is beyond belief
	// Thus need own implementation of child relationships here
	
	std::vector<entity_object_basis_class *> entity_children;

	// \brief Classes extending ofNode can override this method to get
	//        notified when the position changed.
	// openframeWorks onScaleChanged() is unreliable : unexplained crashes of app occurs
	// for no apparent reason other than within openframeWorks a bug exists.
	// Need to suspect that onPositionChanged() may cause crash for no reason and needs
	// to be over looked.
	void onPositionChanged() {
		current_position        = getPosition();
		glm::vec3 eo_translation = current_position - previous_position;
//cout << "in onPositionChanged 01: " << current_position.x << " : " << current_position.y << " : " << current_position.z << std::endl;
//cout << "in onPositionChanged 01A: " << previous_position.x << " : " << previous_position.y << " : " << previous_position.z << std::endl;
//cout << "in onPositionChanged 01A: " << eo_translation.x << " : " << eo_translation.y << " : " << eo_translation.z << std::endl;

		for (size_t i=0; i < geometry->get_number_of_vertices(); i++) {
			geometry->setVertex(i, geometry->getVertex(i) + eo_translation);
		}
		geometry->update_geometry();

		change_children_position(eo_translation);

		previous_position = current_position;
		origin_location   = current_position;
	}

	// recursive pre-order function to translate all children locations consistent with a parent change of location
	void change_children_position(glm::vec3 eo_translation) {
		for (entity_object_basis_class *child : entity_children) {
//cout << "in onPositionChanged 02: " << getPosition().x << " : " << getPosition().y << " : " << getPosition().z << std::endl;
			//change child geometry locations here
			if (child != NULL) {
				for (size_t i = 0; i < child->geometry->get_number_of_vertices(); i++) {
					child->geometry->setVertex(i, child->origin_location + child->geometry->getVertex(i) + eo_translation);
				}
				child->geometry->update_geometry();
				child->origin_location   += eo_translation;
				child->current_position  = child->origin_location;
				child->previous_position = child->origin_location;

				child->change_children_position(eo_translation);
			}
		}
	}

	// \brief Classes extending ofNode can override this methods to get notified
	//        when the orientation changed.
	// openframeWorks onOrientationChanged() is unreliable : unexplained crashes of app occurs
	// for no apparent reason when using geometry->setVertex(i, rotated_point); 
	// other than within openframeWorks a bug exists.
	// Need to suspect that onPositionChanged() may cause crash for no reason and needs
	// to be over looked.

	//rotated_point = origin + (orientation_quaternion * (point - origin));
	//void change_orientation(entity_object_translation_operation_enum entity_object_translation_operation = entity_object_translation_operation_enum::parent_child) {
	void change_orientation() {
		glm::vec3 rotated_point, delta_rotation;

		current_orientation_quat  = getOrientationQuat();
		current_orientation_euler = getOrientationEuler();
		delta_rotation = current_orientation_euler - previous_orientation_euler;
//cout << "in change_orientation 01: " << origin_location.x << " : " << origin_location.y << " : " << origin_location.z << std::endl;

		int nv = geometry->get_number_of_vertices();

		for (size_t i = 0; i < nv; i++) {
			rotated_point = origin_location + (current_orientation_quat * (geometry->getVertex(i) - origin_location));
			geometry->setVertex(i, rotated_point);
		}
		geometry->update_geometry();

		previous_orientation_quat  = current_orientation_quat;
		previous_orientation_euler = current_orientation_euler;

		change_children_orientation(current_orientation_quat, delta_rotation);
	}

	// recursive pre-order function to translate all children rotation relative to parent to be consistent with a parent change of orientation
	void change_children_orientation(glm::quat current_orientation_quat, glm::vec3 delta_rotation) {
		glm::vec3 rotated_point;

		for (entity_object_basis_class *child : entity_children) {
			if (child != NULL) {
				for (size_t i = 0; i < child->geometry->get_number_of_vertices(); i++) {
					rotated_point = origin_location + (current_orientation_quat * (child->geometry->getVertex(i) - origin_location));
					child->geometry->setVertex(i, rotated_point);
				}
				child->geometry->update_geometry();
				child->origin_location   = origin_location + (current_orientation_quat * (child->origin_location - origin_location));
				child->current_position  = child->origin_location;
				child->previous_position = child->origin_location;
				//rotate current child orientation by defined change of parent orientation
				child->current_orientation_euler += delta_rotation;
				child->setOrientation(child->current_orientation_euler);
				child->previous_orientation_euler = child->current_orientation_euler;
				child->previous_orientation_quat  = child->getOrientationQuat();


				child->change_children_orientation(current_orientation_quat, delta_rotation);
			}
//cout << "in change_orientation 02: " <<child->origin_location.x << " : " << child->origin_location.y << " : " << child->origin_location.z << std::endl;		
		}
	}

	// \brief Classes extending ofNode can override this methods to get notified 
	//       when the scale changed.
	// openframeWorks onScaleChanged() is unreliable : unexplained crashes of app occurs
	// within a for loop to change each individual vertex location for no apparent reason
	// other than within openframeWorks a bug exists.

	void change_scale() {
		glm::vec3 scale_factor;
		glm::vec3 relative_location;
		glm::vec3 s = getScale();

//cout << "in onScaleChanged 01: " << origin_location.x << " : " << origin_location.y << " : " << origin_location.z << std::endl;

		int nv = geometry->get_number_of_vertices();

		for (size_t i = 0; i < nv; i++) {
			relative_location = geometry->getVertex(i) - origin_location;
			scale_factor = relative_location * s;
		 	geometry->setVertex(i, origin_location + scale_factor);
		}
		geometry->update_geometry();

		change_children_scale(s);
	}

	void change_children_scale(glm::vec3 s) {
		glm::vec3 scale_factor;
		glm::vec3 relative_location;

		for (entity_object_basis_class* child : entity_children) {
			if (child != NULL) {
				for (size_t i = 0; i < child->geometry->get_number_of_vertices(); i++) {
					relative_location = child->geometry->getVertex(i) - child->origin_location;
					scale_factor = relative_location * s;
					child->geometry->setVertex(i, child->origin_location + scale_factor);
				}
				child->geometry->update_geometry();
				child->change_children_scale(s);
			}
		}
	}

	void add_child(entity_object_basis_class *entity_object) {
		if (entity_object == NULL) return;
		if (get_child(entity_object->entity_id) != NULL) return; // child of entity_id aready exists
		entity_children.push_back(entity_object);
	}

	// This needs to be implemented as a recursive function
	entity_object_basis_class *get_child(id_type entity_id) {
		for (entity_object_basis_class* child : entity_children) {
			if (child->entity_id == entity_id)
				return child;
		}
		return NULL;
	}



	// other functions needed to 

	// *************************
	



	// *************************

	struct entity_associations_struct_type {
		entity_object_basis_class *entity;
		int                        association_type;
	};

	//std::vector<entity_associations_struct_type> entity_associations;
	entity_associations_class entity_associations;

	// Empty basis classes that can have differently defined derived classes 
	// in the application that can be referenced in this one central class
	//entity_object_attribute_struct      attribute;// provided by ofNode
	entity_base_geometry_class         *geometry                 = NULL;
	ofShader						   *geometry_shader          = NULL;// Should this be in entity_base_geometry_class ???? needs testing 
	entity_gui_basis_class             *entity_gui               = NULL;
	entity_node_editor_basis_class     *entity_node_editor_nodes = NULL;



	// Assume for the time being that geometry data is not linked and originates in another object

	void delete_geometry(){
		//for (entity_associations_struct_type entity_association : entity_associations) {
		//	if (entity_association.association_type == ENTITY_ASSOCIATION_GEOMETRY_LINKED) {
		//		geometry = NULL;
		//		return;
		//	}
		//}

		geometry->delete_geometry(true);
		geometry = NULL;
	}

	void delete_geometry_shader() {
		//for (entity_associations_struct_type entity_association : entity_associations) {
		//	if (entity_association.association_type == ENTITY_ASSOCIATION_SHADER_LINKED) {
		//		geometry_shader = NULL;
		//		return;
		//	}
		//}

		geometry_shader->unload();
		geometry_shader = NULL;
	}

	void clear_entity_associations() {
		entity_associations.clear_entity_associations();
	}

	// Following function cannot be placed within entity_associations_class as
	// entity_object_basis_class struicture cannot be defined before entity_associations_class
	// and vice versa. Solution can be to insert entity_associations_class methods and structures
	// into this class which will be considered if entity_associations_class is not needed to be
	// used elsewhere.
	bool delete_all_entity_associations_with_entity_id(int entity_id) {
		if (entity_id < 0) return false;

		for (size_t i = entity_associations.entity_associations.size() - 1; i >= 0; i--) {
			if (entity_associations.entity_associations[i].entity->entity_id == entity_id) {
				entity_associations.remove_entity_association(i);
			}
		}
		return true;
	}

	// -----------------------------------------

	// following Needs testing
	bool render_entity_object() {
		if (!display_object) return false;

		if (geometry_shader == NULL) {
			//error message shader not defined
			return false;
		}

		if (geometry_shader->getProgram() < 1) {
			//error message shader program not defined
			return false;
		}

		void render_using_shader_material();

		return true;
	}


protected:
	void render_using_shader_material() {
		geometry_shader->begin();
		geometry->render();
		geometry_shader->end();
	}
private:
	glm::vec3 previous_position , current_position;
	glm::vec3 origin_location;
	glm::vec3 local_location, global_location;

	glm::quat current_orientation_quat, previous_orientation_quat;
	glm::vec3 current_orientation_euler, previous_orientation_euler;

};
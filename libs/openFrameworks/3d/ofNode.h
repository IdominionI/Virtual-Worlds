
#pragma once

#include "ofParameter.h"
#include <glm/gtc/quaternion.hpp>
#include "ofConstants.h"
#include "glm/mat4x4.hpp"
#include <array>

// VWCUSTOM ++++++++++++++++++
//#include "glm/trigonometric.hpp"
//#include <cmath>

#include "../types/vw_definitions.h"
#include "../Geometry/vw_entity_object_geometry.h"
// +++++++++++++++++++++++++++

class ofBaseRenderer;

// VWCUSTOM ADDITION BEGIN ++++++++++++++++++
enum class node_gizmo_display_enum { none, axis, orientation, scale };
enum class axis_enum {x_axis, y_axis, z_axis, world};

class ofNode;
class node_associations_class {
public:
	~node_associations_class() {
		clear_node_associations();
	}

	struct node_associations_struct_type {
		ofNode* node;
		int            association_type;
	};

	std::vector<node_associations_struct_type> node_associations;

	bool  add_node_association(ofNode* entity, int association_type) {
		if (entity == NULL) return false;

		node_associations_struct_type node_association;
		node_association.association_type = association_type;
		node_association.node = entity;

		node_associations.push_back(node_association);
		return true;
	}

	void clear_node_associations() {
		node_associations.clear();
	}

	bool remove_all_node_associations_for_node(ofNode* node) {
		if (node == NULL) return false;
		for (size_t i = node_associations.size() - 1; i >= 0; i--) {
			if (node_associations[i].node == node) {
				remove_node_association(i);
			}
		}
		return true;
	}

	void remove_node_association(size_t index) {
		if (index < node_associations.size() && index >= 0) {
			node_associations.erase(node_associations.begin() + index);
		}
	}
private:
};
// ADDITION END +++++++++++++++++++++++++++

/// \brief A generic 3d object in space with transformation (position, rotation, scale).

class ofNode {
public:
	/// \cond INTERNAL

	ofNode();
	virtual ~ofNode();
	ofNode(const ofNode& node);
	ofNode(ofNode&& node);
	ofNode& operator=(const ofNode& node);
	ofNode& operator=(ofNode&& node);

	/// \endcond

	// VWCUSTOM ADDITION BEGIN++++++++++++++++++
	int         id = INVALID_ID;
	std::string name = "";
	std::string description = "";
	bool        enabled = true;
	// ADDITION END ++++++++++++++++++++++++++++++++++++++

	/// \name Parent Node
	/// \{

	/// \brief Set parent for the node. The node will inherit transformations from parent.
	///
	/// \param parent Reference to the ofNode which becomes the parent node.
	/// \param bMaintainGlobalTransform Boolean if maintain child's global
	///        transformations (default = false). 
	void setParent(ofNode& parent, bool bMaintainGlobalTransform = false);

	/// \brief Remove parent node linking.
	///
	/// \param bMaintainGlobalTransform Boolean if maintain child's global
	///        transformations (default = false).
	void clearParent(bool bMaintainGlobalTransform = false);

	/// \brief Get the parent node of this node.
	///
	/// \returns Pointer to parent ofNode.
	ofNode* getParent() const;

	/// \}
	/// \name Getters
	/// \{

	/// \brief Get node's local position as a 3D vector.
	///
	/// \returns A 3D vector with the local coordinates.
	glm::vec3 getPosition() const;

	/// \brief Get node's local x position. 
	/// 
	/// \returns Local x coordinate as a float.
	float getX() const;

	/// \brief Get node's local y position.
	///
	/// \returns Local y coordinate as a float.
	float getY() const;

	/// \brief Get node's local z position.
	///
	/// \returns Local z coordinate as a float.
	float getZ() const;

	/// \brief Get the node's local x axis as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local x axis direction.
	glm::vec3 getXAxis() const;

	/// \brief Get the node's local y axis as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local y axis direction.
	glm::vec3 getYAxis() const;

	/// \brief Get the node's local z axis as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local z axis direction.
	glm::vec3 getZAxis() const;

	/// \brief Get direction of node's side aka local x axis, as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local x axis direction.
	glm::vec3 getSideDir() const;

	/// \brief Get direction the node looks at aka local -z axis, as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local -z axis direction.
	glm::vec3 getLookAtDir()const;

	/// \brief Get direction of node's top aka local y axis, as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local y axis direction.
	glm::vec3 getUpDir() const;

	OF_DEPRECATED_MSG("Use Deg/Rad versions.", float getPitch() const);
	OF_DEPRECATED_MSG("Use Deg/Rad versions.", float getHeading() const);
	OF_DEPRECATED_MSG("Use Deg/Rad versions.", float getRoll() const);

	/// \brief Get pitch of node, aka the rotation along local x axis.
	/// \returns The rotation around the local x axis in degrees, as a float.
	float getPitchDeg() const;

	/// \brief Get heading of node, aka the rotation along local y axis.
	/// \returns The rotation around the local y axis in degrees, as a float.
	float getHeadingDeg() const;

	/// \brief Get roll of node, aka the rotation along local z axis.
	/// \returns The rotation around the local z axis in degrees, as a float.
	float getRollDeg() const;

	/// \brief Get pitch of node, aka the rotation along local x axis.
	/// \returns The rotation around the local x axis in degrees, as a float.
	float getPitchRad() const;

	/// \brief Get heading of node, aka the rotation along local y axis.
	/// \returns The rotation around the local y axis in degrees, as a float.
	float getHeadingRad() const;

	/// \brief Get roll of node, aka the rotation along local z axis.
	/// \returns The rotation around the local z axis in degrees, as a float.
	float getRollRad() const;

	/// \brief Get the local orientation of the node as a quaternion.
	/// \returns A quaternion of local orientation (useful for complex rotations)
	glm::quat getOrientationQuat() const;

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", glm::vec3 getOrientationEuler() const);

	/// \brief Get local orientation of node in degrees around x, y, and z axes.
	/// \returns The local x, y and z axes orientation in degrees, as a 3D vector.
	glm::vec3 getOrientationEulerDeg() const;

	/// \brief Get local orientation of node in degrees around x, y, and z axes.
	/// \returns The local x, y and z axes orientation in degrees, as a 3D vector.
	glm::vec3 getOrientationEulerRad() const;

	/// \brief Get local scale of node in xyz axes where 1 is default.
	///
	/// \returns The local scale in the xyz axes where 1 = 100% of size.
	glm::vec3 getScale() const;

	/// \brief Get node's local transformations (position, orientation, scale).
	/// 
	/// \returns A refrence to mat4 containing node's local transformations.
	/// \sa https://open.gl/transformations
	const glm::mat4& getLocalTransformMatrix() const;

	// TODO: optimize and cache these
	// (parent would need to know about its children so it can inform them 
	// to update their global matrices if it itself transforms)

	/// \brief Get node's global transformations (position, orientation, scale).
	/// \returns A refrence to mat4 containing node's global transformations.
	/// \sa https://open.gl/transformations
	glm::mat4 getGlobalTransformMatrix() const;

	/// \brief Get node's global position as a 3D vector.
	/// \returns A 3D vector with the global coordinates.
	glm::vec3 getGlobalPosition() const;

	/// \brief Get the global orientation of the node as a quaternion.
	/// \returns An quaternion of the global orientations(useful for complex rotations)
	glm::quat getGlobalOrientation() const;

	/// \brief Get global scale of node in xyz axes where 1 is default.
	///
	/// \returns The global scale in the xyz axes where 1 = 100% of size.
	glm::vec3 getGlobalScale() const;

	/// \}
	/// \name Setters
	/// \{

	/// \brief Set the local position of the node using xyz coordinates.
	///
	/// \param px Desired local x coordinate as a float.
	/// \param py Desired local y coordinate as a float.
	/// \param pz Desired local z coordinate as a float.
	//void setPosition(float px, float py, float pz);
	void setPosition(float px, float py, float pz);

	/// \brief Set the local position of the node using a 3D vector of coordinates.
	/// 
	/// \param p Desired local xyz coordinates as ref to 3D vector.	
	//void setPosition(const glm::vec3& p);
	void setPosition(const glm::vec3& p);

	/// \brief Set the global position of the node using xyz coordinates.
	///
	/// \param px Desired global x coordinate as a float.
	/// \param py Desired global y coordinate as a float.
	/// \param pz Desired global z coordinate as a float.	
	void setGlobalPosition(float px, float py, float pz);

	/// \brief Set the global position of the node using a 3D vector of coordinates.
	///
	/// \param p Desired global xyz coordinates as ref to 3D vector.
	void setGlobalPosition(const glm::vec3& p);

	/// \brief Set local orientation with a quaternion.
	///
	/// \param q Desired local orientation as ref to an glm::quat.
	void setOrientation(const glm::quat& q);

	/// \brief Set local orientation with xyz euler angles.
	///
	/// \param eulerAngles Desired local xyz angles in degrees, as ref to 3D vector.
	/// \note Using euler angles can cause gimbal lock.
	/// \sa https://en.wikipedia.org/wiki/Gimbal_lock
	void setOrientation(const glm::vec3& eulerAngles);

	/// \brief Set global orientation with a quaternion.
	///
	/// \param q Desired global orientation as ref to an glm::quat.
	void setGlobalOrientation(const glm::quat& q);

	/// \brief Set local uniform scale (x, y, and z are equally scaled).
	///
	/// \param s Desired scale for all axes as a float where 1 = 100%.
	void setScale(float s);

	/// \brief Set local scale for xyz axes individually.
	///
	/// \param sx Desired local scale for x axis as a float where 1 = 100%.
	/// \param sy Desired local scale for y axis as a float where 1 = 100%.
	/// \param sz Desired local scale for z axis as a float where 1 = 100%.
	void setScale(float sx, float sy, float sz);

	/// \brief Set local scale for xyz axes individually with a 3D vector.
	///
	/// \param s Desired local scale for all axes as ref to 3D vector where 1 = 100%.
	void setScale(const glm::vec3& s);

	/// \}
	/// \name Modifiers
	/// \{

	/// \brief Move node by relative amount with xyz coordinates.
	///
	/// \param x Desired relative position change along x axis as a float.
	/// \param y Desired relative position change along y axis as a float.
	/// \param z Desired relative position change along z axis as a float.
	void move(float x, float y, float z);

	/// \brief Move node by relative amount with xyz as ref to 3D vector.
	/// \param offset Desired relative position change along all axes as ref to 3D vector.
	void move(const glm::vec3& offset);

	/// \brief Move node left+right relative to current position (in local x axis).
	///
	/// \param amount Desired relative position change along local x axis as float.
	void truck(float amount);

	/// \brief Move node up+down relative to current position (in local y axis).
	///
	/// \param amount Desired relative position change along local y axis as float.
	void boom(float amount);

	/// \brief Move node backward+forward relative to current position (in local z axis).
	///
	/// \param amount Desired relative position change along local z axis as float.
	void dolly(float amount);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void tilt(float degrees));

	/// \brief Tilt up+down relative to current orientation (around local x axis).
	///
	/// \param degrees Desired relative rotation change along local x axis in degrees as float.
	void tiltDeg(float degrees);

	/// \brief Tilt up+down relative to current orientation (around local x axis).
	///
	/// \param radians Desired relative rotation change along local x axis in radians as float.
	void tiltRad(float radians);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void pan(float degrees));

	/// \brief Rotate left+right relative to current orientation (around local y axis).
	///
	/// \param degrees Desired relative rotation change along local y axis in degrees as float.
	void panDeg(float degrees);

	/// \brief Rotate left+right relative to current orientation (around local y axis).
	///
	/// \param radians Desired relative rotation change along local y axis in radians as float.
	void panRad(float radians);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void roll(float degrees));

	/// \brief Roll left+right relative to current orientation (around local z axis).
	///
	/// \param degrees Desired relative rotation change along local z axis in degrees as float.
	void rollDeg(float degrees);

	/// \brief Roll left+right relative to current orientation (around local z axis).
	///
	/// \param radians Desired relative rotation change along local z axis in radians as float.
	void rollRad(float radians);

	/// \brief Rotate relative to current orientation by quaternion.
	///
	/// \param q Desired relative rotation change as a ref to quaternion.
	void rotate(const glm::quat& q);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void rotate(float degrees, const glm::vec3& v));

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param v Desired axis to rotate around as a ref to cartesian 3D Vector.
	void rotateDeg(float degrees, const glm::vec3& v);

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param radians Desired relative angle change in radians as float.
	/// \param v Desired axis to rotate around as a ref to cartesian 3D Vector.
	void rotateRad(float radians, const glm::vec3& v);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void rotate(float degrees, float vx, float vy, float vz));

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param vx X angle of the axis to rotate around in degrees as float.
	/// \param vy Y angle of the axis to rotate around in degrees as float.
	/// \param vz Z angle of the axis to rotate around in degrees as float.
	void rotateDeg(float degrees, float vx, float vy, float vz);

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param radians Desired relative angle change in radians as float.
	/// \param vx X angle of the axis to rotate around in radians as float.
	/// \param vy Y angle of the axis to rotate around in radians as float.
	/// \param vz Z angle of the axis to rotate around in radians as float.
	void rotateRad(float radians, float vx, float vy, float vz);

	/// \brief Rotate relative to current orientation by quaternion around point.
	///
	/// \param q Desired relative rotation change as a ref to quaternion.
	/// \param point Point to rotate around in local xyz coordinates as ref to 3D vector.	
	void rotateAround(const glm::quat& q, const glm::vec3& point);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void rotateAround(float degrees, const glm::vec3& axis, const glm::vec3& point));

	/// \brief Rotate relative to current orientation around arbitrary axis around point.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param axis The arbitrary axis to rotate around as ref to cartesian 3D vector.
	/// \param point Point to rotate around in local xyz coordinates as ref to 3D vector.
	void rotateAroundDeg(float degrees, const glm::vec3& axis, const glm::vec3& point);

	/// \brief Rotate relative to current orientation around arbitrary axis around point.
	///
	/// \param radians Desired relative angle change in degrees as float.
	/// \param axis The arbitrary axis to rotate around as ref to cartesian 3D vector.
	/// \param point Point to rotate around in local xyz coordinates as ref to 3D vector.
	void rotateAroundRad(float radians, const glm::vec3& axis, const glm::vec3& point);

	/// \brief Orient node to look at point (-z axis pointing to global position).
	///
	/// \param lookAtPosition XYZ coordinates of point to look at as ref to 3D vector.
	/// \note This version calculates the up vector automatically to try to keep
	///       it relatively consistant with the original angle.
	void lookAt(const glm::vec3& lookAtPosition);

	/// \brief Orient node to look at point (-z axis pointing to global position).
	/// \param lookAtPosition XYZ coordinates of point to look at as ref to 3D vector.
	/// \param upVector The desired up axis as a cartesian 3D vector.
	void lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector);

	/// \brief Orient node to look at another node (-z axis pointing at other node).
	///
	// \param lookAtNode A reference to the node to look at.
	/// \note This version calculates the up vector automatically to try to keep
	///       it relatively consistant with the original angle.
	void lookAt(const ofNode& lookAtNode);

	/// \brief Orient node to look at another node (-z axis pointing at other node).
	/// \param lookAtNode A reference to the node to look at.
	/// \param upVector The desired up axis as a ref to cartesian 3D vector.
	void lookAt(const ofNode& lookAtNode, const glm::vec3& upVector);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void orbit(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0)));
	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void orbit(float longitude, float latitude, float radius, ofNode& centerNode));

	/// \brief Orbit node around a global position at a specific radius.
	///
	/// \param longitude The longitudinal position of the node in degrees as float.
	/// \param latitude The latitudinal position of the node in degrees as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerPoint The global position to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitDeg(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0));

	/// \brief Orbit node around another node at a specific radius.
	/// \param longitude The longitudinal position of the node in degrees as float.
	/// \param latitude The latitudinal position of the node in degrees as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerNode The global position to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitDeg(float longitude, float latitude, float radius, ofNode& centerNode);

	/// \brief Orbit node around a global position at a specific radius.
	///
	/// \param longitude The longitudinal position of the node in radians as float.
	/// \param latitude The latitudinal position of the node in radians as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerPoint The global node to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitRad(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0));

	/// \brief Orbit node around another node at a specific radius.
	/// \param longitude The longitudinal position of the node in radians as float.
	/// \param latitude The latitudinal position of the node in radians as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerNode The global node to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitRad(float longitude, float latitude, float radius, ofNode& centerNode);

	/// \}
	/// \name OpenGL Transformation
	/// \{

	/// \brief Set opengl renderer's modelview matrix to this nodes transform.
	/// \param renderer A pointer to the renderer you want to set to this node's transform;
	/// \note If you want to draw something at the position+orientation+scale of this node,
	/// call ofNode::transform(); write your draw code, and ofNode::restoreTransform();
	/// OR A simpler way is to extend ofNode and override ofNode::customDraw();
	void transformGL(ofBaseRenderer* renderer = nullptr) const;

	/// \brief Restore opengl renderer's previous modelview transform matrix.
	/// \param renderer A pointer to the renderer you want to restore transformation to.
	void restoreTransformGL(ofBaseRenderer* renderer = nullptr) const;


	/// \brief Reset this node's transformations, position, rotation and scale.
	void resetTransform();

	/// \}
	/// \name Drawing
	/// \{

	/// \brief If you extend ofNode and wish to change the way it draws, extend this.
	/// \param renderer A pointer to the renderer you want to draw to.
	/// \note Try to not use global functions for rendering and instead use the passed
	/// renderer.
	virtual void customDraw(const ofBaseRenderer* renderer) const;

	/// \brief If you extend ofNode and wish to change the way it draws, extend this.
	/// \note Try to not use global functions for rendering and instead use the passed
	/// renderer.
	virtual void customDraw();


	/// \brief Draw the node as a white cube with xyz axes. 
	/// \note do NOT override this. 
	/// It transforms the node to its position+orientation+scale
	/// and calls the virtual 'customDraw' method above which you CAN override.
	virtual void draw() const;

	/// \}

	// VWCUSTOM ADDITION BEGIN ++++++++++++++++++
	std::vector<ofNode*> node_children;

	entity_base_geometry_class *geometry = NULL;
	//ofShader						   *geometry_shader          = NULL;// Should this be here ???? needs testing 

	node_associations_class node_associations;

	bool  display_bounding_limits = false;

	bool  display_normals = false;
	float normals_length = 20.0;
	bool  draw_normal_faces = false;
	glm::vec4 normal_colors = { 1.0,0.0,1.0,1.0 };

	float axis_size = 50.0f;

	node_gizmo_display_enum gizmo_display = node_gizmo_display_enum::none;

	// ---------- Children functions -------------------------
	void draw_geometry()  const;

	void add_child(ofNode* entity_object) {
		if (entity_object == NULL) return;
		if (get_child(entity_object->id) != NULL) return; // child of entity_id aready exists
		node_children.push_back(entity_object);
	}

	// This needs to be implemented as a recursive function
	ofNode* get_child(int entity_id) {
		for (ofNode* child : node_children) {
			if (child->id == entity_id)
				return child;
		}
		return NULL;
	}

	void unparent_node_tree_children(ofNode* node) {
		for (ofNode* child : node->node_children) {
			unparent_node_tree_children(child);
			child->clearParent(true);
		}

		node->node_children.clear();
	}

	void delete_node_tree_children(ofNode* node) {
		for (ofNode* child : node->node_children) {
			unparent_node_tree_children(child);
			child->clearParent(true);
			delete child;
		}

		node->node_children.clear();
	}

	// -------------------------------------------------------

	void correct_tree_node_data(ofNode* node) {
		bool bMaintainGlobalTransform = true;

		for (ofNode* child : node->node_children) {
			child->setGlobalOrientation(child->current_orientation_quat);
			child->setGlobalPosition(child->origin_location);
			child->setScale(child->current_scale);

			correct_tree_node_data(child);
		}
	}

	// ----------------- Set functions ----------------------

	void set_local_location(float px, float py, float pz, bool translate_geometry, bool translate_children) {
		set_local_location(glm::vec3{ px, py, pz }, translate_geometry, translate_children);
	}

	void set_local_location(const glm::vec3 p, bool translate_geometry, bool translate_children) {
		setPosition(p);
		if (translate_geometry)  update_geometry_location(translate_children);
	}

	void set_global_location(float px, float py, float pz, bool translate_geometry, bool translate_children) {
		set_global_location(glm::vec3{ px, py, pz }, translate_geometry, translate_children);
	}

	void set_global_location(const glm::vec3 p, bool translate_geometry, bool translate_children) {
		setGlobalPosition(p);

		if (translate_geometry)  update_geometry_location(translate_children);
	}

	void set_local_orientation(float pitch, float yaw, float roll, bool translate_geometry, bool translate_children) {
		set_local_orientation(glm::vec3{ pitch, yaw, roll }, translate_geometry, translate_children);
	}

	void set_local_orientation(glm::vec3 lorientation, bool translate_geometry, bool translate_children) {
		setOrientation(lorientation);

		if (translate_geometry) update_geometry_orientation(translate_children);
	}

	void set_global_orientation(float pitch, float yaw, float roll, bool translate_geometry, bool translate_children) {
		set_global_orientation(glm::vec3{ pitch, yaw, roll }, translate_geometry, translate_children);
	}

	void set_global_orientation(glm::vec3 gorientation, bool translate_geometry, bool translate_children) {
		glm::quat q(glm::radians(gorientation));
		setGlobalOrientation(q);

		if (translate_geometry) update_geometry_orientation(translate_children);
	}

	// These scale functions are for global coordinates
	void set_global_scale(float scale, bool translate_geometry, bool translate_children) {
		set_global_scale(glm::vec3{ scale, scale, scale }, translate_geometry, translate_children);
	}

	void set_global_scale(float x_scale, float y_scale, float z_scale, bool translate_geometry, bool translate_children) {
		set_global_scale(glm::vec3{ x_scale, y_scale, z_scale }, translate_geometry, translate_children);
	}

	void set_global_scale(glm::vec3 scale, bool translate_geometry, bool translate_children) {
		//setScale(getScale()*scale);
		if (translate_geometry) update_geometry_scale(scale, translate_children, false);
	}

	void set_local_scale(float scale, bool translate_geometry, bool translate_children) {
		set_local_scale(glm::vec3{ scale, scale, scale }, translate_geometry, translate_children);
	}

	void set_local_scale(float x_scale, float y_scale, float z_scale, bool translate_geometry, bool translate_children) {
		set_local_scale(glm::vec3{ x_scale, y_scale, z_scale }, translate_geometry, translate_children);
	}

	void set_local_scale(glm::vec3 scale, bool translate_geometry, bool translate_children) {
		//setScale(getScale()*scale);
		if (translate_geometry) update_geometry_scale(scale, translate_children, true);
	}

	// -------------------- Basic node geometry transformation functions     -----------------------------------
	// Location
	void translate_local_location(float x, float y, float z, bool translate_geometry, bool translate_children) {
		translate_local_location(glm::vec3{ x, y, z }, translate_geometry, translate_children);
	}

	void translate_local_location(glm::vec3 translation, bool translate_geometry, bool translate_children) {
		truck(translation.x);
		boom(translation.y);
		dolly(translation.z);

		if (translate_geometry) update_geometry_location(translate_children);
	}

	void translate_global_location(float x, float y, float z, bool translate_geometry, bool translate_children) {
		translate_global_location(glm::vec3{ x, y, z }, translate_geometry, translate_children);
	}

	void translate_global_location(glm::vec3 translation, bool translate_geometry, bool translate_children) {
		glm::vec3 gp = getGlobalPosition();
		gp += translation;
		setGlobalPosition(gp);

		if (translate_geometry) update_geometry_location(translate_children);
	}

	void translate_local_radius(float radial_translation, bool translate_geometry, bool translate_children) {
		if (!parent) return; // Have no parent that is the origin of a local coordinate system
		if (glm::length(position.get()) + radial_translation < 0) return; // radial translation is negative and resultant translation 
																		  // has a negative radius which cannot exist in 3d space

		glm::vec3 radial_vector     = glm::normalize(position.get());	  // Unit radial vector
		glm::vec3 local_translation = radial_vector * radial_translation; // translation radial vecto

		move(local_translation);

		if (translate_geometry) update_geometry_location(translate_children);
	}

	//rotations 
	void pitch(float angle_degrees, bool translate_geometry, bool translate_children) {
		tiltDeg(angle_degrees);

		if (translate_geometry) update_geometry_orientation(translate_children);
	}

	void yaw(float angle_degrees, bool translate_geometry, bool translate_children) {
		panDeg(angle_degrees);

		if (translate_geometry) update_geometry_orientation(translate_children);
	}

	void roll(float angle_degrees, bool translate_geometry, bool translate_children) {
		roll(angle_degrees);

		if (translate_geometry) update_geometry_orientation(translate_children);
	}

	//:: Folowing rotations Note tested
	void rotate_about_orgin(float degrees, const glm::vec3& axis, bool translate_geometry, bool translate_children) {
		if (!parent) return; // Have no parent that is the origin of a local coordinate system

		const glm::vec3 origin = parent->getGlobalPosition();

		 rotateAroundDeg(degrees, axis, origin);

		 if (translate_geometry) {
			 update_geometry_location(translate_children);
			 update_geometry_orientation(translate_children);
		 }
	}

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param vx X angle of the axis to rotate around in degrees as float.
	/// \param vy Y angle of the axis to rotate around in degrees as float.
	/// \param vz Z angle of the axis to rotate around in degrees as float.
	void rotate_about_axis(float degrees, float vx, float vy, float vz, bool translate_geometry, bool translate_children) {
		rotateDeg(degrees, vx, vy, vz);

		if (translate_geometry) {
			update_geometry_location(translate_children);
			update_geometry_orientation(translate_children);
		}
	}

	/// \brief Rotate relative to current orientation around arbitrary axis around point.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param axis The arbitrary axis to rotate around as ref to cartesian 3D vector.
	/// \param point Point to rotate around in local xyz coordinates as ref to 3D vector.
	void rotate_around_point(float degrees, const glm::vec3& axis, const glm::vec3& point, bool translate_geometry, bool translate_children) {
		rotateAroundDeg(degrees, axis,point);

		if (translate_geometry) {
			update_geometry_location(translate_children);
			update_geometry_orientation(translate_children);
		}
	}

	/// \brief Orbit node around a global position at a specific radius.
	///
	/// \param longitude The longitudinal position of the node in degrees as float.
	/// \param latitude The latitudinal position of the node in degrees as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerPoint The global position to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbit_about_location(float longitude, float latitude, float radius, bool translate_geometry, bool translate_children, const glm::vec3& centerPoint = glm::vec3(0, 0, 0)) {
		orbitDeg(longitude, latitude, radius, centerPoint);

		if (translate_geometry) {
			update_geometry_location(translate_children);
			update_geometry_orientation(translate_children);
		}
	}

	/// \brief Orbit node around another node at a specific radius.
	/// \param longitude The longitudinal position of the node in degrees as float.
	/// \param latitude The latitudinal position of the node in degrees as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerNode The global position to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbit_about_node(float longitude, float latitude, float radius, ofNode& centerNode, bool translate_geometry, bool translate_children) {
		orbitDeg( longitude,latitude, radius,centerNode);

		if (translate_geometry) {
			update_geometry_location(translate_children);
			update_geometry_orientation(translate_children);
		}
	}

	// -----------------------------------------------------------------------------

	void update_geometry_location(bool translate_children) {
		change_geometry_position(translate_children);
		correct_tree_node_data(this);
	}

	void update_geometry_orientation(bool translate_children) {
		change_geometry_orientation(translate_children);
		correct_tree_node_data(this);
	}

	void update_geometry_scale(glm::vec3 scale, bool translate_children, bool scale_local) {
		change_geometry_scale(scale, translate_children, scale_local);
		correct_tree_node_data(this);
	}

	// -----------------------------------------------------------------------------
	// Basic node funtions to Translate, orientate and scale node geometry
	// Of does nor offer intuitive and easy methods of performing this for localised
	// modifications using its internal and very strangly behaved methods of updating
	// node data that has children and parents. took days of experimentation to overcome
	// what was thought should have been solved. In particular the methods and results
	// of orientation seemed to add parent orientation to the children and totaly mess
	// up the final child axis orientations. 

	void change_geometry_position(bool translate_children) {
		current_position = getGlobalPosition();
		glm::vec3 eo_translation = current_position - previous_position;
//std::cout << "in onPositionChanged 01: " << current_position.x << " : " << current_position.y << " : " << current_position.z << std::endl;
//std::cout << "in onPositionChanged 01A: " << previous_position.x << " : " << previous_position.y << " : " << previous_position.z << std::endl;
//std::cout << "in onPositionChanged 02A: " << eo_translation.x << " : " << eo_translation.y << " : " << eo_translation.z << std::endl;

		for (size_t i = 0; i < geometry->get_number_of_vertices(); i++) {
			//geometry->setVertex(i, geometry->getVertex(i) + eo_translation);
			glm::vec4 v = { eo_translation,0.0f };//+++++++++++++++++++
			geometry->setVertex(i, geometry->getVertex(i) + v);
		}

		geometry->update_geometry();
		if (translate_children) change_children_position(eo_translation);

		previous_position = current_position;
		origin_location   = current_position;
	}

	// recursive pre-order function to translate all children locations consistent with a parent change of location
	void change_children_position(glm::vec3 eo_translation) {
		for (ofNode* child : node_children) {
//std::cout << "in onPositionChanged 02: " << child->previous_position.x << " : " << child->previous_position.y << " : " << child->previous_position.z << std::endl;
//std::cout << "in onPositionChanged 02A: " << child->getGlobalPosition().x << " : " << child->getGlobalPosition().y << " : " << child->getGlobalPosition().z << std::endl;
			//change child geometry locations here

			if (child != NULL) {
				for (size_t i = 0; i < child->geometry->get_number_of_vertices(); i++) {
//child->geometry->setVertex(i, child->origin_location + child->geometry->getVertex(i) + eo_translation);
					//child->geometry->setVertex(i, child->geometry->getVertex(i) + eo_translation);
					glm::vec4 v = { eo_translation,0.0f };//+++++++++++++++++++
					child->geometry->setVertex(i, child->geometry->getVertex(i) + v);//+++++++++++++++++++
				}

				child->geometry->update_geometry();
				child->origin_location = child->getGlobalPosition();
				child->current_position = child->origin_location;
				child->previous_position = child->origin_location;

//std::cout << "in onPositionChanged 03: " << child->origin_location.x << " : " << child->origin_location.y << " : " << child->origin_location.z << std::endl;
//std::cout << "in onPositionChanged 03A: " << child->getGlobalPosition().x << " : " << child->getGlobalPosition().y << " : " << child->getGlobalPosition().z << std::endl;
				child->change_children_position(eo_translation);
			}
		}
		std::cout << std::endl;
	}

	// \brief Classes extending ofNode can override this methods to get notified
	//        when the orientation changed.
	// openframeWorks onOrientationChanged() is unreliable : unexplained crashes of app occurs
	// for no apparent reason when using geometry->setVertex(i, rotated_point); 
	// other than within openframeWorks a bug exists.
	// Need to suspect that onPositionChanged() may cause crash for no reason and needs
	// to be over looked.

	void change_geometry_orientation(bool translate_children) {
		glm::vec3 rotated_point;

		current_orientation_quat  = getOrientationQuat();
		current_orientation_euler = getOrientationEuler();

		for (size_t i = 0; i < geometry->get_number_of_vertices(); i++) {
			rotated_point = origin_location + (current_orientation_quat * (geometry->getVertex(i) - origin_location));
			geometry->setVertex(i, rotated_point);
		}

		for (size_t i = 0; i < geometry->getNumNormals(); i++) {
			rotated_point = current_orientation_quat * (geometry->getNormal(i));
			geometry->setNormal(i, rotated_point);
		}

		geometry->update_geometry();

		previous_orientation_quat  = current_orientation_quat;
		current_orientation_euler  = current_orientation_euler;
		previous_orientation_euler = current_orientation_euler;

		if (translate_children) change_children_orientation(current_orientation_quat);
	}

	// recursive pre-order function to translate all children rotation relative to parent to be consistent with a parent change of orientation
	void change_children_orientation(glm::quat current_orientation_quat){
		glm::vec3 rotated_point;

		for (ofNode* child : node_children) {
			if (child != NULL) {
				for (size_t i = 0; i < child->geometry->get_number_of_vertices(); i++) {
					rotated_point = child->getGlobalPosition() + (current_orientation_quat * (child->geometry->getVertex(i) - child->origin_location));
					child->geometry->setVertex(i, rotated_point);
				}

				for (size_t i = 0; i < child->geometry->getNumNormals(); i++) {
					rotated_point = current_orientation_quat * (child->geometry->getNormal(i));
					child->geometry->setNormal(i, rotated_point);
				}

				child->geometry->update_geometry();
				child->origin_location   = child->getGlobalPosition();
				child->current_position  = child->origin_location;
				child->previous_position = child->origin_location;
				
				//rotate current child orientation by defined change of parent orientation
				child->setOrientation(current_orientation_quat);
				child->current_orientation_quat   = current_orientation_quat;
				child->current_orientation_euler  = child->getOrientationEulerDeg();
				child->previous_orientation_euler = child->current_orientation_euler;

				child->change_children_orientation(current_orientation_quat);
			}
//cout << "in change_orientation 02: " <<child->origin_location.x << " : " << child->origin_location.y << " : " << child->origin_location.z << std::endl;		
		}
	}

	// \brief Classes extending ofNode can override this methods to get notified 
	//       when the scale changed.
	// openframeWorks onScaleChanged() is unreliable : unexplained crashes of app occurs
	// within a for loop to change each individual vertex location for no apparent reason
	// other than within openframeWorks a bug exists.
	void change_geometry_scale(glm::vec3 scale, bool translate_children, bool scale_local) {
		glm::vec3 scale_factor;
		glm::vec3 relative_location;
		glm::vec3 rotated_point;

//std::cout << "in onScaleChanged 01: " << origin_location.x << " : " << origin_location.y << " : " << origin_location.z << std::endl;
//std::cout << "in onScaleChanged 01A: " << getGlobalPosition().x << " : " << getGlobalPosition().y << " : " << getGlobalPosition().z << std::endl;
//std::cout << "in onScaleChanged 01B: " << getXAxis().x << " : " << getXAxis().y << " : " << getXAxis().z << std::endl;

		//for efficiency reasons, best to no have if(scale_local) test within a for loop
		if (scale_local) {
			glm::mat4 ltm = getGlobalTransformMatrix();
			//glm::mat4 ltm = localTransformMatrix;// Does not allways work
			ltm = glm::scale(ltm, scale);
			ltm = ltm * glm::toMat4((const glm::quat&)current_orientation_quat);
			for (size_t i = 0; i < geometry->get_number_of_vertices(); i++) {
				relative_location = geometry->getVertex(i) - origin_location;
				scale_factor = (ltm * glm::vec4{ relative_location, 0.0 });
				geometry->setVertex(i, origin_location + scale_factor);
			}

			for (size_t i = 0; i < geometry->getNumNormals(); i++) {
				scale_factor = (ltm * glm::vec4{ geometry->getNormal(i), 0.0 });
				geometry->setNormal(i, scale_factor);
			}
		} else {// Global scaling
			for (size_t i = 0; i < geometry->get_number_of_vertices(); i++) {
				relative_location = geometry->getVertex(i) - origin_location;
				scale_factor = relative_location * scale;
				geometry->setVertex(i, origin_location + scale_factor);
			}

			for (size_t i = 0; i < geometry->getNumNormals(); i++) {
				scale_factor = geometry->getNormal(i) * scale;
				geometry->setNormal(i, scale_factor);
			}
		}

		previous_position = current_position;
		origin_location   = current_position;
		current_scale    *= current_scale;

//std::cout << "in onScaleChanged 01B: " << origin_location.x << " : " << origin_location.y << " : " << origin_location.z << std::endl;

		geometry->update_geometry();

		if (translate_children) change_children_scale(scale, scale_local);
	}

	void change_children_scale(glm::vec3 scale, bool scale_local) {
		glm::vec3 scale_factor;
		glm::vec3 relative_location;
		glm::vec3 rotated_point;

		for (ofNode* child : node_children) {
			if (child != NULL) {
//std::cout << "in onScaleChanged 02: " << child->origin_location.x << " : " << child->origin_location.y << " : " << child->origin_location.z << std::endl;
//std::cout << "in onScaleChanged 02A: " << child->getGlobalPosition().x << " : " << child->getGlobalPosition().y << " : " << child->getGlobalPosition().z << std::endl;
				//for efficiency reasons, best to no have if(scale_local) test within a for loop
				if (scale_local) {
					glm::mat4 ltm = child->getGlobalTransformMatrix();
					//glm::mat4 ltm = child->localTransformMatrix;// Child transform matrix seems to not work at all !!!!!
					ltm = glm::scale(ltm, scale);
					ltm = ltm * glm::toMat4((const glm::quat&)child->current_orientation_quat);

//std::cout << "in change_children_scale 03: " << child->current_orientation_quat.x << " : " << child->current_orientation_quat.y << " : " << child->current_orientation_quat.z << " : " << child->current_orientation_quat.w << std::endl;
//std::cout << "in change_children_scale 03A: " << current_orientation_quat.x << " : " << current_orientation_quat.y << " : " << current_orientation_quat.z << " : " << current_orientation_quat.w << std::endl;
//std::cout << "in change_children_scale 03B: " << child->getOrientationQuat().x << " : " << child->getOrientationQuat().y << " : " << child->getOrientationQuat().z << " : " << child->getOrientationQuat().w << std::endl;
					for (size_t i = 0; i < child->geometry->get_number_of_vertices(); i++) {
						relative_location = child->geometry->getVertex(i) - child->origin_location;
						scale_factor = (ltm * glm::vec4{ relative_location, 0.0 });
						child->geometry->setVertex(i, child->origin_location + scale_factor);
					}

					for (size_t i = 0; i < child->geometry->getNumNormals(); i++) {
						scale_factor = (ltm * glm::vec4{ child->geometry->getNormal(i), 0.0 });
						child->geometry->setNormal(i, scale_factor);
					}

					child->previous_position = child->current_position;
					child->origin_location = child->current_position;
				} else {
						relative_location = child->origin_location - origin_location;
					glm::vec3 child_scaled_location = origin_location + relative_location*scale;

					for (size_t i = 0; i < child->geometry->get_number_of_vertices(); i++) {
						relative_location = child->geometry->getVertex(i) - child->origin_location;
						scale_factor = relative_location * scale;
						child->geometry->setVertex(i, child_scaled_location + scale_factor);
					}

					for (size_t i = 0; i < child->geometry->getNumNormals(); i++) {
						scale_factor = child->geometry->getNormal(i)*scale;
						child->geometry->setNormal(i, scale_factor);
					}

					//child->origin_location   = child->getGlobalPosition();
					child->origin_location   = child_scaled_location;
					child->current_position  = child->origin_location;
					child->previous_position = child->origin_location;
				}


				child->current_scale *= scale;

//std::cout << "in onScaleChanged 02C: " << child->origin_location.x << " : " << child->origin_location.y << " : " << child->origin_location.z << std::endl;

				child->geometry->update_geometry();
				child->change_children_scale(scale, scale_local);
			}
		}
	}

	void draw_axis_gizmo() {
		transformGL(ofGetCurrentRenderer().get());// Node function
		ofGetCurrentRenderer()->drawAxis(axis_size);
		restoreTransformGL(ofGetCurrentRenderer().get());// Node function

		/*
		axis = geometry_axis;
		transform_geometry_GL(ofGetCurrentRenderer().get());// Node function
		ofGetCurrentRenderer()->drawAxis(axis_size);
		restoreTransformGL(ofGetCurrentRenderer().get());// Node function
		*/
	}
	virtual void draw_orientation_gizmo() {}
	virtual void draw_scale_gizmo() {}
	virtual void draw_bounding_limits() {}


	////--------------------------------------------------------------
	// The inability to have a pointer to this Node class defined in the entity_base_geometry_class
	// without a use of undefined type compile error means drawing normals data for geometry data needs
	// to be done here. Need a workaround or more likly a modification to fix this. Rediculous

	void draw_normals() {
		//draw_normals(normals_length, draw_normal_faces);
		geometry->draw_normals(normals_length, draw_normal_faces, normal_colors);
	}

	void draw_normals(float length, bool bFaceNormals) const {
		if (geometry == nullptr) return;

		//transformGL(ofGetCurrentRenderer().get());

		if (geometry->usingNormals()) {
			const auto& normals = geometry->getNormals();
			const auto& vertices = geometry->getVertices();
			glm::vec3 normal;
			glm::vec3 vert;

			geometry->normalsMesh.setMode(OF_PRIMITIVE_LINES);
			geometry->normalsMesh.getVertices().resize(normals.size() * 2);

			if (bFaceNormals) {
				for (size_t i = 0; i < normals.size(); i++) {
					if (i % 3 == 0) {
						vert = (vertices[i] + vertices[i + 1] + vertices[i + 2]) / 3;
					}
					else if (i % 3 == 1) {
						vert = (vertices[i - 1] + vertices[i] + vertices[i + 1]) / 3;
					}
					else if (i % 3 == 2) {
						vert = (vertices[i - 2] + vertices[i - 1] + vertices[i]) / 3;
					}
					geometry->normalsMesh.setVertex(i * 2, vert);
					normal = glm::normalize(toGlm(normals[i]));
					normal *= length;
					geometry->normalsMesh.setVertex(i * 2 + 1, vert + normal);
				}
			}
			else {
				for (size_t i = 0; i < normals.size(); i++) {
					vert = vertices[i];
					normal = glm::normalize(toGlm(normals[i]));
					geometry->normalsMesh.setVertex(i * 2, vert);
					normal *= length;
					geometry->normalsMesh.setVertex(i * 2 + 1, vert + normal);
				}
			}

			for (int i = 0; i < geometry->normalsMesh.getNumVertices(); i++)
				geometry->normalsMesh.addColor({ 255,0,0 });

			geometry->normalsMesh.enableColors();
			geometry->normalsMesh.draw();
		}
		else {
			ofLogWarning("of3dPrimitive") << "drawNormals(): mesh normals are disabled";
		}

		//restoreTransformGL(ofGetCurrentRenderer().get());
	}

	// Assume for the time being that geometry data is not linked and originates in another object

	void delete_geometry() {
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

		geometry->shader->unload();// This shader program is exclusive to this geometry object and not used elsewhere
		geometry->shader = NULL;
	}

	void clear_node_associations() {
		node_associations.clear_node_associations();
	}

	// Following function cannot be placed within entity_associations_class as
	// entity_object_basis_class struicture cannot be defined before entity_associations_class
	// and vice versa. Solution can be to insert entity_associations_class methods and structures
	// into this class which will be considered if entity_associations_class is not needed to be
	// used elsewhere.
	bool delete_all_entity_associations_with_entity_id(int node_id) {
		if (node_id < 0) return false;

		for (size_t i = node_associations.node_associations.size() - 1; i >= 0; i--) {
			if (node_associations.node_associations[i].node->id == node_id) {
				node_associations.remove_node_association(i);
			}
		}
	}

	bool display_axis = false;

	// ADDITION EMD ++++++++++++++++++++++++++++

protected:
	void createMatrix();
	void updateAxis();

	/// \brief Classes extending ofNode can override this method to get
	///        notified when the position changed.
	virtual void onPositionChanged() {}

	/// \brief Classes extending ofNode can override this methods to get notified
	///        when the orientation changed.
	virtual void onOrientationChanged() {}

	/// \brief Classes extending ofNode can override this methods to get notified 
	///        when the scale changed.
	virtual void onScaleChanged() {}

	ofNode* parent = nullptr;

private:
	void onParentPositionChanged(glm::vec3& position) { onPositionChanged(); }
	void onParentOrientationChanged(glm::quat& orientation) { onOrientationChanged(); }
	void onParentScaleChanged(glm::vec3& scale) { onScaleChanged(); }

	ofParameter<glm::vec3> position;
	ofParameter<glm::quat> orientation;
	ofParameter<glm::vec3> scale;

	std::array<glm::vec3, 3> axis;

	glm::mat4 localTransformMatrix;
	bool legacyCustomDrawOverrided;
	std::set<ofNode*> children;

	void addListener(ofNode& node);
	void removeListener(ofNode& node);
	//	glm::mat4 globalTransformMatrix;

		// VWCUSTOM ADDITION BEGIN ++++++++++++++++++++++++++++++++++
	glm::vec3 origin_location;
	glm::vec3 local_location, global_location;

	glm::vec3 previous_position, current_position;
	glm::quat current_orientation_quat, previous_orientation_quat;
	glm::vec3 current_orientation_euler, previous_orientation_euler;
	glm::vec3 previous_scale, current_scale;

		// ++++++++++++++++++++++++++++++++++++++++++
	glm::mat4 geometry_transform_matrix;

	ofParameter<glm::vec3> geometry_position    = glm::vec3{ 0.0,0.0,0.0 };
	ofParameter<glm::quat> geometry_orientation = glm::quat{ 0.0,0.0,0.0,0.0 };
	ofParameter<glm::vec3> geometry_scale       = glm::vec3{1.0,1.0,1.0};

	std::array<glm::vec3, 3> geometry_axis;
	// ADDITION END ++++++++++++++++++++++++++++++++++

};

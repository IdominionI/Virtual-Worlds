#pragma once

#include "ofNode.h"

class vw_object_base_class  {
public:
	vw_object_base_class() {}
	~vw_object_base_class() {}

	int  object_category_id = INVALID_ID;

	bool display_object = true;

	virtual bool update_object() { return false; }
	virtual bool define_vertices() { return false; }

	// add more virtual functions as needed 
	virtual void display_paramater_data_widget() {};
	//virtual void 

	entity_base_geometry_class *geometry = NULL;
	ofNode                      graphics_node;
	// parameter_widget_base_class     parameter_widget;
	// node_editor_base_class          node_widget;

protected:


private:
};


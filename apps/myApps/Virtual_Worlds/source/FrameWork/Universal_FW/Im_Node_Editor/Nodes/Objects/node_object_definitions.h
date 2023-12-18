#pragma once

/*
        ui_node_type.category          = ENTITY_CATEGORY_HCP_VOXEL;
        ui_node_type.context           = HCP_CONTEXT_VOXEL_MATRIX;
        ui_node_type.function_category = GENERATOR;
        ui_node_type.function          = HCP_VOXEL_MATRIX_VALUE;
*/




// object node category types
#define ENTITY_NODE_CATEGORY_NONE     0
#define ENTITY_NODE_CATEGORY_GEOMETRY 1
#define ENTITY_NODE_CATEGORY_MATERIAL 2
#define ENTITY_NODE_CATEGORY_LIGHT    3



// object node context
#define ENTITY_NODE_CONTEXT_NONE     100
#define ENTITY_NODE_CONTEXT_OBJECT   1000
#define ENTITY_NODE_CONTEXT_GEOMETRY 101
#define ENTITY_NODE_CONTEXT_MATERIAL 102
#define ENTITY_NODE_CONTEXT_LIGHT    103


//object node data types
#define ENTITY_NODE_DATA_NONE       200
#define ENTITY_NODE_DATA_OBJECT     2000
#define ENTITY_NODE_DATA_GEOMETRY   201
#define ENTITY_NODE_DATA_MATERIAL   202
#define ENTITY_NODE_DATA_LIGHT      203

// object node function categories
//#define ENTITY_NODE_FUNCTION_CATEGORY_NONE     0
//#define ENTITY_NODE_FUNCTION_CATEGORY_GEOMETRY 201
//#define ENTITY_NODE_FUNCTION_CATEGORY_MATERIAL 202
//#define ENTITY_NODE_FUNCTION_CATEGORY_LIGHT    203


// object node function
//#define ENTITY_NODE_FUNCTION_NONE     0
//#define ENTITY_NODE_FUNCTION_GEOMETRY 301
//#define ENTITY_NODE_FUNCTION_MATERIAL 302
//#define ENTITY_NODE_FUNCTION_LIGHT    303
#pragma once
#define INVALID_ID                -1
#define NON_ENTITY_ID             -1
#define NON_ENTITY_CATEGORY_ID    -1


#define INVALID_NODE_CATEGORY_ID          -1
#define INVALID_NODE_CONTEXT_ID           -1
//#define INVALID_NODE_FUNCTION_CATEGORY_ID -1
//#define INVALID_NODE_FUNCTION_ID          -1

#define FUNCTION_CATEGORY_GENERATOR 100

#define FUNCTION_CATEGORY_OPERATOR 1000
#define FUNCTION_CATEGORY_MODIFIER 2000


typedef int entity_id_type;
typedef int node_id_type;

enum class scene_node_editor_function_enum { none, create_object_node, delete_Object_node };

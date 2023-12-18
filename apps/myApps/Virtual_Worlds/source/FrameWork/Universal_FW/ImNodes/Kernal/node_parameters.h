#pragma once

#include <string>

// Type definitions of application entity attributes
typedef int         node_id_type;
typedef int         node_category_id_type;
typedef int         node_value_data_id_type;

typedef std::string node_label_type;
typedef std::string name_type;
typedef std::string description_type;
typedef std::string pathname_type;

#define LEFT_MOUSE_BUTTON   0
#define RIGHT_MOUSE_BUTTON  1
#define MIDDLE_MOUSE_BUTTON 2

#define RESTORE          TRUE

#define INVALID_NODE_ID -1
#define INVALID_ID      -1

// ---- Import/export iilse flag data region seperator designation flags ----
#define GRAPH_NODE_START_DATA_FLAG "{"
#define GRAPH_NODE_END_DATA_FLAG   "}"
#define GRAPH_PIN_START_DATA_FLAG  "<"
#define GRAPH_PIN_END_DATA_FLAG    ">"
#define GRAPH_LINK_START_DATA_FLAG "("
#define GRAPH_LINK_END_DATA_FLAG    ")"
#define GRAPH_LINK_NODE_START_DATA_FLAG "["
#define GRAPH_LINK_NODE_END_DATA_FLAG    "]"

#define ENDL                        "\n"

#define GRAPH_NODE_GROUP_START_FLAG "[["
#define GRAPH_NODE_GROUP_END_FLAG   "]]"
#define GRAPH_NODE_START_FLAG       "{{"
#define GRAPH_NODE_END_FLAG         "}}"
#define GRAPH_INPUT_PIN_START_FLAG  "<|"
#define GRAPH_INPUT_PIN_END_FLAG    ">|"
#define GRAPH_OUTPUT_PIN_START_FLAG "|<"
#define GRAPH_OUTPUT_PIN_END_FLAG   "|>"
#define GRAPH_LINK_START_FLAG       "(("
#define GRAPH_LINK_END_FLAG         "))"

#define GRAPH_LINK_NODE_INPUT_START_FLAG  "(|"
#define GRAPH_LINK_NODE_INPUT_END_FLAG    "|)"
#define GRAPH_LINK_NODE_OUTPUT_START_FLAG  "|("
#define GRAPH_LINK_NODE_OUTPUT_END_FLAG    ")|"

// -----------------------------------

#define DATA_VALUE_TYPE_NULL			  -1000 
#define DATA_VALUE_TYPE_BOOL			  1000
#define DATA_VALUE_TYPE_INT				  1001
#define DATA_VALUE_TYPE_UINT			  1002
#define DATA_VALUE_TYPE_FLOAT			  1003
#define DATA_VALUE_TYPE_DOUBLE			  1004
#define DATA_VALUE_TYPE_SHADER_PROGRAM_ID 1100

#define CONTEXT_ID_SHADER_PROGRAM 2100

#define NODE_DATA_TYPE_INT        100

#define NODE_TYPE_ID_GROUP        5000 
#define NODE_TYPE_ID_GROUP_INPUT  5001 
#define NODE_TYPE_ID_GROUP_OUTPUT 5002

#define NODE_CATEGORY_ID_GROUP     5100
#define NODE_CATEGORY_ID_GENERATOR 5110
#define NODE_CATEGORY_ID_MODIFIER  5111

#define NODE_DATA_CONTEXT                   100
#define NODE_GROUP_DATA_CONTEXT             101
#define NODE_GROUP_INPUT_LINK_DATA_CONTEXT  102
#define NODE_GROUP_OUTPUT_LINK_DATA_CONTEXT 103
#define ENTITY_DATA_CONTEXT                 110
#pragma once

#include <string>

typedef int id_type;
typedef int data_type_id;

typedef unsigned int idu_type;
typedef unsigned int index_type;

typedef std::string string_type;

#define INVALID_ID        -1
#define NULL_CATEGORY_ID  UINT_MAX

enum class entity_object_type_enum {none, camera, light, object, node };

#define ENTITY_TYPE_CAMERA      100
#define ENTITY_TYPE_LIGHT       101
#define ENTITY_TYPE_OBJECT      102
#define ENTITY_TYPE_EDITOR_NODE 103


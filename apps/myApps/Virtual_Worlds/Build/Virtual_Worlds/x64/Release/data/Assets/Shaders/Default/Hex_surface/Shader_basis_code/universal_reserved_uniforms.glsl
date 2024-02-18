
uniform mat4 modelViewProjectionMatrix;//model-view-projection matrix
mat4 mvpMatrix; //model-view-projection matrix

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

// Camera data
uniform vec3 uCameraPos;// OpenFrameWorks
uniform vec3 uCamera_forward;
uniform vec3 uCamera_up;
uniform vec3 uCamera_right; 

//Global settings
uniform vec4  global_ambient_color;
uniform float global_ambience;

// Object material settings
uniform vec4 mat_color;
uniform vec4 mat_diffuse;
uniform vec4 mat_ambient;
uniform vec4 mat_specular;
uniform vec4 mat_emissive;

uniform float mat_shinines;
uniform float mat_metallic;
uniform float mat_roughness;
uniform float mat_reflectance;

// Uncomment following as needed when not testing
// reserved light variables
// uniform vec4  light_color;
// uniform vec3  lighting_direction;

// uniform float ambience;
// uniform float specular_strength;
// uniform float lighting_intensity;

//uniform float mat_ambient;
//uniform float mat_specular;


// reserved camera light variables
//uniform int   camera_lighting_type;
//uniform int   use_camera_lighting;
//uniform vec3  lighting_camera_offset;

//uniform int   t_frame;

// Testing only comment out or delete when no needed
// Testing only comment out or delete when no needed
// Need to have as uniforms
int   camera_lighting_type = 0;
int   use_camera_lighting = 0;
vec3  lighting_camera_offset = vec3(0.0,0.0,0.0);

vec4  light_color = vec4(1.0,1.0,1.0,1.0);
vec3  lighting_direction = vec3(0.0,0.0,1.0);
float ambience = .5;
float specular_strength =.5;
float lighting_intensity =.5;
// **************************************

struct Material {
    float diffuse;
    float specular;
    float shininess;

	//float mat_diffuse;
    //float mat_specular;
    //float mat_shininess;
};

// Light Data_structures

struct DirLight {
    vec3 direction;
	vec3 color;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
	vec3 color;
    
    float constant;
    float linear;
    float quadratic;
	float attenuation_factor;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
	vec3 color;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
	float attenuation_factor;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define MAX_NUMBER_DIRECTIONAL_LIGHTS 10
#define MAX_NUMBER_POINT_LIGHTS       10
#define MAX_NUMBER_SPOT_LIGHTS        10

uniform int number_directional_lights;
uniform int number_point_lights;
uniform int number_spot_lights;

uniform vec3 viewPos;

#define TRUE  0
#define FALSE 1




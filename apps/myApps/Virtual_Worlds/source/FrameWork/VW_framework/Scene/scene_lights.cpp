#include "scene_lights.h"

#include "../Shader/shader.h"

	bool scene_lights_manager_class::define_global_light() {
		//scene_global_light = new scene_global_light_class;
		//scene_global_light = new ofLight;
		scene_global_light = add_light(false);
		if (!scene_global_light) {
			// error message
			std::cout << "MESSAGE : Could not define a default global light.\n";
			return false;
		}

		//scene_global_light->define_default_global_light();

		//++++++
		scene_global_light->setDirectional();

        //light->setPosition(100.1, 400, 600);
        scene_global_light->setPosition(0.0, 0.0, 1300.0);
        scene_global_light->lookAt(glm::vec3(0, 0, 0));
        scene_global_light->setAmbientColor({ 0.5, 0.5, 0.5,1.0 });
	    scene_global_light->setDiffuseColor({ 0.5, 0.5, 0.5,1.0 });
	    scene_global_light->setSpecularColor({ 0.5, 0.5, 0.5,1.0 });
		scene_global_light->intensity = 0.75;
		//++++++

		return true;
	}

	void scene_lights_manager_class::update_global_shader_uniforms(GLint shader_id) {
//std::cout << "vw_camera_base::update_shader_uniforms : shader_id 0000 " << shader_id << std::endl;

		if (shader_id < 0) {
			std::cout << "scene_lights_manager_class::update_global_shader_uniforms : shader_id < 0 " << shader_id << std::endl;
			return;
		}

		if (!scene_global_light) {
			std::cout << "scene_lights_manager_class::update_global_shader_uniforms : scene_global_light = NULL : scene global light not defined  " << shader_id << std::endl;
			return;
		}

		shader_class shader;

        glm::vec4 pos = glm::vec4(scene_global_light->getPosition(),1.0f);

		//shader.set_vec4(shader_id, pos,"global_light_location");// Not needed as global light is directional only
        ofFloatColor ambient_color = scene_global_light->getAmbientColor();
        glm::vec4 lac = {ambient_color.r,ambient_color.g,ambient_color.b,ambient_color.a};
		shader.set_vec4(shader_id, lac, "global_light_color");
		
        shader.set_vec3(shader_id, scene_global_light->getLookAtDir(),"global_light_direction");

		shader.set_f1(shader_id, scene_global_light->intensity,"global_light_intensity");


		// If need other global setting to be used by shader glsl code add below 

//std::cout << "vw_camera_base::update_shader_uniforms 1111 : " << renderer->camera_forward_dir.x << " : " << renderer->camera_forward_dir.y << " : " << renderer->camera_forward_dir.z << " : " << std::endl;

	}

	bool scene_lights_manager_class::define_default_spot_light() {
		ofLight *light = add_light();
		if (!light) {
			// error message
			std::cout << "MESSAGE : Could not define a default point light.\n";
			return false;
		}

		light->setSpotlight();
		// more settings defined here

		return true;
    }

    bool scene_lights_manager_class::define_default_point_light() {
		ofLight *light = add_light();
		if (!light) {
			// error message
			std::cout << "MESSAGE : Could not define a default point light.\n";
			return false;
		}

        light->setPointLight();
        define_default_light_common_settings(light);
        light->setSpotlightCutOff(100.0f);
        light->setSpotConcentration(0.0f); // light exponent

		return true;
    }

    bool scene_lights_manager_class::define_default_area_light() {
		ofLight *light = add_light();
		if (!light) {
			// error message
			std::cout << "MESSAGE : Could not define a default point light.\n";
			return false;
		}

		light->setAreaLight(1.0f,1.0f);
		// more settings defined here

		return true;
    }

    bool scene_lights_manager_class::define_default_directional_light() {
		ofLight *light = add_light();
		if (!light) {
			// error message
			std::cout << "MESSAGE : Could not define a default point light.\n";
			return false;
		}

		light->setDirectional();
		// more settings defined here

		return true;
    }

    void scene_lights_manager_class::define_default_light_common_settings(ofLight *light) {
        light->setPosition(0.0, 5.0, 0.0);
        light->lookAt(glm::vec3(0, 0, 0));

        light->setAmbientColor({ 0.5, 0.5, 0.5,1.0 });
	    light->setDiffuseColor({ 0.5, 0.5, 0.5,1.0 });
	    light->setSpecularColor({ 0.5, 0.5, 0.5,1.0 });
        light->setAttenuation();
    }


	//void scene_lights_manager_class::display_light_parameters() {

 //   }

 //   void scene_lights_manager_class::save_light_parameters() {

 //   }

 //   void scene_lights_manager_class::import_light_parameters() {

 //   }
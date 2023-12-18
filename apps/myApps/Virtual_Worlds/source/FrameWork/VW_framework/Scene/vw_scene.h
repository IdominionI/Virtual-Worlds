#pragma once

#include "scene_entities_manager.h"

struct shadow_settings_struct_type {
	float        shadow_bias          = 0.007f;
	float        shadow_normal_bias   = -4.0f;
	float        shadow_sample_radius = 2.0f;
	ofShadowType shadowType           = OF_SHADOW_TYPE_PCF_LOW;
	glm::ivec2   depth_resolution     = { 1024,1024 };
};

class vw_scene_class {
public:
	vw_scene_class() {}
	~vw_scene_class() {}

	scene_entities_manager_class scene_entities_manager;

	void render_scene() {
		setup_scene_lights();
		setup_scene_camera();
		setup_scene_objects();
		
		render_scene_objects();
		render_scene_overlays();

		exit_scene_cameras();
		exit_scene_lights();
	}

	// !!!!!!!!!!!!!!!!!!!!! Scene render shadow management !!!!!!!!!!!!!!!!!!!!!!!!!

	void set_shadow_bias(float value) {
		shadow_settings.shadow_bias = value;
		ofShadow::setAllShadowBias(shadow_settings.shadow_bias);
	}

	void set_shadow_normal_bias(float value) {
		shadow_settings.shadow_normal_bias = value;
		ofShadow::setAllShadowBias(shadow_settings.shadow_normal_bias);
	}

	void set_shadow_depth_resolution(int width_res, int height_res) {
		shadow_settings.depth_resolution = { width_res,height_res };
		ofShadow::setAllShadowDepthResolutions(shadow_settings.depth_resolution.x, shadow_settings.depth_resolution.y);
	}

	void set_shadow_sample_radius(float value) {
		shadow_settings.shadow_sample_radius = value;
		ofShadow::setAllShadowSampleRadius(shadow_settings.shadow_sample_radius);
	}

	void set_scene_shadow_render_settings(shadow_settings_struct_type _shadow_settings) {
		set_shadow_bias(_shadow_settings.shadow_bias);
		set_shadow_normal_bias(_shadow_settings.shadow_normal_bias);
		set_shadow_depth_resolution(_shadow_settings.depth_resolution.x,_shadow_settings.depth_resolution.y);
		set_shadow_sample_radius(_shadow_settings.shadow_sample_radius);
	}

	void set_scene_shadow_render_settings() {
			// shadow bias is the margin of error the shadow depth
			// increasing the bias helps reduce shadow acne, but can cause light leaking
			// try to find a good balance that fits your needs
			// bias default is 0.005
			ofShadow::setAllShadowBias(shadow_settings.shadow_bias);
			// normal bias default is 0
			// moves the bias along the normal of the mesh, helps reduce shadow acne
			ofShadow::setAllShadowNormalBias(shadow_settings.shadow_normal_bias);
			#ifndef TARGET_OPENGLES
			ofShadow::setAllShadowDepthResolutions(shadow_settings.depth_resolution.x, shadow_settings.depth_resolution.y);
			#endif
			ofShadow::setAllShadowTypes(shadow_settings.shadowType);

			ofShadow::setAllShadowSampleRadius(shadow_settings.shadow_sample_radius);
	}

	void enable_render_shadows() {
		// shadows are disabled by default
			ofShadow::enableAllShadows();//enable all shadows that are attached to lights
			render_shadows = true;
	}

	void disable_render_shadows() {
			ofShadow::disableAllShadows();
			render_shadows = false;
	}



	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

protected:

private:
	void setup_scene_lights() {
		scene_entities_manager.scene_lights.scene_lights_render_setup();

		if (render_shadows) {
			size_t number_lights = scene_entities_manager.scene_lights.lights.size();
			for (int i = 0; i < number_lights; i++) {
				auto& light = scene_entities_manager.scene_lights.lights[i];
				if (light->light_is_on) {
					// query the light to see if it has a depth pass
					if (light->shouldRenderShadowDepthPass()) {
						// Get the number of passes required.
						// By default the number of passes is 1. And we could just call beginShadowDepthPass() or beginShadowDepthPass(0);
						// It will be more than one pass if it is a pointlight with setSingleOmniPass set to false
						// or a platform that does not support geometry shaders.
						// Most likely it will be a single pass, but we get the number of passes to be safe.
						int numShadowPasses = light->getNumShadowDepthPasses();
						for (int j = 0; j < numShadowPasses; j++) {
							light->beginShadowDepthPass(j);
							// Shadows have the following gl culling enabled by default
							// this helps reduce z fighting by only rendering the rear facing triangles to the depth map
							// enables face culling
							//glEnable(GL_CULL_FACE);
							// sets the gl triangle winding order, default for ofShadow is GL_CW
							//glFrontFace(mGlFrontFaceWindingOrder);
							// tells OpenGL to cull front faces
							//glCullFace(GL_FRONT);

							// the culling can be disabled by calling
							//light->getShadow().setGlCullingEnabled(false);
							// or the culling winding order can be changed by calling
							//light->getShadow().setFrontFaceWindingOrder(GL_CCW); // default is GL_CW
							render_scene_objects(); // not tested for shadow objects yet
							light->endShadowDepthPass(j);
						}
					}
				}
			}
		}

	};
		
	void setup_scene_camera() {
		scene_entities_manager.scene_cameras.scene_cameras_render_setup();
	}

	void setup_scene_objects() {
	
	}

	void render_scene_objects() {
		scene_entities_manager.render_objects();
	}

	void render_scene_overlays(){
		scene_entities_manager.render_overlays();
	}

	void exit_scene_cameras() {
		scene_entities_manager.scene_cameras.scene_cameras_render_exit();
	}

	void exit_scene_lights() {
		scene_entities_manager.scene_lights.scene_lights_render_exit();
	}

	bool                        render_shadows = false;
	shadow_settings_struct_type shadow_settings;

};

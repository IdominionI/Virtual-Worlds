#include "voxel_automata_functions.h"




	int voxel_automata_hcp_functions_class::get_max_voxel_automata_step_value() {
		int max_number_steps = 0;
		for(voxel_hcp_automata_byte_rule_struct_type hcp_automata_rule : voxel_hcp_automata_rules) {
			if (hcp_automata_rule.end_step > max_number_steps) max_number_steps = hcp_automata_rule.end_step;
		}

		return max_number_steps;
	}


	// Create an empty hcp voxel data object matrix in which to store the resultant cellula autoamata values to
	bool voxel_automata_hcp_functions_class::create_voxel_automata_next_step_matrix() {
		voxel_automata_next_step_matrix = new voxel_hcp_object_class;

		if (voxel_automata_next_step_matrix == NULL) return false;

		voxel_automata_next_step_matrix->voxel_object_data.voxel_size = cloud->voxel_object_data.voxel_size;

		voxel_automata_next_step_matrix->voxel_object_data.matrix_dimension = cloud->voxel_object_data.matrix_dimension;
		voxel_automata_next_step_matrix->voxel_object_data.matrix_origin = cloud->voxel_object_data.matrix_origin;
		voxel_automata_next_step_matrix->voxel_object_data.create_empty_volume_cubic(cloud->voxel_object_data.matrix_dimension.x, cloud->voxel_object_data.matrix_dimension.y, cloud->voxel_object_data.matrix_dimension.z);

//QMessageBox::information(0, "Function Expression Success", "next_step_matrix 00: "+QString::number(voxel_automata_next_step_matrix->voxel_object_data.matrix_dimension.x)+":"+QString::number(voxel_automata_next_step_matrix->voxel_object_data.matrix_dimension.y)+":"+QString::number(voxel_automata_next_step_matrix->voxel_object_data.matrix_dimension.z)+":", QMessageBox::Ok);


		if (voxel_automata_next_step_matrix->voxel_object_data.voxel_matrix_data.size() > 0)
			return true;
		else {
			delete voxel_automata_next_step_matrix;
			return false;
		}
	}


	bool voxel_automata_hcp_functions_class::perform_voxel_automata_generation(int automata_step) {
//printf("in perform_voxel_automata_generation 00 : \n");
		if (cloud == NULL) {
			printf("Function Automata : ERROR: Have not defined an initial voxel matrix\n");
			return false;
		}

		if (!create_voxel_automata_next_step_matrix()) {
			printf("Function Automata : ERROR: Failure to create automata step matrix\n");
			return false;
		}

		if (!perform_voxel_automata_step_generation(automata_step)) {
			printf("Function Automata : ERROR: Failure to create perform automata data at step %i\n",automata_step) ;
			return false;
		}

		//file.close();// testing only

//printf("in perform_voxel_automata_generation 03 : \n");
//if (voxel_hcp_object_to_execute == NULL) {
//	printf("in perform_voxel_automata_generation 03AAA : voxel_hcp_object_to_execute == NULL \n");
//	return false;
//}
		// FOLOWING FUNCTIONS WAS PREVIOUSLY PERFORMED EXTERNALLY AFTER perform_voxel_automata_generation WAS CALLED

		voxel_hcp_object_to_execute->voxel_object_data.delete_voxel_matrix_data();
		voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data = voxel_automata_next_step_matrix->voxel_object_data.voxel_matrix_data;
//printf("in perform_voxel_automata_generation 04 : \n");
		//voxel_hcp_object_to_execute->update_renderer_shader_variables(); // Not present for QML version
		//voxel_hcp_object_to_execute->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
		voxel_hcp_object_to_execute->set_voxel_value_range(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);
		voxel_hcp_object_to_execute->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display

//printf("in perform_voxel_automata_generation 05 : \n");

		return true;
	}

	// generate and save cellula automata data for cellula automata generation step 
	// This procedure requires an initial voxel matrix that has initialised conditions set.
	bool voxel_automata_hcp_functions_class::perform_voxel_automata_step_generation(int step) {
		if (cloud == NULL) {
			//QMessageBox::information(0, "perform_voxel_automata_step_generation", "cloud == NULL", QMessageBox::Ok);
			return false;
		}

//QMessageBox::information(0, "perform_voxel_automata_step_generation", "here00", QMessageBox::Ok);

		// Define the volume that the cellula automata rules are to be conducted in
		voxel_automata_generator_parameters.x_start = cloud->voxel_object_data.voxel_generator_parameters.x_start;
		voxel_automata_generator_parameters.x_end = cloud->voxel_object_data.voxel_generator_parameters.x_end;
		voxel_automata_generator_parameters.y_start = cloud->voxel_object_data.voxel_generator_parameters.y_start;
		voxel_automata_generator_parameters.y_end = cloud->voxel_object_data.voxel_generator_parameters.y_end;
		voxel_automata_generator_parameters.z_start = cloud->voxel_object_data.voxel_generator_parameters.z_start;
		voxel_automata_generator_parameters.z_end = cloud->voxel_object_data.voxel_generator_parameters.z_end;
		voxel_automata_generator_parameters.resolution_step = cloud->voxel_object_data.voxel_generator_parameters.resolution_step;

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "perform_voxel_automata_step_generation 00 : " + QString::number(voxel_automata_generator_parameters.x_start) + " : "+ QString::number(voxel_automata_generator_parameters.resolution_step));
//}

		//define the voxel iteration parameters that define the voxel matrix that apply to the cellula automata rules
		// Same as for the hcp voxel point cloud generation and thus use point cloud generation parameter data
		index_data_type iX = 0, iY, iZ;
		float fX = 0.0f, fY = 0.0f, fZ = 0.0f, fT = 0.0f;

		float z_mult = 2.0f * sqrt(6.0f) / 3.0f;
		float x_res_step = voxel_automata_generator_parameters.resolution_step * 2.0f;
		float y_res_step = voxel_automata_generator_parameters.resolution_step * (3.0f / sqrt(3.0f));
		float z_res_step = voxel_automata_generator_parameters.resolution_step * z_mult;

		float x_size = voxel_automata_generator_parameters.x_end - voxel_automata_generator_parameters.x_start;
		float y_size = voxel_automata_generator_parameters.y_end - voxel_automata_generator_parameters.y_start;
		float z_size = voxel_automata_generator_parameters.z_end - voxel_automata_generator_parameters.z_start;

		int data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0f)
			data_set_x_size = (int)(x_size / x_res_step) + 1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0f)
			data_set_y_size = (int)(y_size / y_res_step) + 1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

		if (z_size / z_res_step - float((int)(z_size / z_res_step)) > 0.0f)
			data_set_z_size = (int)(z_size / z_res_step) + 1;
		else
			data_set_z_size = (int)(z_size / z_res_step);

		// Allow for not all x,y,z to be used
		if (voxel_automata_generator_parameters.x_start == voxel_automata_generator_parameters.x_end)
			voxel_automata_generator_parameters.x_end = voxel_automata_generator_parameters.x_start + voxel_automata_generator_parameters.resolution_step / 2.0f;

		if (voxel_automata_generator_parameters.y_start == voxel_automata_generator_parameters.y_end)
			voxel_automata_generator_parameters.y_end = voxel_automata_generator_parameters.y_start + voxel_automata_generator_parameters.resolution_step / 2.0f;

		if (voxel_automata_generator_parameters.z_start == voxel_automata_generator_parameters.z_end)
			voxel_automata_generator_parameters.z_end = voxel_automata_generator_parameters.z_start + voxel_automata_generator_parameters.resolution_step / 2.0f;


		index_data_type  dim_x, dim_y;

		float progress = 0.0f;
		printf("Performing HCP voxel cellular automata step rules  !!! : PLease wait \n");

		iZ = 0;
		for (fZ = voxel_automata_generator_parameters.z_start; fZ < voxel_automata_generator_parameters.z_end && iZ < data_set_z_size; fZ = fZ + z_res_step) {
			iY = 0;

			// It seens that either because of ImGui or how the CPU processes functions this log panel message is not displayed until after
			// this loop is completed. No worth executing.
			//if (log_panel != NULL) {
			//	log_panel->application_log.AddLog("PERFORMING CELLULAR AUTOMATA RULES !!! : PLease wait \n");
			//}

			printf("\r HCP Voxel cellular automata %5.2f %% completed", progress);
			fflush(stdout);

			dim_y = ((iZ + 1) % 2) * data_set_y_size + (iZ % 2) * (data_set_y_size - 1);
			for (fY = voxel_automata_generator_parameters.y_start; fY < voxel_automata_generator_parameters.y_end && iY < dim_y; fY = fY + y_res_step) {
				iX = 0;
				if (iZ % 2 == 0) { // Even z level
					dim_x = (iY + 1) % 2 * data_set_x_size + (iY % 2) * (data_set_x_size - 1);
				}
				else { // Odd z level
					dim_x = (iY + 1) % 2 * (data_set_x_size - 1) + (iY % 2) * data_set_x_size;
				}
				iX = 0;
//QMessageBox::information(0, "perform_voxel_automata_step_generation", "here00A :"+QString::number(iX)+":"+QString::number(iY)+":"+QString::number(iZ)+":", QMessageBox::Ok);
//QMessageBox::information(0, "perform_voxel_automata_step_generation", "here00AAA"+QString::number(fX)+":"+QString::number(fY)+":"+QString::number(voxel_automata_generator_parameters.z_start)+":", QMessageBox::Ok);
				for (fX = voxel_automata_generator_parameters.x_start; fX < voxel_automata_generator_parameters.x_end && iX < dim_x; fX = fX + x_res_step) {
					perform_voxel_automata_rules(step, iX, iY, iZ);
					iX++;
				}
				iY++;
			}
			iZ++;
			progress = ((float) iZ/ data_set_z_size) *100.0f;
//QMessageBox::information(0, "perform_voxel_automata_step_generation", "here00B", QMessageBox::Ok);
		}
		printf("\r HCP Voxel cellular automata %5.2f %% completed", progress);
		fflush(stdout);
		printf("\nHCP voxel cellular automata step completed \n");
//QMessageBox::information(0, "perform_voxel_automata_step_generation", "here00C", QMessageBox::Ok);
		return true;
	}

	// Perform the cellula automata rules for step at hcp voxel matrix location x_index, y_index, z_index
	// and if all rules are met assign a defined value for this case, otherwise set a rules not met value
	bool voxel_automata_hcp_functions_class::perform_voxel_automata_rules(int step, index_data_type x_index, index_data_type y_index, index_data_type z_index) {
		if (cloud == NULL) return false;
		glm::ivec3 voxel_data_index_coord = { x_index, y_index, z_index };

		voxel_data_type rule_met_status = INVALID_VOXEL_VALUE;
		bool automata_rule_met = false;
		//if( z_index == INVETSIGATE_Z)// testing only
		//stream << "step : " << step << " | inex x: " << x_index <<" : " << y_index << " : " << z_index << '\n';// testing only

		// The first automata activation rule in the order of rules given that is valid or true is the rule that is applied.
		// A future implementation considered is to put all rules that are met into an array and through a selection process the rule that is applied is chosen
		// via an option of first rule or by a randome process that can include biased outcomes
//QMessageBox::information(0, "perform_voxel_automata_rules", "here01", QMessageBox::Ok);
		for (voxel_hcp_automata_byte_rule_struct_type voxel_hcp_automata_rule : voxel_hcp_automata_rules) {
			if (voxel_hcp_automata_rule.active_rule && !automata_rule_met) {
				if (voxel_automata_rule_met(voxel_hcp_automata_rule, step, x_index, y_index, z_index)) {
					// this method of determining the value of the voxel status when more than one rule is met can be chnaged
					rule_met_status = voxel_hcp_automata_rule.voxel_state;
					automata_rule_met = true; // Set flag to indicate that an automata rule has been met
					break;
				}
			}
		}
//QMessageBox::information(0, "perform_voxel_automata_rules", "here01A: "+QString::number(x_index)+":"+QString::number(y_index)+":"+QString::number(z_index)+":", QMessageBox::Ok);

		// This method of determining the value of the voxel status when more than one rule is met can be chnaged
		// If a rule is met then a chnage of the voxel status value is made to the accumulation of all the rule values that has been met
		// otherwise leave the voxel status value unchanged
		if (automata_rule_met) {
			if (rule_met_status > MAX_VOXEL_VALUE)
				rule_met_status = MAX_VOXEL_VALUE;
			else
				if (rule_met_status < INVALID_VOXEL_VALUE)
					rule_met_status = INVALID_VOXEL_VALUE;

//stream << "perform_voxel_automata_rules 00 : rule met : " << QString::number(rule_met_status)<< ":" + QString::number(x_index) + ":" << QString::number(y_index) << ":" + QString::number(z_index) << '\n';// testing only
			voxel_data_type value = voxel_automata_next_step_matrix->voxel_object_data.activate_voxel_matrix_coordinate(voxel_data_index_coord, rule_met_status); // majority rule to activate voxel state
// stream << "perform_voxel_automata_rules 00 : rule met A: " << QString::number(value) << '\n';// testing only
		}
		else {
//stream <<"perform_voxel_automata_rules 01 : rule not met : "  <<  QString::number(x_index) << ":" << QString::number(y_index) << ":" + QString::number(z_index) << '\n';// testing only

			assign_no_voxel_state_change(voxel_data_index_coord); // no majority rule or rule to apply so have no change in voxel state 
		}
//QMessageBox::information(0, "perform_voxel_automata_rules", "here01B", QMessageBox::Ok);
		return true;
	}

	// Return if a cellular automata rule is met for the hcp voxel matrix at location x_index, y_index, z_index
	// A cellular automata rule is met if all nieghbor hcp voxel cells rules apply
	bool voxel_automata_hcp_functions_class::voxel_automata_rule_met(voxel_hcp_automata_byte_rule_struct_type voxel_hcp_automata_byte_rule, int step, index_data_type x_index, index_data_type y_index, index_data_type z_index) {
		if (step >= voxel_hcp_automata_byte_rule.end_step || step < voxel_hcp_automata_byte_rule.start_step) return false;
//QMessageBox::information(0, "voxel_autoamta_rule_met", "voxel_automata_rule_met 000 "+QString::number(x_index)+":"+QString::number(y_index)+":"+QString::number(z_index), QMessageBox::Ok);
		std::vector<voxel_data_type> voxel_neighbours_state = get_voxel_neighbours_state(x_index, y_index, z_index);
//QMessageBox::information(0, "voxel_autoamta_rule_met", "voxel_automata_rule_met "+QString::number(voxel_neighbours_state), QMessageBox::Ok);

		short neighbours_activity_rule = 0;

		// Determine if the cellula automata rule for neighboring voxel i is met or not 
		for (size_t i = 0; i < NUMBER_HCP_NEIGHBOURS; i++) {
			bool neighbours_lower_activity_rule = false, neighbours_upper_activity_rule = false;

			switch (voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].lower_rule) {
			case lower_rule_condition_enum_type::ignore: neighbours_lower_activity_rule = true;	break;
			case lower_rule_condition_enum_type::LT: if (voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].lower_rule_value < voxel_neighbours_state[i])
				neighbours_lower_activity_rule = true; break;
			case lower_rule_condition_enum_type::LTEQ: if (voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].lower_rule_value <= voxel_neighbours_state[i])
				neighbours_lower_activity_rule = true; break;;
			}

			switch (voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].upper_rule) {
			case upper_rule_condition_enum_type::ignore: neighbours_upper_activity_rule = true;	break;
			case upper_rule_condition_enum_type::NE: if (voxel_neighbours_state[i] != voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value)
				neighbours_upper_activity_rule = true; break;
			case upper_rule_condition_enum_type::EQ: if (voxel_neighbours_state[i] == voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value)
				neighbours_upper_activity_rule = true; break;;
			case upper_rule_condition_enum_type::LT: if (voxel_neighbours_state[i] < voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value)
				neighbours_upper_activity_rule = true; break;
			case upper_rule_condition_enum_type::LTEQ: if (voxel_neighbours_state[i] <= voxel_hcp_automata_byte_rule.neighbours_rule_condition[i].upper_rule_value)
				neighbours_upper_activity_rule = true; break;;
			}

			if (!neighbours_lower_activity_rule || !neighbours_upper_activity_rule) return false;
		}

		return true; // All neighbor rules met
	}

	// Get all the voxel neighbor cell values of hcp voxel at matrix location x_index, y_index, z_index stored in a std::vector list
	std::vector<voxel_data_type> voxel_automata_hcp_functions_class::get_voxel_neighbours_state(index_data_type x_index, index_data_type y_index, index_data_type z_index) {
		std::vector<voxel_data_type> neighbours_activity;
//QMessageBox::information(0, "get_voxel_neighbours_state", "here03", QMessageBox::Ok);
		for (size_t neighbour = 0; neighbour < NUMBER_HCP_NEIGHBOURS; neighbour++) {

			voxel_data_type voxel_state = get_voxel_neighbour_state(neighbour, x_index, y_index, z_index);

			neighbours_activity.push_back(voxel_state);
			//if( z_index == INVETSIGATE_Z)// testing only
			//	stream << "| voxel activcation status 1"<< '\n';// testing only

		}

//stream << "| neighbours_activity :"<< neighbours_activity << '\n'; // testing only
		return neighbours_activity;
	}

	// Get the neighbor cell status value of a hcp voxel that is at voxel matrix location x_index, y_index, z_index
	voxel_data_type voxel_automata_hcp_functions_class::get_voxel_neighbour_state(size_t neighbour, index_data_type x_index, index_data_type y_index, index_data_type z_index) {
		glm::ivec3 voxel_neighbour_index_coord = get_voxel_neighbour_index_coordinates(neighbour, x_index, y_index, z_index);

//if( z_index == INVETSIGATE_Z)// testing only
//stream << "| neighbour : " << neighbour << " inex x: " << voxel_neighbour_index_coord.x <<" y: " << voxel_neighbour_index_coord.y << " z: " << voxel_neighbour_index_coord.z ;// testing only

		if (!valid_index_coordinate(voxel_neighbour_index_coord)) {

//if(z_index == 1 && y_index == 2 && x_index == 8)
//QMessageBox::information(0, "get_voxel_neighbour_state", "get_voxel_neighbour_state00 invalid  Cloud : " + QString::number(neighbour) + ":" + QString::number(x_index) + ":" + QString::number(y_index) + ":" + QString::number(z_index) +
//							" \nneighbour :" + QString::number(voxel_neighbour_index_coord.x) + ":" + QString::number(voxel_neighbour_index_coord.y) + ":" + QString::number(voxel_neighbour_index_coord.z) +
//							"\n activation status:" + QString::number(INVALID_VOXEL_VALUE), QMessageBox::Ok);

			return INVALID_VOXEL_VALUE; // *** Need to change this for a non valid value
		}

//if( z_index == INVETSIGATE_Z)// testing only
//stream << " | Cloud : " << " inex x: " << voxel_data_index_coord.x <<" y: " << voxel_data_index_coord.y << " z: " << voxel_data_index_coord.z ;// testing only

//if( z_index == INVETSIGATE_Z)// testing only
//	stream << " | Index : " << cloud->voxel_object_data.get_voxel_matrix_data_index(voxel_data_index_coord); ;// testing only

//if(z_index == 1 && y_index == 2 && x_index == 8)
//QMessageBox::information(0, "get_voxel_neighbour_state", "get_voxel_neighbour_state01 valid Cloud : " + QString::number(neighbour) + ":" + QString::number(x_index) + ":" + QString::number(y_index) + ":" + QString::number(z_index) + 
//							" \nneighbour :" + QString::number(voxel_neighbour_index_coord.x) + ":" + QString::number(voxel_neighbour_index_coord.y) + ":" + QString::number(voxel_neighbour_index_coord.z) + 
//							"\n activation status:" + QString::number(cloud->voxel_object_data.voxel_matrix_coordinate_activation_status(voxel_neighbour_index_coord)), QMessageBox::Ok);

		return cloud->voxel_object_data.voxel_matrix_coordinate_activation_status(voxel_neighbour_index_coord);
	}

	glm::ivec3 voxel_automata_hcp_functions_class::get_voxel_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index, int z_index) {
		glm::ivec3 neighbor_index_coordinate;
//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here05 "+QString::number( neighbour)+":"+QString::number(x_index)+":"+QString::number(y_index)+":"+QString::number(z_index), QMessageBox::Ok);
		if (z_index % 2 == 0) {  // Even Z 
//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here05A", QMessageBox::Ok);
			if (y_index % 2 == 0) { // Even Y 
				switch (neighbour) {
				case 0: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 1: neighbor_index_coordinate.x = x_index; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;


				case 3: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 9:  neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 10: neighbor_index_coordinate.x = x_index; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
			else {// Odd Y 
				switch (neighbour) {
				case 0: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 9:  neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 10: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index;   neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
		}
		else { // Odd Z 
			if (y_index % 2 == 0) { // Even Y 
				switch (neighbour) {
				case 0: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 9:  neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 10: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
			else {// Odd Y
				switch (neighbour) {
				case 0: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 1: neighbor_index_coordinate.x = x_index;   neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index;   neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 3: neighbor_index_coordinate.x = x_index;    neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 9:  neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 10: neighbor_index_coordinate.x = x_index;  neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index;  neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
		}

		return neighbor_index_coordinate;
	}

	// Determine if hcp voxel cell index coordinate voxel_index_coordinate is valid or not
	// Ie if it is outside the volume and limits of the hcp voxel matrix 
	bool voxel_automata_hcp_functions_class::valid_index_coordinate(glm::ivec3 voxel_index_coordinate) {
//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here06", QMessageBox::Ok);

		if (voxel_index_coordinate.z < 0 || voxel_index_coordinate.z > cloud->voxel_object_data.matrix_dimension.z - 1) return false;
		if (voxel_index_coordinate.y < 0 || voxel_index_coordinate.x < 0) return false;

		if (voxel_index_coordinate.z % 2 == 0) {// even z level
			if (voxel_index_coordinate.y > cloud->voxel_object_data.matrix_dimension.y - 1) return false;

			if (voxel_index_coordinate.y % 2 == 0) { // even y coord
				if (voxel_index_coordinate.x > cloud->voxel_object_data.matrix_dimension.x - 1) return false;
			}
			else {
				if (voxel_index_coordinate.x > cloud->voxel_object_data.matrix_dimension.x - 2) return false;
			}
		}
		else {// odd z level
			if (voxel_index_coordinate.y > cloud->voxel_object_data.matrix_dimension.y - 2) return false;

			if (voxel_index_coordinate.y % 2 == 0) {// even y coord
				if (voxel_index_coordinate.x > cloud->voxel_object_data.matrix_dimension.x - 2) return false;
			}
			else {
				if (voxel_index_coordinate.x > cloud->voxel_object_data.matrix_dimension.x - 1) return false;
			}
		}

//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here06A", QMessageBox::Ok);
		return true;
	}

	void voxel_automata_hcp_functions_class::assign_no_voxel_state_change(glm::ivec3 voxel_data_index_coord) {
		voxel_data_type voxel_activation_status = cloud->voxel_object_data.voxel_matrix_coordinate_activation_status(voxel_data_index_coord);
		//voxel_data_type default_inactive_value = DEFAULT_INACTIVE_VALUE;
		//if(voxel_activation_status > DEFAULT_INACTIVE_VALUE) // *** Need to change this for a valid activation value
		voxel_automata_next_step_matrix->voxel_object_data.activate_voxel_matrix_coordinate(voxel_data_index_coord, voxel_activation_status);
		//else
		//	voxel_automata_next_step_matrix->voxel_object_data.deactivate_voxel_matrix_coordinate(voxel_data_index_coord);
	}

	// Copy into the cellular automata data structure variabel voxel_automata_generator_parameters the cellula automata parameter values
	// that are stored in the virtual worlds scene data model that hcp voxel voxel_automata_initial_data points to
	bool voxel_automata_hcp_functions_class::load_voxel_automata_initial_data(voxel_hcp_object_class* voxel_automata_initial_data) {
		if (voxel_automata_initial_data->voxel_object_data.voxel_matrix_data.size() == 0) return false;
		if (voxel_automata_initial_data->voxel_object_data.voxel_size != voxel_automata_generator_parameters.resolution_step)
			voxel_automata_generator_parameters.resolution_step = voxel_automata_initial_data->voxel_object_data.voxel_size;

		//QMessageBox::information(NULL, "","load_voxel_automata_initial_data 001", QMessageBox::Ok);
		if (cloud == NULL) {
			cloud = new voxel_hcp_object_class;
			if (cloud == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Memory Allocation Error:\nCould not allocate memory to import initial voxel automata data.\n");
				return false;
			}
		}

		cloud->voxel_object_data.voxel_matrix_data.clear();
		cloud->voxel_object_data.voxel_matrix_data.shrink_to_fit();
		if (!create_voxel_matrix()) return false;
//QMessageBox::information(NULL, "","load_voxel_automata_initial_data 002", QMessageBox::Ok);
		index_data_type iX = 0, iY, iZ;

//QMessageBox::information(NULL, "","assign_no_voxel_state_chnage 002 : " + QString::number(origin_offset.x())+":"+QString::number(coordinate_offset.x())+":"+QString::number(coordinate_offset_xi), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_voxel_state_chnage 002AA : " + QString::number(cloud->voxel_object_data.matrix_dimension.x)+":"+QString::number(cloud->voxel_object_data.matrix_dimension.y)+":"+QString::number(cloud->voxel_object_data.matrix_dimension.z), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_voxel_state_chnage 002AAB : " + QString::number(cloud->voxel_object_data.voxel_matrix_data.size()), QMessageBox::Ok);

		index_data_type data_set_x_size, data_set_y_size, data_set_z_size;
		index_data_type  dim_x, dim_y;
		bool even_z_level;

		data_set_x_size = cloud->voxel_object_data.matrix_dimension.x;
		data_set_y_size = cloud->voxel_object_data.matrix_dimension.y;
		data_set_z_size = cloud->voxel_object_data.matrix_dimension.z;

		iZ = 0;
		for (iZ = 0; iZ < data_set_z_size; iZ++) {
			if (iZ % 2 == 0)
				even_z_level = true;
			else
				even_z_level = false;

			dim_y = ((iZ + 1) % 2) * data_set_y_size + (iZ % 2) * (data_set_y_size - 1);

			for (iY = 0; iY < dim_y; iY++) {
				if (even_z_level) {
					dim_x = (iY + 1) % 2 * data_set_x_size + (iY % 2) * (data_set_x_size - 1);
				}
				else {
					dim_x = (iY + 1) % 2 * (data_set_x_size - 1) + (iY % 2) * data_set_x_size;
				}
				for (iX = 0; iX < dim_x; iX++) {
					if (voxel_automata_initial_data->voxel_object_data.voxel_matrix_coordinate_activation_status({ iX, iY, iZ }) > voxel_data_type(0)) {  // *** Need to change this for a valid activation value
//QMessageBox::information(NULL, "","assign_no_voxel_state_chnage 002B : " + QString::number(initial_iX)+":"+QString::number(initial_iY)+":"+QString::number(initial_iZ), QMessageBox::Ok);
//QMessageBox::information(NULL, "","assign_no_voxel_state_chnage 002C : " + QString::number(automata_data_index_coord.matrix_index.x)+":"+QString::number(automata_data_index_coord.matrix_index.y)+":"+ QString::number(automata_data_index_coord.matrix_index.z)+":"+QString::number(automata_data_index_coord.j_bit_index), QMessageBox::Ok);
						cloud->voxel_object_data.activate_voxel_matrix_coordinate({ iX, iY, iZ }, 1);
					}
				}
			}
		}
//QMessageBox::information(NULL, "","assign_no_voxel_state_chnage 003", QMessageBox::Ok);
		return true;

	}

	// Create an empty hcp voxel matrix
	bool voxel_automata_hcp_functions_class::create_voxel_matrix() {

		// ########### CREATE EMPTY VOXEL CLOUD MATRIX #################
		float x_size = voxel_automata_generator_parameters.x_end - voxel_automata_generator_parameters.x_start;
		float y_size = voxel_automata_generator_parameters.y_end - voxel_automata_generator_parameters.y_start;
		float z_size = voxel_automata_generator_parameters.z_end - voxel_automata_generator_parameters.z_start;

		float z_mult = 2.0f * sqrt(6.0f) / 3.0f;

		float x_res_step = voxel_automata_generator_parameters.resolution_step * 2.0f;
		float y_res_step = voxel_automata_generator_parameters.resolution_step * (3.0f / sqrt(3.0f));
		float z_res_step = voxel_automata_generator_parameters.resolution_step * z_mult;

		int data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0)
			data_set_x_size = (int)(x_size / x_res_step) + 1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0)
			data_set_y_size = (int)(y_size / y_res_step) + 1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

		if (z_size / z_res_step - float((int)(z_size / z_res_step)) > 0.0)
			data_set_z_size = (int)(z_size / z_res_step) + 1;
		else
			data_set_z_size = (int)(z_size / z_res_step);
//QMessageBox::information(0, "Automata", "Automata create_voxel_matrix 00: "+QString::number(data_set_x_size)+":"+QString::number(data_set_y_size)+":"+QString::number(data_set_z_size)+":", QMessageBox::Ok);

		glm::vec3 origin = { voxel_automata_generator_parameters.x_start,voxel_automata_generator_parameters.y_start,voxel_automata_generator_parameters.z_start };

		cloud->voxel_object_data.voxel_size = voxel_automata_generator_parameters.resolution_step;

		cloud->voxel_object_data.matrix_dimension = { data_set_x_size,data_set_y_size,data_set_z_size };
		cloud->voxel_object_data.matrix_origin = origin;
		cloud->voxel_object_data.create_empty_volume_cubic(data_set_x_size, data_set_y_size, data_set_z_size);
//QMessageBox::information(0, "Automata", "Automata create_voxel_matrix 01: "+QString::number(cloud->voxel_object_data.voxel_matrix_data.size())+":", QMessageBox::Ok);


		if (cloud->voxel_object_data.voxel_matrix_data.size() > 0)
			return true;
		else
			return false;
	}

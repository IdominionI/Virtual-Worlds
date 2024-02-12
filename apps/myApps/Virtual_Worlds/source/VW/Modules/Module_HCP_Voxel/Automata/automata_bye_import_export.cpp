#include "automata_byte_import_export.h"

#include <Universal_FW/Kernal/FWstring_tools.h>
#include <Universal_FW/Kernal/FWfile_tools.h>

#define ENDL '\n'

int import_export_byte_automata_rules_class::open_file_stream(std::string filename, int write_mode) {

	if (write_mode == OVER_WRITE_FUNCTION_FILE) {
		stream.open(filename, std::ios::out | std::ios::out);

		if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
	}
	else
		if (!stream.is_open()) return false;

	return true;
}

void import_export_byte_automata_rules_class::close_file_stream() {
	stream.close();
}

bool import_export_byte_automata_rules_class::export_hcp_automata_byte_rules(std::string file_pathname) {
//std::cout << "", file_pathname, //QMessageBox::Ok);

	if (file_pathname.size() == 0) {
//std::cout << "", "No file name defined to save data to \n Save voxel automata byte rules aborted", //QMessageBox::Ok);
		return false;
	}

	if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
//std::cout << "", "Unable to open file\n" + file_pathname +"\n to save automata data to.", //QMessageBox::Ok);
		return false;
	}

	stream << RULE_DATA_START << ENDL;

	write_hcp_automata_byte_rules(stream);

	stream << RULE_DATA_END << ENDL;

	close_file_stream();

	return true;
}

void import_export_byte_automata_rules_class::write_hcp_automata_byte_rules(std::fstream &stream) {

	for (voxel_hcp_automata_byte_rule_struct_type byte_automata_rule : byte_automata_rules) {
		stream << RULE_BLOCK_START << ENDL;

		stream << byte_automata_rule.name << ENDL;
		stream << byte_automata_rule.active_rule << ENDL;
		stream << byte_automata_rule.voxel_state << ENDL;
		stream << byte_automata_rule.start_step << ENDL;
		stream << byte_automata_rule.end_step << ENDL;

		stream << byte_automata_rule.voxel_self_rule_condition.lower_rule_value << RULE_DELIMINATOR;
		switch (byte_automata_rule.voxel_self_rule_condition.lower_rule) {
			case lower_rule_condition_enum_type::ignore : stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
			case lower_rule_condition_enum_type::LT     : stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
			case lower_rule_condition_enum_type::LTEQ   : stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
		}

		switch (byte_automata_rule.voxel_self_rule_condition.upper_rule) {
			case upper_rule_condition_enum_type::ignore : stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
			case upper_rule_condition_enum_type::NE     : stream << NEIGHBOR_NOT_EQUAL_RULE_CODE << RULE_DELIMINATOR;         break;
			case upper_rule_condition_enum_type::EQ     : stream << NEIGHBOR_EQUAL_RULE_CODE << RULE_DELIMINATOR;             break;
			case upper_rule_condition_enum_type::LT     : stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
			case upper_rule_condition_enum_type::LTEQ   : stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
		}
		stream << byte_automata_rule.voxel_self_rule_condition.upper_rule_value << ENDL;

		for (int i = 0; i < NUMBER_HCP_NEIGHBOURS; i++) {
			stream << byte_automata_rule.neighbours_rule_condition[i].lower_rule_value << RULE_DELIMINATOR;
			switch (byte_automata_rule.neighbours_rule_condition[i].lower_rule) {
				case lower_rule_condition_enum_type::ignore : stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
				case lower_rule_condition_enum_type::LT     : stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
				case lower_rule_condition_enum_type::LTEQ   : stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
			}

			switch (byte_automata_rule.neighbours_rule_condition[i].upper_rule) {
				case upper_rule_condition_enum_type::ignore : stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
				case upper_rule_condition_enum_type::NE     : stream << NEIGHBOR_NOT_EQUAL_RULE_CODE << RULE_DELIMINATOR;         break;
				case upper_rule_condition_enum_type::EQ     : stream << NEIGHBOR_EQUAL_RULE_CODE << RULE_DELIMINATOR;             break;
				case upper_rule_condition_enum_type::LT     : stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
				case upper_rule_condition_enum_type::LTEQ   : stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
			}
			stream << byte_automata_rule.neighbours_rule_condition[i].upper_rule_value << ENDL;
		}

		stream << RULE_BLOCK_END << ENDL;
	}
}

// -------------------------------------------------------------------

bool import_export_byte_automata_rules_class::open_file_to_read(std::string filename) {
	stream.open(filename, std::ios::in);

	if (!stream)
		return false;
	else
		if (!stream.is_open()) return false;

	return true;
}

bool import_export_byte_automata_rules_class::import_hcp_automata_byte_rules(std::vector <voxel_hcp_automata_byte_rule_struct_type> &automata_byte_rules_parameters, std::string file_pathname) {
//std::cout << "", file_pathname, //QMessageBox::Ok);

	if (file_pathname.size() == 0) {
//std::cout << "", "No file name defined to import data from \n Import automata byte rules aborted", //QMessageBox::Ok);
		return false;
	}

	std::fstream working_model_file(file_pathname, std::ios::in);

	if (!working_model_file) {
//std::cout << "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find read Import voxel generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);
		return false;
	}

	automata_byte_rules_parameters.clear();
	automata_byte_rules_parameters.shrink_to_fit();

	std::string working_model_string = FW::filetools::read_all(working_model_file);

	std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
	int line_number = 0;

	read_automata_byte_rules_into_byte_rules_parameters(lines, automata_byte_rules_parameters, line_number);

	stream.close();
	return true;
}

bool import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters(std::vector<std::string> lines, std::vector <voxel_hcp_automata_byte_rule_struct_type> &generator_parameters, int &line_number) {
	std::string line;

	// First line read must be a START_HEADER_FLAG
	line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 1111 : " << line_number << ":" << lines[line_number] << std::endl;
	if (!FW::stringtools::contains(lines[line_number], RULE_DATA_START)) { // There msut allways be at least two nodes. The input and output link nodes
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
		return false;
	}

	line_number++;

	line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 2222 : " << line_number << ":" << lines[line_number] << std::endl;
	while (!FW::stringtools::contains(lines[line_number], RULE_DATA_END)) {
		voxel_hcp_automata_byte_rule_struct_type voxel_hcp_automata_rule;
		if (!read_automata_byte_rule(lines, voxel_hcp_automata_rule, line_number)) return false;

		generator_parameters.push_back(voxel_hcp_automata_rule);

		line_number++;
	}

	return true;
}

bool import_export_byte_automata_rules_class::read_automata_byte_rule(std::vector<std::string> lines, voxel_hcp_automata_byte_rule_struct_type& voxel_hcp_automata_rule, int &line_number) {
	std::string line;

	line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule AAAAAAA : " << line_number << ":" << lines[line_number] << std::endl;
	// next line read must be a RULE_BLOCK_START flag
	if (!FW::stringtools::contains(lines[line_number], RULE_BLOCK_START)) {
//std::cout << "Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing RULE_BLOCK_START flag", //QMessageBox::Ok);
		return false;
	}

	//next 5 lines must have rule name,active_rule,voxel_state,start_step,end_step in that order
	line_number++;
	line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule BBBBBB : " << line_number << ":" << lines[line_number] << std::endl;
	voxel_hcp_automata_rule.name = line;

	line_number++;
	line = lines[line_number];
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule CCCCCC : " << line_number << ":" << lines[line_number] << std::endl;
	if (stoi(line) == 0) voxel_hcp_automata_rule.active_rule = false; else voxel_hcp_automata_rule.active_rule = true;

	line_number++;
	line = lines[line_number];
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule DDDDDD : " << line_number << ":" << lines[line_number] << std::endl;
	voxel_hcp_automata_rule.voxel_state = stoi(line);

	line_number++;
	line = lines[line_number];
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule EEEEE : " << line_number << ":" << lines[line_number] << std::endl;
	voxel_hcp_automata_rule.start_step = stoi(line);

	line_number++;
	line = lines[line_number];
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule FFFFF : " << line_number << ":" << lines[line_number] << std::endl;
	voxel_hcp_automata_rule.end_step = stoi(line);


	// next line contains the self rule definitions where each rule defenintion is seperated by the deliminator RULE_DELIMINATOR
	line_number++;
	line = lines[line_number];
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule 00000AA : " << line_number << ":" << lines[line_number] << std::endl;

	std::vector<std::string> rule_list;
	rule_list = FW::stringtools::split(line, RULE_DELIMINATOR);

	if (rule_list.size() < 4) {
//std::cout <<"Import Automata byte rules", "Error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n"+"Incorrect parameters for self rule", //QMessageBox::Ok);
		return false;
	}


	voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule_value = stoi(rule_list[0]);

	switch (stoi(rule_list[1])) {
		case NEIGHBOR_IGNOR_RULE_CODE              : voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule = lower_rule_condition_enum_type::ignore; break;
		case NEIGHBOR_LESS_THAN_RULE_CODE          : voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule = lower_rule_condition_enum_type::LT;     break;
		case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule = lower_rule_condition_enum_type::LTEQ;   break;
	}

	switch (stoi(rule_list[2])) {
		case NEIGHBOR_IGNOR_RULE_CODE              : voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::ignore; break;
		case NEIGHBOR_NOT_EQUAL_RULE_CODE          : voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::NE;     break;
		case NEIGHBOR_EQUAL_RULE_CODE              : voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::EQ;     break;
		case NEIGHBOR_LESS_THAN_RULE_CODE          : voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::LT;    break;
		case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::LTEQ;   break;
	}
	voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule_value = stoi(rule_list[3]);// .toInt();


	// next NUMBER_HCP_NEIGHBOURS lines must have the neighbour activity states for the rule
	for (int i = 0; i < NUMBER_HCP_NEIGHBOURS; i++) {
		line_number++;
		line = lines[line_number];
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule 11111AA : " << line_number << ":" << lines[line_number] << std::endl;

		std::vector<std::string> rule_list;
		rule_list = FW::stringtools::split(line, RULE_DELIMINATOR);

		if (rule_list.size() < 4) {
///std::cout <<"Import Automata byte rules", "Error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n"+"Incorrect parameters for self rule", //QMessageBox::Ok);
			return false;
		}

		voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule_value = stoi(rule_list[0]);// .toInt();
		switch (stoi(rule_list[1])) {
			case NEIGHBOR_IGNOR_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule = lower_rule_condition_enum_type::ignore; break;
			case NEIGHBOR_LESS_THAN_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule = lower_rule_condition_enum_type::LT;     break;
			case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule = lower_rule_condition_enum_type::LTEQ;   break;
		}

		switch (stoi(rule_list[2])) {
			case NEIGHBOR_IGNOR_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::ignore; break;
			case NEIGHBOR_NOT_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::NE;     break;
			case NEIGHBOR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::EQ;     break;
			case NEIGHBOR_LESS_THAN_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::LT;    break;
			case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::LTEQ;   break;
		}
		voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule_value = stoi(rule_list[3]);// .toInt();
	}

	// next line read must be a RULE_BLOCK_END flag
	line_number++;
	line = lines[line_number];
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule 2222AA : " << line_number << ":" << lines[line_number] << std::endl;

	if (!FW::stringtools::contains(lines[line_number], RULE_BLOCK_END)) {
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing Rule block end flag", //QMessageBox::Ok);
		return false;
	}



	return true;
}


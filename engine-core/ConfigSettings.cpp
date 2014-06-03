#include "ConfigSettings.h"

#include <cassert>

typedef pair <string, string> String_Pair;
ConfigSettings ConfigSettings::config;

ConfigSettings::ConfigSettings(string fname, string template_fname) {
	file_name = fname;
	template_file_name = template_fname;

	if (!loadSettingsFile()){ //loads settings
		copySettingsTemplate();
		cout << "oi";
	}
	else{
		copyMissingSettings();
	}

	getValue("tube_radius", tube_radius);
	getValue("fluid_force", fluid_force);
	getValue("default_drag_coefficient", drag_coefficient);
	getValue("default_max_speed", max_speed);
	getValue("default_max_force", max_force);
	getValue("default_mass", mass);

	// Steering Behavior
	getValue("frame_rate", frame_rate);
	getValue("deceleration_tweaker", deceleration_tweaker);
	getValue("wander_jitter", wander_jitter);
	getValue("wander_radius", wander_radius);
	getValue("wander_distance", wander_distance);
	getValue("weight_wander", weight_wander);
	getValue("weight_follow_path", weight_follow_path);
	getValue("way_point_seek_distance", way_point_seek_distance);

	/* DEBUG
	assert(getValue("tube_radius", tube_radius));
	assert(getValue("fluid_force", fluid_force));
	assert(getValue("default_drag_coefficient", drag_coefficient));
	assert(getValue("default_max_speed", max_speed));
	assert(getValue("default_max_force", max_force));
	assert(getValue("default_mass", mass));

	// Steering Behavior
	assert(getValue("frame_rate", frame_rate));
	assert(getValue("deceleration_tweaker", deceleration_tweaker));
	assert(getValue("wander_jitter", wander_jitter));
	assert(getValue("wander_radius", wander_radius));
	assert(getValue("wander_distance", wander_distance));
	assert(getValue("weight_wander", weight_wander));
	assert(getValue("weight_follow_path", weight_follow_path));
	assert(getValue("way_point_seek_distance", way_point_seek_distance));
	*/
	tube_radius_sq = tube_radius * tube_radius;
}

ConfigSettings::~ConfigSettings() {
}

//Reads and loads the settings
bool ConfigSettings::loadSettingsFile(){

	ifstream myfile;
	myfile.open(file_name);

	if (myfile.is_open()){
		string line;
		while (myfile.good()){
			getline(myfile, line);
			size_t pos = line.find_first_of(':');

			if (pos > 0 && pos < line.length()){
				string key, value;
				key = line.substr(0, pos);
				value = line.substr(pos + 2);

				settings.insert(String_Pair(key, value));
			}
		}
		settings_loaded = true;
		myfile.close();
		return true;
	}

	return false;
}

//clears data read and loads from the settings file
bool ConfigSettings::reloadSettingsFile(){
	settings.clear();
	return loadSettingsFile();
}

//set the default settings from the config file
//loads the config file while copying
void ConfigSettings::copySettingsTemplate() {
	ifstream template_file;
	ofstream myfile;

	template_file.open(template_file_name.c_str());
	myfile.open(file_name.c_str());

	if (template_file.is_open()){
		string line;
		while (template_file.good()){
			getline(template_file, line);

			size_t pos = line.find_first_of(':');
			if (pos > 0 && pos < line.length()){
				string key, value;
				key = line.substr(0, pos);
				value = line.substr(pos + 2);


				if (myfile.is_open()){
					myfile << line << endl;
				}

				settings.insert(String_Pair(key, value)); //loads the config file while copying
			}
		}
		settings_loaded = true;
		template_file.close();
		myfile.close();
	}
}

void ConfigSettings::copyMissingSettings() {

	ifstream template_file;
	fstream myfile;

	template_file.open(template_file_name.c_str());
	myfile.open(file_name.c_str(), fstream::in | fstream::out | fstream::app);

	if (template_file.is_open()){
		string line;
		while (template_file.good()){
			getline(template_file, line);

			size_t pos = line.find_first_of(':');
			if (pos > 0 && pos < line.length()){
				string key, value;
				key = line.substr(0, pos);
				value = line.substr(pos + 2);

				hash_map <string, string>::iterator i = settings.find(key);

				if (i == settings.end()){
					if (myfile.is_open()){
						myfile << line << endl;
					}
					settings.insert(String_Pair(key, value));
				}
			}
		}
		template_file.close();
		myfile.close();
	}

}

void ConfigSettings::saveSettingsFile(){

	ofstream myfile;

	myfile.open(file_name.c_str());

	if (myfile.is_open()){
		string line;
		hash_map <string, string>::iterator endIter = settings.end();
		hash_map <string, string>::iterator iter;

		for (iter = settings.begin(); iter != endIter; iter++){
			myfile << iter->first << ": " << iter->second << endl;
		}

		myfile.close();
	}

}

bool ConfigSettings::getValue(string key, string & ret){
	hash_map <string, string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = i->second;
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(string key, bool & ret){
	hash_map <string, string>::iterator i = settings.find(key);

	if (i != settings.end()){
		if (i->second == "true"){
			ret = true;
		}
		else{
			ret = false;
		}
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(string key, int & ret){
	hash_map <string, string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = atoi(i->second.c_str()); //error results in 0
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(string key, float & ret){
	hash_map <string, string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = (float)atof(i->second.c_str()); //error results in 0
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(string key, double & ret){
	hash_map <string, string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = atof(i->second.c_str()); //error results in 0
		return true;
	}
	return false;
}

void ConfigSettings::updateValue(string key, string value){
	settings.insert(String_Pair(key, value));
}


void ConfigSettings::updateValue(string key, bool value){
	if (value){
		settings.insert(String_Pair(key, "true"));
	}
	else{
		settings.insert(String_Pair(key, "false"));
	}
}

void ConfigSettings::updateValue(string key, int value){
	stringstream s;
	s << value;

	settings.insert(String_Pair(key, s.str()));
}


void ConfigSettings::updateValue(string key, float value){
	stringstream s;
	s << value;

	settings.insert(String_Pair(key, s.str()));
}

void ConfigSettings::updateValue(string key, double value){
	stringstream s;
	s << value;

	settings.insert(String_Pair(key, s.str()));
}

//returns true if loaded the saved settings, otherwise false
bool ConfigSettings::checkIfLoaded() {
	return settings_loaded;
}
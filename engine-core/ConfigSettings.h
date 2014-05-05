#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <hash_map>
#include <iterator>

#include "engine-core.h"

using namespace std;

const string str_settings_file = "resources/Config.ini";
const string str_template_settings_file = "/resources/ConfigTemplate.ini";

class COREDLL ConfigSettings {

public:
	static string str_screen_width;
	static string str_screen_height;
	static string str_full_screen;

public:
	static ConfigSettings * config; // Use this static reference instead of your own call to the constructor

	bool checkIfLoaded();
	void saveSettingsFile();
	bool loadSettingsFile();
	bool reloadSettingsFile();

	void copySettingsTemplate();
	void copyMissingSettings();

	bool getValue(string key, bool & ret);
	bool getValue(string key, int & ret);
	bool getValue(string key, float & ret);
	bool getValue(string key, double & ret);
	bool getValue(string key, string & ret);

	void updateValue(string key, bool value);
	void updateValue(string key, int value);
	void updateValue(string key, float value);
	void updateValue(string key, double value);
	void updateValue(string key, string value);

private:
	//default parameters for a default constructor option
	ConfigSettings(string file_name=str_settings_file, string template_file_name=str_template_settings_file);


	hash_map<string,string> *settings;
	bool settings_loaded;

	string file_name;
	string template_file_name;
};


namespace Utility{
	COREDLL ConfigSettings* configInstance();
}
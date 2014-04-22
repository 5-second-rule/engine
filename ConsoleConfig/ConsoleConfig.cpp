// ConsoleConfig.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\engine-core\engine-core.h"
#include "..\engine-core\ConfigSettings.h"
#include <iostream>

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	int ScreenWidth, t;
	if (ConfigSettings::config->checkIfLoaded()){
		ConfigSettings::config->getValue("ScreenWidth", ScreenWidth);
		ConfigSettings::config->updateValue("New", 24);
		ConfigSettings::config->getValue("New", t);
		cout << ScreenWidth << "\t" << t << endl;
		ConfigSettings::config->saveSettingsFile();
	}
	else {
		cout << "Not Loaded" << endl;
	}
	
	system("pause");
}


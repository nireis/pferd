#ifndef tconfreader_h
#define tconfreader_h

#include "travelconf.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

class tconfreader {
public:
	vector<travelconf>* readConf();
	tconfreader(string fname);
private:
	bool readLine();
	string buffer;
	vector<string> value;
	string filename;
	ifstream f;
	bool working;
};

#endif

#include "conf.h"

void readConf(string fname, conf* cfg){
	string buffer;
	ifstream f;
	f.open(fname.c_str(), ios::in);

	if(f.is_open()){
		f.seekg(0, ios::beg);
		while(!f.eof()){
			getline(f,buffer,'\n');
			if(buffer != ""){
				parseLine(&buffer, cfg);
			}
		}
	}
	else{
		cout << "Fehler: Öffnen der Konfigurationsdatei \"" << fname << "\" fehlgeschlagen." << endl;
	}
	f.close();
}

void parseLine(string* line, conf* cfg){
	/*
	 * Wir prüfen das erste Wort (bis zu ersten Leerzeichen) und entscheiden daran,
	 * welche Konfigurationsvariable gesetzt werden soll und lesen dann den zweiten
	 * Wert aus um diese zu setzten.
	 */
	string::iterator it_begin = line->begin();
	string::iterator it_pos = line->begin()-1;
	string key;
	vector<string> value;
	unsigned int num_comp = 1;
	do{
		it_pos++;
		// Die Werte aus der Datei auslesen.
		if(*it_pos == ' ' || it_pos == line->end()){
			string tmp_str;
			tmp_str.assign(it_begin, it_pos);
			if(num_comp == 1){
				key = tmp_str;
			}
			else{
				value.push_back(tmp_str);
			}
			it_begin = it_pos+1;
			num_comp++;
		}
	}
	while(it_pos != line->end());
	// Den Keystring prüfen nach der zugehörigen Option.
	if(key == "showVis"){
		if(value.size() == 1){
			cfg->showVis = toBool(&value[0]);
		}
		else{
			cout << "Fehler beim Einlesen des Wertes von \"showVis\":" << endl;
			cout << "Mehr als ein Wert oder kein Wert angegeben." << endl;
		}
	}
	else if(key == "playSound"){
		if(value.size() == 1){
			cfg->playSound = toBool(&value[0]);
		}
		else{
			cout << "Fehler beim Einlesen des Wertes von \"playSound\":" << endl;
			cout << "Mehr als ein Wert oder kein Wert angegeben." << endl;
		}
	}
	else if(key == "chConstVerbose"){
		if(value.size() == 1){
			cfg->chConstVerbose = toBool(&value[0]);
		}
		else{
			cout << "Fehler beim Einlesen des Wertes von \"chConstVerbose\":" << endl;
			cout << "Mehr als ein Wert oder kein Wert angegeben." << endl;
		}
	}
	else if(key == "tConfFile"){
		if(value.size() == 1){
			cfg->tConfFile = value[0];
		}
		else{
			cout << "Fehler beim Einlesen des Wertes von \"tConfFile\":" << endl;
			cout << "Mehr als ein Wert oder kein Wert angegeben." << endl;
		}
	}
	else if(key == "soundFile"){
		if(value.size() == 1){
			cfg->soundFile = value[0];
		}
		else{
			cout << "Fehler beim Einlesen des Wertes von \"soundFile\":" << endl;
			cout << "Mehr als ein Wert oder kein Wert angegeben." << endl;
		}
	}
	else{
		cout << "Fehler beim lesen eines keys. Die Option \"" << key << "\" ist unbekannt." << endl;
	}
}

bool toBool(string* s){
	if(*s == "true"){
		return true;
	}
	if(*s != "false"){
		cout << "Kann \"" << *s << "\" nicht von string nach bool parsen." << endl;
	}
	return false;
}

int toInt(string* s){
	stringstream ss(*s);
	int i;
	ss >> i;
	return i;
}

unsigned int toUInt(string* s){
	stringstream ss(*s);
	unsigned int i;
	ss >> i;
	return i;
}

double toDouble(string* s){
	stringstream ss(*s);
	double i;
	ss >> i;
	return i;
}

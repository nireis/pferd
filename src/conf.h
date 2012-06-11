#ifndef conf_h
#define conf_h

#include "structs.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

/*
 * Funktion um die Konfigurationsdatei "fname" einzulesen und in das conf-struct "confstr"
 * zu schreiben, welches per Pointer übergeben wird.
 */
void readConf(string fname, conf* cfg);

/*
 * Funktion um eine gelesene Zeile der Datei zu parsen und die Daten in "confstr" einzutragen.
 */
void parseLine(string* line, conf* cfg);

/*
 * Cast von string nach bool.
 */
bool toBool(string* s);

/*
 * Cast von string nach int.
 */
int toInt(string* s);

/*
 * Cast von string nach unsigned int.
 */
unsigned int toUInt(string* s);

/*
 * Cast von string nach int.
 */
double toDouble(string* s);

#endif

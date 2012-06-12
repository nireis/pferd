#include "graph.h"
#include "graphalgs.h"
#include "CHConstruction.h"
#include "parser.h"
#include <ctime>
#include "structs.h"
#include "ch.h"
#include "rlparser.h"
#include "vis.h"
#include "clust.h"
#include "chdijkstra.h"
#include "dijkstra.h"
#include "sim.h"
#include "conf.h"
#include "travel.h"
#include <thread>
#include <fstream>
#include <stdio.h>

using namespace std;

void startVisThread(volatile bool* active, bool* running, bool* renderMode, Graph** g_pp){
	vis anzeige;
	while(*running){
		if(*renderMode){
			anzeige.start(active, true);
		}
		else{
			if(*g_pp){
				anzeige.initVis(*g_pp);
				anzeige.start(active, false);
			}
		}
		*active = true;
	}
}

void startMusic(string* filename){
	// Musik in Endlosschleife.
	while(true){
		#ifdef _WIN32
			//TODO
		#else
			FILE* pptr;
			char temp[1024];
			sprintf(temp, "mpg123 -q %s", filename->c_str());
			pptr = popen(temp, "w");
			pclose(pptr);
		#endif
	}
}

int main(int argc, char *argv[]){
	cout << "            _|\\__/|, " << endl;
	cout << "          ,((\\````\\\\_" << endl;
	cout << "        ,(())     `))\\ "<< endl;
	cout << "      ,(()))       ,_ \\ "<< endl;
	cout << "     ((())'   |        \\ "<< endl;
	cout << "     )))))     >.__     \\ "<< endl;
	cout << "     ((('     /    `-. .c|" << endl;
	cout << "hjw          /        `-`'" << endl;
	cout << "( http://www.asciiworld.com/-Horses-.html )" << endl;
	cout << " " << endl;
	cout << " " << endl;

	string file;
	Graph** g_pp = new Graph*;
	bool running = true;
	bool renderMode = true;
	thread t_sound;
	thread t_vis;
	volatile bool active;

	bool setVisPerParameter = false;
	bool VisPerParameter = false;
	bool setSoundPerParameter = false;
	bool SoundPerParameter = false;

	if(argc < 2 ){
		cout << "---" << endl 
				<< "-- Aufruf der Binärdatei wie folgt: " << argv[0] << " -g <Graphendatei> [-v]" << endl
				<< "-- -g <Graphdatei> : Pfad zu einer Datei, die als Graphdatei gelesen werden kann." << endl
				<< "-- -v : Visualisierung starten " << endl
				<< "-- -chv : Contraction Hierarchie teilt Statusmeldungen mit " << endl
				<< "---" << endl;
		return 0;
	} else {
		int i = 1;
		while(i < argc){
			if( string(argv[i]) == "-g"){
				if( i+1 < argc){
					file = argv[i+1];
					i += 2;
				} else {
					cout << "Input Error: no Graph File " << endl;
					return 0;
				}
			} else
			if( string(argv[i]) == "-v"){
				setVisPerParameter = true;
				if( i+1 < argc){
					if(string(argv[i+1]) == "off"){
						VisPerParameter = false;
					} else
					if(string(argv[i+1]) == "on"){
						VisPerParameter = true;
					} else {
						cout << "Vis Parameter ungültig: " << argv[i+1] << endl;
						setVisPerParameter = false;
					}
				} else {
					cout << "Vis Parameter ungültig: kein on/off angegeben" << endl;
					setVisPerParameter = false;
				}
				i += 2;
			} else
			if( string(argv[i]) == "-s"){
				setSoundPerParameter = true;
				if( i+1 < argc){
					if(string(argv[i+1]) == "off"){
						SoundPerParameter = false;
					} else
					if(string(argv[i+1]) == "on"){
						SoundPerParameter = true;
					} else {
						cout << "Sound Parameter ungültig: " << argv[i+1] << endl;
						setSoundPerParameter = false;
					}
				} else {
					cout << "Sond Parameter ungültig: kein on/off angegeben" << endl;
					setSoundPerParameter = false;
				}
				i += 2;
			} else {
				/* check if argv[i] existing file */
				ifstream pcheckfile(argv[i]);
				if(pcheckfile){
					file = argv[i];
					i++;
				} else {
					cout << "Input Error. " << endl;
					return 0;
				}
			}
		}
	}

	ifstream checkfile(file.c_str());
	if(!checkfile){
		cout << "-> angegebene Datei '"<< file <<"' existiert nicht." << endl;
		return 0;
	}

	// Lese pferdrc
	cout << "> Lese die Konfigurationsdatei \"pferdrc\"." << endl;
	conf co = conf();
	readConf("pferdrc", &co);

	// Starte Sound
	if(setSoundPerParameter)
		co.playSound = SoundPerParameter;
	if(co.playSound){
		cout << "> Starte Sound" << endl;
		t_sound = thread(&startMusic, &co.soundFile);
	}

	// Starte Visualisierung
	if(setVisPerParameter)
		co.showVis = VisPerParameter;
	if(co.showVis){
		cout << "> Starte Visualisierung" << endl;
		t_vis = thread(&startVisThread, &active, &running, &renderMode, g_pp);
	}

	clock_t start,finish;
	double time;

	cout << "> Erstelle Graph mit Datei " << file << endl;
	start = clock();
	Graph g;
	*g_pp = &g;

	g.setGraph(file, true);

	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "> Zeit zum Initialisieren des Graphen: " << time << endl;

	// starte simulationssachen ab hier
	travelers tr = travelers();

	cout << "> Erzeuge den Verkehr." << endl;
	travelCenter tc = travelCenter(&g, &tr, &co);
	tc.run();

	cout << "> Starte die Simulation." << endl;
	sim s(&g, &tr, &co);
	//cout << "> Warte auf normale Runde." << endl;
	//s.calcOneRoundNormal();
	//s.resetGraph();
	while(!s.eqFound()){
		int rounds;
		cout << "> Warte auf Runde der Simulation." << endl;
		if(rounds != 0){
			for(int i=0; i<rounds; i++){
				s.calcOneRoundCH();
			}
		}
		else{
			while(!s.eqFound()){
				s.calcOneRoundCH();
			}
		}
		if(co.showVis){
			// Zeige Map an.
			active = false;
			renderMode = false;
		}
		cout << "Übergebe die Anzahl der Runden, welche berechnet werden sollen." << endl;
		cout << "Übergebe die 0, wenn so lange gerechnet werden soll, bis ein Equilibrium" << endl;
		cout << "gefunden wird: ";
		cin >> rounds;
		if(co.showVis){
			// Zeige Pferd an
			active = false;
			renderMode = true;
		}
	}

	if(co.showVis){
		cout << "> Warte auf VisThread: join()" << endl;
		running = false;
		t_vis.join();
	}

	cout << "> Exit Pferd" << endl;

	*g_pp = 0;
	delete g_pp; g_pp = 0;

	return 0;
}

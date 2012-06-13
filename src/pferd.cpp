#include "graph.h"
#include "graphalgs.h"
#include "CHConstruction.h"
#include "parser.h"
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
#include <ctime>

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

	int setVisPerParameter = 0;
	int setSoundPerParameter = 0;
	int setCHverbosePerParameter = 0;

	if(argc < 2 ){
		cout << "---" << endl 
				<< "-- Aufruf der Binärdatei wie folgt: " << argv[0] << " -g <Graphendatei> [-v]" << endl
				<< "-- -g <Graphdatei> : Pfad zu einer Datei, die als Graphdatei gelesen werden kann." << endl
				<< "-- -v {on,off} : Visualisierung starten/nicht starten " << endl
				<< "-- -chv {on,off}: Contraction Hierarchie teilt Statusmeldungen mit oder nicht " << endl
				<< "-- -s {on,off} : Sound starten/nicht starten " << endl
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
				setVisPerParameter = 2;
				if( i+1 < argc){
					if(string(argv[i+1]) == "on"){
						setVisPerParameter = 2;
					} else
					if(string(argv[i+1]) == "off"){
						setVisPerParameter = 1;
					} else {
						cout << "Vis Parameter ungültig: " << argv[i+1] << endl;
						setVisPerParameter = 0;
					}
				} else {
					cout << "Vis Parameter ungültig: kein on/off angegeben" << endl;
					setVisPerParameter = 0;
				}
				i += 2;
			} else
			if( string(argv[i]) == "-chv"){
				setVisPerParameter = 2;
				if( i+1 < argc){
					if(string(argv[i+1]) == "on"){
						setCHverbosePerParameter = 2;
					} else
					if(string(argv[i+1]) == "off"){
						setCHverbosePerParameter = 1;
					} else {
						cout << "CH Verbose Parameter ungültig: " << argv[i+1] << endl;
						setCHverbosePerParameter = 0;
					}
				} else {
					cout << "CH Verbose Parameter ungültig: kein on/off angegeben" << endl;
					setCHverbosePerParameter = 0;
				}
				i += 2;
			} else
			if( string(argv[i]) == "-s"){
				setSoundPerParameter = 0;
				if( i+1 < argc){
					if(string(argv[i+1]) == "on"){
						setSoundPerParameter = 2;
					} else
					if(string(argv[i+1]) == "off"){
						setSoundPerParameter = 1;
					} else {
						cout << "Sound Parameter ungültig: " << argv[i+1] << endl;
						setSoundPerParameter = 0;
					}
				} else {
					cout << "Sond Parameter ungültig: kein on/off angegeben" << endl;
					setSoundPerParameter = 0;
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

	//ggf CH Verbose anpassen
	if(setCHverbosePerParameter)
		co.chConstVerbose = setCHverbosePerParameter-1;

	// Starte Sound
	if(setSoundPerParameter)
		co.playSound = setSoundPerParameter-1;
	if(co.playSound){
		cout << "> Starte Sound" << endl;
		t_sound = thread(&startMusic, &co.soundFile);
	}

	// Starte Visualisierung
	if(setVisPerParameter)
		co.showVis = setVisPerParameter-1;
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
	cout << "> Graph Parameter ~ Knoten: " << g.getNodeCount() 
		<< ", Kanten: " << g.getEdgeCount() << endl;

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
	unsigned int c = 3;
	while(c){
		c--;
		cout << "> Warte auf Runde der Simulation." << endl;
		s.calcOneRoundNormal();
		//s.calcOneRoundCH();
		if(co.showVis){
			active = false;
			renderMode = false;
			cout << "> Drücke ANY KEY um eine weitere Runde zu simulieren..." << endl;
			cin.get();
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

	//*g_pp = 0;
	//delete g_pp; g_pp = 0;

	return 0;
}


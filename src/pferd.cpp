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

/*
 * Vis Stuff for easy use 
 */
bool running = true;
bool renderMode = true;
volatile bool active=false;

struct vis_stuff {
	int argc;
	char** argv;
	volatile bool* active;
	bool* running;
	bool* renderMode;
	Graph** g_pp;
	std::list<Edge>* shortcuts;

	vis_stuff(int a, char** b, volatile bool* c, bool* d, bool* e, Graph** f, std::list<Edge>* scuts) :
		argc(a),
		argv(b),
		active(c),
		running(d),
		renderMode(e),
		g_pp(f),
		shortcuts(scuts)
	{}
	//vis_stuff( const vis_stuff& vs ) :
	//	argc(vs.argc),
	//	argv(vs.argv),
	//	active(vs.active),
	//	running(vs.running),
	//	renderMode(vs.renderMode),
	//	g_pp(vs.g_pp)
	//{}
	~vis_stuff()
	{
		argv=0;
		active=0;
		running=0;
		renderMode=0;
		g_pp=0;
		shortcuts = 0;
	}
};

void startVisThread( vis_stuff vs ){
	vis anzeige(vs.argc, vs.argv);
	while(*vs.running){
		if(*vs.renderMode){ 
		// if false draw graph new
		// if true draw horse

			anzeige.start(vs.active, true);
		}
		else{
			if(*vs.g_pp){
				anzeige.initVis(*vs.g_pp, vs.shortcuts);
				anzeige.start(vs.active, false);
			}
		}
		*vs.active = true;
	}
}
void setVisHorse(){
	active = false;
	renderMode = true;
}
void setVisGraph(){
		active = false;
		renderMode = false;
}

void startMusic(string* filename){
	// Musik in Endlosschleife.
	//while(true){
		#ifdef _WIN32
			//TODO
		#else
			FILE* pptr;
			char temp[1024];
			sprintf(temp, "mpg123 -q %s", filename->c_str());
			pptr = popen(temp, "w");
			pclose(pptr);
		#endif
	//}
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
	thread t_sound;
	thread t_vis;

	int setVisPerParameter = 0;
	int setSoundPerParameter = 0;
	int setCHverbosePerParameter = 0;
	bool setXperParameter = false;
	int setXParameter = 0;

	if(argc < 2 ){
		cout << "---" << endl 
				<< "-- Aufruf der Binärdatei wie folgt: " << argv[0] << " -g <Graphendatei> [-v]" << endl
				<< "-- -g <Graphdatei> : Pfad zu einer Datei, die als Graphdatei gelesen werden kann." << endl
				<< "-- -v {on,off} : Visualisierung starten/nicht starten " << endl
				<< "-- -chv {on,off}: Contraction Hierarchie teilt Statusmeldungen mit oder nicht " << endl
				<< "-- -s {on,off} : Sound starten/nicht starten " << endl
				<< "-- -X integer : benutze Modus X mit Rundenwert integer (mit Vorzeichen) " << endl
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
			} else 
			if( string(argv[i]) == "-X"){
				if( i+1 < argc ){
					setXParameter = atoi(argv[i+1]);
					setXperParameter = true;
				}
				i += 2;
			} else {
				/* check if argv[i] existing file */
				ifstream pcheckfile(argv[i]);
				if(pcheckfile){
					file = argv[i];
					i++;
				} else {
					cout << "Input Error. Ignoring " << argv[i] << endl;
					// return 0;
					i++;
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
	list<Edge>* shortcuts = 0;
	if(setVisPerParameter)
		co.showVis = setVisPerParameter-1;
	if(co.showVis){
		shortcuts = new list<Edge>();
		cout << "> Starte Visualisierung" << endl;
		t_vis = thread( &startVisThread, vis_stuff( argc, argv, &active, &running, &renderMode, g_pp , shortcuts) );
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
	sim s(&g, &tr, &co, shortcuts);

	// TODO all2all + Vis Modus bereitstellen
	//cout << "> Warte auf normale Runde." << endl;
	//s.calcOneRoundNormal();
	//s.resetGraph();
	
	bool eq_found = false;
	int rounds = -1;

	// Abfrage nach Modus: SimCH
	cout << endl;
	cout << "Übergebe die Anzahl X der Runden , welche berechnet werden sollen:" << endl;
	cout << "-- X = 0 => Führe Simulationsrunden durch, bis Gleichgewicht erreicht." << endl
		<< "            (starte danach Visualisierung, falls diese aktiv)." << endl;
	cout << "-- X > 0 => Führe X Simulationsrunden hintereinander durch." << endl
		<< "            (starte danach Visualisierung, falls diese aktiv)." << endl;
	cout << "-- X < 0 => Führe |X| Simulationsrunden hintereinander durch." << endl 
		<< "            (starte nach JEDER Runde Visualisierung, falls diese aktiv)." << endl;
	cout << "" << endl;

	if(! setXperParameter){
		cout << " >> X = " ;
		cin >> rounds;
		cin.clear();
		cin.sync();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} else {
		cout << "Benutze Parametereingabe X = " << setXParameter << endl;
		rounds = setXParameter;
	}

	cout << "" << endl;

//	cout << "INTERMISSION: Zeige Karte an" << endl;
//		setVisGraph();
//		cout << "> Drücke EINGABETASTE um auszublenden" << endl;
//		cin.get();
//		setVisHorse();


	//anhand Modus jeweils auswählen
	if( rounds == 0){ 
		cout << "> >> Modus finde Gleichgewicht alleine" << endl;
		// loope unendlich bis eq_found==true und zeige dann evtl Vis an
		while( ! eq_found ){
			rounds++;
			cout << endl << "> Warte auf Runde " << rounds << " der Simulation." << endl;
			s.calcOneRoundCH();
			eq_found = s.eqFound();
		}
		setVisGraph();
		cout << "> Gleichgewicht=" << eq_found << " gefunden nach " << rounds << " gelaufenen Runden" << endl;
		cout << "> Drücke EINGABETASTE um Programm zu beenden" << endl;
		cin.get();
	} 
	else 
	if( rounds > 0){ 
		cout << "> >> Modus finde Gleichgewicht in " << rounds << " Runden hintereinander " << endl;
		// loope rounds runden und zeige vis evtl am ende an
		unsigned int  running_rounds = 0;
		for(unsigned int i = 0; i < (unsigned int)(rounds); i++){
			cout << endl << "> Warte auf Runde "<< running_rounds++ <<" der Simulation." << endl;
			s.calcOneRoundCH();
		}
		eq_found = s.eqFound();
		setVisGraph();
		cout << "> Gleichgewicht=" << eq_found << " gefunden nach " << rounds << " manuellen Runden" << endl;
		cout << "> Drücke EINGABETASTE um Programm zu beenden" << endl;
		cin.get();
	} 
	else { 
		// rounds negativ 
		//=> loope |rounds| runden und zeige nach jeder runde vis an
		cout << "> >> Modus finde Gleichgewicht in " << "|" << rounds << "| " << " Runden hintereinander, mit Stop pro Runde " << endl;
		int running_rounds = 1;
		for(unsigned int i = 0; i < (unsigned int)(-rounds); i++){
			cout << endl << "> Warte auf Runde "<< running_rounds++ <<" der Simulation." << endl;
			s.calcOneRoundCH();

			cout << "Runde " << running_rounds << " fertig";
			if(co.showVis){
				cout << ", zeige neuen Graph in Vis an" << endl;
			} else {
				cout << endl;
			}

			// für alle runden ausser die letzte
			if(i < (unsigned int)(-rounds-1)){
				setVisGraph();
				cout << "> Drücke EINGABETASTE für weitere Runde";
				if(co.showVis){
					cout << " und wechseln der Vis " << endl;
				} else {
					cout << endl;
				}
				cin.get();
				setVisHorse();
			}

		}
		eq_found = s.eqFound();
		setVisGraph();
		cout << "> Gleichgewicht=" << eq_found << " gefunden nach " << -rounds << " manuellen Runden" << endl;
		cout << "> Drücke EINGABETASTE um Programm zu beenden" << endl;
		cin.get();
	}


	if(co.showVis){
		delete shortcuts; 
		cout << "> Warte auf VisThread: join()" << endl;
		running = false;
		setVisHorse();
		t_vis.join();
	}
	shortcuts = 0;

	cout << "> Exit Pferd" << endl;

	*g_pp = 0;
	delete g_pp; g_pp = 0;

	return 0;
}

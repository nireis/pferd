#include "graph.h"
#include "graphalgs.h"
#include "CHConstruction.h"
#include "parser.h"
#include <ctime>
#include "structs.h"
#include "ch.h"
#include "rlparser.h"
#include "openGLrender.h"
#include "vis.h"

using namespace std;

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


	// string file = "../data/15000.txt.grp";
   
   if(argc != 2){
		cout << "---" << endl 
				<< "-- Aufruf der Binärdatei wie folgt: " << argv[0] << " Graphendatei " << endl
				<< "-- Graphdatei: Pfad zu einer Datei, die als Graphdatei gelesen werden kann." << endl
				<< "---" << endl;
		return 0;
	} 

	string file = argv[1];

	ifstream checkfile(file.c_str());
	if(!checkfile){
		cout << "-> angegebene Datei existiert nicht." << endl;
		return 0;
	}
   

	clock_t start,finish;
	double alltime = 0.0;
	double time;

	cout << "Erstelle Graph mit Datei " << file << endl;
	start = clock();
	Graph g = Graph();

	g.setGraph(file, true);

	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit zum Initialisieren des Graphen: " << time << endl;

	cout << "Neuen Graph erstellen: Eingabetaste" << endl;
	cin.get();

	cout << "Erstelle neuen Graph: " << endl;
	start = clock();
	SCGraph scg = SCGraph(&g);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "SCGraph erstellt in zeit: : " << time << endl;
	

	for(int i=0; i<1; i++){
		start = clock();
		TDijkstra<SCGraph>(&scg, i);
		finish = clock();
		time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Zeit für Template-Dijkstra mit SCGraph von " << i << " aus: "<< time << endl;
	}

	list<Shortcut>* sclist = scg.getShortcutListPointer();
	list<unsigned int>* nodelist = scg.getBlackNodesListPointer();
	list<Shortcut> drawSClist = list<Shortcut>();
	CHConstruction<SCGraph> chc(&scg);

	bool run = true;

	unsigned int j = 1;

	while(run){
		cout << "Berechne Shortcuts" << endl;
		start = clock();
		run = chc.calcOneRound(sclist, nodelist);
		finish = clock();
		time = (double(finish)-double(start));
		alltime += time;
		time = time /CLOCKS_PER_SEC;
		cout << "Zeit: " << time << endl;

		cout << "Merge Shortcuts in SCGraph. " << endl;
		start = clock();
		scg.mergeRoundNegative(j);
		finish = clock();
		time = (double(finish)-double(start));
		alltime += time;
		time = time /CLOCKS_PER_SEC;
		cout << "Zeit: " << time << endl;
		cout << " => Runde " << j << " fertig."  << endl;
		j++;
	}
	cout << "Insgesamt gebrauchte Zeit für Runden: " << (alltime/CLOCKS_PER_SEC) / 60.0 << " Minuten " << endl;
	cout << "Merge Shortcuts und original-Graph. " << endl;
	start = clock();
	scg.mergeShortcutsAndGraph(j);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit: " << time << endl;

	cout << "Runden fertig. Eingabetaste für Ende des Programms." << endl;

	vis anzeige(&scg);
	anzeige.start();

	cin.get();

	return 0;
}

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
#include "sim.h"

#include <thread>

using namespace std;

void startVisThread(SCGraph* g){
	cout << "> Starte Visualisierung" << endl;
	vis anzeige(g); anzeige.start();
}
//void startVisThread(Graph* g){
//	cout << "> Starte Visualisierung" << endl;
//	vis anzeige(g); anzeige.start();
//}


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
	bool startVis = false;
	bool chverbose = false;

	if(argc < 3 || argc > 5){
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
					cout << "Input Error. " << endl;
					return 0;
				}
			} else
			if( string(argv[i]) == "-v"){
				startVis = true;
				i++;
			} else
			if( string(argv[i]) == "-chv"){
					chverbose = true;
					i++;
			} else {
				cout << "Input Error. " << endl;
				return 0;
			}
		}
	}

	ifstream checkfile(file.c_str());
	if(!checkfile){
		cout << "-> angegebene Datei '"<< file <<"' existiert nicht." << endl;
		return 0;
	}

	clock_t start,finish;
	double time;

	cout << "Erstelle Graph mit Datei " << file << endl;
	start = clock();
	Graph g = Graph();

	g.setGraph(file, true);

	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit zum Initialisieren des Graphen: " << time << endl;

	/* erstelle Simulation und i
	 * lasse Kantengewichte an 
	 * Reisezeit des Kantentyps anpassen 
	 */
	cout << "> Erstelle Simulation und passe Kantengewichte an" << endl;
	simulation sim(&g);
	sim.setEdgeValues(g.getEdgeDataPointer(), g.getEdgeCount());
	g.updateEdgeValues();

	cout << "Erstelle neuen Graph: " << endl;
	start = clock();
	SCGraph scg = SCGraph(&g);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "SCGraph erstellt in zeit: : " << time << endl;

	cout << "> Berechne CH auf SCGraph" << endl;
	CH hy(&g, &scg);
	//hy.calcCHverbose();
	hy.calcCH(chverbose);

	//CHDijkstraTest(&g, &scg, 149909);
	
	/* male per cluster ein paar gebiete an */
	{
		cout << "> Erstelle Cluster und starte Pendler" << endl;
		double step = 0.01/32.0;
		cluster cl(&g, step);
		/* zweites Argument ist die Randlänge einer Zelle
		 * in einer unbestimmten Einheit 
		 */

		unsigned int count = 150;
		cl.setMostPopulatedCells( count );
		
		list<unsigned int> starts;
		list<unsigned int> targets;

		double perc = 0.1;
		unsigned int upper = (unsigned int)(((double)count)*perc);
		
		cl.getNodesLower(5,(count-upper)/1, &starts);

		while( ! starts.empty()){
			EdgesIterator it = scg.getOutEdgesIt( starts.front() );
			starts.pop_front();
			while(it.hasNext()){
				Edge* e = it.getNext();
				if( ! scg.isShortcut(e->id) )
					scg.addEdgeLoad(e->id,50);
			}
		}

		cl.getNodesUpper(1,upper, &targets);
		cl.getNodesLower(38,(count-upper)/1, &starts);

		/* starte Dijkstras von starts zu targets */
		cout << "> Starte Dijkstras " << endl;
		CHDijkstras chd(&scg);
		for(list<unsigned int>::iterator it = targets.begin(); 
				it != targets.end(); it++)
		{
			for(list<unsigned int>::iterator it2 = starts.begin();
					it2 != starts.end(); it2++)
			{
				chd.oneToOne(*it2, *it, 1);
			}
		}
		
	}
	scg.updateEdgeLoads();
	scg.shareShortcutLoads();

	sim.setSCGraph(&scg);
	sim.setEdgeColours(scg.getEdgeDataPointer(), scg.getEdgeCount());

	if( startVis ){
		thread t = thread(&startVisThread, &scg);
		t.join();
		//cout << "> Starte Visualisierung" << endl;
		//vis anzeige(&scg); anzeige.start();
	}

	cout << "> Exit Pferd" << endl;

	return 0;
}
	
	// Markiere einen Weg im Graph
	/* von Tübinger Vorstadt, Herman-Kurz-Schule / Christuskirche 
	 * bis Jüngingen, Killertal Apotheke/ Cafe Anlitz
	 * auf 15K Graph
	 */
	//CHDijkstra(&scg, 2271, 252); 

	/*CHDijkstras chd(&scg);
	for(unsigned int i=0; i<=scg.getNodeCount(); i++){
		if(chd.oneToOne(5, i) != CHDijkstra(&scg, 5, i)){
			cout << "Error!" << endl;
		}
	}*/


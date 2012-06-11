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

using namespace std;

void startVisThread(bool* active){
	cout << "> Starte Visualisierung" << endl;
	vis anzeige; anzeige.start(active, true);
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
	// bool startVis = false;
	// bool chverbose = false;

	if(argc < 2 || argc > 5){
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
				// startVis = true;
				i++;
			} else
			if( string(argv[i]) == "-chv"){
					// chverbose = true;
					i++;
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

	// Starte Visualisierung

	thread t;
	bool active;
	if(co.showVis){
		cout << "> Starte Visualisierung" << endl;
		t = thread(&startVisThread, &active);
	}

	clock_t start,finish;
	double time;

	cout << "> Erstelle Graph mit Datei " << file << endl;
	start = clock();
	Graph g = Graph();

	g.setGraph(file, true);

	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "> Zeit zum Initialisieren des Graphen: " << time << endl;

	// starte simulationssachen ab hier
	
	travelers tr = travelers();

	/* traffic options */
//	co.mode = 2;
//	co.max_travelers = 50;
//	co.source_count = 10; // ??
//	co.target_count = 10; // ??
//
//	co.weight_lower_bound = 20;
//	co.weight_upper_bound = 40;
//	
//	/* cluster options */
//	co.clust_step = 0.01/8.0;
//	co.clust_top_percentage = 0.1;
//	co.clust_count_top_clusters = 20;
//	co.clust_top_uppers = 2;
//	co.clust_top_lowers = 8;
//	co.clust_top_upper_nodecount_per_cluster = 3;
//	co.clust_top_lower_nodecount_per_cluster = 20000;

	cout << "> Erzeuge den Verkehr." << endl;
	travelCenter tc = travelCenter(&g, &tr, &co);
	tc.run();

	cout << "> Starte die Simulation." << endl;
	sim s(&g, &tr, &co);
	cout << "> Warte auf Runde." << endl;
	s.calcOneRoundNormal();
	s.resetGraph();
	s.calcOneRoundCH();

	cout << "> Warte auf Thread: join()" << endl;
	if(co.showVis){
		t.join();
	}
	cout << "> Exit Pferd" << endl;
	return 0;
}

//	/* erstelle Simulation und i
//	 * lasse Kantengewichte an 
//	 * Reisezeit des Kantentyps anpassen 
//	 */
//	cout << "> Erstelle Simulation und passe Kantengewichte an" << endl;
//	simulation osim(&g);
//	osim.setEdgeValues(g.getEdgeDataPointer(), g.getEdgeCount());
//	g.updateEdgeValues();
//
	//cout << "Erstelle neuen Graph: " << endl;
	//start = clock();
	//SCGraph scg = SCGraph(&g);
	//finish = clock();
	//time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	//cout << "SCGraph erstellt in zeit: : " << time << endl;

	//cout << "> Berechne CH auf SCGraph" << endl;
	//CH hy(&g, &scg);
	////hy.calcCHverbose();
	//hy.calcCH(chverbose);
//
//	//CHDijkstraTest(&g, &scg, 149909);
//	
//	/* male per cluster ein paar gebiete an */
//	list<unsigned int> starts;
//	list<unsigned int> targets;
//	list<openGL_Cluster> clist;
//	double step = 0.01/32.0;
//	{
//		cout << "> Erstelle Cluster und starte Pendler" << endl;
//		cluster cl(&g, step);
//		/* zweites Argument ist die Randlänge einer Zelle
//		 * in einer unbestimmten Einheit
//		 */
//
//		unsigned int count = 40;
//		cl.setMostPopulatedCells( count );
//
//		double perc = 0.1;
//		unsigned int upper = (unsigned int)(((double)count)*perc);
//		
//		//cl.getNodesLower(5,(count-upper), &starts);
//
//		while( ! starts.empty()){
//			EdgesIterator it = scg.getOutEdgesIt( starts.front() );
//			starts.pop_front();
//			while(it.hasNext()){
//				Edge* e = it.getNext();
//				if( ! scg.isShortcut(e->id) )
//					scg.addEdgeLoad(e->id,1);
//			}
//		}
//
//		cl.getNodesUpper(1,upper, &targets, &clist);
//		cl.getNodesLower(68,(count-upper), &starts, &clist);
//
//		/* starte Dijkstras von starts zu targets */
//		cout << "> Starte Dijkstras " << endl;
//		CHDijkstra chd(&scg);
//		for(list<unsigned int>::iterator it = targets.begin(); 
//				it != targets.end(); it++)
//		{
//			for(list<unsigned int>::iterator it2 = starts.begin();
//					it2 != starts.end(); it2++)
//			{
//				chd.oneToOne(*it2, *it, 1);
//			}
//		}
//		
//	scg.updateEdgeLoads();
//	scg.shareShortcutLoads();
//	}
//	
//
//	osim.setSCGraph(&scg);
//	osim.setEdgeColours(scg.getEdgeDataPointer(), scg.getEdgeCount());
//
//	for(list<unsigned int>::iterator it = targets.begin(); 
//			it != targets.end(); it++)
//	{
//		clist.push_front( 
//			openGL_Cluster(
//			scg.getNodeData(*it).lon, /* X */
//			scg.getNodeData(*it).lat, /* Y */
//			step, /* Radius == Step Size */
//			1.0 /* Colour == NodeID */
//			) );
//	}



//list<unsigned int> starts;
//list<unsigned int> targets;
//list<openGL_Cluster> clist;
//double step = 0.01/12.0;
//	cout << "> Erstelle Cluster und starte Pendler" << endl;
//	cluster cl(&g, step);
//	/* zweites Argument ist die Randlänge einer Zelle
//	 * in einer unbestimmten Einheit
//	 */
//
//	unsigned int count = 14;
//	cl.setMostPopulatedCells( count );
//
//	double perc = 0.2;
//	unsigned int upper = (unsigned int)(((double)count)*perc);
//
//	cl.getNodesUpper(1,upper, &targets, &clist);
//	cl.getNodesLower(40,(count-upper), &starts, &clist);
//
//	tr.traffic.push_back(pendler());
//	tr.traffic[0].weight = 1;
//	while( ! starts.empty() ){
//		tr.traffic[0].source.push_back( starts.front() );
//		starts.pop_front();
//	}
//	while( ! targets.empty() ){
//		tr.traffic[0].target.push_back( targets.front() );
//		targets.pop_front();
//	}
//
//	while( ! clist.empty() ){
//		tr.circles.push_back( clist.front() );
//		clist.pop_front();
//	}

	
//	CHDijkstra* chd = new CHDijkstra(&scg);
//	
//		for(unsigned int i = 0; i<tr.traffic[0].source.size() ; i++){
//			for(unsigned int j = 0; j <tr.traffic[0].target.size() ; j++){
//				chd->oneToOne(tr.traffic[0].source[i], tr.traffic[0].target[j], tr.traffic[0].weight);
//			}
//		}
//
//	delete chd; chd = 0;
//	
//	scg.updateEdgeLoads();
//	scg.shareShortcutLoads();
//	g.getEdgeLoads(&scg);
//	g.updateEdgeLoads();
//
//	for(unsigned int i = 0; i < g.getEdgeCount(); i++){
//		EdgeData* ed = g.getEdgeDataPointer();
//		if(ed[i].load != 0)
//			ed[i].colour = 1.0;
//	}

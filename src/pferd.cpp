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

	string file;
	bool startVis = false;

	if(argc < 3 || argc > 4){
		cout << "---" << endl 
				<< "-- Aufruf der Binärdatei wie folgt: " << argv[0] << " -g <Graphendatei> [-v]" << endl
				<< "-- -g <Graphdatei> : Pfad zu einer Datei, die als Graphdatei gelesen werden kann." << endl
				<< "-- -v : Visualisierung starten " << endl
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

	cout << "Erstelle neuen Graph: " << endl;
	start = clock();
	SCGraph scg = SCGraph(&g);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "SCGraph erstellt in zeit: : " << time << endl;

	//CH hy(&g, &scg);
	////hy.calcCHverbose();
	//hy.calcCH();

	cluster cl(&g, 0.01/16.0); /* 8.0 für gröber, 32.0 für sehr fein */
	scg.mergeShortcutsAndGraph(1);

	/*
	 * ein paar kanten einfärben, 
	 * indem man ihnen ein load von mindestens 1 gibt
	 * und diese dann in der vis anzeigen lassen
	 */
	//unsigned int* colors = 
	//	new unsigned int[scg.getEdgeCount() + scg.getShortcutCount() ];
	//for(unsigned int i = 0; i < scg.getEdgeCount() 
	//		+ scg.getShortcutCount(); i++){
	//	if( scg.isShortcut(i) ){ 
	//		colors[i] = 1;
	//	} else {
	//		colors[i] = 0;
	//	}
	//}
	//scg.updateEdgeLoads(colors, scg.getEdgeCount() + scg.getShortcutCount());
	//scg.shareShortcutLoads(); // shortcut loads verteilen

//	for(unsigned int i=0; i<scg.getNodeCount(); i++){
//		EdgesIterator it = scg.getOutEdgesIt(i);
//		while(it.hasNext()){
//			EdgesIterator it1 = scg.getOutEdgesIt(i);
//			int count = 0;
//			unsigned int id1 = it.getNext()->other_node;
//			while(it1.hasNext()){
//				if(id1 == it1.getNext()->other_node){
//					count++;
//				}
//			}
//			if(count > 1){
//				cout << i << endl;
//				cout << "Fuck you!" << endl;
//			}
//		}
//	}
//
//	for(unsigned int i=0; i<g.getNodeCount(); i++){
//		EdgesIterator it = g.getOutEdgesIt(i);
//		while(it.hasNext()){
//			EdgesIterator it1 = g.getOutEdgesIt(i);
//			int count = 0;
//			unsigned int id1 = it.getNext()->other_node;
//			while(it1.hasNext()){
//				if(id1 == it1.getNext()->other_node){
//					count++;
//				}
//			}
//			if(count > 1){
//				cout << "Penis!" << endl;
//			}
//		}
//	}

	//CHDijkstraTest(&g, &scg, 149909);
	
	// Markiere einen Weg im Graph
	/* von Tübinger Vorstadt, Herman-Kurz-Schule / Christuskirche 
	 * bis Jüngingen, Killertal Apotheke/ Cafe Anlitz
	 * auf 15K Graph
	 */
	//CHDijkstra(&scg, 2271, 252); 
	//scg.updateEdgeLoads();
	//scg.shareShortcutLoads();


	if( startVis ){
		vis anzeige(&scg); anzeige.start();
	}

	return 0;
}

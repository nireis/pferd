#include "graph.h"
#include "graphalgs.h"
#include "CHConstruction.h"
#include "parser.h"
#include <ctime>
#include "structs.h"
#include "ch.h"
#include "rlparser.h"

using namespace std;



//	void CallSCGraph(Graph* g){
//		clock_t start,finish;
//		double time;
//		
//		SCGraph scg = SCGraph(g);
//		scg.setShortcuts();
//		
//		for(int i=0; i<10; i++){
//			start = clock();
//			Dijkstra(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für normalen Dijkstra von " << i << " aus: "<< time << endl;
//		}
//		for(int i=0; i<10; i++){
//			start = clock();
//			TDijkstra<SCGraph>(&scg, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für Template-Dijkstra mit statischer CH von " << i << " aus: "<< time << endl;
//		}
//		
//		cout << "Taste drücken zum Erstellen von m Shortcuts. " << endl;
//		cin.get();
//		
//		start = clock();
//			unsigned int scc = g->getEdgeCount();
//				for(unsigned int i = 0; i < scc; i++){
//				Shortcut s;
//				s.value = ((int)( i*i*3.141592)) % 100;
//				s.source = i%g->getNodeCount();
//				s.target = i*(i+1)%(g->getNodeCount());
//		      s.papa_edge = i*i % g->getEdgeCount();
//		      s.mama_edge = i*(i+1) % g->getEdgeCount();
//				scg.addShortcut(s);
//			}
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Shortcuts bereitgelegt. Benötigte Zeit:" << time << endl;
//			cout << "Nun initialisieren der Shortcuts.Taste drücken. " << endl;
//			cin.get();
//		
//			start = clock();
//			scg.setShortcuts();
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit, m Shortcuts und m vorhandene Kanten zu initialisieren: " << time << endl;
//			cout << "Nun löschen der Shortcuts. Taste drücken. " << endl;
//			cin.get(); 
//		
//			scg.clearShortcutlist();
//			scg.clearShortcuts();
//		
//		//	start = clock();
//		//	unsigned int is = scg.calcIndepSet();
//		//	finish = clock();
//		//	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//		//
//		//	cout << "Habe Independent Set der Größe > " << is << " < aufgebaut in: " << time << endl;
//	}
//	
//	void dijkstra_tests(Graph* g){
//	
//		clock_t start,finish;
//		double time;
//	
//		cout << "Dijkstra angefangen." << endl;
//		for(int i=0; i<10; i++){
//			start = clock();
//			DirectDijkstra(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für Dijkstra direkt auf dem Graphen von " << i << " aus: "<< time << endl;
//		}
//		for(int i=0; i<10; i++){
//			start = clock();
//			Dijkstra(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für normalen Dijkstra von " << i << " aus: "<< time << endl;
//		}
//		for(int i=0; i<10; i++){
//			start = clock();
//			Dijkstra_plus(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für 'optimierten' Dijkstra und eigenem Bitvector von " << i << " aus: "<< time << endl;
//		}
//		
//			start = clock();
//			DijkstraC<Graph> dc = DijkstraC<Graph>( g );
//			finish = clock();
//			cout << "Zeit zum Erstellen des Dijkstra-Objektes: "<< time << endl;
//		for(int i=0; i<10; i++){
//			start = clock();
//			dc.run(i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für Dijkstra-Klasse normal von " << i << " aus: "<< time << endl;
//		}
//		
//		 cout << " -- ab hier frisst das ganze >3GB Speicher --" << endl;
//		 cout << "Taste drücken zum Erstellen zweier CHs (statisch und dynamisch). " << endl;
//		 cin.get();
//		 CallSCGraph(g);
//	
//	/* DIJKSTRA TESTS
//	 *
//		for(unsigned int i=5010; i<5011; i++){
//			cout << "Wechsle auf den bidirektionalen Dijkstra." << endl;
//			start = clock();
//			cout << "Distanz: " << BiDijkstra(&g,0,i) << endl;
//			finish = clock();
//			cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
//	
//			cout << "Wechsle auf den Dijkstra, welcher direkt auf den Graphenstrukturen arbeitet." << endl;
//			start = clock();
//			cout << "Distanz: " << DirectDijkstra(&g,0,i) << endl;
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
//	
//			cout << "Wechsle auf den normalen Dijkstra." << endl;
//			start = clock();
//			cout << "Distanz: " << Dijkstra(&g,0,i) << endl;
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
//		}
//	*/
//	}



void testSCGraph(Graph* g){
	clock_t start,finish;
	double time;
	
	SCGraph scg = SCGraph(g);
	
	cout << scg.mergeRound(1) << endl;
	start = clock();
	cout << scg.mergeShortcutsAndGraph() << endl;
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit um Graph zu kopieren: " << time << endl;
	
	cout << scg.isShortcut(0) << " " << scg.isShortcut( scg.getEdgeCount() ) << endl;
	
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
	double time;

cout << "Erstelle Graph mit Datei " << file << endl;
Graph g = Graph();

// Geht noch nicht!
const char* rlfile = argv[1];
cout << "Ragel start" << endl;
start = clock();
ParserNode** rlnode = new ParserNode*;
ParserEdge** rledge = new ParserEdge*;

RlParser rl(rlfile);
rl.run(rlnode, rledge);
finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit zum parsen: " << time << endl;
cout << "Ragel end" << endl;

// Nur mal um meinen Speicher nicht überzustrapazieren.
delete[] *rlnode;
delete[] *rledge;
delete rlnode;
delete rledge;

start = clock();

g.setGraph(file, true);

finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit zum initialisieren des Graphen: " << time << endl;

cout << "Testen des Maximal Independent Set Algs!" << endl;
start = clock();
independent_set(&g);
finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit für das Maximal Independent Set + Test: " << time << endl;

// dijkstra_tests(&g);
testSCGraph(&g);

cout << "Taste drücken zum Beenden..."<< endl;
cin.get();

	return 0;
}

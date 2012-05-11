#include "graph.h"
#include "graphalgs.h"
#include "CHConstruction.h"
#include "parser.h"
#include <ctime>
#include "structs.h"
#include "ch.h"
#include "rlparser.h"
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
	

	for(int i=0; i<1; i++){
		start = clock();
		TDijkstra<SCGraph>(&scg, i);
		finish = clock();
		time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Zeit für Template-Dijkstra mit SCGraph von " << i << " aus: "<< time << endl;
	}

	CH hy(&g, &scg);
	//hy.calcCHverbose();
	hy.calcCH();

	/*
	{  // Graph ausgeben 

		for(unsigned int i = 0; i < g.getNodeCount(); i++){
			EdgesIterator ei = g.getOutEdgesIt(i);
			cout << "Node: " << i << endl;
			cout << "-- outgoing Edges:" << endl;
			while( ei.hasNext() ){
				Edge* e = ei.getNext();
				cout << "--- = ID:    " << e->id << endl;
				cout << "--- = Other: " << e->other_node << endl;
				cout << "--- = Value: " << e->value << endl;
				cout << "--- = --- " << endl;
				unsigned int other = e->other_node;
				EdgesIterator oei = g.getInEdgesIt(other);
				bool in_edge_zur_out_edge_gefunden = false;
				cout << "--- ~ Other Node " << other << " incoming Edges: " << endl;
				while(oei.hasNext()){
					Edge* oe = oei.getNext();
					if( oe->other_node == i){
						in_edge_zur_out_edge_gefunden = true;
					}
					cout << "--- --- = ID:    " << oe->id << endl;
					cout << "--- --- = Other: " << oe->other_node << endl;
					cout << "--- --- = Value: " << oe->value << endl;
					cout << "--- --- = --- " << endl;
				}
				if( !in_edge_zur_out_edge_gefunden ){
					cout << "--- --- IN_EDGE_TEST_FAILED " << endl;
				//	cout << "rückführende kante nicht gefunden für: " << endl;
				//	cout << "- von " << i << " zu " << other 
				//		<< " über edge " << e->id;
				//	cout << endl;
				}
			}
		}
	}
	*/

	{ // graph konsistenz testen 

		bool g_test_ok = true;
		for(unsigned int i = 0; i < g.getNodeCount(); i++){
			EdgesIterator ei = g.getOutEdgesIt(i);
			while( ei.hasNext() ){
				Edge* e = ei.getNext();
				unsigned int other = e->other_node;
				EdgesIterator oei = g.getInEdgesIt(other);
				bool in_edge_zur_out_edge_gefunden = false;
				while(oei.hasNext()){
					Edge* oe = oei.getNext();
					if( oe->other_node == i)
						in_edge_zur_out_edge_gefunden = true;
				}
				if( !in_edge_zur_out_edge_gefunden ){
					g_test_ok = false;
					cout << "rückführende kante nicht gefunden für: " << endl;
					cout << "- von " << i << " zu " << other 
						<< " über edge " << e->id;
					cout << endl;
				}
			}
		}
		cout << " >>>> G Test ok? : " << g_test_ok << endl;
		
		bool scg_test_ok = true;
		for(unsigned int i = 0; i < scg.getNodeCount(); i++){
			EdgesIterator ei = scg.getOutEdgesIt(i);
			while( ei.hasNext() ){
				Edge* e = ei.getNext();
				unsigned int other = e->other_node;
				EdgesIterator oei = scg.getInEdgesIt(other);
				bool in_edge_zur_out_edge_gefunden = false;
				while(oei.hasNext()){
					Edge* oe = oei.getNext();
					if( oe->other_node == i)
						in_edge_zur_out_edge_gefunden = true;
				}
				if( !in_edge_zur_out_edge_gefunden ){
					scg_test_ok = false;
					cout << "rückführende kante nicht gefunden für: " << endl;
					cout << "- von " << i << " zu " << other 
						<< " über edge " << e->id;
					if( scg.isShortcut(e->id) )
						cout << " (sc) " ;
					cout << endl;
				}
			}
		}
		cout << " >>>> SCG Test ok? : " << scg_test_ok << endl;
	}
	{ // graph konsistenz testen ~ rückwärts

		bool g_test_ok = true;
		for(unsigned int i = 0; i < g.getNodeCount(); i++){
			EdgesIterator ei = g.getInEdgesIt(i);
			while( ei.hasNext() ){
				Edge* e = ei.getNext();
				unsigned int other = e->other_node;
				EdgesIterator oei = g.getOutEdgesIt(other);
				bool in_edge_zur_out_edge_gefunden = false;
				while(oei.hasNext()){
					Edge* oe = oei.getNext();
					if( oe->other_node == i)
						in_edge_zur_out_edge_gefunden = true;
				}
				if( !in_edge_zur_out_edge_gefunden ){
					g_test_ok = false;
					cout << "rückführende kante nicht gefunden für: " << endl;
					cout << "- von " << i << " zu " << other 
						<< " über edge " << e->id;
					cout << endl;
				}
			}
		}
		cout << " >>>> G Rückwärts-Test ok? : " << g_test_ok << endl;
		
		bool scg_test_ok = true;
		for(unsigned int i = 0; i < scg.getNodeCount(); i++){
			EdgesIterator ei = scg.getInEdgesIt(i);
			while( ei.hasNext() ){
				Edge* e = ei.getNext();
				unsigned int other = e->other_node;
				EdgesIterator oei = scg.getOutEdgesIt(other);
				bool in_edge_zur_out_edge_gefunden = false;
				while(oei.hasNext()){
					Edge* oe = oei.getNext();
					if( oe->other_node == i)
						in_edge_zur_out_edge_gefunden = true;
				}
				if( !in_edge_zur_out_edge_gefunden ){
					scg_test_ok = false;
					cout << "rückführende kante nicht gefunden für: " << endl;
					cout << "- von " << i << " zu " << other 
						<< " über edge " << e->id;
					if( scg.isShortcut(e->id) )
						cout << " (sc) " ;
					cout << endl;
				}
			}
		}
		cout << " >>>> SCG Rückwärts-Test ok? : " << scg_test_ok << endl;
	}

	{ // edges pointer mit data-indizes abgleichen
		Edge* in_edges = g.copyInEdge(0);
		for(unsigned int i = 0; i < g.getNodeCount(); i++){
		//
		}
	}



	/*
	 * ein paar kanten einfärben, 
	 * indem man ihnen ein load von mindestens 1 gibt
	 * und diese dann in der vis anzeigen lassen
	 */
//	unsigned int* colors = 
//		new unsigned int[scg.getEdgeCount() + scg.getShortcutCount() ];
//	for(unsigned int i = 0; i < scg.getEdgeCount() 
//			+ scg.getShortcutCount(); i++){
//		if(0 == i%4 ){ 
//			colors[i] = 1;
//		} else {
//			colors[i] = 0;
//		}
//	}
//	scg.updateEdgeLoads(colors, scg.getEdgeCount() + scg.getShortcutCount());
//	// scg.shareShortcutLoads(); // shortcut loads verteilen
//
//	vis anzeige(&scg); anzeige.start();


	CHDijkstraTest(&g, &scg, 15000);

	return 0;
}

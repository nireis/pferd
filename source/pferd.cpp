#include "graph.h"
#include "graphalgs.h"
#include "parser.h"
#include <ctime>

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

Graph g = Graph();

start = clock();

g.setGraph(file);

finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit zum initialisieren des Graphen: " << time << endl;


unsigned int node = 0;
for(unsigned int i = 0; i <= node; i++){
	
	NodeData nd = g.getNodeData(i);
	cout.precision(15);

	cout << "node: " << i << endl;
	cout << "ID: " << nd.id << endl;
	cout << "Lat: " << nd.lat << endl;
	cout << "Lon: " << nd.lon << endl;
	cout << "El: " << nd.elevation << endl << endl;
	
	Graph::OutEdgesIterator it = g.getOutEdgesIt(i);
	while(it.hasNext()){
		Edge* e = it.getNext();
		cout << "other_node: " << e->other_node << endl;
				Graph::InEdgesIterator ieit = g.getInEdgesIt(e->other_node);
		cout << "value: " << e->value << endl;
		cout << "type: " << g.getEdgeData(e->id).type << endl;
		cout << "id: " << e->id << endl << endl;
				cout << "   alle einkommenden Kanten von " << e->other_node << endl;
				while(ieit.hasNext()){
					Edge* ee = ieit.getNext();
					cout << "   v: " << ee->value << endl;
					cout << "   other: " << ee->other_node << endl;
					cout << "   id: " << ee->id << endl;
				}
	}
	cout << " === === " << endl;
}


cout << "Dijkstra angefangen." << endl;
start = clock();

unsigned int k = 0;

Dijkstra(&g, k);

finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit für normalen Dijkstra von 0 aus: "<< time << endl;

start = clock();

DirectDijkstra(&g, k);

finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit für Dijkstra direkt auf dem Graphen: "<< time << endl;

	for(unsigned int i=5010; i<5011; i++){
		cout << "Wechsle auf den bidirektionalen Dijkstra." << endl;
		start = clock();
		cout << "Distanz: " << BiDijkstra(&g,0,i) << endl;
		finish = clock();
		cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;

		cout << "Wechsle auf den Dijkstra, welcher direkt auf den Graphenstrukturen arbeitet." << endl;
		start = clock();
		cout << "Distanz: " << DirectDijkstra(&g,0,i) << endl;
		finish = clock();
		time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;

		cout << "Wechsle auf den normalen Dijkstra." << endl;
		start = clock();
		cout << "Distanz: " << Dijkstra(&g,0,i) << endl;
		finish = clock();
		time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
	}



cout << "Taste drücken zum Erstellen von Shortcuts. " << endl;
cin.get();
start = clock();
	unsigned int scc = g.getEdgeCount()*2;
		for(unsigned int i = 0; i < scc; i++){
		Shortcut s;
		s.value = ((int)( i*i*3.141592)) % 100;
		s.source = i%g.getNodeCount();
		s.target = i*(i+1)%(g.getNodeCount());
		s.id = g.getEdgeCount() + i;
      s.papa_edge = i*i % g.getEdgeCount();
      s.mama_edge = i*(i+1) % g.getEdgeCount();
		g.addShortcut(s);
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Shortcuts angelegt. Nun initialisieren. Benötigte Zeit:" << time << endl;
	cout << "Taste drücken zum Fortfahren! " << endl;
	cin.get();

	start = clock();
	g.setShortcutOffsets();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit, 2m Shortcuts zu initialisieren: " << time << endl;
	cout << "Taste drücken zum Fortfahren. " << endl;
	cin.get(); 

	g.clearShortcutlist();


cout << "Taste drücken zum Beenden..."<< endl;
cin.get();

	return 0;
}

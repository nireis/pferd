#include "graph.h"
#include "graphalgs.h"
#include "parser.h"
#include <ctime>
#include "structs.h"
#include "ch.h"

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
Graph2 g = Graph2();

start = clock();

g.setGraph(file, true);
//g.computeIterators();

finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit zum initialisieren des Graphen: " << time << endl;

cout << "Dijkstra angefangen." << endl; /*
for(int i=0; i<10; i++){
	start = clock();
	DirectDijkstra(&g, i);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit für Dijkstra direkt auf dem Graphen von " << i << " aus: "<< time << endl;
}*/
for(int i=0; i<10; i++){
	start = clock();
	Dijkstra2(&g, i);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	//cout << "Zeit für normalen Dijkstra von " << i << " aus: "<< time << endl;
	cout << "Zeit für normalen Dijkstra mit Pointern auf Kanten und lokalen U_elements von " << i << " aus: "<< time << endl;
}
for(int i=0; i<10; i++){
	start = clock();
	Dijkstra2A(&g, i);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	//cout << "Zeit für normalen Dijkstra von " << i << " aus: "<< time << endl;
	cout << "Zeit für 'optimierten' Dijkstra von " << i << " aus: "<< time << endl;
}

//Graph2 g2 = Graph2();
//start = clock();
//
//g2.setGraph(file);
//
//finish = clock();
//time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//cout << "Zeit zum initialisieren des Graphen2: " << time << endl;
//
//cout << "Dijkstra angefangen." << endl;
//for(int i=0; i<10; i++){
//	start = clock();
//	DirectDijkstra(&g, i);
//	finish = clock();
//	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//	cout << "Zeit für Dijkstra direkt auf dem Graphen von " << i << " aus: "<< time << endl;
//}
//for(int i=0; i<10; i++){
//	start = clock();
//	Dijkstra2(&g2, i);
//	finish = clock();
//	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//	cout << "Zeit für normalen Dijkstra2 von " << i << " aus: "<< time << endl;
//}
//
//cout << "Vergleiche Graph und Graph2 " << endl;
//if(g.getNodeCount() != g2.getNodeCount())
//	cout << "NodeCounts undlgeich, g: " << g.getNodeCount() <<", g2: " <<  g2.getNodeCount() << endl;
//if(g.getEdgeCount() != g2.getEdgeCount())
//	cout << "EdgeCounts undlgeich, g: " << g.getEdgeCount() <<", g2: " <<  g2.getEdgeCount() << endl;
//
//	cout << "=== OutEdges Differenzen zwischen g und g2 ===" << endl;
//for(unsigned int n = 0; n < g.getNodeCount(); n++){
//	Graph::EdgesIterator it = g.getOutEdgesIt(n);
//	Graph2::EdgesIterator it2 = g2.getOutEdgesIt(n);
//	while( it.hasNext() && it2.hasNext() ){
//		Edge e = it.getNext();
//		Edge e2 = it2.getNext();
//		if( e.id != e2.id)
//			cout << "IDs ungleich: g: " << e.id << ", g2: " << e2.id << endl;
//		if( e.value != e2.value)
//			cout << "VALUEs ungleich: g: " << e.value << ", g2: " << e2.value << endl;
//		if( e.other_node != e2.other_node)
//			cout << "OTHER_NODEs ungleich: g: " << e.other_node << ", g2: " << e2.other_node << endl;
//	}
//	if( it.hasNext() || it2.hasNext() )
//		cout << "Knoten " << n << "hat unterschiedlich viele OutEdges: g: " << it.hasNext() << ", g2: " << it2.hasNext() << endl;
//}
//	cout << "=== InEdges Differenzen zwischen g und g2 ===" << endl;
//for(unsigned int n = 0; n < g.getNodeCount(); n++){
//	Graph::EdgesIterator it = g.getInEdgesIt(n);
//	Graph2::EdgesIterator it2 = g2.getInEdgesIt(n);
//	while( it.hasNext() && it2.hasNext() ){
//		Edge e = it.getNext();
//		Edge e2 = it2.getNext();
//		if( e.id != e2.id)
//			cout << "IDs ungleich: g: " << e.id << ", g2: " << e2.id << endl;
//		if( e.value != e2.value)
//			cout << "VALUEs ungleich: g: " << e.value << ", g2: " << e2.value << endl;
//		if( e.other_node != e2.other_node)
//			cout << "OTHER_NODEs ungleich: g: " << e.other_node << ", g2: " << e2.other_node << endl;
//	}
//	if( it.hasNext() || it2.hasNext() )
//		cout << "Knoten " << n << "hat unterschiedlich viele InEdges: g: " << it.hasNext() << ", g2: " << it2.hasNext() << endl;
//}

/*

cout << "Taste drücken zum Erstellen zweier CHs (statisch und dynamisch). " << endl;
cin.get();

CH h = CH(&g);
h.setShortcuts();
DynCH dch = DynCH(&g);


for(int i=0; i<10; i++){
	start = clock();
	Dijkstra(&g, i);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit für normalen Dijkstra von " << i << " aus: "<< time << endl;
}
for(int i=0; i<10; i++){
	start = clock();
	Dijkstra_4I(&g, i);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit für Interface-Dijkstra mit Graph von " << i << " aus: "<< time << endl;
}
for(int i=0; i<10; i++){
	start = clock();
	Dijkstra_4I(&h, i);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit für Interface-Dijkstra mit statischer CH von " << i << " aus: "<< time << endl;
}
for(int i=0; i<10; i++){
	start = clock();
	Dijkstra_4I(&dch, i);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit für Interface-Dijkstra mit dynamischer CH von " << i << " aus: "<< time << endl;
}

cout << "Taste drücken zum Erstellen von m Shortcuts. " << endl;
cin.get();

start = clock();
	unsigned int scc = g.getEdgeCount();//*2;
		for(unsigned int i = 0; i < scc; i++){
		Shortcut s;
		s.value = ((int)( i*i*3.141592)) % 100;
		s.source = i%g.getNodeCount();
		s.target = i*(i+1)%(g.getNodeCount());
      s.papa_edge = i*i % g.getEdgeCount();
      s.mama_edge = i*(i+1) % g.getEdgeCount();
		h.addShortcut(s);
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Shortcuts bereitgelegt. Benötigte Zeit:" << time << endl;
	cout << "Nun initialisieren der Shortcuts.Taste drücken. " << endl;
	cin.get();

	start = clock();
	h.setShortcuts();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit, m Shortcuts und m vorhandene Kanten zu initialisieren: " << time << endl;
	cout << "Nun löschen der Shortcuts. Taste drücken. " << endl;
	cin.get(); 

	h.clearShortcutlist();
	h.clearShortcuts();

	start = clock();
	unsigned int is = h.calcIndepSet();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;

	cout << "Habe Independent Set der Größe > " << is << " < aufgebaut in: " << time << endl;

cout << "Es folgt weiteres Testzeug. Taste drücken." << endl;
cin.get();

//long double dd = 0;
unsigned int node = 0;//g.getNodeCount() - 1;
//for(unsigned int i = 0; i <= node; i++){
//	NodeData nd = g.getNodeData(i);
//	dd+= nd.id ;
//	dd+= nd.elevation ;
//	dd+= nd.lon ;
//	dd+= nd.lat ;
//	EdgesIterator it = g.getOutEdgesIt(i);
//	while (it.hasNext()){
//		Edge e = * it.getNext();
//		dd += e.value ;
//		dd += e.other_node ;
//		dd += e.id ;
//		EdgeData ed = g.getEdgeData(e.id);
//		dd += ed.out_index ;
//		dd += ed.in_index ;
//		dd += ed.distance ;
//		dd += ed.type ;
//		dd += ed.load ;
//	}
//	it = g.getInEdgesIt(i);
//	while (it.hasNext()){
//		Edge e = * it.getNext();
//		dd += e.value ;
//		dd += e.other_node ;
//		dd += e.id ;
//		EdgeData ed = g.getEdgeData(e.id);
//		dd += ed.out_index ;
//		dd += ed.in_index ;
//		dd += ed.distance ;
//		dd += ed.type ;
//		dd += ed.load ;
//	}
//}
Graph::EdgesIterator it;
Graph::EdgesIterator ieit;
for(unsigned int i = 0; i <= node; i++){
	
	NodeData nd = g.getNodeData(i);
	cout.precision(15);

	cout << "node: " << i << endl;
	cout << "ID: " << nd.id << endl;
	cout << "Lat: " << nd.lat << endl;
	cout << "Lon: " << nd.lon << endl;
	cout << "El: " << nd.elevation << endl << endl;
	
	it = g.getOutEdgesIt(i);
	while(it.hasNext()){
		Edge e = it.getNext();
		cout << "other_node: " << e.other_node << endl;
		
		ieit = g.getInEdgesIt(e.other_node);
		
		cout << "value: " << e.value << endl;
		cout << "type: " << g.getEdgeData(e.id).type << endl;
		cout << "id: " << e.id << endl << endl;

		EdgeData ed;
		ed = g.getEdgeData(e.id);

				while(ieit.hasNext()){
					Edge ee = ieit.getNext();
					cout << "   v: " << ee.value << endl;
					cout << "   other: " << ee.other_node << endl;
					cout << "   id: " << ee.id << endl;
				}
	}
	cout << " === === " << endl;
}

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
*/
cout << "Taste drücken zum Beenden..."<< endl;
cin.get();

	return 0;
}

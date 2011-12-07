#include "graph.h"
#include "graphalgs.h"
#include "parser.h"
#include "simple_parser.h"
#include <ctime>

int main(int argc, char *argv[]){
	
	
	// Test des Graphen: instanziieren und alles mal ausprobieren 

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

	if(argc == 1 || argc == 2 || argc > 3){
		cout << "---" << endl 
				<< "-- Aufruf der Binärdatei wie folgt: " << argv[0] << " Modus"<< " Graphendatei " << endl
				<< "-- Modus: 1==Simple_Structs ; sonstige Zahl==Structs" << endl 
				<< "-- Graphdatei: Pfad zu einer Datei, die als Graphdatei gelesen werden kann." << endl
				<< "---" << endl;
		return 0;
	} 

	string file = argv[2];
	int i = atoi(argv[1]);

	ifstream checkfile(file.c_str());
	if(!checkfile){
		cout << "-> angegebene Datei existiert nicht." << endl;
		return 0;
	}

	clock_t start,finish;
	double time;

	// file = "../data/15K.txt";

if(i == 1){

	cout << "Hallo! willkommen im Testlauf!" << endl 
		<< "Starte parsen der Datei '" << file << "'" << endl;
	start = clock();
		simple_parser p = simple_parser();
		p.readFile(file);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Parsen fertig. Benötigte Sekunden: "<< time << endl 
			<< "Erstelle und initialisiere Graph." << endl;
	start = clock();
		cout <<"Graph(NodeCount, EdgeCount, Nodes, Edges)" << p.getNodeCount() <<" " << p.getEdgeCount() <<" " << p.getNodes() <<" " << p.getEdges() << endl;
		Graph<Simple_Edge, Simple_Node, Shortcut> g = 
			Graph<Simple_Edge, Simple_Node, Shortcut>(p.getNodeCount(), p.getEdgeCount(), 
					p.getNodes(), p.getEdges());
	
//	cout << "Taste drücken, damit   GRAPH   initialisiert wird. "; 
//	cin.get(); 

		g.initOffsets();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Graph initialisiert! Benötigte Sekunden: "<<time << endl
			<< "Hier die Informationen zur den Kanten des Knotens 0:" << endl;
	start = clock();
	Graph<Simple_Edge, Simple_Node, Shortcut>::OutEdgesIterator it = g.getOutEdgesIt(0);
	Graph<Simple_Edge, Simple_Node, Shortcut>::InEdgesIterator iit = g.getInEdgesIt(0);
	finish = clock();
		cout << "-CPU-Clocks, um alle Kanten eines Knotens zu bestimmen: " << (finish-start) << endl;
	
	cout << "Nun mit einem OutEdgesIterator "<< endl 
		<< "   die AUSgehenden Kanten von Node 0:" << endl;
	start = clock();
	while(it.hasNext()){
		//Simple_Edge &e(it.getNext());
		Simple_Edge *e = it.getNext();
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e->source << endl;
		cout <<" target: " << e->target<< endl;
		cout <<" distance: " << e->distance << endl;
		
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Betnötigte Zeit zum iterieren über die Kanten: " << time << endl;
		
	cout << "Nun mit einem OutEdgesIterator "<< endl 
		<< "   die EINgehenden Kanten von Node 0:" << endl;
	start = clock();
	while(iit.hasNext()){
		//Simple_Edge &e(it.getNext());
		Simple_Edge *e = iit.getNext();
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e->source << endl;
		cout <<" target: " << e->target<< endl;
		cout <<" distance: " << e->distance << endl;
		
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Betnötigte Zeit zum iterieren über die Kanten: " << time << endl;
	
	p.~simple_parser();
//	cout << "Taste drücken, damit   GRAPH-Objekt   zerstört wird. "; 
//	cin.get(); 
		
	g.~Graph<Simple_Edge, Simple_Node, Shortcut>();
	
} else {

	cout << "Hallo! willkommen im Testlauf!" << endl 
		<< "Starte parsen der Datei '" << file << "'" << endl;
start = clock();
	parser p = parser();
	p.readFile(file);
finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Parsen fertig. Gebrauchte Sekunden: "<< time << endl 
		<< "Erstelle und initialisiere Graph." << endl;
start = clock();
	Graph<Edge, Node, Shortcut> g = 
		Graph<Edge, Node, Shortcut>(p.getNodeCount(), p.getEdgeCount(), 
				p.getNodes(), p.getEdges());

//	cout << "Taste drücken, damit   GRAPH   initialisiert wird. "; 
//	cin.get(); 

		g.initOffsets();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Graph initialisiert! Benötigte Sekunden: "<<time << endl
			<< "Hier die Informationen zur den Kanten des Knotens 0:" << endl;
	start = clock();
	Graph<Edge, Node, Shortcut>::OutEdgesIterator it = g.getOutEdgesIt(0);
	Graph<Edge, Node, Shortcut>::InEdgesIterator iit = g.getInEdgesIt(0);
	finish = clock();
		cout << "-CPU-Clocks, um alle Kanten eines Knotens zu bestimmen: " << (finish-start) << endl;
	
	cout << "Nun mit einem OutEdgesIterator "<< endl 
		<< "   die AUSgehenden Kanten von Node 0:" << endl;
	start = clock();
	while(it.hasNext()){
		//Simple_Edge &e(it.getNext());
		Edge *e = it.getNext();
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e->source << endl;
		cout <<" target: " << e->target<< endl;
		cout <<" distance: " << e->distance << endl;
		cout << " type : " << e->type << endl;
		cout << " id : "<< e->id << endl;
		
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Betnötigte Zeit zum iterieren über die Kanten: " << time << endl;
		
	cout << "Nun mit einem OutEdgesIterator "<< endl 
		<< "   die EINgehenden Kanten von Node 0:" << endl;
	start = clock();
	while(iit.hasNext()){
		//Simple_Edge &e(it.getNext());
		Edge *e = iit.getNext();
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e->source << endl;
		cout <<" target: " << e->target<< endl;
		cout <<" distance: " << e->distance << endl;
		cout << " type : " << e->type << endl;
		cout << " id : "<< e->id << endl;
		
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Betnötigte Zeit zum iterieren über die Kanten: " << time << endl;

//	cout << "Taste drücken, damit   DIJKSTRA   beginnt. "; 
//	cin.get(); 

	/*
	 * Test der Graphalgs Klasse!
	 */
	cout << "Dijkstra beginnt." << endl;
	start = clock();
	Graphalgs ga = Graphalgs(&g);
	ga.Dijkstra(0);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Dijkstra beendet in sek " << time << endl;

	p.~parser();

//	cout << "Taste drücken, damit   GRAPH-Objekt   zerstört wird. "; 
//	cin.get(); 
		
	g.~Graph<Edge, Node, Shortcut>();
}

	cout << "Taste drücken zum beenden ... "; 
	cin.get(); 

	return 0;
}

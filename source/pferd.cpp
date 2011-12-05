#include "graph.h"
#include "graphalgs.h"
#include "parser.h"
#include "simple_parser.h"
#include <ctime>

int main(int argc, char *argv[]){
	
	
	// Test des Graphen: instanziieren und alles mal ausprobieren 
// 	Graph<Edge, Node, Shortcut> g = Graph<Edge, Node, Shortcut>();
// 	g.initOffsets();
// 	g.clearShortcuts();
// 	g.initShortcutOffsets();
// 	Shortcut s;
// 	g.addShortcut(s);
// 	Graph<Edge, Node, Shortcut>::OutEdgesOfNode oe = g.getAdjOutEdges(0);
// 	Graph<Edge, Node, Shortcut>::InEdgesOfNode ie = g.getAdjInEdges(0);
	 ///*
// 	unsigned int a = 1;
// 	Graph<Edge, Node, Shortcut>::OutEdgesOfNode oe = Graph<Edge, Node, Shortcut>::OutEdgesOfNode(a, &g);
// 	Graph<Edge, Node, Shortcut>::InEdgesOfNode ie = Graph<Edge, Node, Shortcut>::InEdgesOfNode(a, &g);
// 	Edge e;
// 	oe.getEdge(a-1, e);
// 	oe.getEdge(a, e);
// 	 //*/

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
	
		g.initOffsets();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Graph initialisiert! Benötigte Sekunden: "<<time << endl
			<< "Hier die Informationen zur ersten Kante des Knotens 0:" << endl;
	start = clock();
	 	Graph<Simple_Edge, Simple_Node, Shortcut>::OutEdgesOfNode oe = g.getAdjOutEdges(0);
	 	Graph<Simple_Edge, Simple_Node, Shortcut>::InEdgesOfNode ie = g.getAdjInEdges(0);
	finish = clock();
		cout << "-CPU-Clocks, um ausgehende und eingehende Kanten "<< endl 
			<<"   eines Knotens zu bestimmen: " << (finish-start) << endl;
		Edge e;
	start = clock();
		oe.getEdge(1, e);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "-CPU-Clocks, eine Kante zu bekommen: " << finish-start<< endl;
		cout <<"-oe: Node ID (0) : " << oe.node_id << endl;
		cout <<"-oe: EdgeCount : " << oe.edge_count << endl;
		cout <<"-oe: getEdge(1) (S, T, DI) : " << endl;
		cout <<"         source: " << e.source << endl;
		cout <<"            target: " << e.target<< endl;
		cout <<"             distance: " << e.distance << endl;
	
	cout << "Nun mit einem OutEdgesIterator "<< endl 
		<< "   die ausgehenden Kanten von Node 0:" << endl;
	Graph<Simple_Edge, Simple_Node, Shortcut>::OutEdgesIterator it = g.getOutEdges(0);
	while(it.hasNext()){
		Simple_Edge &e(it.getNext());
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e.source << endl;
		cout <<" target: " << e.target<< endl;
		cout <<" distance: " << e.distance << endl;
		
	}

		
	p.~simple_parser();
	cout << "Taste drücken, damit   GRAPH-Objekt   zerstört wird. "; 
	cin.get(); 
		
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

	g.initOffsets();
finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Graph initialisiert! Gebrauchte Sekunden: "<<time << endl
		<< "Hier die Informationen zur ersten Kante des Knotens 0:" << endl;
start = clock();
 	Graph<Edge, Node, Shortcut>::OutEdgesOfNode oe = g.getAdjOutEdges(0);
 	Graph<Edge, Node, Shortcut>::InEdgesOfNode ie = g.getAdjInEdges(0);
finish = clock();
		cout << "-CPU-Clocks, um ausgehende und eingehende Kanten "<< endl 
			<<"   eines Knotens zu bestimmen: " << (finish-start) << endl;
	Edge e;
start = clock();
	oe.getEdge(1, e);
finish = clock();
	cout << "-CPU-Clocks, eine Kante zu bekommen: " << finish-start<< endl;
	cout <<"-oe: Node ID (0) : " << oe.node_id << endl;
	cout <<"-oe: EdgeCount : " << oe.edge_count << endl;
	cout <<"-oe: getEdge(1) (S, T, DI, TY), ID : " << e.id << endl;
	cout <<"         source: " << e.source << endl;
	cout <<"            target: " << e.target<< endl;
	cout <<"             distance: " << e.distance << endl;
	cout <<"                     type: " << e.type << endl;
	
	cout << "Nun mit einem OutEdgesIterator "<< endl 
		<< "   die ausgehenden Kanten von Node 0:" << endl;
	Graph<Edge, Node, Shortcut>::OutEdgesIterator it = g.getOutEdges(0);
	while(it.hasNext()){
		Edge &e(it.getNext());
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e.source << endl;
		cout <<" target: " << e.target<< endl;
		cout <<" distance: " << e.distance << endl;
		cout <<" type: " << e.type << endl;
		cout <<" edge-id : " << e.id << endl;
		
	}
	
	
	p.~parser();
	cout << "Taste drücken, damit   GRAPH-Objekt   zerstört wird. "; 
	cin.get(); 
		
	g.~Graph<Edge, Node, Shortcut>();
}
	
//	parser testParser;
//	Node* testArray1;
//	Edge* testArray2;
//	
//	testParser.readFile("../data/15K.txt");
//	cout << testParser.getNodeCount() << endl;
//	cout << testParser.getEdgeCount() << endl;
//	
//	testArray1 = testParser.getNodes();
//	cout << testArray1[1].lat << endl;
//	cout << testArray1[1].lon << endl;
//	cout << testArray1[1].elevation << endl;
//	
//	testArray2 = testParser.getEdges();
//	cout << testArray2[0].source << endl;
//	cout << testArray2[0].target << endl;
//	cout << testArray2[0].id << endl;
//	cout << testArray2[0].distance << endl;
//	cout << testArray2[0].type << endl;
//	
//	delete[] testArray1;
//	delete[] testArray2;


	/*
	 * Test der Graphalgs Klasse!
	 */
//	Graphalgs ga = Graphalgs(&g);
//	ga.Dijkstra(0);

	cout << "Taste drücken zum beenden ... "; 
	cin.get(); 

	return 0;
}

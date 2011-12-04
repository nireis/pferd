#include "graph.h"
#include "graphalgs.h"
#include "parser.h"

int main(){
	
	
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

	string file = "../data/15K.txt";
	cout << "Hallo! willkommen im Testlauf!" << endl 
		<< "Starte parsen der Dateii '" << file << "'" << endl;
	parser p = parser();
	p.readFile(file);
	cout << "Parsen fertig." << endl 
		<< "Erstelle und initialisiere Graph." << endl;
	Graph<Edge, Node, Shortcut> g = 
		Graph<Edge, Node, Shortcut>(p.getNodeCount(), p.getEdgeCount(), 
				p.getNodes(), p.getEdges());

	g.initOffsets();
	cout << "Graph initialisiert!" << endl
		<< "Hier die Informationen zur ersten Kante des Knotens 0:" << endl;
 	Graph<Edge, Node, Shortcut>::OutEdgesOfNode oe = g.getAdjOutEdges(0);
 	Graph<Edge, Node, Shortcut>::InEdgesOfNode ie = g.getAdjInEdges(0);
	Edge e;
	oe.getEdge(1, e);
	cout <<"oe: Node ID (0) : " << oe.node_id << endl;
	cout <<"oe: EdgeCount (3 ausgehende) : " << oe.edge_count << endl;
	cout <<"oe: getEdge(1) (0, 1, 22, 9), ID : " << e.id << endl;
	cout <<"        source: " << e.source << endl;
	cout <<"           target: " << e.target<< endl;
	cout <<"            distance: " << e.distance << endl;
	cout <<"                    type: " << e.type << endl;


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


	return 0;
}

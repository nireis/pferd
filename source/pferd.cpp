#include "graph.h"
#include "graphalgs.h"
#include "parser.h"

int main(){
	
	
	// Test des Graphen: instanziieren und alles mal ausprobieren 
	Graph<Edge, Node, Shortcut> g = Graph<Edge, Node, Shortcut>();
	g.initOffsets();
	g.clearShortcuts();
	g.initShortcutOffsets();
	Shortcut s;
	g.addShortcut(s);
	g.getAdjOutEdges(0);
	g.getAdjInEdges(0);
	 ///*
	unsigned int a = 1;
	Graph<Edge, Node, Shortcut>::OutEdgesOfNode oe = Graph<Edge, Node, Shortcut>::OutEdgesOfNode(a, &g);
	Graph<Edge, Node, Shortcut>::InEdgesOfNode ie = Graph<Edge, Node, Shortcut>::InEdgesOfNode(a, &g);
	Edge e;
	oe.getEdge(a-1, e);
	oe.getEdge(a, e);
	 //*/
	
	/*
	 * Test der Graphalgs Klasse!
	 */
	Graphalgs ga = Graphalgs(&g);
	ga.Dijkstra(0);

	parser p = parser();
	p.readFile("../data/15K.txt");

	return 0;
}

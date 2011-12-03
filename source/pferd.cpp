#include "graph.h"
#include "graphalgs.h"

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
	 /*
	Graph<Edge, Node, Shortcut>::OutEdgesOfNode oe = Graph<Edge, Node, Shortcut>::OutEdgesOfNode();
	Graph<Edge, Node, Shortcut>::InEdgesOfNode ie = Graph<Edge, Node, Shortcut>::InEdgesOfNode();
	 */
	
	/*
	 * Test der Graphalgs Klasse!
	 */
	Graphalgs ga = Graphalgs(&g);
	ga.Dijkstra(0);

	return 0;
}

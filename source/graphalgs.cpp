#include "graphalgs.h"

Graphalgs::Graphalgs(Graph<Edge, Node, Shortcut>* g){
	this->g = g;
}

void Graphalgs::Dijkstra(int node_id){
	// Hier liegen am Ende die kürzesten Distanzen
	int dist[getNodeCount()];
	// Hier sehen wir ob ein Knoten schon seinen kürzesten Pfad gefunden hat
	bool found[getNodeCount()];
	// Hier können wir den Pfad zu den Knoten rekonstruieren
	int in_edge_nr[getNodeCount()];
	// Tmp für die ausgehenden Kanten (schonmal auf den Startknoten gesetzt)
	OutEdgesOfNode<Edge, Node, Shortcut> tmpOutEdges = OutEdgesOfNode<Edge,Node,Shortcut>(node_id);
	Edge* tmpEdge;
	// Init, damit alles passt (Vorsichtsmaßnahme)
	for (int i=0; i<getNodeCound(); i++){
		found[i] = false;
	}
	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found[node_id] = true;
	for(int i=0; i<tmpOutEdges.edge_count; i++){
		tmpOutEdges(i,tmpEdge);
		dist[tmpEdge.target] = tmpEdge.distance;
		found[tmpEdge.target] = true;
	}
}

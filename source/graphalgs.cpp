#include "graphalgs.h"

Graphalgs::Graphalgs(Graph<Edge, Node, Shortcut>* g){
	this->g = g;
}

void Graphalgs::Dijkstra(int node_id){
// Hier liegen am Ende die kürzesten Distanzen

/* gibt fehler */ //unsigned int dist[getNodeCount()];

// Hier sehen wir ob ein Knoten schon seinen kürzesten Pfad gefunden hat

/* gibt fehler */ //bool found[getNodeCount()];

// Hier können wir den Pfad zu den Knoten rekonstruieren

/* gibt fehler */ //unsigned int in_edge_nr[getNodeCount()];

// == das hier wird hoffentlich nicht mehr gebraucht
// == statt dessen 
// == Graph<Edge, Node, Shortcut>::OutEdgesIterator benutzen, siehe main
// 
// Tmp für die ausgehenden Kanten (schonmal auf den Startknoten gesetzt)
//OutEdgesOfNode<Edge, Node, Shortcut> tmpOutEdges = OutEdgesOfNode<Edge, Node, Shortcut>(node_id);

/* gibt warnung */ //Edge* tmpEdge;

// Init, damit alles passt (Vorsichtsmaßnahme)

/* gibt fehler */ //for (int i=0; i<getNodeCound(); i++){
/* gibt fehler */ //	found[i] = false;
/* gibt fehler */ //}

// Den ersten Knoten abarbeiten

/* gibt fehler */ //dist[node_id] = 0;
/* gibt fehler */ //found[node_id] = true;
/* gibt fehler */ //for(int i=0; i<tmpOutEdges.edge_count; i++){
/* gibt fehler */ //	tmpOutEdges(i,tmpEdge);
/* gibt fehler */ //	dist[tmpEdge->target] = tmpEdge->distance;
/* gibt fehler */ //	found[tmpEdge->target] = true;
/* gibt fehler */ //}
}

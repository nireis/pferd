#include "graphalgs.h"
#include <queue>
using namespace std;

Graphalgs::Graphalgs(Graph<Edge, Node, Shortcut>* g){
	this->g = g;
}

void Graphalgs::Dijkstra(int node_id){
	// Hier liegen am Ende die kürzesten Distanzen
	unsigned int* dist;
	// Hier sehen wir ob ein Knoten schon seinen kürzesten Pfad gefunden hat
	bool* found;
	// Hier können wir den Pfad zu den Knoten rekonstruieren
	// unsigned int* in_edge_nr;
	// Iterator um die ausgehenden Kanten durchzugehen (erster Wert node_id)
	Graph<Edge, Node, Shortcut>::OutEdgesIterator it = g->getOutEdgesIt(node_id);
	// Pointer um die akutelle Kante zu behandeln
	Edge* nextEdge;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	// Die ganzen Arrays initialisieren
	dist = new unsigned int[g->getNodeCount()];
	found = new bool[g->getNodeCount()];
	// in_edge_nr = new unsigned int[g->getNodeCount()];

	// Init, damit alles passt (Vorsichtsmaßnahme)
	for (unsigned int i = 0; i < g->getNodeCount(); i++){
		found[i] = false;
	}

	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found[node_id] = true;
	while(it.hasNext()){
		nextEdge = it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_element(nextEdge->distance,nextEdge->target,node_id)); //...
	}

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	while(!U.empty()){
		tmpid = U.top().id;
		// TEST
		// cout << "Knoten wird jetzt bearbeitet: " << tmpid << endl;
		// cout << "Die Kante kam von: " << U.top().sid << endl; //...
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		if(!found[tmpid]){
			// TEST
			// cout << tmpid << ": " << U.top().distance << endl;
			dist[tmpid] = U.top().distance;
			found[tmpid] = true;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				nextEdge = it.getNext();
				// cout << "Die Kante führt zu: " << nextEdge->target << endl; // ...
				// Wenn sie noch nicht gefunden wurde...
				if(!found[nextEdge->id]){
					// ...tu sie in U
					U.push(U_element(
								nextEdge->distance+dist[tmpid],nextEdge->target,tmpid)); //...
				}
			}
		}
		U.pop();
	}

	// TEST: Ergebnisse ausgeben
	for(unsigned int i=0; i < g->getNodeCount(); i++){
		// cout << dist[i] << endl;
	}
	// Alles frei geben, was wir nichtmehr brauchen
	delete[] dist;
	delete[] found;
	// delete[] in_edge_nr;
}

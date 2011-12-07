#include "graphalgs.h"
#include <queue>
using namespace std;

Graphalgs::Graphalgs(Graph<Edge, Node, Shortcut>* g){
	this->g = g;
}

void Graphalgs::Dijkstra(unsigned int node_id){
	// Hier liegen am Ende die kürzesten Distanzen
	unsigned int* dist;
	// Hier sehen wir ob ein Knoten schon seinen kürzesten Pfad gefunden hat
	bool* found;
	// Hier können wir den Pfad zu den Knoten rekonstruieren
	unsigned int* in_edge_id;
	// Iterator um die ausgehenden Kanten durchzugehen (erster Wert node_id)
	Graph<Edge, Node, Shortcut>::OutEdgesIterator it = g->getOutEdgesIt(node_id);
	// Pointer um die akutelle Kante zu behandeln
	Edge* nextEdge;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	// Die ganzen Arrays initialisieren
	dist = new unsigned int[g->getNodeCount()];
	found = new bool[g->getNodeCount()];
	in_edge_id = new unsigned int[g->getNodeCount()];

	// Init, damit alles passt (Vorsichtsmaßnahme)
	/*for (unsigned int i = 0; i < g->getNodeCount(); i++){
		found[i] = false;
	}*/

	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found[node_id] = true;
	while(it.hasNext()){
		nextEdge = it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_element(nextEdge->distance,nextEdge->target,nextEdge->id));
	}

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		if(!found[U.top().id]){
			tmpid = U.top().id;
			dist[tmpid] = U.top().distance;
			found[tmpid] = true;
			in_edge_id[tmpid] = U.top().eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				nextEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found[nextEdge->id]){
					// ...tu sie in U
					U.push(U_element(
								nextEdge->distance+dist[tmpid],nextEdge->target,nextEdge->id));
				}
			}
		}
		U.pop();
	}

	// TEST: Ergebnisse ausgeben
	/*for(unsigned int i=0; i < g->getNodeCount(); i++){
		cout << dist[i] << endl;
	}*/
	// Alles frei geben, was wir nichtmehr brauchen
	delete[] dist;
	delete[] found;
	delete[] in_edge_id;
}

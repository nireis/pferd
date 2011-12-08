#include "graphalgs.h"
#include <queue>
#include <ctime>
#include <limits>
using namespace std;

Graphalgs::Graphalgs(Graph<Edge, Node, Shortcut>* g){
	this->g = g;
}

void Graphalgs::Dijkstra(unsigned int node_id){
	// Iterator für die ausgehenden Kanten eines Knotens
	Graph<Edge, Node, Shortcut>::OutEdgesIterator it = g->getOutEdgesIt(node_id);
	// Pointer um die akutelle Kante zu behandeln
	Edge* nextEdge;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

	unsigned int nr_of_nodes = g->getNodeCount();
	// Die ganzen Vektoren initialisieren
	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<bool> found(nr_of_nodes,false);
	vector<unsigned int> in_edge_id(nr_of_nodes);

	//TEST
	clock_t start,finish;
	start = clock();

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
		tmpid = U.top().id;
		if(!found[tmpid]){
			dist[tmpid] = U.top().distance;
			found[tmpid] = true;
			in_edge_id[tmpid] = U.top().eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				nextEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found[nextEdge->target]){
					// ...tu sie in U
					U.push(U_element(
								nextEdge->distance+dist[tmpid],nextEdge->target,nextEdge->id));
				}
			}
		}
		U.pop();
	}

	// TEST
	finish = clock();
	cout << "Zeit für Dijkstra in Sek: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;

	// TEST: Ergebnisse ausgeben
	//	for(unsigned int i=0; i < g->getNodeCount(); i++){
	//		cout << dist[i] << endl;
	//	}
	// Test: Dijkstra verifizieren
	// Schauen ob die Distanzen stimmen und auch minimal sind
	Graph<Edge, Node, Shortcut>::InEdgesIterator iit;
	Edge* inEdge;
	for(unsigned int i=0; i < g->getNodeCount(); i++){
		iit = g->getInEdgesIt(i);
		// Wenn es kein Startknoten bzw. unerreichbarer Knoten ist
		if(dist[i] != numeric_limits<unsigned int>::max() && dist[i] != 0){
			Edge* tmpedge = g->getEdge(in_edge_id[i]);
			// Wenn die Distanz nicht stimmt...
			if(tmpedge->distance + dist[tmpedge->source] != dist[i]){
				// DISTANZ in Dijkstra nicht konsistent!!
				cout << "Distanz stimmt nicht für: " << i << endl;
			}
		}
		while(iit.hasNext()){
			inEdge = iit.getNext();		
			if(dist[i] > inEdge->distance + dist[inEdge->source] && dist[inEdge->source] != numeric_limits<unsigned int>::max()){
				// Die MINIMALE DISTANZ im Dijkstra ist nicht konsistent!!
				cout << "Minimal stimmt nicht für: ";
				cout << "Edge ID: " << in_edge_id[i];
				cout << " Source Node: " << inEdge->source;
				cout << " Target Node: " << inEdge->target << " (" << i << ")" << endl;
				cout << " Dijkstra Dist: " << dist[i];
				cout << " Scheinbar min Dist: " << inEdge->distance + dist[inEdge->source] << endl;
			}
		}
	}
}

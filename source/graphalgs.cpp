#include "graphalgs.h"
#include <queue>
#include <ctime>
#include <limits>
#include <iostream>
using namespace std;

// struct für die Elemente aus U incl. Vergleich
struct U_element{
	unsigned int distance;
	unsigned int id; 
	unsigned int eid;

	U_element(unsigned int d, unsigned int i, unsigned int ei){
		distance = d;
		id = i;
		eid = ei; 
	}   

	U_element(){
		distance = 0;
		id = 0;
		eid = 0;
	}   
};  

// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
class Compare_U_element{
	public:
		bool operator()(U_element& u1, U_element& u2){
			return u1.distance > u2.distance;
		}   
};

void Dijkstra(Graph<Edge, Node, Shortcut>* g, unsigned int node_id){
	// TEST mit Iterator
	for(int i=0; i<10; i++){
		// Iterator für die ausgehenden Kanten eines Knotens
		Graph<Edge, Node, Shortcut>::OutEdgesIterator it = g->getOutEdgesIt(node_id);
		// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
		std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

		unsigned int nr_of_nodes = g->getNodeCount();
		vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
		vector<unsigned int> in_edge_id(nr_of_nodes);

		Edge* currentEdge;

		vector<bool> found(nr_of_nodes,false);

		//TEST
		clock_t start,finish;
		start = clock();

		// Den ersten Knoten abarbeiten
		dist[node_id] = 0;
		found[node_id] = true;
		while(it.hasNext()){
			currentEdge = it.getNext();
			// Die Knoten mit ihrer Distanz in U stecken
			U.push(U_element(currentEdge->distance,currentEdge->target,currentEdge->id));
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
					currentEdge = it.getNext();
					// Wenn sie noch nicht gefunden wurde...
					if(!found[currentEdge->target]){
						// ...tu sie in U
						U.push(U_element(
									currentEdge->distance+dist[tmpid],currentEdge->target,currentEdge->id));
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

	cout << "Wechsel auf anderen Dijkstra!" << endl;

	// TEST mit direkt auf Graphstrukturen arbeiten
	for(int i=0; i<10; i++){
		// Pointer um die akutelle Kante zu behandeln
		Edge* currentEdge;
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
		for(unsigned int i=g->getLowerEdgeBound(node_id); i<g->getUpperEdgeBound(node_id); i++){
			currentEdge = g->getEdge(i);
			// Die Knoten mit ihrer Distanz in U stecken
			U.push(U_element(currentEdge->distance,currentEdge->target,currentEdge->id));
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
				for(unsigned int i=g->getLowerEdgeBound(tmpid); i<g->getUpperEdgeBound(tmpid); i++){
					currentEdge = g->getEdge(i);
					// Wenn sie noch nicht gefunden wurde...
					if(!found[currentEdge->target]){
						// ...tu sie in U
						U.push(U_element(
									currentEdge->distance+dist[tmpid],currentEdge->target,currentEdge->id));
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
}

void BiDijkstra(Graph<Edge, Node, Shortcut>* g, unsigned int node_id1, unsigned int node_id2){
	// Iterator für die ausgehenden und eingehenden Kanten eines Knotens
	Graph<Edge, Node, Shortcut>::OutEdgesIterator itout = g->getOutEdgesIt(node_id1);
	Graph<Edge, Node, Shortcut>::InEdgesIterator itin = g->getInEdgesIt(node_id2);

	// Die priotiry_queues, welche der Menge U im Dijkstra entsprechen
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U1;
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U2;

	unsigned int nr_of_nodes = g->getNodeCount();
	unsigned int current_min = numeric_limits<unsigned int>::max();
	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<unsigned int> dijk_edge_id(nr_of_nodes);

	Edge* currentEdge;

	vector<bool> found1(nr_of_nodes,false);
	vector<bool> found2(nr_of_nodes,false);

	//TEST
	clock_t start,finish;
	start = clock();

	// Den ersten Knoten abarbeiten
	/*dist[node_id] = 0;
	found[node_id] = true;
	while(it.hasNext()){
		currentEdge = it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_element(currentEdge->distance,currentEdge->target,currentEdge->id));
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
				currentEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge->target]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge->distance+dist[tmpid],currentEdge->target,currentEdge->id));
				}
			}
		}
		U.pop();
	}*/
	// TEST
	finish = clock();
	cout << "Zeit für Dijkstra in Sek: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
}

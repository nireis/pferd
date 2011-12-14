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

	U_element(unsigned int d, unsigned int i, unsigned int ei)
		:distance(d),id(i),eid(ei){}   

	U_element()
		:distance(0),id(0),eid(0){}   
};  

// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
class Compare_U_element{
	public:
		bool operator()(U_element& u1, U_element& u2){
			return u1.distance > u2.distance;
		}   
};

// struct für die Elemente aus U incl. Vergleich
struct U_element_bi{
	unsigned int distance;
	unsigned int id; 
	unsigned int eid;
	bool found_by;

	U_element_bi(unsigned int d, unsigned int i, unsigned int ei, bool f_b)
		:distance(d),id(i),eid(ei),found_by(f_b){}   

	U_element_bi()
		:distance(0),id(0),eid(0),found_by(0){}   
};  

// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
class Compare_U_element_bi{
	public:
		bool operator()(U_element_bi& u1, U_element_bi& u2){
			return u1.distance > u2.distance;
		}   
};

void Dijkstra(Graph* g, unsigned int node_id){
	// Iterator für die ausgehenden Kanten eines Knotens
	Graph::OutEdgesIterator it = g->getOutEdgesIt(node_id);
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

	Edge* currentEdge;

	unsigned int nr_of_nodes = g->getNodeCount();
	vector<bool> found(nr_of_nodes,false);

	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<unsigned int> in_edge_id(nr_of_nodes);

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
	Graph::InEdgesIterator iit;
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
	cout << "Distanz von Knoten 5000:" << dist[5000] << endl;
}

void DirectDijkstra(Graph* g, unsigned int node_id){
	// Pointer um die akutelle Kante zu behandeln
	Edge currentEdge;
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
		currentEdge = * g->getEdge(i);
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_element(currentEdge.distance,currentEdge.target,currentEdge.id));
	}

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	U_element t;
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		t = U.top();
		tmpid = t.id;
		if(!found[tmpid]){
			dist[tmpid] = t.distance;
			found[tmpid] = true;
			in_edge_id[tmpid] = t.eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			for(unsigned int i=g->getLowerEdgeBound(tmpid); i<g->getUpperEdgeBound(tmpid); i++){
				currentEdge = * g->getEdge(i);
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge.target]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge.distance+dist[tmpid],currentEdge.target,currentEdge.id));
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
	Graph::InEdgesIterator iit;
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

void BiDijkstra(Graph* g, unsigned int node_id0, unsigned int node_id1){
	// Iterator für die ausgehenden und eingehenden Kanten eines Knotens
	Graph::OutEdgesIterator itout = g->getOutEdgesIt(node_id0);
	Graph::InEdgesIterator itin = g->getInEdgesIt(node_id1);

	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element_bi, std::vector<U_element_bi>, Compare_U_element_bi> U;

	unsigned int nr_of_nodes = g->getNodeCount();
	unsigned int current_min_path = numeric_limits<unsigned int>::max();
	unsigned int min_edge_id;
	unsigned int prob_min_val;
	unsigned int tmpid;
	unsigned int tmptgtsrc;
	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<unsigned int> dijk_edge_id(nr_of_nodes);

	Edge* currentEdge;

	vector<bool> found0(nr_of_nodes,false);
	vector<bool> found1(nr_of_nodes,false);

	//TEST
	clock_t start,finish;
	start = clock();

	// Die ersten Knoten abarbeiten
	U.push(U_element_bi(0,node_id0,0,0));
	U.push(U_element_bi(0,node_id1,0,1));

	// Die restlichen Knoten abarbeiten
	while(current_min_path > 2*U.top().distance && !U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!(found0[tmpid] || found1[tmpid])){
			dist[tmpid] = U.top().distance;
	 		dijk_edge_id[tmpid] = U.top().eid;
			if(!U.top().found_by){
				found0[tmpid] = true;
				itout = g->getOutEdgesIt(tmpid);
				while(itout.hasNext()){
					currentEdge = itout.getNext();
					// Wenn sie noch nicht gefunden wurde...
					if(!found0[currentEdge->target]){
						tmptgtsrc = currentEdge->target;
						// ...und der nächste Knoten schon vom anderen Dijkstra gefunden wurde...
						if(found1[tmptgtsrc]){
							// ...neues Minimum zuweisen wenn nötig, sonst...
							prob_min_val = dist[tmpid] + dist[tmptgtsrc] + currentEdge->distance;
							if(prob_min_val < current_min_path){
								current_min_path = prob_min_val;
								min_edge_id = currentEdge->id;
							}
						}
						else{
							// ...tu sie in U
							U.push(U_element_bi(
										currentEdge->distance+dist[tmpid],currentEdge->target,currentEdge->id,0));
						}
					}
				}
			}
			else{
				found1[tmpid] = true;
				itin = g->getInEdgesIt(tmpid);
				while(itin.hasNext()){
					currentEdge = itin.getNext();
					// Wenn sie noch nicht gefunden wurde...
					if(!found1[currentEdge->source]){
						tmptgtsrc = currentEdge->source;
						// ...und der nächste Knoten schon vom anderen Dijkstra gefunden wurde...
						if(found0[tmptgtsrc]){
							// ...neues Minimum zuweisen wenn nötig, sonst...
							prob_min_val = dist[tmpid] + dist[tmptgtsrc] + currentEdge->distance;
							if(prob_min_val < current_min_path){
								current_min_path = prob_min_val;
								min_edge_id = currentEdge->id;
							}
						}
						else{
							// ...tu sie in U
							U.push(U_element_bi(
										currentEdge->distance+dist[tmpid],currentEdge->source,currentEdge->id,1));
						}
					}
				}
			}
		}
		U.pop();
	}
	// TEST
	finish = clock();
	cout << "Zeit für Dijkstra in Sek: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
	cout << "Distanz: " << current_min_path << endl;
}

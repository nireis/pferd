#include "graphalgs.h"
#include <queue>
#include <ctime>
#include <limits>
#include <iostream>
#include "structs.h"
using namespace std;

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
	EdgesIterator it = g->getOutEdgesIt(node_id);
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

	Edge* currentEdge;

	unsigned int nr_of_nodes = g->getNodeCount();
	vector<bool> found(nr_of_nodes,false);

	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<unsigned int> in_edge_id(nr_of_nodes);

	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found[node_id] = true;
	while(it.hasNext()){
		currentEdge = it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_element(currentEdge->value,currentEdge->other_node,currentEdge->id));
	}

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	unsigned int tmpdist;
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!found[tmpid]){
			tmpdist = U.top().distance;
			dist[tmpid] = tmpdist;
			found[tmpid] = true;
			in_edge_id[tmpid] = U.top().eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				currentEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge->other_node]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge->value+tmpdist,currentEdge->other_node,currentEdge->id));
				}
			}
		}
		U.pop();
	}

//	// TEST: Ergebnisse ausgeben
//	//	for(unsigned int i=0; i < g->getNodeCount(); i++){
//	//		cout << dist[i] << endl;
//	//	}
//	// Test: Dijkstra verifizieren
//	// Schauen ob die Distanzen stimmen und auch minimal sind
//	EdgesIterator iit;
//	Edge* inEdge;
//	for(unsigned int i=0; i < g->getNodeCount(); i++){
//		iit = g->getInEdgesIt(i);
//		// Wenn es kein Startknoten bzw. unerreichbarer Knoten ist
//		if(dist[i] != numeric_limits<unsigned int>::max() && dist[i] != 0){
//			Edge* tmpedge = g->getInEdge(in_edge_id[i]);
//			// Wenn die Distanz nicht stimmt...
//			if(tmpedge->value + dist[tmpedge->other_node] != dist[i]){
//				// DISTANZ in Dijkstra nicht konsistent!!
//				cout << "Distanz stimmt nicht für: " << i << endl;
//			}
//		}
//		while(iit.hasNext()){
//			inEdge = iit.getNext();		
//			if(dist[i] > inEdge->value + dist[inEdge->other_node] && dist[inEdge->other_node] != numeric_limits<unsigned int>::max()){
//				// Die MINIMALE DISTANZ im Dijkstra ist nicht konsistent!!
//				cout << "Minimal stimmt nicht für: ";
//				cout << "Edge ID: " << in_edge_id[i];
//				cout << " Other Node: " << inEdge->other_node;
//				//cout << " Target Node: " << inEdge->target << " (" << i << ")" << endl;
//				cout << " Dijkstra Dist: " << dist[i];
//				cout << " Scheinbar min Dist: " << inEdge->value + dist[inEdge->other_node] << endl;
//			}
//		}
//	}
}

bool Dijkstra(Graph* g, unsigned int start_node_id, unsigned int end_node_id,
		unsigned int over_node_id){
	// Iterator für die ausgehenden Kanten eines Knotens
	EdgesIterator it = g->getOutEdgesIt(start_node_id);
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

	Edge* currentEdge;

	unsigned int nr_of_nodes = g->getNodeCount();
	vector<bool> found(nr_of_nodes,false);

	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<unsigned int> in_edge_id(nr_of_nodes);

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,start_node_id,0));

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!found[tmpid]){
			dist[tmpid] = U.top().distance;
			found[tmpid] = true;
			// Testen ob das unser Zielknoten ist
			if(tmpid == end_node_id){
				break;
			}
			in_edge_id[tmpid] = U.top().eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				currentEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge->other_node]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id));
				}
			}
		}
		U.pop();
	}
	
	// Backtracing des kürzesten Pfades und prüfen, ob er über over_node_id geht.
	if(found[end_node_id]){
		tmpid = end_node_id;
		while(tmpid != start_node_id){
			if(tmpid == over_node_id){
				return true;
			}
			tmpid = g->getOutEdge(in_edge_id[tmpid])->other_node;
		}
	}
	return false;
}



unsigned int Dijkstra(Graph* g, unsigned int node_id0, unsigned int node_id1){
	// Iterator für die ausgehenden Kanten eines Knotens
	EdgesIterator it = g->getOutEdgesIt(node_id0);
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

	Edge* currentEdge;

	unsigned int nr_of_nodes = g->getNodeCount();
	vector<bool> found(nr_of_nodes,false);

	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<unsigned int> in_edge_id(nr_of_nodes);

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!found[tmpid]){
			dist[tmpid] = U.top().distance;
			// Testen ob das unser Zielknoten ist
			if(tmpid == node_id1){
				break;
			}
			found[tmpid] = true;
			in_edge_id[tmpid] = U.top().eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				currentEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge->other_node]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id));
				}
			}
		}
		U.pop();
	}
	return dist[node_id1];
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

	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found[node_id] = true;
	for(unsigned int i=g->getLowerOutEdgeBound(node_id); i<g->getUpperOutEdgeBound(node_id); i++){
		currentEdge = * g->getOutEdge(i);
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_element(currentEdge.value,currentEdge.other_node,currentEdge.id));
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
			for(unsigned int i=g->getLowerOutEdgeBound(tmpid); i<g->getUpperOutEdgeBound(tmpid); i++){
				currentEdge = * g->getOutEdge(i);
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge.other_node]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge.value+dist[tmpid],currentEdge.other_node,currentEdge.id));
				}
			}
		}
		U.pop();
	}

//	// TEST: Ergebnisse ausgeben
//	//	for(unsigned int i=0; i < g->getNodeCount(); i++){
//	//		cout << dist[i] << endl;
//	//	}
//	// Test: Dijkstra verifizieren
//	// Schauen ob die Distanzen stimmen und auch minimal sind
//	EdgesIterator iit;
//	Edge* inEdge;
//	for(unsigned int i=0; i < g->getNodeCount(); i++){
//		iit = g->getInEdgesIt(i);
//		// Wenn es kein Startknoten bzw. unerreichbarer Knoten ist
//		if(dist[i] != numeric_limits<unsigned int>::max() && dist[i] != 0){
//			Edge* tmpedge = g->getInEdge(in_edge_id[i]);
//			// Wenn die Distanz nicht stimmt...
//			if(tmpedge->value + dist[tmpedge->other_node] != dist[i]){
//				// DISTANZ in Dijkstra nicht konsistent!!
//				cout << "Distanz stimmt nicht für: " << i << endl;
//			}
//		}
//		while(iit.hasNext()){
//			inEdge = iit.getNext();		
//			if(dist[i] > inEdge->value + dist[inEdge->other_node] && dist[inEdge->other_node] != numeric_limits<unsigned int>::max()){
//				// Die MINIMALE DISTANZ im Dijkstra ist nicht konsistent!!
//				cout << "Minimal stimmt nicht für: ";
//				cout << "Edge ID: " << in_edge_id[i];
//				cout << " Other Node: " << inEdge->other_node;
//				//cout << " Target Node: " << inEdge->target << " (" << i << ")" << endl;
//				cout << " Dijkstra Dist: " << dist[i];
//				cout << " Scheinbar min Dist: " << inEdge->value + dist[inEdge->other_node] << endl;
//			}
//		}
//	}
}

unsigned int DirectDijkstra(Graph* g, unsigned int node_id0, unsigned int node_id1){
	// Pointer um die akutelle Kante zu behandeln
	Edge currentEdge;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

	unsigned int nr_of_nodes = g->getNodeCount();
	// Die ganzen Vektoren initialisieren
	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	vector<bool> found(nr_of_nodes,false);
	vector<unsigned int> in_edge_id(nr_of_nodes);

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	U_element t;
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		t = U.top();
		tmpid = t.id;
		if(!found[tmpid]){
			dist[tmpid] = t.distance;
			if(tmpid == node_id1){
				break;
			}
			found[tmpid] = true;
			in_edge_id[tmpid] = t.eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			for(unsigned int i=g->getLowerOutEdgeBound(tmpid); i<g->getUpperOutEdgeBound(tmpid); i++){
				currentEdge = * g->getOutEdge(i);
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge.other_node]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge.value+dist[tmpid],currentEdge.other_node,currentEdge.id));
				}
			}
		}
		U.pop();
	}
	return dist[node_id1];
}

unsigned int BiDijkstra(Graph* g, unsigned int node_id0, unsigned int node_id1){
	// Iterator für die ausgehenden und eingehenden Kanten eines Knotens
	EdgesIterator itout = g->getOutEdgesIt(node_id0);
	EdgesIterator itin = g->getInEdgesIt(node_id1);

	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element_bi, std::vector<U_element_bi>, Compare_U_element_bi> U;

	unsigned int nr_of_nodes = g->getNodeCount();
	unsigned int current_min_path = numeric_limits<unsigned int>::max();
	unsigned int min_edge_id;
	unsigned int prob_min_val;
	unsigned int tmpid;
	unsigned int tmptgtsrc;
	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());

	vector<bool> found0(nr_of_nodes,false);
	vector<bool> found1(nr_of_nodes,false);
	vector< vector<bool> > found(2,vector<bool> (nr_of_nodes,false));

	Edge* currentEdge;

	// Die ersten Knoten abarbeiten
	if(node_id0 == node_id1){
		return 0;
	}
	else{
		U.push(U_element_bi(0,node_id0,0,0));
		U.push(U_element_bi(0,node_id1,0,1));
	}

	// Die restlichen Knoten abarbeiten
	while(current_min_path > 2*U.top().distance && !U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!(found[0][tmpid] || found[1][tmpid])){
			dist[tmpid] = U.top().distance;
			if(!U.top().found_by){
				found[0][tmpid] = true;
				itout = g->getOutEdgesIt(tmpid);
				while(itout.hasNext()){
					currentEdge = itout.getNext();
					// Wenn sie noch nicht gefunden wurde...
					if(!found[0][currentEdge->other_node]){
						tmptgtsrc = currentEdge->other_node;
						// ...und der nächste Knoten schon vom anderen Dijkstra gefunden wurde...
						if(found[1][tmptgtsrc]){
							// ...neues Minimum zuweisen wenn nötig, sonst...
							prob_min_val = dist[tmpid] + dist[tmptgtsrc] + currentEdge->value;
							if(prob_min_val < current_min_path){
								current_min_path = prob_min_val;
								min_edge_id = currentEdge->id;
							}
						}
						else{
							// ...tu sie in U
							U.push(U_element_bi(
										currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id,0));
						}
					}
				}
			}
			else{
				found[1][tmpid] = true;
				itin = g->getInEdgesIt(tmpid);
				while(itin.hasNext()){
					currentEdge = itin.getNext();
					// Wenn sie noch nicht gefunden wurde...
					if(!found[1][currentEdge->other_node]){
						tmptgtsrc = currentEdge->other_node;
						// ...und der nächste Knoten schon vom anderen Dijkstra gefunden wurde...
						if(found[0][tmptgtsrc]){
							// ...neues Minimum zuweisen wenn nötig, sonst...
							prob_min_val = dist[tmpid] + dist[tmptgtsrc] + currentEdge->value;
							if(prob_min_val < current_min_path){
								current_min_path = prob_min_val;
								min_edge_id = currentEdge->id;
							}
						}
						else{
							// ...tu sie in U
							U.push(U_element_bi(
										currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id,1));
						}
					}
				}
			}
		}
		U.pop();
	}
	return current_min_path;
}


/*
 * !! 
 *		experimenteller Dijkstra 
 *		mit Optimierungen 
 *	!!
 */
struct U_elementA{
	unsigned int distance;
	unsigned int id; 
	//unsigned int eid;

	U_elementA(unsigned int d, unsigned int i)
		:distance(d),id(i){}//,eid(ei){}   

	U_elementA()
		:distance(0),id(0){}   
};  

// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
class Compare_U_elementA{
	public:
		bool operator()(U_elementA& u1, U_elementA& u2){
			return u1.distance > u2.distance;
		}   
};
void Dijkstra_plus(Graph* g, unsigned int node_id){
	// Iterator für die ausgehenden Kanten eines Knotens
	EdgesIterator it = g->getOutEdgesIt(node_id);

	Edge* currentEdge;
	unsigned int nr_of_nodes = g->getNodeCount();

	//vector<unsigned int> in_edge_id(nr_of_nodes);
	//vector<bool> found(nr_of_nodes,false);
	bitvec found = bitvec(nr_of_nodes);
	found.reset();
	//boost::dynamic_bitset< nr_of_nodes > bfound ;
	//vector<unsigned int> dist(nr_of_nodes);//,numeric_limits<unsigned int>::max());
	uintvec dist = uintvec(nr_of_nodes);//,numeric_limits<unsigned int>::max());
	//uintvec in_edge_id = uintvec(nr_of_nodes);
	
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_elementA, std::vector<U_elementA>, Compare_U_elementA> U;

	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found.set(node_id);// = true;
	while(it.hasNext()){
		currentEdge =  it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_elementA(currentEdge->value,currentEdge->other_node));
	}

	unsigned int tmpid;
	unsigned int tmpdist;
	// Die restlichen Knoten abarbeiten
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!found.get(tmpid)){
			tmpdist = U.top().distance;
			dist[tmpid] = tmpdist;
			found.set(tmpid);// = true;
			//in_edge_id[tmpid] = ue.eid;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				currentEdge =  it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found.get(currentEdge->other_node)){
					// ...tu sie in U
					U.push(U_elementA(
								currentEdge->value+tmpdist,currentEdge->other_node));
				}
			}
		}
		U.pop();
	}
}
/*
 * !! 
 *		experimenteller Dijkstra 
 *		mit Optimierungen 
 *	!!
 *		ENDE
 */

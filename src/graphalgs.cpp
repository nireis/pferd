#include "graphalgs.h"
#include <queue>
#include <ctime>
#include <cstdlib>
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
	if(node_id0 == node_id1){
		return 0;
	}
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	while(!U.empty()){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!found[tmpid]){
			dist[tmpid] = U.top().distance;
			// Testen ob das unser Zielknoten ist
			found[tmpid] = true;
			in_edge_id[tmpid] = U.top().eid;
			if(tmpid == node_id1){
				break;
			}
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
	if(found[node_id1]){
		tmpid = node_id1;
		while(tmpid != node_id0){
			cout << tmpid << endl;
			tmpid = g->getInEdge(g->getEdgeData(in_edge_id[tmpid]).in_index)->other_node;
		}
	}
	cout << "=============" << endl;
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
	unsigned int min_path_length = numeric_limits<unsigned int>::max();
	unsigned int min_edge_id;
	unsigned int tmp_min_path_length;
	unsigned int tmpid;
	unsigned int currentEdgeTarget;
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
	while(min_path_length > 2*U.top().distance && !U.empty()){
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
						currentEdgeTarget = currentEdge->other_node;
						// ...und der nächste Knoten schon vom anderen Dijkstra gefunden wurde...
						if(found[1][currentEdgeTarget]){
							// ...neues Minimum zuweisen wenn nötig, sonst...
							tmp_min_path_length = dist[tmpid] + dist[currentEdgeTarget] + currentEdge->value;
							if(tmp_min_path_length < min_path_length){
								min_path_length = tmp_min_path_length;
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
						currentEdgeTarget = currentEdge->other_node;
						// ...und der nächste Knoten schon vom anderen Dijkstra gefunden wurde...
						if(found[0][currentEdgeTarget]){
							// ...neues Minimum zuweisen wenn nötig, sonst...
							tmp_min_path_length = dist[tmpid] + dist[currentEdgeTarget] + currentEdge->value;
							if(tmp_min_path_length < min_path_length){
								min_path_length = tmp_min_path_length;
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
	// damit compilerwarnung weg geht
	min_edge_id = 0;
	return min_path_length + min_edge_id;
}


/*
 * Testet das Max Ind Set auf seine Korrektheit.
 *
 * @solution Ist das angebliche Max Ind Set.
 */
void independent_set_test(Graph* g, list<unsigned int> solution){
	unsigned int nr_of_nodes = g->getNodeCount();
	vector<bool> found(nr_of_nodes,false);
	EdgesIterator it;
	unsigned int tmpnode;

	cout << "Anzahl Knoten der Independent Set Lösung: " << solution.size() << endl;

	while(!solution.empty()){
		tmpnode = solution.front();
		solution.pop_front();
		// Wurde der Knoten schon von einem anderen erreicht?
		if(found[tmpnode]){
			cout << "Independent Set NICHT korrekt!" << endl;
			return;
		}
		else{
			// Alle erreichten markieren
			found[tmpnode] = true;
			it = g->getOutEdgesIt(tmpnode);
			while(it.hasNext()){
				found[it.getNext()->other_node] = true;
			}
			it = g->getInEdgesIt(tmpnode);
			while(it.hasNext()){
				found[it.getNext()->other_node] = true;
			}
		}
	}
	// Schauen dass alle markiert sind
	for(unsigned int i=0; i<nr_of_nodes; i++){
		if(!found[i]){
			cout << "Maximal Independent Set NICHT korrekt!" << endl;
			return;
		}
	}
	cout << "Maximal Independent Set Korrekt!" << endl;
}

/*
 * Berechnet ein Maximales Independent Set.
 *
 * @return Liste der Knoten des Max Ind Sets.
 */
list<unsigned int> independent_set(Graph* g){
	unsigned int nr_of_nodes = g->getNodeCount();
	list<unsigned int> solution;
	vector<bool> found(nr_of_nodes,false);
	EdgesIterator it;
	// Random Nummer für den Startknoten
	srand((unsigned)time(0));
	unsigned int r = rand() % nr_of_nodes;

	// Erster Part der Knoten (wegen der Randomisierung)
	for(unsigned int i=r; i<nr_of_nodes; i++){
		// Prüfen ob der Knoten aufgenommen werden kann
		if(!found[i]){
			solution.push_front(i);
			// Alle ausgehenden Kanten verfolgen
			it = g->getOutEdgesIt(i);
			while(it.hasNext()){
				found[it.getNext()->other_node] = true;
			}
			// Alle eingehenden Kanten verfolgen
			it = g->getInEdgesIt(i);
			while(it.hasNext()){
				found[it.getNext()->other_node] = true;
			}
		}
	}
	// Zweiter Part der Knoten
	for(unsigned int i=0; i<r; i++){
		// Prüfen ob der Knoten aufgenommen werden kann
		if(!found[i]){
			solution.push_front(i);
			// Alle ausgehenden Kanten verfolgen
			it = g->getOutEdgesIt(i);
			while(it.hasNext()){
				found[it.getNext()->other_node] = true;
			}
			// Alle eingehenden Kanten verfolgen
			it = g->getInEdgesIt(i);
			while(it.hasNext()){
				found[it.getNext()->other_node] = true;
			}
		}
	}
	// independent_set_test(g, solution); //TEST
	return solution;
}

unsigned int CHDijkstra(SCGraph* g, unsigned int node_id0, unsigned int node_id1,
		list<unsigned int>* path){
	// Iterator für die ausgehenden und eingehenden Kanten eines Knotens
	// TODO Iterator je nach Art über Parameter 1 und 0 bekommen, so dass die
	// beiden großen if-Schleifen (*gnihihi*) verschwinden können.
	EdgesIterator itout = g->getOutEdgesIt(node_id0);
	EdgesIterator itin = g->getInEdgesIt(node_id1);

	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element_bi, std::vector<U_element_bi>, Compare_U_element_bi> U;

	unsigned int nr_of_nodes = g->getNodeCount();
	unsigned int min_path_length = numeric_limits<unsigned int>::max();
	unsigned int tmp_min_path_length;
	unsigned int min_path_center;
	unsigned int tmpid;
	unsigned int currentEdgeTarget;
	vector<unsigned int> dist(nr_of_nodes,numeric_limits<unsigned int>::max());
	// TODO Möglicherweise found und found_by mergen, je nach Geschwindigkeitsänderung.
	vector< vector<bool> > found(2,vector<bool> (nr_of_nodes,false));
	// Gibt an ob von Dijkstra 0/1 ein bestimmter Knoten schon gefunden wurde.
	vector< vector<unsigned int> > found_by(2,vector<unsigned int> (nr_of_nodes));
	Edge* currentEdge;

	// Die ersten Knoten abarbeiten
	if(node_id0 == node_id1){
		min_path_center = node_id0;
		return 0;
	}
	else{
		// Die Kanten der ersten beiden Knoten anschauen und die targets zu
		// U hinzufügen, wenn das Level größer ist.
		found[0][node_id0] = true;
		itout = g->getOutEdgesIt(node_id0);
		while(itout.hasNext()){
			currentEdge = itout.getNext();
			if(currentEdge->other_lvl > g->getNodeLVL(node_id0)){
				currentEdgeTarget = currentEdge->other_node;
				// Wenn wir schon den anderen Knoten gefunden haben...
				if(currentEdgeTarget == node_id1){
					min_path_length = currentEdge->value;
					// Die Werte für das Backtracing setzen.
					min_path_center = node_id1;
					found_by[0][node_id1] = currentEdge->id;
				}
				else{
					// ...sonst tu sie in U
					U.push(U_element_bi(
						currentEdge->value,currentEdge->other_node,currentEdge->id,0));
				}
			}
		}
		found[1][node_id1] = true;
		itin = g->getInEdgesIt(node_id1);
		while(itin.hasNext()){
			currentEdge = itin.getNext();
			if(currentEdge->other_lvl > g->getNodeLVL(node_id1)){
				currentEdgeTarget = currentEdge->other_node;
				// Wenn wir schon den anderen Knoten gefunden haben...
				if(currentEdgeTarget == node_id0){
					min_path_length = currentEdge->value;
					// Die Werte für das Backtracing setzen.
					min_path_center = node_id0;
					found_by[1][node_id0] = currentEdge->id;
				}
				else{
					// ...sonst tu sie in U
					U.push(U_element_bi(
						currentEdge->value,currentEdge->other_node,currentEdge->id,1));
				}
			}
		}
	}
	dist[node_id0] = 0;
	dist[node_id1] = 0;

	// Die restlichen Knoten abarbeiten
	while(min_path_length >= U.top().distance && !U.empty()){
		tmpid = U.top().id;
		if(!U.top().found_by){
			if(!found[0][tmpid]){
				found[0][tmpid] = true;
				found_by[0][tmpid] = U.top().eid;
				// Wir prüfen ob sich die Dijkstras treffen...
				if(found[1][tmpid]){
					tmp_min_path_length = dist[tmpid] + U.top().distance;
					// ...und weisen ein Minimum zu wenn nötig.
					if(tmp_min_path_length < min_path_length){
						min_path_length = tmp_min_path_length;
						min_path_center = tmpid;
					}
				}
				dist[tmpid] = U.top().distance;
				itout = g->getOutEdgesIt(tmpid);
				while(itout.hasNext()){
					currentEdge = itout.getNext();
					currentEdgeTarget = currentEdge->other_node;
					// Wir laufen nur bergauf...
					if(currentEdge->other_lvl > g->getNodeLVL(tmpid)){
						// ...und tun die Knoten in U, wenn sie noch nicht abgearbeitet wurden.
						if(!found[0][currentEdgeTarget]){
							U.push(U_element_bi(
								currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id,0));
						}
					}
				}
			}
		}
		else{
			if(!found[1][tmpid]){
				found[1][tmpid] = true;
				found_by[1][tmpid] = U.top().eid;
				// Wir prüfen ob sich die Dijkstras treffen...
				if(found[0][tmpid]){
					tmp_min_path_length = dist[tmpid] + U.top().distance;
					// ...und weisen ein Minimum zu wenn nötig.
					if(tmp_min_path_length < min_path_length){
						min_path_length = tmp_min_path_length;
						min_path_center = tmpid;
					}
				}
				dist[tmpid] = U.top().distance;
				itin = g->getInEdgesIt(tmpid);
				while(itin.hasNext()){
					currentEdge = itin.getNext();
					currentEdgeTarget = currentEdge->other_node;
					// Wir laufen nur bergauf...
					if(currentEdge->other_lvl > g->getNodeLVL(tmpid)){
						// ...und tun die Knoten in U, wenn sie noch nicht abgearbeitet wurden.
						if(!found[1][currentEdgeTarget]){
							U.push(U_element_bi(
								currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id,1));
						}
					}
				}
			}
		}
		U.pop();
	}
	// Backtracing.
	unsigned int takenedge;
	// Nur, wenn ein Pfad existiert...
	if(min_path_length != numeric_limits<unsigned int>::max()){
		tmpid = min_path_center;
		// ...gehe von dem Treffpunkt jeweils zurück zum Anfangsknoten
		// und speichere die Kanten-IDs.
		while(tmpid != node_id0){
			cout << tmpid << endl;
			takenedge = found_by[0][tmpid];
			path->push_front(takenedge);
			// TODO EdgeData by reference?
			tmpid = g->getInEdge(g->getEdgeData(takenedge).in_index)->other_node;
		}
		tmpid = min_path_center;
		while(tmpid != node_id1){
			cout << tmpid << endl;
			takenedge = found_by[1][tmpid];
			path->push_front(takenedge);
			tmpid = g->getOutEdge(g->getEdgeData(takenedge).out_index)->other_node;
		}
	}
	return min_path_length;
}

bool CHDijkstraTest(Graph* g, SCGraph* scg, unsigned int maxid){
	unsigned int dist0;
	unsigned int dist1;
	for(unsigned int i=1056; i<=maxid; i++){
		list<unsigned int> path;
		dist0 = Dijkstra(g, 0, i);
		dist1 = CHDijkstra(scg, 0, i, &path);
		cout << "Dijkstra dist für Knoten " << i << ": " << dist0 << ", CHDijkstra dist: " << dist1 << endl;
		if(dist0 != dist1){
			return false;
		}
	}
	return true;
}

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
			// TODO
			// tmpid muss node-id des zielknotens sein, 
			// wenn über in_edge_id die kante wiederbesorgt
			// werden soll
			//
			//=> verschoben nach unten in die if abfrage
			//
			///**/ in_edge_id[tmpid] = U.top().eid;
			if(tmpid == node_id1){
				// TODO U komplett leeren, 
				// damit weitere kanten knoten, die noch
				// in u liegen, nicht weiter
				// betrachtet werden ?
				break;
			}
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				currentEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!found[currentEdge->other_node]){
					in_edge_id[currentEdge->other_node] = currentEdge->id; // => u_element.eid redudant
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
//			cout << tmpid << endl;
			tmpid = g->getInEdge( in_edge_id[tmpid] )->other_node;
		}
	}
//	cout << "=============" << endl;
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

unsigned int CHDijkstra(SCGraph* g, unsigned int node_id0, unsigned int node_id1){
	// Wegen Codeschönheit: Array für Source und Target anlegen.
	vector<unsigned int> node_id(2);
	node_id[0] = node_id0;
	node_id[1] = node_id1;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element_bi, std::vector<U_element_bi>, Compare_U_element_bi> U;
	unsigned int nr_of_nodes = g->getNodeCount();
	unsigned int min_path_length = numeric_limits<unsigned int>::max();
	unsigned int min_path_center;
	vector<unsigned int> dist(nr_of_nodes);
	// Gibt an mit welcher Kante Dijkstra 0/1 einen bestimmten Knoten schon gefunden hat.
	// Wenn nicht ist der Wert -1.
	vector< vector<int> > found_by(2,vector<int> (nr_of_nodes,-1));

	unsigned int numEdges = 0;
	unsigned int numNodes = 2;

	// Die ersten Knoten abarbeiten
	if(node_id0 == node_id1){
		min_path_center = node_id0;
		return 0;
	}
	// Die Kanten der ersten beiden Knoten anschauen und die targets zu
	// U hinzufügen, wenn das Level größer ist.
	for(int i=0; i<2; i++){
		found_by[i][node_id[i]] = (int)node_id[i];
		EdgesIterator it = g->getEdgesIt(i, node_id[i]);
		while(it.hasNext()){
			Edge* currentEdge = it.getNext();
			if(currentEdge->other_lvl > g->getNodeLVL(node_id[i])){
				unsigned int currentEdgeTarget = currentEdge->other_node;
				// Wenn wir schon den anderen Knoten gefunden haben...
				if(currentEdgeTarget == node_id[(i+1)%2]){
					min_path_length = currentEdge->value;
					// Die Werte für das Backtracing setzen.
					min_path_center = node_id[(i+1)%2];
					found_by[i][node_id[(i+1)%2]] = (int)currentEdge->id;
				}
				else{
					// ...sonst tu sie in U
					U.push(U_element_bi(
						currentEdge->value,currentEdge->other_node,currentEdge->id,i));
				}
			}
			else{
				break;
			}
			numEdges++;
		}
	}
	dist[node_id0] = 0;
	dist[node_id1] = 0;

	// Die restlichen Knoten abarbeiten
	while(!U.empty() && min_path_length > U.top().distance){
		numNodes++;
		int i = U.top().found_by;
		unsigned int tmpid = U.top().id;
		if(found_by[i][tmpid] == -1){
			found_by[i][tmpid] = (int)U.top().eid;
			unsigned int tmp_min_path_length = dist[tmpid] + U.top().distance;
			// Wir prüfen ob ein minimaler Pfad gefunden wurde.
			if(found_by[(i+1)%2][tmpid] != (-1) && tmp_min_path_length < min_path_length){
				min_path_length = tmp_min_path_length;
				min_path_center = tmpid;
			}
			dist[tmpid] = U.top().distance;
			EdgesIterator it = g->getEdgesIt(i, tmpid);
			while(it.hasNext()){
				Edge* currentEdge = it.getNext();
				unsigned int currentEdgeTarget = currentEdge->other_node;
				// Wir laufen nur bergauf...
				if(currentEdge->other_lvl > g->getNodeLVL(tmpid)){
					// ...und tun die Knoten in U, wenn sie noch nicht abgearbeitet wurden.
					if(found_by[i][currentEdgeTarget] == -1){
						U.push(U_element_bi(
							currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id,i));
					}
				}
				else{
					break;
				}
				numEdges++;
			}
		}
		U.pop();
	}

	// Backtracing.
	// Nur, wenn ein Pfad existiert...
	if(min_path_length != numeric_limits<unsigned int>::max()){
		// ...gehe von dem Treffpunkt jeweils zurück zum Anfangsknoten
		// und speichere die Kanten-IDs.
		for(int i=0; i<2; i++){
			unsigned int tmpid = min_path_center;
			while(tmpid != node_id[i]){
//				cout << tmpid << endl;
				int takenedge = found_by[i][tmpid];
				// path->push_front(takenedge);
				g->addEdgeLoad(takenedge);
				tmpid = g->getEdge(i, (unsigned int)takenedge)->other_node;
			}
		}
	}

	// cout << "numNodes: " << numNodes << endl;
	// cout << "numEdges: " << numEdges << endl;

	return min_path_length;
}

void CHDijkstra(SCGraph* g, unsigned int node_id0, vector<unsigned int>* targets){
	// Von den targets alle aufsteigenden Kanten besuchen und markieren.
	/* !TODO! wenn die CH fertig ist, gibt es
	 *
	 * g->getEdgeCount() + g->getShortcutCount()
	 * viele Kanten insgesammt, die maximale Edge-ID ist
	 * (g->getEdgeCount() + g->getShortcutCount()) -1
	 */
	vector<unsigned int> marked(g->getEdgeCount() + g->getShortcutCount(), false);
	markAscEdges(g, targets, &marked);

	// Von node_id0 aus einen "normalen" Dijkstra machen und dabei aufsteigende
	// und markierte Kanten benutzen. Sobald wir alle targets gefunden haben
	// brechen wir ab.
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	unsigned int nr_of_nodes = g->getNodeCount();
	unsigned int tmpnode;
	// Ist -1 wenn der Knoten noch nicht gefunden wurde, sonst steht die
	// id des Knotens drin, von dem er gefunden wurde.
	vector<int> found_by(nr_of_nodes,-1);
	// TODO zum Testen ist hier noch ein dist-vector drin.
	vector<unsigned int> dist(nr_of_nodes);

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	for(unsigned int i=0; i<targets->size(); i++){
		unsigned int target = (*targets)[i];
		if(found_by[target] == -1){
			while(!U.empty() && U.top().id != target){
				// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
				tmpnode = U.top().id;
				if(found_by[tmpnode] == -1){
					dist[tmpnode] = U.top().distance;
					found_by[tmpnode] = (int)U.top().eid;
					// Die ausgehenden Kanten durchgehen und wenn sie aufwärts gehen oder
					// markiert sind auf ihnen weitersuchen.
					EdgesIterator it = g->getOutEdgesIt(tmpnode);
					while(it.hasNext()){
						Edge* tmpedge = it.getNext();
						// Wenn der Knoten noch nicht gefunden wurde UND (sein Level größer ist
						// ODER die Kante markiert ist).
						if(found_by[tmpedge->other_node] == -1
								&& (tmpedge->other_lvl > g->getNodeLVL(tmpnode) || marked[tmpedge->id])){
							// ...tu sie in U
							U.push(U_element(
										tmpedge->value+U.top().distance,tmpedge->other_node,tmpedge->id));
						}
					}
				}
				U.pop();
			}
		}
		int tmpfoundby = -1;
		// Die Distanz des Knotens setzen, je nachdem ob er...
		if(found_by[target] != -1){
			//...schon gefunden wurde...
			(*targets)[i] = dist[target];
			tmpfoundby = found_by[target];
		}
		else{
			if(!U.empty() && U.top().id == target){
				(*targets)[i] = U.top().distance;
				tmpfoundby = (int)U.top().eid;
				//...erst gerade gefunden wurde...
				if(target == targets->back()){
					dist[target] = U.top().distance;
					found_by[target] = (int)U.top().eid;
				}
			}
			else{
				//...oder garnicht gefunden wurde.
				(*targets)[i] = numeric_limits<unsigned int>::max();
				dist[target] = numeric_limits<unsigned int>::max();
			}
		}
		// Backtracing der Knoten.
		if((*targets)[i] != numeric_limits<unsigned int>::max()){
			cout << "=== Node " << target << " ===" << endl;
			// ersten Knoten
			tmpnode = target;
			if(tmpnode != node_id0){
				cout << tmpnode << endl;
				int takenedge = tmpfoundby;
				// path->push_front(takenedge);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
			// restlichen Knoten
			while(tmpnode != node_id0){
				cout << tmpnode << endl;
				int takenedge = found_by[tmpnode];
				// path->push_front(takenedge);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
		}
	}
}

void markAscEdges(SCGraph* g, vector<unsigned int>* nodes, vector<unsigned int>* marked){
	vector<unsigned int> todo;
	// Erstmal alle Startknoten einfürgen. TODO man könnte auch
	// vllt direkt den nodes Vektor benutzen, je nach Implementierung
	// des Rests.
	// TODO Der marked Vektor muss im Moment noch komplett mit false initialisiert
	// übergeben werden.
	todo.assign(nodes->begin(), nodes->end());
	while(!todo.empty()){
		unsigned int tmpnode = todo.back();todo.pop_back();
		EdgesIterator it = g->getInEdgesIt(tmpnode);
		while(it.hasNext()){
			Edge* tmpedge = it.getNext();
			// Wenn wir nicht schon hier waren und es nach oben geht.
			if(!((*marked)[tmpedge->id]) && tmpedge->other_lvl > g->getNodeLVL(tmpnode)){
				(*marked)[tmpedge->id] = true;
				todo.push_back(tmpedge->other_node);
			}
		}
	}
}

bool CHDijkstraTest(Graph* g, SCGraph* scg, unsigned int maxid){
	clock_t start,finish;
	double time1 = 0;
	double time2 = 0;
	unsigned int numDij = 0;//g->getNodeCount();
	CHDijkstras chd(scg);
	cout << "Starte Dijkstras von Knoten 0 aus." << endl;
	unsigned int nc = scg->getNodeCount();
//	for(unsigned int i=0; i< nc; i += 5000){
//		start = clock();
//		unsigned int val1 = CHDijkstra(scg, 0, i);
//		finish = clock();
//		time1 += (double(finish)-double(start))/CLOCKS_PER_SEC;
//		/*if(CHDijkstra(scg, 0, i) != Dijkstra(g, 0, i)){
//			cout << "ERROR!" << endl;
//		//	cout << "CHDijkstra Distanz: " << CHDijkstra(scg, 0, i) << ", Dijkstra Distanz: " << endl;
//		// Dijkstra(g, 0, i) << endl;
//		// CHDijkstra(scg, 0, i);
//		}*/
//		//vector<unsigned int> tgt(1,i);
//		start = clock();
//		unsigned int val2 = chd.oneToOne(0, i);
//		finish = clock();
//		time2 += (double(finish)-double(start))/CLOCKS_PER_SEC;
//		numDij++;
//		if(val1 != val2){
//			cout << "Error" << endl;
//		}
//	}
	unsigned int val2 = 0;
	for(unsigned int i=0; i< nc; i += 50){
		unsigned int s = (42*i + 23) % g->getNodeCount();
		start = clock();
		val2 += chd.oneToOne(s, i);
		finish = clock();
		time2 += (double(finish)-double(start))/CLOCKS_PER_SEC;
		numDij++;
	}
	cout << "Zeit insgesamt für " << numDij << " Dijkstras mit Klasse: " << time2 << endl;
	cout << "Zeit pro Dijkstra mit Klasse im Schnitt: " << time2/numDij << endl;
	cout << val2 << endl;
	return true;
}

#ifndef ch_h
#define ch_h

#include <queue>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <iostream>

#include "graph.h"
#include "structs.h"
#include "CHConstruction.h"


class CH {
	private:
		Graph* g;
		SCGraph* scg;

		CHConstruction<SCGraph> algos;

		unsigned int rounds;

	public:
		CH(Graph* gr, SCGraph* scgr);


};




// ===============================================

// struct für die Elemente aus U incl. Vergleich
struct S_element{
	unsigned int distance;
	unsigned int id; 
	unsigned int eid;

	S_element(unsigned int d, unsigned int i, unsigned int ei)
		:distance(d),id(i),eid(ei){}   

	S_element()
		:distance(0),id(0),eid(0){}   
};  

// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
class Compare_S_element{
	public:
		bool operator()(S_element& u1, S_element& u2){
			return u1.distance > u2.distance;
		}   
};



void Dijkstra_plus(Graph* g, unsigned int node_id);










/*
 * Dijkstra Klasse zum experimentieren
 *
 * die Idee war, dass wir gewissen Strukturen
 * nur zu reseten brauchen,
 * statt sie neu allokieren zu lassen
 *
 * stellt sich raus, das wrappen in der Klasse ist langsamer,
 * wahrscheinlich, weil die lokalität der jeweils laufenden Dijkstra-Funktion
 * durch die Klasse kaputt gemacht wird. Aber nur ein wenig. 
 *
 * Zumindest auf dem Deutschland-Graph.
 *	
 *	Wäre eine Dijkstra Klasse in anderen Situationen trotzdem sinnvoller als reine
 *	Dijkstra-Funktionen?
 *
 */
template <typename G>
class DijkstraC {
	//==========================================
	struct S_elementA{
		unsigned int distance;
		unsigned int id; 
		//unsigned int eid;
	
		S_elementA(unsigned int d, unsigned int i)
			:distance(d),id(i){}//,eid(ei){}   
	
		S_elementA()
			:distance(0),id(0){}   
	};  
	
	// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
	class Compare_S_elementA{
		public:
			bool operator()(S_elementA& u1, S_elementA& u2){
				return u1.distance > u2.distance;
			}   
	};
	//==========================================

	private:
		G* g;

		unsigned int nr_of_nodes;
		bitvec found;
		uintvec dist;
		uintvec in_edge_id;
		std::priority_queue<S_elementA, std::vector<S_elementA>, Compare_S_elementA> U;
			Edge* currentEdge;
			unsigned int tmpid;
			unsigned int tmpdist;

	public:
		DijkstraC() : nr_of_nodes(0), found(), dist(), in_edge_id(){}
		DijkstraC(G* gr) : g(gr), nr_of_nodes(gr->getNodeCount()), found(gr->getNodeCount()), dist(gr->getNodeCount()), in_edge_id(gr->getNodeCount()) {
			
		}
		~DijkstraC(){}

		void run(unsigned int node_id){
			EdgesIterator it = g->getOutEdgesIt(node_id);
		
			U = std::priority_queue<S_elementA, std::vector<S_elementA>, Compare_S_elementA>();
			
			found.reset();
			//in_edge_id.reset();
		
			// Den ersten Knoten abarbeiten
			dist[node_id] = 0;
			found.set(node_id);
			while(it.hasNext()){
				currentEdge =  it.getNext();
				U.push(S_elementA(currentEdge->value,currentEdge->other_node));
			}
		
			// Die restlichen Knoten abarbeiten
			while(!U.empty()){
				// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
				tmpid = U.top().id;
				tmpdist = U.top().distance;
				if(!found.get(tmpid)){
					dist[tmpid] = tmpdist;
					found.set(tmpid);
					//in_edge_id[tmpid] = ue.eid;
					// Die ausgehenden Kanten durchgehen und in U werfen
					it = g->getOutEdgesIt(tmpid);
					while(it.hasNext()){
						currentEdge =  it.getNext();
						// Wenn sie noch nicht gefunden wurde...
						if(!found.get(currentEdge->other_node)){
							// ...tu sie in U
							U.push(S_elementA(
										currentEdge->value+tmpdist,currentEdge->other_node));
						}
					}
				}
				U.pop();
			}
		}
		
};








/*
 * template-Dijkstra für CHs
 */
template <typename G>
void TDijkstra(G* g, unsigned int node_id){
	// Iterator für die ausgehenden Kanten eines Knotens
	EdgesIterator it = g->getOutEdgesIt(node_id);
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<S_element, std::vector<S_element>, Compare_S_element> U;

	Edge* currentEdge;

	unsigned int nr_of_nodes = g->getNodeCount();
	std::vector<bool> found(nr_of_nodes,false);

	std::vector<unsigned int> dist(nr_of_nodes,std::numeric_limits<unsigned int>::max());
	std::vector<unsigned int> in_edge_id(nr_of_nodes);

	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found[node_id] = true;
	while(it.hasNext()){
		currentEdge = it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(S_element(currentEdge->value,currentEdge->other_node,currentEdge->id));
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
					U.push(S_element(
								currentEdge->value+tmpdist,currentEdge->other_node,currentEdge->id));
				}
			}
		}
		U.pop();
	}
}






























#endif

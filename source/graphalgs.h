#ifndef graphalgs_h
#define graphalgs_h

#include "graph.h"
#include <queue>
#include <limits>
#include <list>
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

/*
 * Dijkstra mit Iterator.
 */
void Dijkstra(Graph* g, unsigned int node_id);
bool Dijkstra(Graph* g, unsigned int start_node_id,
		unsigned int end_node_id, unsigned int over_node_id);
void Dijkstra_plus(Graph* g, unsigned int node_id);

/*
 * Dijkstra mit Iterator, welcher sobal er node_id1 gefunden hat abbricht.
 */
unsigned int Dijkstra(Graph* g, unsigned int node_id0, unsigned int node_id1);

/*
 * Dijkstra, welcher direkt auf den Graphstrukturen arbeitet.
 */
void DirectDijkstra(Graph* g, unsigned int node_id);

/*
 * Dijkstra, welcher direkt auf den Graphstrukturen arbeitet und abbricht sobald er
 * node_id1 gefunden hat.
 */
unsigned int DirectDijkstra(Graph* g, unsigned int node_id0, unsigned int node_id1);

/*
 * Ein Bidirektionaler Dijkstra, welcher mit Iterator arbeitet.
 *
 * @return Gibt die minimale Distanz zwischen node_id0 und node_id1 zurück.
 */
unsigned int BiDijkstra(Graph* g, unsigned int node_id0, unsigned int node_id1);

/*
 * Maximal Independent Set Funktionen
 */
void independent_set_test(Graph* g, list<unsigned int> solution);
list<unsigned int> independent_set(Graph* g);


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
	//==========================================

	private:
		G* g;

		unsigned int nr_of_nodes;
		bitvec found;
		uintvec dist;
		uintvec in_edge_id;
		std::priority_queue<U_elementA, std::vector<U_elementA>, Compare_U_elementA> U;
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
		
			U = std::priority_queue<U_elementA, std::vector<U_elementA>, Compare_U_elementA>();
			
			found.reset();
			//in_edge_id.reset();
		
			// Den ersten Knoten abarbeiten
			dist[node_id] = 0;
			found.set(node_id);
			while(it.hasNext()){
				currentEdge =  it.getNext();
				U.push(U_elementA(currentEdge->value,currentEdge->other_node));
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
							U.push(U_elementA(
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
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;

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
}










#endif

#include "ch.h"



















// ==========================================================

/*
 * !! 
 *		experimenteller Dijkstra 
 *		mit Optimierungen 
 *	!!
 */
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
	std::priority_queue<S_elementA, std::vector<S_elementA>, Compare_S_elementA> U;

	// Den ersten Knoten abarbeiten
	dist[node_id] = 0;
	found.set(node_id);// = true;
	while(it.hasNext()){
		currentEdge =  it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(S_elementA(currentEdge->value,currentEdge->other_node));
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
					U.push(S_elementA(
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



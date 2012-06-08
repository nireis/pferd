#ifndef graphalgs_h
#define graphalgs_h

#include "graph.h"
#include "chdijkstra.h"
#include "dijkstra.h"
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
void DijkstraFunc(Graph* g, unsigned int node_id);
bool DijkstraFunc(Graph* g, unsigned int start_node_id,
		unsigned int end_node_id, unsigned int over_node_id);

/*
 * Dijkstra mit Iterator, welcher sobal er node_id1 gefunden hat abbricht.
 */
unsigned int DijkstraFunc(Graph* g, unsigned int node_id0, unsigned int node_id1);

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
 * One to one Dijkstra auf der CH.
 */
unsigned int CHDijkstraFunc(SCGraph* g, unsigned int node_id0, unsigned int node_id1);

/*
 * One to many Dijkstra auf der CH. Die Distanzen werden in den targets-vector geschrieben
 * and die entsprechende Stelle.
 */
void CHDijkstraFunc(SCGraph* g, unsigned int node_id0, vector<unsigned int>* targets);

/*
 * Markiert alle aufsteigenden Kanten von nodes aus.
 */
void markAscEdges(SCGraph* g, vector<unsigned int>* nodes, vector<unsigned int>* marked);

/*
 * Test für den CHDijkstra.
 */
bool CHDijkstraTest(Graph* g, SCGraph* scg, unsigned int maxnode);

#endif

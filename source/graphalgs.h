#ifndef graphalgs_h
#define graphalgs_h

#include "graph.h"

/*
 * Dijkstra mit Iterator.
 */
void Dijkstra(Graph* g, unsigned int node_id);
void Dijkstra2(Graph2* g, unsigned int node_id);
void Dijkstra2A(Graph2* g, unsigned int node_id);
void Dijkstra_4I(Dijkstra_Interface* g, unsigned int node_id);
bool Dijkstra_4I(Dijkstra_Interface* g, unsigned int start_node_id,
		unsigned int end_node_id, unsigned int over_node_id);

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

#endif

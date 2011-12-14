#ifndef graphalgs_h
#define graphalgs_h

#include "graph.h"

/*
 * Dijkstra mit Iterator.
 */
void Dijkstra(Graph* g, unsigned int node_id);

/*
 * Dijkstra, welcher direkt auf den Graphstrukturen arbeitet.
 */
void DirectDijkstra(Graph* g, unsigned int node_id);

/*
 * Ein Bidirektionaler Dijkstra, welcher mit Iterator arbeitet.
 */
void BiDijkstra(Graph* g, unsigned int node_id1, unsigned int node_id2);

#endif

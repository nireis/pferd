#include "graph.h"

template <typename E, typename N, typename S>
Graph<E, N, S>::Graph(){
	node_count = 0;
	edge_count = 0;
	nodes = 0;
	edges = 0;
	in_edges = 0;
	out_edges = 0;
	in_shortcuts = 0;
	out_shortcuts = 0;
}

template <typename E, typename N, typename S>
Graph<E, N, S>::Graph(unsigned int nc, unsigned int ec, // Graph von aussen setzen
		N* n, E* e,
		E** ie, E** oe,
		S** is, S** os){
	node_count = nc;
	edge_count = ec;
	nodes = n;
	edges = e;
	in_edges = ie;
	out_edges = oe;
	in_shortcuts = is;
	out_shortcuts = os;
}

template <typename E, typename N, typename S>
Graph<E, N, S>::Graph(unsigned int nc, unsigned int ec, // Graph klein initialisieren
		N* n, E* e){
	node_count = nc;
	edge_count = ec;
	nodes = n;
	edges = e;
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::initOffsets(){
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::initShortcutOffsets(){
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::clearShortcuts(){
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::addShortcut(S& sc){
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::getEdges(unsigned int node_id, E** aedges, int& count){
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::sortEdgesBy(bool sortby){
}

/* methoden implementieren, um:
  * graph zu initialisieren -> offsets setzen
  * shortuts zu verwalten
  * gewichte/auslastung der kanten zu verwalten
  */

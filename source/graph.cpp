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

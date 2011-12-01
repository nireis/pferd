#include "graph.h"

Graph::Graph(){
	node_count = 0;
	edge_count = 0;
	nodes = 0;
	edges = 0;
	in_edges = 0;
	out_edges = 0;
	in_shortcuts = 0;
	out_shortcuts = 0;
}

Graph::Graph(unsigned int nc, unsigned int ec, // Graph von aussen setzen
		Simple_Node* sn, Simple_Edge* se,
		Simple_Edge** ie, Simple_Edge** oe,
		Shortcut** is, Shortcut** os){
	node_count = nc;
	edge_count = ec;
	nodes = sn;
	edges = se;
	in_edges = ie;
	out_edges = oe;
	in_shortcuts = is;
	out_shortcuts = os;
}

Graph::Graph(unsigned int nc, unsigned int ec, // Graph klein initialisieren
		Simple_Node* sn, Simple_Edge* se){
	node_count = nc;
	edge_count = ec;
	nodes = sn;
	edges = se;
}

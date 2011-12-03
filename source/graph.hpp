/*
 * Diese Datei gibt es nur, 
 * damit die graph.h nicht aufgebläht wird.
 * ja nicht irgendwo includieren ausser in die graph.h
 */

#ifndef graph_hpp
#define graph_hpp

template <typename E, typename N, typename S>
Graph<E, N, S>::Graph(){
	node_count = 0;
	edge_count = 0;
	nodes = 0;
	edges = 0;
	in_edges = 0;
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
	in_edges = 0;
	in_shortcuts = 0;
	out_shortcuts = 0;
}

template <typename E, typename N, typename S>
Graph<E, N, S>::~Graph(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] in_shortcuts; in_shortcuts = 0;
	delete[] out_shortcuts; out_shortcuts = 0;
	shortcuts.clear();
}

template <typename E, typename N, typename S>
/*  initOffsets()
 *
 *  richtet die offsets der nodes ein so,
 *  dass die outgoing-edges direkt im 'E* edges' array auffindbar sind
 *  über die out_offsets und
 *  die incoming-edges auffindbar sind über pointer in 'E** in_edges'
 *  über die in_offsets in den nodes
 */
void Graph<E, N, S>::initOffsets(){
	if (!edges || !nodes || !node_count || !edge_count)
		return;

	in_edges = new E*[edge_count];

	// voraussetzung, damit das alles funktioniert, ist,
	// dass die kanten den sources nach aufsteigend sortiert sind,
	// so, wie die nodes den IDs nach selbst aufsteigend sortiert sind
	unsigned int number_of_edges;
	unsigned int j = 0;
	
	std::list<E*> edge_pointers_of_incomming_edges_for_nodes[node_count];
	// wenn wir uns die incoming-edges fuer jeden knoten merken,
	// brauchen wir das spaeter nur in die offsets reinzusetzen. 

	for(unsigned int i = 0; i < node_count; i++){ // durchlaufe alle nodes
		
		number_of_edges = 0;
		while(edges[j].source == i){ // zu jedem node zaehle ausgehende kanten
			//merke fuer target node, wer darauf zeigt
			edge_pointers_of_incomming_edges_for_nodes[edges[j].target]
				.push_front( &edges[j] ); 
			// bereite in targets offsets vor
			nodes[ edges[j].target +1 ].in_edge_offset ++; 
			number_of_edges++;
			j++;
		}
		nodes[i+1].out_edge_offset = nodes[i].out_edge_offset 
			+ number_of_edges; //setze offset
	}

	j = 0;
	// nun die incoming edges verteilen
	for(unsigned int i = 0; i < node_count; i++){
		// summiere offsets von vorne, damit die differenzen spaeter stimmen
		nodes[i+1].in_edge_offset = nodes[i+1].in_edge_offset + nodes[i].in_edge_offset;
		while( !edge_pointers_of_incomming_edges_for_nodes[i].empty() ){
			// gib die pointer auf die zugehörigen edges in das array
			in_edges[j] = edge_pointers_of_incomming_edges_for_nodes[i].front();
			edge_pointers_of_incomming_edges_for_nodes[i].pop_front();
			j++;
		}
	}
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::initShortcutOffsets(){
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::clearShortcuts(){
	delete[] in_shortcuts; in_shortcuts = 0;
	delete[] out_shortcuts; out_shortcuts = 0;
	shortcuts.clear();
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::addShortcut(S& sc){
}

// template <typename E, typename N, typename S>
// Graph::OutEdgesOfNode Graph<E, N, S>::getAdjOutEdges(unsigned int node_id){
// 	Graph<E, N, S>::OutEdgesOfNode oe = OutEdgesOfNode(node_id, this);
// 	return oe;
// }
// 
// template <typename E, typename N, typename S>
// Graph::InEdgesOfNode Graph<E, N, S>::getAdjInEdges(unsigned int node_id){
// 	Graph<E, N, S>::InEdgesOfNode ie = InEdgesOfNode(node_id, this);
// 	return ie;
// }

template <typename E, typename N, typename S>
Graph<E, N, S>::AbstractEdgesOfNode::~AbstractEdgesOfNode(){
	//nix zu tun ?
}



template <typename E, typename N, typename S>
Graph<E, N, S>::OutEdgesOfNode::OutEdgesOfNode(){
	this->node_id = 0;
	this->edge_count = 0;
	this->nodes_array_base = 0;
}

template <typename E, typename N, typename S>
Graph<E, N, S>::OutEdgesOfNode::~OutEdgesOfNode(){
	//nix zu tun ?
}

template <typename E, typename N, typename S>
Graph<E, N, S>::OutEdgesOfNode::OutEdgesOfNode(unsigned int node_id, Graph<E, N, S> *g){
	this->node_id = node_id;
	this->edge_count = g->nodes[this->node_id+1].out_edge_offset - g->nodes[this->node_id].out_edge_offset;
	this->nodes_array_base = 0;
	if(this->node_id < g->node_count)
		this->nodes_array_base = & g->edges[g->nodes[this->node_id].out_edge_offset];
}

template <typename E, typename N, typename S>
bool Graph<E, N, S>::OutEdgesOfNode::getEdge(unsigned int edge_id, E& e){
	if(0 < edge_id && edge_id <= this->edge_count){ // user soll keinen blödsinn machen
		e = nodes_array_base[edge_id - 1]; // hier passiert immer ein arbeit... 
		// macht uns das viel langsamer?
		return true;
	} else { // entweder keine edges oder id falsch
		return false;
	}
}




template <typename E, typename N, typename S>
Graph<E, N, S>::InEdgesOfNode::InEdgesOfNode(){
	this->node_id = 0;
	this->edge_count = 0;
	this->nodes_array_base = 0;
}

template <typename E, typename N, typename S>
Graph<E, N, S>::InEdgesOfNode::~InEdgesOfNode(){
	//nix zu tun ?
}

template <typename E, typename N, typename S>
Graph<E, N, S>::InEdgesOfNode::InEdgesOfNode(unsigned int node_id, Graph<E, N, S> *g){
	this->node_id = node_id;
	this->edge_count = g->nodes[node_id+1].in_edge_offset 
		- g->nodes[this->node_id].in_edge_offset;
	this->nodes_array_base = 0;
	if(this->node_id < g->node_count)
		this->nodes_array_base = & g->in_edges[g->nodes[this->node_id].in_edge_offset];
}

template <typename E, typename N, typename S>
bool Graph<E, N, S>::InEdgesOfNode::getEdge(unsigned int edge_id, E& e){
	if(0 < edge_id && edge_id <= this->edge_count){ // user soll keinen blödsinn machen
		e = * nodes_array_base[edge_id - 1]; // hier passiert immer ein arbeit... 
		return true;
	} else { // entweder keine edges oder id falsch
		return false;
	}
}




#endif

/*
 * Diese Datei gibt es nur, 
 * damit die graph.h nicht aufgebläht wird.
 * ja nicht irgendwo includieren ausser in die graph.h
 */

#include <iostream>

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
Edge* Graph<E, N, S>::getEdge(unsigned int id){
	return &edges[id];
}

template <typename E, typename N, typename S>
unsigned int Graph<E, N, S>::getLowerEdgeBound(unsigned int id){
	return nodes[id].out_edge_offset;
}

template <typename E, typename N, typename S>
unsigned int Graph<E, N, S>::getUpperEdgeBound(unsigned int id){
	return nodes[id+1].out_edge_offset;
}

template <typename E, typename N, typename S>
unsigned int Graph<E, N, S>::getNodeCount(){
	return node_count;
}

template <typename E, typename N, typename S>
unsigned int Graph<E, N, S>::getEdgeCount(){
	return edge_count;
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


// #include <iostream>
// 	std::cout << "Kurz inne halten: nun sind alle Datenstrukturen" << std::endl;
// 	std::cout<< "für den Graphen initialisiert und müssen nur noch befüllt werden." << std::endl; 
// 	std::cout<< "Schau Dir den Speicherverbrauch an. Taste drücken zum Fortfahren." << std::endl;
// 	std::cin.get();


	// voraussetzung, damit das alles funktioniert, ist,
	// dass die kanten den sources nach aufsteigend sortiert sind,
	// so, wie die nodes den IDs nach selbst aufsteigend sortiert sind

	//TODO: diese Liste eventuell durch eine eigene ersetzen?
	//könnte man bei den shortcuts eh gebrauchen
	
	//hier ein kleiner switch, zum testen von listen
	int switsch = 1;
	if(switsch == 1){
		SList<E*>** pointing_at_income;
		pointing_at_income = new SList<E*>*[node_count];
//	std::cout<< "Listengröße akzeptabel? Nun initialisieren: " << std::endl;
//	std::cin.get();
	
		for(unsigned int i = 0; i < node_count; i++)
			pointing_at_income[i] = new SList<E*>();
//	std::cout<< "Listen wurden angelegt. Nun befüllen: " << std::endl;
//	std::cin.get();
	
		// wenn wir uns die incoming-edges fuer jeden knoten merken,
		// brauchen wir das spaeter nur in die offsets reinzusetzen. 
	
		for(unsigned int j = 0; j < edge_count; j++){
			//merke fuer target node, wer darauf zeigt
			(*pointing_at_income[edges[j].target])
				.push( &edges[j] ); 
			// bereite in targets offsets vor
			nodes[ edges[j].target +1 ].in_edge_offset ++; 
			// bereite in sources offsets vor
			nodes[ edges[j].source +1 ].out_edge_offset ++; 
		}
	
//	std::cout<< "Listen wurden befülllt. Nun zurückschreiben:" << std::endl;
//	std::cin.get();
		unsigned int j = 0;
		// nun die incoming edges verteilen
		for(unsigned int i = 0; i < node_count; i++){
			// summiere offsets von vorne, damit die differenzen spaeter stimmen
			nodes[i+1].in_edge_offset = nodes[i+1].in_edge_offset + nodes[i].in_edge_offset;
			nodes[i+1].out_edge_offset = nodes[i+1].out_edge_offset + nodes[i].out_edge_offset;
			
			while( !(*pointing_at_income[i]).empty() ){
				// gib die pointer auf die zugehörigen edges in das array
				in_edges[j] = (*pointing_at_income[i]).pop();
				j++;
			}
		}
//	std::cout<< "Listen wurden gepopt. Nun löschem:" << std::endl;
//	std::cin.get();
		for(unsigned int it=0; it < node_count; it++){
			(*pointing_at_income[it]).clear();
			delete pointing_at_income[it];
		}
		delete[] pointing_at_income;
	} else {
		std::list<E*>** edge_pointers_of_incomming_edges_for_nodes;
		edge_pointers_of_incomming_edges_for_nodes = new std::list<E*>*[node_count];
	
		for(unsigned int i = 0; i < node_count; i++)
			edge_pointers_of_incomming_edges_for_nodes[i] = new std::list<E*>();
	
		// wenn wir uns die incoming-edges fuer jeden knoten merken,
		// brauchen wir das spaeter nur in die offsets reinzusetzen. 
	
		for(unsigned int j = 0; j < edge_count; j++){
			//merke fuer target node, wer darauf zeigt
			(*edge_pointers_of_incomming_edges_for_nodes[edges[j].target])
				.push_front( &edges[j] ); 
			// bereite in targets offsets vor
			nodes[ edges[j].target +1 ].in_edge_offset ++; 
			// bereite in sources offsets vor
			nodes[ edges[j].source +1 ].out_edge_offset ++; 
		}
	
		unsigned int j = 0;
		// nun die incoming edges verteilen
		for(unsigned int i = 0; i < node_count; i++){
			// summiere offsets von vorne, damit die differenzen spaeter stimmen
			nodes[i+1].in_edge_offset = nodes[i+1].in_edge_offset + nodes[i].in_edge_offset;
			nodes[i+1].out_edge_offset = nodes[i+1].out_edge_offset + nodes[i].out_edge_offset;
			
			while( !(*edge_pointers_of_incomming_edges_for_nodes[i]).empty() ){
				// gib die pointer auf die zugehörigen edges in das array
				in_edges[j] = (*edge_pointers_of_incomming_edges_for_nodes[i]).front();
				(*edge_pointers_of_incomming_edges_for_nodes[i]).pop_front();
				j++;
			}
		}
		for(unsigned int it=0; it < node_count; it++){
			(*edge_pointers_of_incomming_edges_for_nodes[it]).clear();
			delete edge_pointers_of_incomming_edges_for_nodes[it];
		}
		delete[] edge_pointers_of_incomming_edges_for_nodes;
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





#endif

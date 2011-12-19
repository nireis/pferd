#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

Graph::Graph() : 
	node_count(0), 
	nodes_in_offs(0), 
	nodes_out_offs(0),
	node_data(0),

	edge_count(0), 
	out_edges(0), 
	in_edges(0), 
	edge_data(0), 

	shortcut_count(0), 
	out_shortcuts(0), 
	in_shortcuts(0), 
	shortcut_data(0) {
	shortcutlist = SListExt<S>();
}


Graph::~Graph(){
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] node_data; node_data = 0;

	delete[] out_edges; out_edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] edge_data; edge_data = 0;

	delete[] out_shortcuts; out_shortcuts = 0;
	delete[] in_shortcuts; in_shortcuts = 0;
	delete[] shortcut_data; shortcut_data = 0;

	shortcutlist.clear();
}

bool Graph::setGraph(std::string graphdata){
	/*
	 * 1. datei existiert und ist keine von uns erstellte datei => parser aufrufen
	 * 2. datei existiert und ist von uns geschriebene .grp datei => einlesen
	 */

	int s = 1;
	fstream checkfile;
	std::string tmp =  graphdata;

	checkfile.open( tmp.c_str(), ios::in );
	if( ! checkfile.is_open() ){
		checkfile.close();
		cout << "Angegebene Datei nicht gefunden: " << tmp << endl;
		return false;
	} 

	tmp =  graphdata;

	if( tmp.erase(0, tmp.size()-4) == ".grp" ){
		s = 2;
		cout << "Lese aus Binärdatei: " << graphdata<< endl;
	} else {
		s = 1;
		cout << "Parse Textdatei: " << graphdata << endl;
	}

	switch(s){
		case 1: 
	{
			// parser case
			parser p = parser(graphdata);
		
			node_count = p.getNodeCount();
			edge_count = p.getEdgeCount();
		
			nodes_in_offs = new N[node_count + 1];
			nodes_out_offs = new N[node_count + 1];
			node_data = new ND[node_count];
		
			out_edges = new E[edge_count];
			in_edges = new E[edge_count];
			edge_data = new ED[edge_count];
		
			ParserNode* nodes = new ParserNode[node_count];
			ParserEdge* edges = new ParserEdge[edge_count];
		
			p.getNodesAndEdges(nodes, edges);
		
			// nodes/edges verarbeiten
			for(unsigned int i = 0; i < edge_count; i++){
				nodes_in_offs[ edges[i].target +1 ].edge_offset++ ;
				nodes_out_offs[ edges[i].source +1 ].edge_offset++ ;
			}
			for(unsigned int i = 0; i < node_count; i++){
				nodes_in_offs[ i +1 ].edge_offset = 
					nodes_in_offs[ i +1 ].edge_offset + nodes_in_offs[ i ].edge_offset;
				nodes_out_offs[ i +1 ].edge_offset = 
					nodes_out_offs[ i +1 ].edge_offset + nodes_out_offs[ i ].edge_offset;
			}
			for(unsigned int i = 0; i < node_count; i++){
				node_data[i] = 
					NodeData( nodes[i].id, nodes[i].elevation, nodes[i].lat, nodes[i].lon );
			}
			for(unsigned int i = 0; i < edge_count; i++){
				out_edges[i] = Edge( i, edges[i].distance, edges[i].target );
				in_edges[i].other_node = node_count;
				edge_data[i] = EdgeData( i, 0, edges[i].distance, edges[i].type, 0 );
			}
			
			for(unsigned int i = 0; i < edge_count; i++){
				unsigned int index = nodes_in_offs[ edges[i].target ].edge_offset;
				while( in_edges[index].other_node != node_count ){
					index++;
				}
				in_edges[index] = Edge( i, edges[i].distance, edges[i].source );
				edge_data[i].in_index = index;
			}

			delete[] nodes;
			delete[] edges;
			
			writeBinaryGraphFile(graphdata);

			return true;
			break;
	}
		case 2:
	{
			return readBinaryGraphFile(graphdata);
			break;
	}
	} // close switch

	return 0;
}

void Graph::writeBinaryGraphFile(std::string graphdata){
	fstream f(graphdata.append(".grp").c_str(), ios::out|ios::binary);
	int i = 42;
	f.write((char*)&i, sizeof(int));

	f.write((char*) &node_count, sizeof(unsigned int));
	f.write((char*) &edge_count, sizeof(unsigned int));

	f.write((char*) nodes_in_offs, (long)(node_count+1) * sizeof(N));
	f.write((char*) nodes_out_offs, (long)(node_count+1) * sizeof(N));
	f.write((char*) node_data, (long)node_count * sizeof(ND));

	f.write((char*) in_edges, (long)edge_count * sizeof(E));
	f.write((char*) out_edges, (long)edge_count * sizeof(E));
	f.write((char*) edge_data, (long)edge_count * sizeof(ED));

	f.close();
	cout << "Graph-Binärdatei geschrieben: " << graphdata << endl;
}
bool Graph::readBinaryGraphFile(std::string graphdata){
	fstream f(graphdata.c_str(), ios::in|ios::binary);
	
	int i;
	f.read((char*) &i, sizeof(int));
	if(i!=42){
		f.close();
		return false;
	} else {
		f.read((char*) &node_count, sizeof(unsigned int));
		f.read((char*) &edge_count, sizeof(unsigned int));

		nodes_in_offs = new N[node_count+1];
		nodes_out_offs = new N[node_count+1];
		node_data = new ND[node_count];

		in_edges = new E[edge_count];
		out_edges = new E[edge_count];
		edge_data = new ED[edge_count];

		f.read((char*) nodes_in_offs, (long)(node_count+1) * sizeof(N));
		f.read((char*) nodes_out_offs, (long)(node_count+1) * sizeof(N));
		f.read((char*) node_data, (long)node_count * sizeof(ND));
	
		f.read((char*) in_edges, (long)edge_count * sizeof(E));
		f.read((char*) out_edges, (long)edge_count * sizeof(E));
		f.read((char*) edge_data, (long)edge_count * sizeof(ED));
		
		f.close();
		return true;
	}
}


void Graph::initShortcutOffsets(){
//	if(shortcuts != 0){
//		for(unsigned int i = 0; i < node_count; i++){
//			nodes[ i ].out_shortcut_offset = 0;
//			nodes[ i ].in_shortcut_offset = 0;
//		}
//	}
//	if(in_shortcuts != 0)
//		delete[] in_shortcuts;
//
//	if(shortcuts != 0)
//		delete[] shortcuts;
//
//	shortcut_count = shortcutlist.size();
//	shortcuts = new S[shortcut_count];
//	in_shortcuts = new S*[shortcut_count];
//
//	for(unsigned int i = 0; i < shortcut_count; i++){
//		in_shortcuts[i] = 0;
//	}
//
//	SListExt<S>::Iterator it = shortcutlist.getIterator();
//
//	S s;
//	unsigned int j = 0;
//	// bereits offsets und in_shortcuts vor
//	while( it.hasNext() ){
//		j++;
//		s = it.getNext();
//		nodes[ s.source +1 ].out_shortcut_offset++;
//		nodes[ s.target +1 ].in_shortcut_offset++;
//	}
//	// setze offsets korrekt
//	for(unsigned int i = 0; i < node_count; i++){
//		nodes[i+1].in_shortcut_offset 
//			= nodes[i+1].in_shortcut_offset + nodes[i].in_shortcut_offset;
//		nodes[i+1].out_shortcut_offset 
//			= nodes[i+1].out_shortcut_offset + nodes[i].out_shortcut_offset;
//	}
//	// trage shortcuts in das array für ausgehende sc ein / umsetzung von liste auf array	
//	while( !shortcutlist.empty() ){
//		j = 0;
//		s = shortcutlist.pop();
//		while( shortcuts[ nodes [ s.source ].out_shortcut_offset + j].id != 0 ){
//			j++;
//		}
//		s.id = 1;
//		shortcuts[ nodes [ s.source ].out_shortcut_offset + j] 
//			= s;
//	}
//	// trage in_shortcuts ein 
//	for(unsigned int i = 0; i < shortcut_count; i++){
//		j = 0;
//		shortcuts[i].id = edge_count + i + 1; //TODO nochmal überdenken
//		// dies wird uns später arbeit sparen
//		// hierdurch sind shortcuts (fast) normale edges,
//		// die wir sofort erkennen können
//
//		// suche dir das Offset für in_edges im target, 
//		// suche in diesem bereich einen noch leeren eintrag,
//		// trage dort die entsprechende kante ein
//		while( in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] != 0 ){
//			j++;
//		}
//		in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] 
//			= & shortcuts[i];
//	}
}

/* 
 * siehe oben, nur bekommen wir eine
 * nach source-knoten aufsteigend sortiertes
 * array von shortcuts
 *
 * auch hier löschen wir alle bisher angelegten shortcuts
 */
void Graph::initShortcutOffsets(S* scarray, unsigned int scc){
//	if(shortcuts != 0){
//		for(unsigned int i = 0; i < node_count; i++){
//			nodes[ i ].out_shortcut_offset = 0;
//			nodes[ i ].in_shortcut_offset = 0;
//		}
//	}
//	if(in_shortcuts != 0)
//		delete[] in_shortcuts;
//
//	if(shortcuts != 0)
//		delete[] shortcuts;
//
//	shortcut_count = scc;
//	shortcuts = scarray;
//	in_shortcuts = new S*[shortcut_count];
//
//	for(unsigned int i = 0; i < node_count; i++){
//		nodes[ i ].out_shortcut_offset = 0;
//		nodes[ i ].in_shortcut_offset = 0;
//	}
//
//	// bereits offsets und in_shortcuts vor
//	for(unsigned int i = 0; i < shortcut_count; i++){
//		in_shortcuts[i] = 0;
//		nodes[ shortcuts[i].source +1 ].out_shortcut_offset++;
//		nodes[ shortcuts[i].target +1 ].in_shortcut_offset++;
//	}
//	// setze offsets korrekt
//	for(unsigned int i = 0; i < node_count; i++){
//		nodes[i+1].in_shortcut_offset 
//			= nodes[i+1].in_shortcut_offset + nodes[i].in_shortcut_offset;
//		nodes[i+1].out_shortcut_offset 
//			= nodes[i+1].out_shortcut_offset + nodes[i].out_shortcut_offset;
//	}
//	unsigned int j = 0;
//	for(unsigned int i = 0; i < shortcut_count; i++){
//		j = 0;
//		shortcuts[i].id = edge_count + i + 1; //TODO nochmal überdenken
//		// suche in diesem bereich einen noch leeren eintrag,
//		// trage dort die entsprechende kante ein
//		while( in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] != 0 ){
//			j++;
//		}
//		in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] 
//			= & shortcuts[i];
//	}
}

void Graph::clearShortcuts(){
	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_offs[ i ].shortcut_offset = 0;
	}
	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_offs[ i ].shortcut_offset = 0;
	}
	delete[] shortcut_data; shortcut_data = 0;
	delete[] in_shortcuts; in_shortcuts = 0;
	delete[] out_shortcuts; out_shortcuts = 0;
	shortcut_count = 0;
	shortcutlist.clear();
}

void Graph::addShortcut(S sc){
	shortcutlist.push(sc);
}


unsigned int Graph::getNodeCount(){
	return node_count;
}
unsigned int Graph::getEdgeCount(){
	return edge_count;
}
unsigned int Graph::getShortcutCount(){
	return shortcut_count;
}
ND Graph::getNodeData(unsigned int id){
	return node_data[id];
}
ED Graph::getEdgeData(unsigned int id){
	return edge_data[id];
}
SD Graph::getShortcutData(unsigned int id){
	return shortcut_data[id]; // TODO indexprüfung 
}

// Andres stuff
unsigned int Graph::getLowerOutEdgeBound(unsigned int id){
	return nodes_out_offs[id].edge_offset;
}
unsigned int Graph::getUpperOutEdgeBound(unsigned int id){
	return nodes_out_offs[id+1].edge_offset;
}
unsigned int Graph::getLowerInEdgeBound(unsigned int id){
	return nodes_in_offs[id].edge_offset;
}
unsigned int Graph::getUpperInEdgeBound(unsigned int id){
	return nodes_in_offs[id+1].edge_offset;
}
//TODO andres stuff
E* Graph::getOutEdge(unsigned int id){
	if(id < edge_count)
		return & out_edges[id ];

	E* e=0;
	return e;
}
E* Graph::getInEdge(unsigned int id){
	if(id < edge_count)
		return & in_edges[ edge_data[id].in_index ];

	E* e=0;
	return e;
}


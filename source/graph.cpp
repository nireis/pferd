#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

Graph::Graph() : 
	is_set(0),
	BinID(43),
	node_count(0), 
	nodes_in_offs(0), 
	nodes_out_offs(0),
	node_data(0),

	edge_count(0), 
	out_edges(0), 
	in_edges(0), 
	edge_data(0) {
}


Graph::~Graph(){
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] node_data; node_data = 0;

	delete[] out_edges; out_edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] edge_data; edge_data = 0;
}

bool Graph::setGraph(std::string graphdata){
	if( is_set )
		return false;

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
	checkfile.close();

	tmp =  graphdata;

	if( tmp.erase(0, tmp.size()-4) == ".grp"){
		tmp =  graphdata;
		checkfile.open( tmp.c_str(), ios::in|ios::binary);
		int check;
		checkfile.read((char*) &check, sizeof(int));
		if(check!=BinID){
			s = 1;
			//cout << "check: " << check << ", " << "binid : " << BinID << endl;
			graphdata.erase( graphdata.size() - 4, graphdata.size() ) ;
			cout << "Parse Textdatei: " << graphdata << endl;
		} else {
			s = 2;
			//cout << "check: " << check << ", " << "binid : " << BinID << endl;
			cout << "Lese aus Binärdatei: " << graphdata<< endl;
		}
		checkfile.close();
	} else {
		s = 1;
		cout << "Parse Textdatei: " << graphdata << endl;
	}

	switch(s){
		case 1: 
	{
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
			for(unsigned int i = 0; i <= node_count; i++){
				nodes_in_offs[ i ] = 0 ;
				nodes_out_offs[ i ] = 0 ;
			}
			for(unsigned int i = 0; i < edge_count; i++){
				nodes_in_offs[ edges[i].target +1 ] ++ ;
				nodes_out_offs[ edges[i].source +1 ] ++ ;
			}
			for(unsigned int i = 0; i < node_count; i++){
				nodes_in_offs[ i +1 ] = 
					nodes_in_offs[ i +1 ] + nodes_in_offs[ i ] ;
				nodes_out_offs[ i +1 ] = 
					nodes_out_offs[ i +1 ] + nodes_out_offs[ i ] ;
			}
			for(unsigned int i = 0; i < node_count; i++){
				node_data[i] = 
					NodeData( nodes[i].id, nodes[i].elevation, nodes[i].lat, nodes[i].lon );
			}
			for(unsigned int i = 0; i < edge_count; i++){
				out_edges[i] = E( i, edges[i].distance, edges[i].target );
				in_edges[i].other_node = node_count;
				edge_data[i] = ED( i, 0, edges[i].distance, edges[i].type, 0 );
			}
			
			for(unsigned int i = 0; i < edge_count; i++){
				unsigned int index = nodes_in_offs[ edges[i].target ] ;
				while( in_edges[index].other_node != node_count ){
					index++;
				}
				in_edges[index] = E( i, edges[i].distance, edges[i].source );
				edge_data[i].in_index = index;
			}

			delete[] nodes; nodes = 0;
			delete[] edges; edges = 0;
			
			// TODO optional machen, ob binärdatei
			// geschrieben wird
			writeBinaryGraphFile(graphdata);

			is_set = 1;
			return true;
			break;
	}
		case 2:
	{
			is_set = 1;
			return readBinaryGraphFile(graphdata);
			break;
	}
	} // close switch

	return 0;
}

void Graph::writeBinaryGraphFile(std::string graphdata){
	fstream f(graphdata.append(".grp").c_str(), ios::out|ios::binary);
	
	f.write((char*) &BinID, sizeof(int));

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
	if(i!=BinID){
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

unsigned int Graph::getNodeCount(){
	return node_count;
}
unsigned int Graph::getEdgeCount(){
	return edge_count;
}

ND Graph::getNodeData(unsigned int id){
	if(id < node_count)
		return node_data[id];

	ND nd;
	return nd;
}
ED Graph::getEdgeData(unsigned int id){
	if(id < edge_count)
		return edge_data[id];
	
	ED ed;
	return ed;
}

/* 
 * keine Indexprüfung !
 */
unsigned int Graph::getLowerOutEdgeBound(unsigned int id){
	return nodes_out_offs[id] ;
}
unsigned int Graph::getUpperOutEdgeBound(unsigned int id){
	return nodes_out_offs[id+1] ;
}
unsigned int Graph::getLowerInEdgeBound(unsigned int id){
	return nodes_in_offs[id] ;
}
unsigned int Graph::getUpperInEdgeBound(unsigned int id){
	return nodes_in_offs[id+1] ;
}

// andres stuff
// später weg, oder behalten wir das arbeiten
// direkt auf dem Graph?
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


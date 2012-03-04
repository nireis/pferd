#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

Graph::Graph() : 
	is_set(0),
	node_count(0), 
	edge_count(0), 
	nodes_in_offs(0), 
	in_edges(0), 
	nodes_out_offs(0),
	out_edges(0), 
	node_data(0),
	edge_data(0) {
}
const int Graph::BinID = 45;
const std::string Graph::dateiendung = "grp";


Graph::~Graph(){
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] node_data; node_data = 0;

	delete[] out_edges; out_edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] edge_data; edge_data = 0;
}

bool Graph::setGraph(std::string graphdata, bool write_binary){
	if( is_set )
		return false;

	int s = checkDataFile(graphdata);
	int ss;
	int r = 0;
	string tmp;
	switch( s )
	{
		case -1:
			tmp = graphdata;
			tmp.erase(tmp.size()-4, tmp.size());
			ss = checkDataFile(tmp);
			if(ss == 2){
				cout << graphdata << " ist veraltete Binärdatei." << endl;
				cout << "Versuche Datei " << tmp << endl;
				r = parseTextGraphFile(tmp);
				if( write_binary ){
					writeBinaryGraphFile(tmp);
				}
			} else {
				cout << graphdata << " ist veraltete Binärdatei." << endl;
				cout << tmp << " Textdatei nicht gefunden." << endl;
				r = 0;
			}
			break;
		case 0:
			cout << graphdata << " Datei existiert nicht oder Dateityp unbekannt." << endl;
			r = 0;
			break;
		case 1:
			r = readBinaryGraphFile(graphdata);
			break;
		case 2:
			r = parseTextGraphFile(graphdata);
			if( write_binary ){
				writeBinaryGraphFile(graphdata);
			}
			break;
	}
	return r;
}

int Graph::checkDataFile(std::string graphdata){
	fstream checkfile;
	std::string tmp =  graphdata;
	std::size_t p;
	p = tmp.find_last_of(".");
	
	int r;
	if( tmp.substr(p+1) == "txt" ){
		r = 2;
	} else 
	if(tmp.substr(p+1) == dateiendung ){
		r = 1;
	} else {
		return 0;
	}

	tmp = graphdata;
	checkfile.open( tmp.c_str(), ios::in );
	if( ! checkfile.is_open() ){
		checkfile.close();
		return 0;
	} else {
		if( r == 1 ){
			int check;
			checkfile.read((char*) &check, sizeof(int));
			if(check!=BinID){
				r = -1;
			}
		}
	}
	checkfile.close();

	return r;
}
void Graph::writeBinaryGraphFile(std::string graphdata){
	fstream f(graphdata.append(".").append(dateiendung).c_str(), ios::out|ios::binary);
	
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
		in_edges = new E[edge_count];
		nodes_out_offs = new N[node_count+1];
		out_edges = new E[edge_count];
		node_data = new ND[node_count];
		edge_data = new ED[edge_count];

		f.read((char*) nodes_in_offs, (long)(node_count+1) * sizeof(N));
		f.read((char*) nodes_out_offs, (long)(node_count+1) * sizeof(N));
		f.read((char*) node_data, (long)node_count * sizeof(ND));
	
		f.read((char*) in_edges, (long)edge_count * sizeof(E));
		f.read((char*) out_edges, (long)edge_count * sizeof(E));
		f.read((char*) edge_data, (long)edge_count * sizeof(ED));
		
		f.close();
		is_set = true;
		return true;
	}
}
bool Graph::parseTextGraphFile(std::string graphdata){
	parser p = parser(graphdata);

	node_count = p.getNodeCount();
	edge_count = p.getEdgeCount();

	nodes_in_offs = new N[node_count + 1];
	in_edges = new E[edge_count];
	nodes_out_offs = new N[node_count + 1];
	out_edges = new E[edge_count];
	node_data = new ND[node_count];
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

	is_set = true;
	return 1;
}

unsigned int Graph::getNodeCount(){
	return node_count;
}
unsigned int Graph::getEdgeCount(){
	return edge_count;
}
unsigned int Graph::getEdgeCount(unsigned int node_id){
        unsigned int in = nodes_in_offs[node_id+1] - nodes_in_offs[node_id];
        unsigned int out = nodes_out_offs[node_id+1] - nodes_out_offs[node_id];
        return in+out;
}

ND Graph::getNodeData(unsigned int id){
//	if(id < node_count)
		return node_data[id];

//	ND nd;
//	return nd;
}
ED Graph::getEdgeData(unsigned int id){
//	if(id < edge_count)
		return edge_data[id];
	
//	ED ed;
//	return ed;
}

E* Graph::getOutEdge(unsigned int id){
//	if(id < edge_count)
		return out_edges + id; //edge_data[id].out_index ;
//	E* e=0;
//	return e;
}
E* Graph::getInEdge(unsigned int id){
//	if(id < edge_count)
		return in_edges + edge_data[id].in_index;
//	E* e=0;
//	return e;
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

/* == Graph ende == */



SCGraph::SCGraph(Graph* gr) : 
	g(gr),
	shortcut_count( 0 ), 
	node_data( gr->getNodeDataPointer() ),
	// shortcut_data( 0 ),
	shortcutlist(),
	round_shortcutlist(),
	round_node_blacklist(),
	free_edge_ids()
{
	// init-daten aus übegebenem Graph übernehmen
	node_count = gr->getNodeCount();
	edge_count = gr->getEdgeCount();
	blacklist.init( node_count );
	blacklist.reset();
	node_lvl = new unsigned int[ node_count ];
	nodes_in_offs = new N[ node_count +1 ];
	nodes_out_offs = new N[ node_count +1 ];
	in_edges = new E[ edge_count ];
	out_edges = new E[ edge_count ];
	node_in_edges_count = new unsigned int[ node_count ];
	node_out_edges_count = new unsigned int[ node_count ];
	edge_data = new ED[ edge_count ];

	for(unsigned int i = 0; i < node_count; i++){
		node_lvl[i] = 0;
	}

	for(unsigned int i = 0; i <= node_count; i++){
		nodes_in_offs[i] = gr->getLowerInEdgeBound(i);
	}

	for(unsigned int i = 0; i <= node_count; i++){
		nodes_out_offs[i] = gr->getLowerOutEdgeBound(i);
	}
	
	for(unsigned int i = 0; i < node_count; i++){
		node_in_edges_count[i] = nodes_in_offs[i+1] - nodes_in_offs[i];
	}

	for(unsigned int i = 0; i < node_count; i++){
		node_out_edges_count[i] = nodes_out_offs[i+1] - nodes_out_offs[i];
	}

	for(unsigned int i = 0; i < edge_count; i++){
		in_edges[i] = * gr->getInEdge(i);
	}

	for(unsigned int i = 0; i < edge_count; i++){
		out_edges[i] = * gr->getOutEdge(i);
	}
	
	for(unsigned int i = 0; i < edge_count; i++){
		edge_data[i] = gr->getEdgeData(i);
	}
}

SCGraph::~SCGraph(){
	g = 0;
	node_data = 0;
	delete[] node_lvl; node_lvl = 0;

	clearEverything();

	shortcutlist.clear();
	free_edge_ids.clear();
	round_shortcutlist.clear();
	round_node_blacklist.clear();
}

void SCGraph::addShortcut(S sc){
	round_shortcutlist.push(sc);
}

void SCGraph::blacklistNode(unsigned int node_id){
	round_node_blacklist.push(node_id);
}

bool SCGraph::isBlack(unsigned int node_id){
	return blacklist.get(node_id);
}

bool SCGraph::mergeRound(unsigned int lvl){
	
	SList<unsigned int>::Iterator it = round_node_blacklist.getIterator();
	// frei werdende edge-ids für shortcuts sichern
	unsigned int scount = round_shortcutlist.size();
	EdgesIterator eit;
	while( scount > 0 ){
		if( it.hasNext() ){
			eit = getInEdgesIt( it.getNext() ) ;
			while( eit.hasNext() && (scount > 0) ){
				free_edge_ids.push( (eit.getNext())->id );
				scount --;
			}
		}
	}
	unsigned int n;
	/* der große manitu mache, dass dieser code das tut, was er soll */

	// markieren und nach hinten schieben ungültiger kanten
	while( ! round_node_blacklist.empty() ){
		n = round_node_blacklist.pop();
		node_lvl[n] = lvl;
		
		// das andere ende der in-edges löschen
		for(unsigned int i = nodes_in_offs[n]; i < nodes_in_offs[n]+node_in_edges_count[n]; i++){
			unsigned int edgeid = in_edges[ i ].id;
		//	if( edgeid >= edge_count ){
		//		edgeid = edgeid - edge_count;
		//	}
			unsigned int other = in_edges[ i ].other_node;
			node_out_edges_count[ other ] --;
			// lezte aktive edge merken
			E tmp = out_edges[ nodes_out_offs[ other ] + node_out_edges_count[ other ] ]; 
			// letzte edge "ungültig" setzen
			out_edges[ nodes_out_offs[ other ] + node_out_edges_count[ other ] ] = E(); 
			// überschreibe auf n zeigende kante
			out_edges[ edge_data[ edgeid ].out_index ] = tmp; 
			// passe index der verschobenen kante an
			edge_data[ tmp.id ].out_index = edge_data[ edgeid ].out_index;
		}
		// das andere ende der out-edges löschen
		for(unsigned int i = nodes_out_offs[n]; i < nodes_out_offs[n] + node_out_edges_count[n]; i++){
			unsigned int edgeid = out_edges[ i ].id;
		//	if( edgeid >= edge_count ){
		//		edgeid = edgeid - edge_count;
		//	}
			unsigned int other = out_edges[ i ].other_node;
			node_in_edges_count[ other ] --;
			E tmp = in_edges[ nodes_in_offs[ other ] + node_in_edges_count[ other ] ];
			in_edges[ nodes_in_offs[ other ] + node_in_edges_count[ other ] ] = E();
			in_edges[ edge_data[ edgeid ].in_index ] = tmp;
			edge_data[ tmp.id ].in_index = edge_data[ edgeid ].in_index;
		}

		node_in_edges_count[n] = 0;
		node_out_edges_count[n] = 0;
		blacklist.set(n);
	}
	
	// shortcuts hinzufügen
	while( !round_shortcutlist.empty() ){
		S stmp = round_shortcutlist.pop();
		unsigned int edgeid = free_edge_ids.pop();
		unsigned int oindex = nodes_out_offs[ stmp.source ] + node_out_edges_count[ stmp.source ];
		unsigned int iindex = nodes_in_offs[ stmp.target ] + node_in_edges_count[ stmp.target ];
		shortcutlist.push(stmp);
		
		in_edges[ iindex ] = E(/*edge_count +*/ edgeid, stmp.value, stmp.source);
		node_in_edges_count[ stmp.target ]++;
	
		out_edges[ oindex ] = E(/*edge_count +*/ edgeid, stmp.value, stmp.target);
		node_out_edges_count[ stmp.source ]++;

		edge_data[ edgeid ] = ED(oindex, iindex, stmp.value, 0, 0);

		shortcut_count++;
	}

	return true; 
}

void SCGraph::clearEverything(){
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] in_edges; in_edges = 0;
	delete[] out_edges; out_edges = 0;
	delete[] node_in_edges_count; node_in_edges_count = 0;
	delete[] node_out_edges_count; node_out_edges_count = 0;
	delete[] edge_data; edge_data = 0;
	free_edge_ids.clear();
	// delete[] shortcut_data; shortcut_data = 0;
}

bool SCGraph::mergeShortcutsAndGraph(){
	if(g == 0)
		return false;

	if( ! g->isSet() )
		return false;
	
	// um fragmentierung ein wenig "vorzubeugen" verlagern wir aus der liste
	// in ein array, um den platz der gelöschten liste dann für neue arrays zu haben
	shortcut_count = shortcutlist.size();
	S* shortcuts = new S[shortcut_count];

	for(unsigned int i=0; i < shortcut_count; i++){
		if( shortcutlist.empty() )
			return false; // TODO falls false, nicht alles wegwerfen

		shortcuts[i] = shortcutlist.pop();
	}
	if( !shortcutlist.empty() )
		return false;

	clearEverything();

	if( !round_shortcutlist.empty() )
		return false;

	if( !round_node_blacklist.empty() )
		return false;

	blacklist.del();
	
	nodes_in_offs = new N[ node_count +1 ];
	in_edges = new E[ edge_count + shortcut_count ](); 
	nodes_out_offs = new N[ node_count +1 ];
	out_edges = new E[ edge_count + shortcut_count ]();

	edge_data = new ED[ edge_count + shortcut_count ];

	for(unsigned int i = 0; i < edge_count; i++){
		edge_data[i] = g->getEdgeData(i);
	}

	// offsets aus dem graph übernehmen
	nodes_in_offs[0] = 0;
	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_offs[i + 1] = 
			g->getUpperInEdgeBound(i) - g->getLowerInEdgeBound(i) ;
	}
	nodes_out_offs[0] = 0;
	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_offs[i + 1] = 
			g->getUpperOutEdgeBound(i) - g->getLowerOutEdgeBound(i) ;
	}

	// shortcuts hinzufügen
	S s;
	for(unsigned int i=0; i < shortcut_count; i++ ){
		s = shortcuts[i];
		nodes_in_offs[ s.target +1 ]++;
		nodes_out_offs[ s.source +1 ]++;
	}
	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_offs[i+1] 
			= nodes_in_offs[i+1] + nodes_in_offs[i];
		nodes_out_offs[i+1] 
			= nodes_out_offs[i+1] + nodes_out_offs[i];
	}
	unsigned int o;
	unsigned int i;
	for(unsigned int c = 0; c < shortcut_count; c++ ){
		s = shortcuts[c];
		o = nodes_out_offs[ s.source ];
		i = nodes_in_offs[ s.target ];
		while( out_edges[ o ].id != 0 ){
			o++;
		}
		while( in_edges[ i ].id != 0 ){
			i++;
		}
		out_edges[ o ] = E(edge_count + c, s.value, s.target);
		in_edges[ i ] = E(edge_count + c, s.value, s.source);
		edge_data[ edge_count + c ] = ED(o, i, s.papa_edge, s.mama_edge, 0);
	}
	// edges verteilen
	EdgesIterator eit;
	for(unsigned int c = 0; c < node_count; c++){
		eit = g->getOutEdgesIt(c);
		o = nodes_out_offs[c + 1] - 1;
		while( eit.hasNext() ){
			out_edges[ o ] = * eit.getNext();
			edge_data[ out_edges[ o ].id ].out_index = o;
			// in edge_data index aktualisieren
			o--;
		}
	}
	for(unsigned int c = 0; c < node_count; c++){
		eit = g->getInEdgesIt(c);
		i = nodes_in_offs[c + 1] - 1;
		while( eit.hasNext() ){
			in_edges[ i ] = * eit.getNext();
			edge_data[ in_edges[ i ].id ].in_index = i;
			i--;
		}
	}
	delete[] shortcuts; shortcuts = 0;

	return true;
}

bool SCGraph::isShortcut(unsigned int edge_id){
	return !(edge_id < edge_count);
}

unsigned int SCGraph::getNodeLVL(unsigned int node_id){
	return node_lvl[node_id];
}

unsigned int SCGraph::getNodeCount(){
	return node_count;
}
unsigned int SCGraph::getEdgeCount(){
	return edge_count;
}
unsigned int SCGraph::getEdgeCount(unsigned int node_id){
	unsigned int in = nodes_in_offs[node_id+1] - nodes_in_offs[node_id];
	unsigned int out = nodes_out_offs[node_id+1] - nodes_out_offs[node_id];
	return in+out;
}
unsigned int SCGraph::getEdgeCount_Round(unsigned int node_id){
	unsigned int in = node_in_edges_count[node_id];
	unsigned int out = node_out_edges_count[node_id];
	return in+out;
}
unsigned int SCGraph::getShortcutCount(){
	return shortcut_count;
}

ND SCGraph::getNodeData(unsigned int node_id){
	return node_data[ node_id ];
}
ED SCGraph::getEdgeData(unsigned int edge_id){
	return edge_data[ edge_id ];
}
//SD SCGraph::getShortcutData(unsigned int id){
//	return shortcut_data[id - edge_count];
//}


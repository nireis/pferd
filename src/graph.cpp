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
const int Graph::BinID = 47;
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
		return in_edges + id; //edge_data[id].in_index;
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
	shortcutlist(),
	round_shortcutlist(),
	round_node_blacklist()
{
	// init-daten aus übegebenem Graph übernehmen
	node_count = gr->getNodeCount();
	edge_count = gr->getEdgeCount();
	current_edge_arrays_size = edge_count;
	node_data = new ND[ node_count ];
	node_lvl = new unsigned int[ node_count ];
	nodes_in_offs = new N[ node_count +1 ];
	nodes_out_offs = new N[ node_count +1 ];
	node_in_edges_count = new unsigned int[ node_count ];
	node_out_edges_count = new unsigned int[ node_count ];
	edge_data = new ED[ edge_count ];
	in_edges = new E[ edge_count ];
	out_edges = new E[ edge_count ];
	goodNodes = new unsigned int[ node_count ];
	goodNodesSize = node_count;

	for(unsigned int i = 0; i < node_count; i++){
		goodNodes[i] = i;
	}

	for(unsigned int i = 0; i < node_count; i++){
		node_lvl[i] = 0;
	}

	// TODO
	for(unsigned int i = 0; i < node_count; i++){
		node_data[i] = gr->getNodeData(i);
		node_data[i].elevation = 0;
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
		edge_data[i] = gr->getEdgeData(i);
	}

	for(unsigned int i = 0; i < edge_count; i++){
		in_edges[i] = * (gr->getInEdge(i));
		in_edges[i].other_lvl = edge_data[ in_edges[i].id ].out_index ;
	}

	for(unsigned int i = 0; i < edge_count; i++){
		out_edges[i] = * (gr->getOutEdge(i));
		out_edges[i].other_lvl = edge_data[ out_edges[i].id ].in_index ;
	}
}

SCGraph::~SCGraph(){
	g = 0;
	delete[] node_data; node_data = 0;
	delete[] node_lvl; node_lvl = 0;

	clearAlmostEverything();

	// shortcutlist.clear();
	while( ! shortcutlist.empty() ){
		ShortcutArray a = shortcutlist.pop();
		delete[] a.array; a.array = 0;
	}
	round_shortcutlist.clear();
	round_node_blacklist.clear();
	delete[] goodNodes; goodNodes = 0;
}

void SCGraph::addShortcut(S sc){
	round_shortcutlist.push_front(sc);
}

void SCGraph::blacklistNode(unsigned int node_id){
	round_node_blacklist.push_front(node_id);
}

void printEdges(EdgesIterator it){
	while( it.hasNext() ){
		Edge* e = it.getNext();
		cout << " ## e-id: " << e->id << " , e-value: " << e->value << " , e-otherLVL: " << e->other_lvl << " , e-otherNODE: " << e->other_node << endl;
	}
}

bool SCGraph::mergeRound(unsigned int lvl){
	unsigned int n;

	goodNodesSize = goodNodesSize - round_node_blacklist.size();

	// nehme kontrahierte knoten raus
	while( ! round_node_blacklist.empty() ){
		n = round_node_blacklist.front();
		node_lvl[n] = lvl;
		node_data[n].elevation = lvl; // TODO

		//schneide andere enden der in-edges ab
		for(unsigned int i = nodes_in_offs[n]; i < nodes_in_offs[n] + node_in_edges_count[n];i++){
			unsigned int othernode = in_edges[i].other_node;
			in_edges[i].other_node = node_count + n;
			unsigned int otherlvl = in_edges[i].other_lvl;

			node_out_edges_count[ othernode ] --;
			
			// tausche letzte gültige edge mit ungültiger
			E lastedge = out_edges[ nodes_out_offs[ othernode ] + node_out_edges_count[ othernode ] ];
			out_edges[ otherlvl ] = lastedge;
			out_edges[ nodes_out_offs[ othernode ] + node_out_edges_count[ othernode ] ].other_node = node_count + othernode;

			// sag partner von getauschter gültiger, wo neuer sitzplatz
			in_edges[ lastedge.other_lvl ].other_lvl = otherlvl;

		}
		//schneide andere enden der out-edges ab
		for(unsigned int i = nodes_out_offs[n]; i < nodes_out_offs[n] + node_out_edges_count[n] ;i++){
			unsigned int othernode = out_edges[i].other_node;
			out_edges[i].other_node = node_count + n;
			unsigned int otherlvl = out_edges[i].other_lvl;

			node_in_edges_count[ othernode ] --;

			E lastedge = in_edges[ nodes_in_offs[ othernode ] + node_in_edges_count[ othernode ] ];
			in_edges[ otherlvl ] = lastedge;
			in_edges[ nodes_in_offs[ othernode ] + node_in_edges_count[ othernode ] ].other_node = node_count + othernode;

			out_edges[ lastedge.other_lvl ].other_lvl = otherlvl;
		}

		//mache kanten von n unerreichbar
		node_in_edges_count[n] = 0;
		node_out_edges_count[n] = 0;
		round_node_blacklist.pop_front();
	}

	unsigned int goodNodesCounter = 0;
	for(unsigned int i = 0; i < node_count; i++){
		if( node_lvl[i] == 0 ){
			goodNodes[ goodNodesCounter ] = i;
			goodNodesCounter++;
		}
	}

	// neue anzahl an kanten zählen
	unsigned int edgesum = 0;
	for(unsigned int i = 0; i < node_count; i++){
		edgesum = edgesum + node_in_edges_count[i];
	}

	unsigned int roundshortcutssize = round_shortcutlist.size();

	unsigned int old_edge_arrays_size = current_edge_arrays_size;

	current_edge_arrays_size = edgesum + roundshortcutssize;
	
	// neue offsets vorbereiten
	E* tmpedges = new E[ current_edge_arrays_size ];

	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_offs[i+1] = node_out_edges_count[i];
	}

	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_offs[i+1] = node_in_edges_count[i];
	}

	// shortcuts umlagern und offsets um shortcuts erweitern
	S* roundshortcuts = new S[ roundshortcutssize ];

	for(unsigned int i = 0; i < roundshortcutssize; i++){
		S stmp = round_shortcutlist.front();
		round_shortcutlist.pop_front();
		roundshortcuts[i] = stmp; // TODO evtl ausseinander ziehen
		nodes_out_offs[ stmp.source +1]++;
		nodes_in_offs[ stmp.target +1]++;
	}

	// merke shortcuts
	shortcutlist.push(ShortcutArray(roundshortcuts, roundshortcutssize));
	
	// out-edges umverlagern
	for(unsigned int i = 0; i < current_edge_arrays_size; i++){
		tmpedges[i].other_node = node_count;
	}
	unsigned int newI = 0;
	unsigned int oldI = 0;
	for(unsigned int i = 0; i < node_count; i++){
		unsigned int j = node_out_edges_count[i]; // anzahl aktueller knoten von i
		unsigned int k = nodes_out_offs[i+1] - j; // anzahl shortcuts die hinzukommen
		while(j>0){ // vorhandene edges übertragen
			tmpedges[ newI ] = out_edges[ oldI ];
			j--;
			oldI++;
			newI++;
		}
		newI = newI + k;
		while(oldI < old_edge_arrays_size && out_edges[ oldI ].other_node == node_count + i){
			oldI++;
		}
	}

	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_offs[i+1] = nodes_out_offs[i+1] + nodes_out_offs[i];
	}
	
	// sage anderen enden der out edges bescheid, wo neuer index
	for(unsigned int i = 0; i < current_edge_arrays_size; i++){
		if( tmpedges[i].other_node != node_count )
			in_edges[ tmpedges[i].other_lvl ].other_lvl = i;
	}

	/* nun liegen alle out-edges mit platz für shortcuts in tmpedges
	 * und die in-edges können diese erreichen
	 */

	if(current_edge_arrays_size > old_edge_arrays_size){
		// wir brauchen nun größere arrays für in/out_edges
		delete[] out_edges;
		out_edges = tmpedges;
		tmpedges = new E[ current_edge_arrays_size ];

	} else {
		// behalte alte edges arrays, verwende tmpedges wieder
		for(unsigned int i = 0; i < current_edge_arrays_size; i++){
			out_edges[i] = tmpedges[i];
		}
	}

	/* nun verteilen der in-edges in tmpedges */
	for(unsigned int i = 0; i < current_edge_arrays_size; i++){
		tmpedges[i].other_node = node_count;
	}
	newI = oldI = 0;
	for(unsigned int i = 0; i < node_count; i++){
		unsigned int j = node_in_edges_count[i]; // anzahl aktueller knoten von i
		unsigned int k = nodes_in_offs[i+1] - j; // anzahl shortcuts die hinzukommen
		while(j>0){ // vorhandene edges übertragen
			tmpedges[ newI ] = in_edges[ oldI ];
			j--;
			oldI++;
			newI++;
		}
		newI = newI + k;
		// überspringe ungültige edges im alten array
		while(oldI < old_edge_arrays_size && in_edges[ oldI ].other_node == node_count + i){
			oldI++;
		}
	}

	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_offs[i+1] = nodes_in_offs[i+1] + nodes_in_offs[i];
	}
	// sage anderen enden der in-edges bescheid, wo neuer index
	for(unsigned int i = 0; i < current_edge_arrays_size; i++){
		if( tmpedges[i].other_node != node_count )
			out_edges[ tmpedges[i].other_lvl ].other_lvl = i;
	}

	// kopiere in-edges aus tmpedges zurück
	if(current_edge_arrays_size > old_edge_arrays_size){
		// wir brauchen nun größere arrays für in/out_edges
		delete[] in_edges;
		in_edges = tmpedges;
		tmpedges = 0;
	} else {
		// behalte alte edges arrays
		for(unsigned int i = 0; i < current_edge_arrays_size; i++){
			in_edges[i] = tmpedges[i];
		}
		delete[] tmpedges; tmpedges = 0;
	}

	// verteile shortcuts
	for(unsigned int i = 0; i < roundshortcutssize; i++){
		S stmp = roundshortcuts[i];
		unsigned int oindex = nodes_out_offs[ stmp.source +1] -1;
		while( out_edges[ oindex ].other_node != node_count ){
			oindex--;
		}
		unsigned int iindex = nodes_in_offs[ stmp.target +1] -1;
		while( in_edges[ iindex ].other_node != node_count ){
			iindex--;
		}

		out_edges[ oindex ] = Edge(edge_count + shortcut_count + i, stmp.value, stmp.target, iindex);
		in_edges[ iindex ] = Edge(edge_count + shortcut_count + i, stmp.value, stmp.source, oindex);
	}
	shortcut_count = shortcut_count + roundshortcutssize;

	// neue anzahl an kanten verteilen
	for(unsigned int i = 0; i < node_count; i++){
		node_in_edges_count[i] = nodes_in_offs[i+1] - nodes_in_offs[i];
	}
	for(unsigned int i = 0; i < node_count; i++){
		node_out_edges_count[i] = nodes_out_offs[i+1] - nodes_out_offs[i];
	}

//	cout << "übrige Graphknoten: " << goodNodesSize << " ( " << (((double) goodNodesSize)/((double) node_count)) * 100.0 << "% der urspr. Knoten ) " << endl; // TODO
//	cout << "aktuelle Anzahl Kanten: " << ( (double)current_edge_arrays_size / (double)edge_count ) * 100.0 << "% der urspr. Kanten" << endl; // TODO

	return true;
}

void SCGraph::clearAlmostEverything(){
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] in_edges; in_edges = 0;
	delete[] out_edges; out_edges = 0;
	delete[] edge_data; edge_data = 0;
	delete[] node_in_edges_count; node_in_edges_count = 0;
	delete[] node_out_edges_count; node_out_edges_count = 0;
}

bool SCGraph::mergeShortcutsAndGraph(unsigned int lvl){
	if(g == 0)
		return false;

	if( ! g->isSet() )
		return false;
	
	// um fragmentierung ein wenig "vorzubeugen" verlagern wir aus der liste
	// in ein array, um den platz der gelöschten liste dann für neue arrays zu haben
	S* shortcuts = new S[shortcut_count];

	unsigned int sindex = 0;
	while(  !shortcutlist.empty() ){
		ShortcutArray sa = shortcutlist.pop();
		for(unsigned int j = 0; j < sa.size; j++){
			shortcuts[sindex] = sa.array[j];
			sindex++;
		}
		delete[] sa.array; sa.array = 0;
	}

	clearAlmostEverything();

	// letztes lvl verteilen
	for(unsigned int i = 0; i < node_count; i++){
		if( node_lvl[i] == 0){
			node_lvl[i] = lvl;
			node_data[i].elevation = lvl; // TODO
		}
	}
	
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

	for(unsigned int i = 0; i < edge_count + shortcut_count; i++){
		out_edges[i].other_lvl = node_lvl[ out_edges[i].other_node ];
	}
	for(unsigned int i = 0; i < edge_count + shortcut_count; i++){
		in_edges[i].other_lvl = node_lvl[ in_edges[i].other_node ];
	}

	return true;
}

bool SCGraph::isShortcut(unsigned int edge_id){
	return ( edge_count <= edge_id );
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

unsigned int* SCGraph::getGoodNodes(){
	return goodNodes;
}
unsigned int SCGraph::getGoodNodesSize(){
	return goodNodesSize;
}


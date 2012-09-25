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
	edge_data(0),
	EdgeLoads(0){
}
const int Graph::BinID = 51;
const std::string Graph::dateiendung = "grp";


Graph::~Graph(){
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] node_data; node_data = 0;

	delete[] out_edges; out_edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] edge_data; edge_data = 0;
	delete[] EdgeLoads; EdgeLoads = 0;
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

	/* Edge Loads needed for slow Sim */
	EdgeLoads = new unsigned int[edge_count];
	for(unsigned int i=0; i<edge_count; i++){
		EdgeLoads[i] = 0;
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

	/* vorausgesetzt, die parser edges sind aufsteigend
	 * nach source-node und aufsteigend nach target-nodes
	 * sortiert, funktioniert das entfernen doppelter kanten,
	 * indem die kante mit der niedrigsten distanz behalten wird
	 */
	unsigned int old_edge_count = edge_count;
	unsigned int slow = 0;
	unsigned int fast = 1;
	while( fast < old_edge_count){
		if( edges[slow].source == edges[fast].source 
				&& edges[slow].target == edges[fast].target ){
			if(edges[slow].distance <= edges[fast].distance){
				/* slow nach vorne kopieren, altes slow killen */
				cout << "Killing ParserEdge Nr. " << fast 
					<< ", killed Type: " << edges[fast].type 
					<< ", keeping Type " << edges[slow].type << endl;
				edges[fast] = edges[slow];
				edges[slow].source = node_count;
				fast++;
				slow++;
				
			} else {
				/* slow killen */
				cout << "Killing ParserEdge Nr. " << slow 
					<< ", killed Type: " << edges[slow].type 
					<< ", keeping Type " << edges[fast].type << endl;
				edges[slow].source = node_count;
				fast++;
				slow++;
			}
		} else {
			slow++;
			fast++;
		}
	}
	slow = 0;
	fast = 0;
	while(fast < old_edge_count){
		if( edges[fast].source == node_count ){
			edge_count--;
			fast++;
		} else {
			edges[slow] = edges[fast];
			fast++;
			slow++;
		}
	}
	if(old_edge_count != edge_count)
		cout << "Graph: " << old_edge_count-edge_count << " doppelte Kanten entfernt." << endl;

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

NodeData Graph::getNodeData(unsigned int id){
	return node_data[id];
}
EdgeData Graph::getEdgeData(unsigned int id){
	return edge_data[id];
}

Edge* Graph::getOutEdge(unsigned int edge_id){
	return out_edges + edge_id;
}
Edge* Graph::getInEdge(unsigned int edge_id){
	return in_edges + edge_data[edge_id].in_index;
}
/*
 * hier out1_in0 semantik 
 * genau umgekehrt im vgl. zu getEdgesIt
 * wegen backtracing im dijkstra (?)
 */
Edge* Graph::getEdge(bool out1_in0, unsigned int edge_id){
	if(out1_in0){
		return out_edges+edge_data[edge_id].out_index;
	}
	else{
		return in_edges+edge_data[edge_id].in_index;
	}
}
Edge* Graph::copyOutEdge(unsigned int edge_id){
	return out_edges + edge_id;
}
Edge* Graph::copyInEdge(unsigned int edge_id){
	return in_edges + edge_id;
}
void Graph::addEdgeLoad(unsigned int edge_id){
	EdgeLoads[edge_id]++;
}
void Graph::addEdgeLoad(unsigned int edge_id, unsigned int times){
	EdgeLoads[edge_id] += times;
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
void Graph::getEdgeLoads(SCGraph* g){
	EdgeData* scedge_data = g->getEdgeDataPointer();
	for(unsigned int i = 0; i < edge_count; i++){
		edge_data[i].load = scedge_data[i].load;
	}
}

void Graph::updateEdgeValues(){
	for(unsigned int i = 0; i < edge_count; i++){
		out_edges[ edge_data[i].out_index ].value = edge_data[i].value;
	}
	for(unsigned int i = 0; i < edge_count; i++){
		in_edges[ edge_data[i].in_index ].value = edge_data[i].value;
	}
}
void Graph::updateEdgeLoads(){
	for(unsigned int i = 0; i < edge_count; i++){
		edge_data[i].load = EdgeLoads[i];
		EdgeLoads[i] = 0; 
	}
}

/*
 * ========= CH Graph ==========
 */

SCGraph::SCGraph(Graph* gr) : 
	g(gr),
	node_lvl(new unsigned int[gr->getNodeCount()]),
	node_count(gr->getNodeCount()),
	edge_count(gr->getEdgeCount()),
	shortcut_count(0),
	goodNodes(new unsigned int[gr->getNodeCount()]),
	goodNodesIndex( new unsigned int[gr->getNodeCount()]),
	goodNodesSize(gr->getNodeCount()),
	current_edge_arrays_size(gr->getEdgeCount()),
	nodes_out_edges( new AdjEdges[gr->getNodeCount()] ),
	nodes_in_edges( new AdjEdges[gr->getNodeCount()] ),
	nodes_in_offs(0),
	in_edges( 0 ),
	round_in_edges( new Edge[gr->getEdgeCount()] ),
	nodes_out_offs(0),
	out_edges( 0 ),
	round_out_edges( new Edge[gr->getEdgeCount()] ),
	node_data(gr->getNodeDataPointer()),
	edge_data(0),
	shortcutlist( new list<Shortcut>[gr->getNodeCount()] ),
	round_shortcutlist( new list<Shortcut> ),
	round_node_blacklist(),
	goodNodesSorted(new uint_pair[gr->getNodeCount()]),
	EdgeLoads(0)
{
	for(unsigned int i = 0; i < node_count; i++){
		node_lvl[i] = 0;
	}
	for(unsigned int i = 0; i < node_count; i++){
		goodNodes[i] = i;
	}
	for(unsigned int i = 0; i < node_count; i++){
		goodNodesIndex[i] = i;
	}

	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_edges[i].count = 
			gr->getUpperOutEdgeBound(i) - gr->getLowerOutEdgeBound(i);
	}
	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_edges[i].start = round_out_edges + gr->getLowerOutEdgeBound(i);
	}
	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_edges[i].count = 
			gr->getUpperInEdgeBound(i) - gr->getLowerInEdgeBound(i);
	}
	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_edges[i].start = round_in_edges + gr->getLowerInEdgeBound(i);
	}
	for(unsigned int i = 0; i < edge_count; i++){
		round_in_edges[i] = * gr->copyInEdge(i);
	}
	for(unsigned int i = 0; i < edge_count; i++){
		round_out_edges[i] = * gr->copyOutEdge(i);
	}
	fillGoodNodes();
	
}

SCGraph::~SCGraph(){
	g = 0;
	delete[] node_lvl; node_lvl = 0;
	delete[] goodNodes; goodNodes = 0;
	delete[] goodNodesIndex; goodNodesIndex = 0;
	delete[] nodes_out_edges; nodes_out_edges = 0;
	delete[] nodes_in_edges; nodes_in_edges = 0;
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] round_in_edges; round_in_edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] round_out_edges; round_out_edges = 0;
	delete[] out_edges; out_edges = 0;
	node_data = 0;
	delete[] edge_data; edge_data = 0;
	delete[] shortcutlist; shortcutlist = 0;
	delete round_shortcutlist; round_shortcutlist = 0;
	round_node_blacklist.clear();
	delete[] goodNodesSorted; goodNodesSorted = 0;
	delete[] EdgeLoads; EdgeLoads = 0;
}

void SCGraph::prepareNodes(unsigned int lvl){
	/* schwarze Nodes rausschmeissen */
	unsigned int deleted_nodes = round_node_blacklist.size();
	while( ! round_node_blacklist.empty() ){
		unsigned int bnode = round_node_blacklist.front();
		round_node_blacklist.pop_front();

		 goodNodes[ goodNodesIndex[bnode] ] = node_count;
		 goodNodesIndex[bnode] = node_count;
		 node_lvl[ bnode ] = lvl;
	}

	/* erreichbare goodNodes im Array sortieren */
	unsigned int slow = 0;
	unsigned int fast = 0;
	while( slow < goodNodesSize && fast < goodNodesSize ){
		if( goodNodes[ fast ] == node_count){
			fast++;
		} else {
			goodNodes[slow] = goodNodes[fast];
			slow++;
			fast++;
		}
	}
	goodNodesSize -= deleted_nodes;

	/* Indizes der goodNodes aktualisieren */
	for(unsigned int i = 0; i < goodNodesSize; i++){
		goodNodesIndex[ goodNodes[i] ] = i;
	}
}

bool compare_shortcuts_asc_by_target(Shortcut one, Shortcut two){
	/* returnt true if one before two */
	bool ret = false;
	if( one.target < two.target ){
		ret = true;
	} else if( one.target == two.target){
		ret = (one.value < two.value);	
	}
	return ret;
}
bool equal_if_targets_equal(Shortcut one, Shortcut two){
	return (one.target == two.target);
	/* wenn true, wird one von unique gekillt */
}
unsigned int SCGraph::sortShortcuts(){

	list<Shortcut>* tmp_shortcutlist = new list<Shortcut>[goodNodesSize];

	/* einkommende SCs temporär verteilen */
	while( ! round_shortcutlist->empty() ){
		Shortcut sc = round_shortcutlist->front();
		round_shortcutlist->pop_front();

		tmp_shortcutlist[ goodNodesIndex[sc.source] ].push_front(sc);
	}

	/* SCs jeweils sortieren/duplikate bereinigen */
	/*TODO auch alte doppelte SCs erkennen */
	for(unsigned int i = 0; i < goodNodesSize; i++){
		tmp_shortcutlist[ i ].sort(compare_shortcuts_asc_by_target);
		tmp_shortcutlist[ i ].unique( equal_if_targets_equal );
	}
	unsigned int new_sc_count = 0;
	unsigned int tmp_graph_edge_count = g->getEdgeCount();
	for(unsigned int i = 0; i < goodNodesSize; i++){
		for(list<Shortcut>::iterator si = tmp_shortcutlist[ i ].begin();
				si != tmp_shortcutlist[ i ].end(); si++)
		{
			si->id = tmp_graph_edge_count + shortcut_count + new_sc_count;
			new_sc_count++;
		}
	}

	/* neue SCs übernehmen */
	for(unsigned int i = 0; i < goodNodesSize; i++){
		if( ! tmp_shortcutlist[i].empty() ){
			unsigned int gn = goodNodes[i];
			shortcutlist[ gn ].
				splice(shortcutlist[ gn ].end(), 
						tmp_shortcutlist[ i ]);
		}
	}

	delete[] tmp_shortcutlist; tmp_shortcutlist = 0;
	
	return new_sc_count;
}
void SCGraph::buildGraph(){
	
	list<Edge>* ValidOutEdges = new list<Edge>[goodNodesSize];

	/* In-Edges Counts auf 0*/
	for(unsigned int i = 0; i < goodNodesSize; i++){
		unsigned int gn = goodNodes[i];
		nodes_in_edges[gn].count = 0;
	}
	/* Graph Edges vorbereiten und auszählen*/
	unsigned int new_ec = 0;
	for(unsigned int i = 0; i < goodNodesSize; i++){
		unsigned int gn = goodNodes[i];
		/* Edges zu schwarzen Nodes rauszählen */
		EdgesIterator ei = g->getOutEdgesIt(gn);
		while(ei.hasNext()){
			Edge* te = ei.getNext();
			if(goodNodesIndex[te->other_node] != node_count){
				ValidOutEdges[i].push_back( *te );
				/* In-Edge bescheid sagen */
				nodes_in_edges[te->other_node].count++;
			}
		}
		/* Shortcuts zu schwarzen Nodes rauszählen */
		for(list<Shortcut>::iterator si = shortcutlist[gn].begin();
				si != shortcutlist[gn].end(); si++)
		{
			Shortcut sc = *si;
			if( goodNodesIndex[sc.target] != node_count){
				ValidOutEdges[i].
					push_back( Edge(sc.id, sc.value, sc.target) );
				/* In-Edge bescheid sagen */
				nodes_in_edges[sc.target].count++;
			}
		}
		/* Out-Edges/neue Kantenanzahl  zählen */
		nodes_out_edges[ gn ].count = 
			ValidOutEdges[i].size();
		new_ec += ValidOutEdges[i].size();
	}

	/* Evtl neue Edge-Arrays anlegen */
	if((current_edge_arrays_size) < ( new_ec )){
		cout << "SCGraph Shortcut Merging: brauche neue Edge-Arrays" << endl;
		delete[] round_out_edges;
		delete[] round_in_edges;
		round_out_edges = new Edge[new_ec];
		round_in_edges = new Edge[new_ec];
		current_edge_arrays_size = new_ec;
	}
	edge_count = new_ec;

	/* Edge-Iterator-Starts eintragen */
	if( goodNodesSize > 0)
	{
		/* Out */
		unsigned int gon = goodNodes[0];
		nodes_out_edges[gon].start = round_out_edges;
		Edge* start_next = round_out_edges + nodes_out_edges[gon].count;
		for(unsigned int i = 1; i < goodNodesSize; i++){
			unsigned int gn = goodNodes[i];
			nodes_out_edges[gn].start = start_next;
			start_next += nodes_out_edges[gn].count;
		}
		/* In */
		nodes_in_edges[gon].start = round_in_edges;
		start_next = round_in_edges + nodes_in_edges[gon].count;
		for(unsigned int i = 1; i < goodNodesSize; i++){
			unsigned int gn = goodNodes[i];
			nodes_in_edges[gn].start = start_next;
			start_next += nodes_in_edges[gn].count;
		}
	}

	/* Edges in Out Array eintragen */
	unsigned int eindex = 0;
	for(unsigned int i = 0; i < goodNodesSize; i++){
		while(! ValidOutEdges[i].empty()){
			round_out_edges[eindex] = ValidOutEdges[i].front();
			ValidOutEdges[i].pop_front();
			eindex++;
		}
	}

	/* ValidOutEdges ist nun leer
	 * => benutze für In-Edges
	 */

	/* In-Edges vorbereiten */
	for(unsigned int i = 0; i < goodNodesSize; i++){
		unsigned int gn = goodNodes[i];
		unsigned int ec = nodes_out_edges[gn].count;
		Edge* edge = nodes_out_edges[gn].start;
		while(ec > 0){
			ValidOutEdges[goodNodesIndex[edge->other_node]].
				push_front( Edge(edge->id, edge->value, gn) );
			edge++;
			ec--;
		}
	}
	/* Edges in In Array eintragen */
	eindex = 0;
	for(unsigned int i = 0; i < goodNodesSize; i++){
		while(! ValidOutEdges[i].empty()){
			round_in_edges[eindex] = ValidOutEdges[i].front();
			ValidOutEdges[i].pop_front();
			eindex++;
		}
	}

	delete[] ValidOutEdges; ValidOutEdges = 0;

}
bool compare_uint_pair_asc_by_key(uint_pair one, uint_pair two){
	return one.key < two.key;
}
void SCGraph::fillGoodNodes(){
	for(unsigned int i = 0; i < goodNodesSize; i++){
		unsigned int gn = goodNodes[i];
		goodNodesSorted[i] = 
				uint_pair(gn, 
					nodes_in_edges[gn].count * nodes_out_edges[gn].count);
	}
	std::sort
		(goodNodesSorted, 
		 goodNodesSorted+goodNodesSize, 
		 compare_uint_pair_asc_by_key);
}

bool compare_edges_desc_by_lvl(Edge one, Edge two){
	/* one soll vor two */
	return (one.other_lvl > two.other_lvl);
}
void SCGraph::buildGraphFinal(){

	/* Shortcuts sichern */
	Shortcut* shortcuts = new Shortcut[shortcut_count];
	unsigned int sindex = 0;
	for(unsigned int i = 0; i < node_count; i++){
		while(!shortcutlist[i].empty()){
			shortcuts[sindex] = shortcutlist[i].front();
			shortcutlist[i].pop_front();
			sindex++;
		}
	}

	/* unnützes löschen */
	delete[] goodNodes; goodNodes = 0;
	delete[] goodNodesIndex; goodNodesIndex = 0;
	delete[] nodes_out_edges; nodes_out_edges = 0;
	delete[] nodes_in_edges; nodes_in_edges = 0;
	delete[] round_in_edges; round_in_edges = 0;
	delete[] round_out_edges; round_out_edges = 0;
	delete[] shortcutlist; shortcutlist = 0;
	delete round_shortcutlist; round_shortcutlist = 0;

	/* neue Strukturen anlegen */
	edge_count = g->getEdgeCount();
	nodes_in_offs = new unsigned int[node_count +1];
	in_edges = new Edge[edge_count + shortcut_count];
	nodes_out_offs = new unsigned int[node_count +1];
	out_edges = new Edge[edge_count + shortcut_count];
	edge_data = new EdgeData[edge_count+shortcut_count];
	EdgeLoads = new unsigned int[edge_count+shortcut_count];

	for(unsigned int i = 0; i < edge_count + shortcut_count; i++){
		EdgeLoads[i] = 0;
	}
	for(unsigned int i = 0; i < edge_count; i++){
		edge_data[i] = g->getEdgeData(i);
	}
	for(unsigned int c = 0; c < shortcut_count; c++ ){
		Shortcut s = shortcuts[c];
		edge_data[ s.id ] = ED(0, 0, s.papa_edge, s.mama_edge, 0);
	}

	/* Edges der Knoten abzählen */
	nodes_out_offs[0] = 0;
	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_offs[i+1] = 
			g->getUpperOutEdgeBound(i)  - g->getLowerOutEdgeBound(i);
	}
	nodes_in_offs[0] = 0;
	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_offs[i+1] = 
			g->getUpperInEdgeBound(i)  - g->getLowerInEdgeBound(i);
	}
	/* Shortcuts abzählen, Edge Offsets*/
	for(unsigned int i = 0; i < shortcut_count; i++){
		nodes_out_offs[shortcuts[i].source +1]++;
	}
	for(unsigned int i = 0; i < shortcut_count; i++){
		nodes_in_offs[shortcuts[i].target +1]++;
	}
	for(unsigned int i = 0; i < node_count; i++){
		nodes_out_offs[i+1] += nodes_out_offs[i];
	}
	for(unsigned int i = 0; i < node_count; i++){
		nodes_in_offs[i+1] += nodes_in_offs[i];
	}

	/* Shortcuts verteilen */
	for(unsigned int i = 0; i < edge_count+shortcut_count; i++){
		out_edges[i].id = 0;
	}
	for(unsigned int i = 0; i < edge_count+shortcut_count; i++){
		in_edges[i].id = 0;
	}
	unsigned int o;
	unsigned int i;
	for(unsigned int c = 0; c < shortcut_count; c++ ){
		Shortcut s = shortcuts[c];
		o = nodes_out_offs[ s.source ];
		while( out_edges[ o ].id != 0 ){
			o++;
		}
		out_edges[ o ] = E(s.id, s.value, s.target );
	}
	for(unsigned int c = 0; c < shortcut_count; c++ ){
		Shortcut s = shortcuts[c];
		i = nodes_in_offs[ s.target ];
		while( in_edges[ i ].id != 0 ){
			i++;
		}
		in_edges[ i ] = E(s.id, s.value, s.source );
	}
	/* Graph Edges übernehmen */
	EdgesIterator eit;
	for(unsigned int c = 0; c < node_count; c++){
		eit = g->getOutEdgesIt(c);
		o = nodes_out_offs[c + 1] - 1;
		while( eit.hasNext() ){
			out_edges[ o ] = * eit.getNext();
			o--;
		}
	}
	for(unsigned int c = 0; c < node_count; c++){
		eit = g->getInEdgesIt(c);
		i = nodes_in_offs[c + 1] - 1;
		while( eit.hasNext() ){
			in_edges[ i ] = * eit.getNext();
			i--;
		}
	}
	delete[] shortcuts; shortcuts = 0;

	/* lvl an die Edges verteilen */
	for( i = 0; i < edge_count + shortcut_count; i++){
		out_edges[i].other_lvl = node_lvl[ out_edges[i].other_node ];
	}
	for( i = 0; i < edge_count + shortcut_count; i++){
		in_edges[i].other_lvl = node_lvl[ in_edges[i].other_node ];
	}

	/* Eges absteigend nach other_lvl sortieren */
	for( i = 0; i < node_count; i++){
		Edge* start = out_edges + nodes_out_offs[i];
		Edge* end = out_edges + nodes_out_offs[i+1];
		std::sort(start, end, compare_edges_desc_by_lvl);
	}
	for( i = 0; i < node_count; i++){
		Edge* start = in_edges + nodes_in_offs[i];
		Edge* end = in_edges + nodes_in_offs[i+1];
		std::sort(start, end, compare_edges_desc_by_lvl);
	}

	/* Edge-indizes in EdgeData anpassen*/
	for( o = 0; o < edge_count+shortcut_count; o++){
		edge_data[ out_edges[ o ].id ].out_index = o;
	}
	for( i = 0; i < edge_count+shortcut_count; i++){
		edge_data[ in_edges[ i ].id ].in_index = i;
	}

}

void SCGraph::addShortcut(unsigned int source_node_id, S sc){
	round_shortcutlist->push_front(sc);
}
void SCGraph::blacklistNode(unsigned int node_id){
	round_node_blacklist.push_front(node_id);
}

bool SCGraph::mergeRound(unsigned int lvl, bool verbose){
	prepareNodes(lvl);
	unsigned int new_sc_count = sortShortcuts();
	shortcut_count += new_sc_count;
	buildGraph();
	fillGoodNodes();
	
	if(verbose){
		cout << new_sc_count << " neue Shortcuts" << endl;
		cout << "Übrige Graphknoten: " << goodNodesSize 
			<< " ( " 
			<< (((double) goodNodesSize)/((double) node_count)) * 100.0 
			<< "% der urspr. Knoten ) " << endl;
		cout << "aktuelle Anzahl Kanten: " 
			<< ( (double)edge_count / (double)g->getEdgeCount() ) * 100.0 
			<< "% der urspr. Kanten" << endl;
	}
	return true; 
}

bool SCGraph::mergeShortcutsAndGraph(unsigned int lvl, bool verbose){
	prepareNodes(lvl);
	shortcut_count += sortShortcuts();
	buildGraphFinal();
	if(verbose){
		cout << "Edges: " << edge_count 
			<< ", Shortcuts: " << shortcut_count << endl;
		cout << "SCGraph merged with Graph and Shortcuts. " << endl;
	}
	return true; 
}

bool SCGraph::isShortcut(unsigned int edge_id){
	return (edge_id >= edge_count);
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
	return (nodes_out_edges[node_id].count+ 
				nodes_in_edges[node_id].count);
}

unsigned int SCGraph::getShortcutCount(){
	return shortcut_count;
}

NodeData SCGraph::getNodeData(unsigned int node_id){
	return node_data[node_id];
}
EdgeData SCGraph::getEdgeData(unsigned int edge_id){
	return edge_data[edge_id];
}

Edge* SCGraph::getOutEdge(unsigned int edge_id){
	return out_edges+edge_data[edge_id].out_index;
}
Edge* SCGraph::getInEdge(unsigned int edge_id){
	return in_edges+edge_data[edge_id].in_index;
}
Edge* SCGraph::copyOutEdge(unsigned int edge_id){
    	return out_edges + edge_id;
}
Edge* SCGraph::copyInEdge(unsigned int edge_id){
		return in_edges + edge_id;
}
/*
 * hier out1_in0 semantik 
 * genau umgekehrt im vgl. zu getEdgesIt
 * wegen backtracing im dijkstra (?)
 */
Edge* SCGraph::getEdge(bool out1_in0, unsigned int edge_id){
	if(out1_in0){
		return out_edges+edge_data[edge_id].out_index;
	}
	else{
		return in_edges+edge_data[edge_id].in_index;
	}
}

void SCGraph::updateEdgeLoads(std::list< uint_pair >* edge_load_values){
	while( ! edge_load_values->empty() ){
		edge_data[ edge_load_values->front().id ].load = 
			edge_load_values->front().key;
		edge_load_values->pop_front();
	}
}
void SCGraph::updateEdgeLoads(
		unsigned int* edge_loads, 
		unsigned int array_length)
{
	for(unsigned int i = 0; i < array_length; i++){
		edge_data[i].load = edge_loads[i];
	}
}
void SCGraph::shareShortcutLoads(){
	// die jungen shortcuts stehen hinten in edge_data
	// jeder junge shortcut hat höchstens zwei ältere 
	// shortcuts als eltern
	for(unsigned int i = edge_count+shortcut_count-1; i >= edge_count; i--){
		edge_data[edge_data[i].distance /* papa */].load 
			+= edge_data[i].load;
		edge_data[edge_data[i].type /* mama */].load 
			+= edge_data[i].load; 
		// edge_data[i].load = 0; 
	}
}
void SCGraph::updateEdgeLoads(){
	for(unsigned int i = 0; i < edge_count+shortcut_count; i++){
		edge_data[i].load = EdgeLoads[i];
		EdgeLoads[i] = 0; 
	}
	//shareShortcutLoads();
}
void SCGraph::addEdgeLoad(unsigned int edge_id){
	EdgeLoads[edge_id]++;
}
void SCGraph::addEdgeLoad(unsigned int edge_id, unsigned int times){
	EdgeLoads[edge_id] += times;
}


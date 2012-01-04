#include "ch.h"

CH::CH(Graph* gr) : 
	g(gr),
	nodes_in_offs(new N[ gr->getNodeCount() +1 ]),
	nodes_out_offs(new N[ gr->getNodeCount() +1 ]),
	node_order(new unsigned int[ gr->getNodeCount() ]),
	node_count(gr->getNodeCount()),
	edge_count(gr->getEdgeCount()),
	shortcut_count(gr->getEdgeCount()), 
	out_edges( new E[ gr->getEdgeCount() ] ), 
	in_edges( new E[ gr->getEdgeCount() ] ), 
	shortcut_data( new SD[ gr->getEdgeCount() ] ),
	shortcutlist(),
	uintlist() {}

CH::~CH(){
	g = 0;
	delete[] nodes_in_offs; nodes_in_offs = 0;
	delete[] nodes_out_offs; nodes_out_offs = 0;
	delete[] out_edges; out_edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] shortcut_data; shortcut_data = 0;
	delete[] node_order; node_order = 0;

	shortcutlist.clear();
	uintlist.clear();
}

bool CH::setShortcuts(){
	if(g == 0)
		return false;

	if( ! g->isSet() )
		return false;
	
	// um fragmentierung ein wenig "vorzubeugen" verlagern wir aus der liste
	// in ein array, um den platz der gelöschten liste dann für neue arrays zu haben
	shortcut_count = shortcutlist.size();
	S* shortcuts = new S[shortcut_count];

	SList<S>::Iterator it = shortcutlist.getIterator();
	for(unsigned int i=0; i < shortcut_count; i++){
		if(it.hasNext())
			shortcuts[i] = it.getNext();
	}

	clearShortcutOffsets();
	shortcutlist.clear();

	out_edges = new E[ edge_count + shortcut_count];
	in_edges = new E[ edge_count + shortcut_count];
	shortcut_data = new SD[shortcut_count];

	// offsets aus dem graphen übernehmen
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
		shortcut_data[c] = SD(s.papa_edge, s.mama_edge);
	}
	EdgesIterator eit;
	for(unsigned int c = 0; c < node_count; c++){
		eit = g->getOutEdgesIt(c);
		o = nodes_out_offs[c + 1] - 1;
		while( eit.hasNext() ){
			out_edges[ o ] = eit.getNext();
			o--;
		}
	}
	for(unsigned int c = 0; c < node_count; c++){
		eit = g->getInEdgesIt(c);
		i = nodes_in_offs[c + 1] - 1;
		while( eit.hasNext() ){
			in_edges[ i ] = eit.getNext();
			i--;
		}
	}
	delete[] shortcuts;

	return true;
}

void CH::clearShortcuts(){
	clearShortcutOffsets();
	shortcutlist.clear();
}
void CH::clearShortcutlist(){
	shortcutlist.clear();
}
void CH::clearShortcutOffsets(){
	if(shortcut_data != 0){ 
	// shortcuts werden nur gemeinsam mit shortcut_data gesetzt
		for(unsigned int i = 0; i < node_count; i++){
			nodes_out_offs[ i+1 ] = 0;
		}
		for(unsigned int i = 0; i < node_count; i++){
			nodes_in_offs[ i+1 ] = 0;
		}
		delete[] shortcut_data; shortcut_data = 0;
		delete[] in_edges; in_edges = 0;
		delete[] out_edges; out_edges = 0;
		shortcut_count = 0;
	}
}

void CH::addShortcut(S sc){
	shortcutlist.push(sc);
}

SD CH::getShortcutData(unsigned int id){
	if(id >= edge_count && id < (edge_count + shortcut_count))
		return shortcut_data[id - edge_count];
	
	SD sd;
	return sd;
}
unsigned int CH::getShortcutCount(){
	return shortcut_count;
}

unsigned int CH::getNodeCount(){
	return node_count;
}
unsigned int CH::getEdgeCount(){
	return edge_count;
}

unsigned int CH::calcIndepSet(){
	for(unsigned int i = 0; i < node_count; i++){
		node_order[i] = 0;
	}
	for(unsigned int i = 0; i < node_count; i++){
		if(node_order[i] == 0){
			uintlist.push(i);
			node_order[i] = 2;
			EdgesIterator it = g->getOutEdgesIt(i);
			while( it.hasNext() ){
				node_order[ it.getNext() . other_node ] = 1;
			}
		} 
	}
	for(unsigned int i = 0; i < node_count; i++){
		if( node_order[i] < 1 )
			return 0;
	}

	return uintlist.size();
}


DynCH::DynCH(Graph* gr) : 
	out_edges(new std::vector<E>[gr->getNodeCount()]),
	in_edges(new std::vector<E>[gr->getNodeCount()]),
	g(gr),
	node_order(new unsigned int[gr->getNodeCount()]),
	node_count(gr->getNodeCount()),
	edge_count(gr->getEdgeCount()),
	shortcut_count(0),
	shortcut_data(),
	uintlist() {
		for(unsigned int i = 0; i < node_count; i++){
			EdgesIterator it = gr->getOutEdgesIt(i);
			while( it.hasNext() ){
				out_edges[i].push_back( it.getNext() );
			}
		}
		for(unsigned int i = 0; i < node_count; i++){
			EdgesIterator it = gr->getInEdgesIt(i);
			while( it.hasNext() ){
				in_edges[i].push_back( it.getNext() );
			}
		}
	}
DynCH::~DynCH(){
	delete[] out_edges; out_edges = 0;
	delete[] in_edges; in_edges = 0;
	delete[] node_order; node_order = 0;
}

unsigned int DynCH::getNodeCount(){
	return node_count;
}
unsigned int DynCH::getEdgeCount(){
	return edge_count;
}
unsigned int DynCH::getShortcutCount(){
	return shortcut_count;
}

void DynCH::addShortcut(S sc){
	shortcut_data.push( sc_id( SD(sc.papa_edge, sc.mama_edge) , shortcut_count ) );
	out_edges[ sc.source ].push_back( E(edge_count + shortcut_count, sc.value, sc.target ) );
	in_edges[ sc.target ].push_back( E(edge_count + shortcut_count, sc.value, sc.source ) );
	shortcut_count ++;
}

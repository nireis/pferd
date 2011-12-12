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
	shortcut_count = 0;
	nodes = 0;
	edges = 0;
	shortcuts = 0;
	in_edges = 0;
	in_shortcuts = 0;
	shortcutlist = SListExt<S>();
}

template <typename E, typename N, typename S>
Graph<E, N, S>::Graph(unsigned int nc, unsigned int ec, unsigned int sc,
		N* n, E* e,
		S* s){
	node_count = nc;
	edge_count = ec;
	shortcut_count = sc;
	nodes = n;
	edges = e;
	shortcuts = s;
	in_edges = 0;
	in_shortcuts = 0;
	shortcutlist = SListExt<S>();
}

template <typename E, typename N, typename S>
Graph<E, N, S>::Graph(unsigned int nc, unsigned int ec, 
		N* n, E* e){
	node_count = nc;
	edge_count = ec;
	shortcut_count = 0;
	nodes = n;
	edges = e;
	shortcuts = 0;
	in_edges = 0;
	in_shortcuts = 0;
	shortcutlist = SListExt<S>();
}

template <typename E, typename N, typename S>
Graph<E, N, S>::~Graph(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
	delete[] shortcuts; shortcuts = 0;
	delete[] in_edges; in_edges = 0;
	delete[] in_shortcuts; in_shortcuts = 0;
	shortcutlist.clear();
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
unsigned int Graph<E, N, S>::getShortcutCount(){
	return shortcut_count;
}

template <typename E, typename N, typename S>
/*  initOffsets()
 *
 *  richtet die offsets der nodes ein so,
 *  dass die outgoing-edges direkt im 'E* edges' array auffindbar sind
 *  über die out_offsets und
 *  die incoming-edges auffindbar sind über pointer in 'E** in_edges'
 *  über die in_offsets in den nodes
 *
 *  Prinzip: Bucket-Sort 
 */
void Graph<E, N, S>::initOffsets(){
	if (!edges || !nodes || !node_count || !edge_count)
		return; // wenn graph leer ist
	in_edges = new E*[edge_count];

	/* 
	 * voraussetzung, damit das alles funktioniert, ist,
	 * dass die kanten den sources nach aufsteigend sortiert sind,
	 * so, wie die nodes den IDs nach selbst aufsteigend sortiert sin 
	 * 
	 * das ganze geht in (n + 2*m)
	 */

	for(unsigned int j = 0; j < edge_count; j++){
		// bereite in targets offsets vor
		nodes[ edges[j].target +1 ].in_edge_offset ++; 
		// bereite in sources offsets vor
		nodes[ edges[j].source +1 ].out_edge_offset ++; 
		in_edges[j] = 0;
	}

	for(unsigned int i = 0; i < node_count; i++){
		// summiere offsets von vorne, damit die differenzen spaeter stimmen
		nodes[i+1].in_edge_offset = nodes[i+1].in_edge_offset + nodes[i].in_edge_offset;
		nodes[i+1].out_edge_offset = nodes[i+1].out_edge_offset + nodes[i].out_edge_offset;
	}

	unsigned int j = 0;
	for(unsigned int i = 0; i < edge_count; i++){
		j = 0;
		// suche dir das Offset für in_edges im target, 
		// suche in diesem bereich einen noch leeren eintrag,
		// trage dort die entsprechende kante ein
		while( in_edges[ nodes[ edges[i].target ].in_edge_offset + j] != 0 ){
			j++;
		}
		in_edges[ nodes[ edges[i].target ].in_edge_offset + j] 
			= & edges[i];
	}
}

template <typename E, typename N, typename S>
/* initShortcutOffsets
 *
 * siehe initOffsets
 * nur, dass wir unsere gefüllte Liste nehmen
 * und diese in ein sinnvolles array umsetzen,
 * bevor wir mit den offsets anfangen
 *
 * wenn dies aufgerufen wird, wird davon ausgegangen,
 * dass für die alten shortcuts keine verwendung mehr besteht
 */
void Graph<E, N, S>::initShortcutOffsets(){
	if(shortcuts != 0){
		for(unsigned int i = 0; i < node_count; i++){
			nodes[ i ].out_shortcut_offset = 0;
			nodes[ i ].in_shortcut_offset = 0;
		}
	}
	if(in_shortcuts != 0)
		delete[] in_shortcuts;

	if(shortcuts != 0)
		delete[] shortcuts;

	shortcut_count = shortcutlist.size();
	shortcuts = new S[shortcut_count];
	in_shortcuts = new S*[shortcut_count];

	for(unsigned int i = 0; i < shortcut_count; i++){
		in_shortcuts[i] = 0;
	}

	typename SListExt<S>::Iterator it = shortcutlist.getIterator();

	S s;
	unsigned int j = 0;
	// bereits offsets und in_shortcuts vor
	while( it.hasNext() ){
		j++;
		s = it.getNext();
		nodes[ s.source +1 ].out_shortcut_offset++;
		nodes[ s.target +1 ].in_shortcut_offset++;
	}
	// setze offsets korrekt
	for(unsigned int i = 0; i < node_count; i++){
		nodes[i+1].in_shortcut_offset 
			= nodes[i+1].in_shortcut_offset + nodes[i].in_shortcut_offset;
		nodes[i+1].out_shortcut_offset 
			= nodes[i+1].out_shortcut_offset + nodes[i].out_shortcut_offset;
	}
	// trage shortcuts in das array für ausgehende sc ein / umsetzung von liste auf array	
	while( !shortcutlist.empty() ){
		j = 0;
		s = shortcutlist.pop();
		while( shortcuts[ nodes [ s.source ].out_shortcut_offset + j].id != 0 ){
			j++;
		}
		s.id = 1;
		shortcuts[ nodes [ s.source ].out_shortcut_offset + j] 
			= s;
	}
	// trage in_shortcuts ein 
	for(unsigned int i = 0; i < shortcut_count; i++){
		j = 0;
		shortcuts[i].id = edge_count + i + 1; //TODO nochmal überdenken
		// dies wird uns später arbeit sparen
		// hierdurch sind shortcuts (fast) normale edges,
		// die wir sofort erkennen können

		// suche dir das Offset für in_edges im target, 
		// suche in diesem bereich einen noch leeren eintrag,
		// trage dort die entsprechende kante ein
		while( in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] != 0 ){
			j++;
		}
		in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] 
			= & shortcuts[i];
	}
}

template <typename E, typename N, typename S>
/* initShortcutOffsets
 *
 * siehe oben, nur bekommen wir eine
 * nach source-knoten aufsteigend sortiertes
 * array von shortcuts
 *
 * auch hier löschen wir alle bisher angelegten shortcuts
 */
void Graph<E, N, S>::initShortcutOffsets(S* scarray, unsigned int scc){
	if(shortcuts != 0){
		for(unsigned int i = 0; i < node_count; i++){
			nodes[ i ].out_shortcut_offset = 0;
			nodes[ i ].in_shortcut_offset = 0;
		}
	}
	if(in_shortcuts != 0)
		delete[] in_shortcuts;

	if(shortcuts != 0)
		delete[] shortcuts;

	shortcut_count = scc;
	shortcuts = scarray;
	in_shortcuts = new S*[shortcut_count];

	for(unsigned int i = 0; i < node_count; i++){
		nodes[ i ].out_shortcut_offset = 0;
		nodes[ i ].in_shortcut_offset = 0;
	}

	// bereits offsets und in_shortcuts vor
	for(unsigned int i = 0; i < shortcut_count; i++){
		in_shortcuts[i] = 0;
		nodes[ shortcuts[i].source +1 ].out_shortcut_offset++;
		nodes[ shortcuts[i].target +1 ].in_shortcut_offset++;
	}
	// setze offsets korrekt
	for(unsigned int i = 0; i < node_count; i++){
		nodes[i+1].in_shortcut_offset 
			= nodes[i+1].in_shortcut_offset + nodes[i].in_shortcut_offset;
		nodes[i+1].out_shortcut_offset 
			= nodes[i+1].out_shortcut_offset + nodes[i].out_shortcut_offset;
	}
	unsigned int j = 0;
	for(unsigned int i = 0; i < shortcut_count; i++){
		j = 0;
		shortcuts[i].id = edge_count + i + 1; //TODO nochmal überdenken
		// suche in diesem bereich einen noch leeren eintrag,
		// trage dort die entsprechende kante ein
		while( in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] != 0 ){
			j++;
		}
		in_shortcuts[ nodes[ shortcuts[i].target ].in_shortcut_offset + j] 
			= & shortcuts[i];
	}
}



template <typename E, typename N, typename S>
void Graph<E, N, S>::clearShortcuts(){
	for(unsigned int i = 0; i < node_count; i++){
		nodes[ i ].out_shortcut_offset = 0;
		nodes[ i ].in_shortcut_offset = 0;
	}
	delete[] in_shortcuts; in_shortcuts = 0;
	delete[] shortcuts; shortcuts = 0;
	shortcutlist.clear();
}

template <typename E, typename N, typename S>
void Graph<E, N, S>::addShortcut(S sc){
	shortcutlist.push(sc);
}

template <typename E, typename N, typename S>
E Graph<E, N, S>::getEdge(unsigned int id){
	if(id < edge_count)
		return edges[id];

	E e;
	return e;
}

template <typename E, typename N, typename S>
N Graph<E, N, S>::getNode(unsigned int id){
	if(id < node_count)
		return nodes[id];
	// TODO  shortcuts sidn edges, diese sollte hier aufrufbar sein
	N n;
	return n;
}

template <typename E, typename N, typename S>
S Graph<E, N, S>::getShortcut(unsigned int id){
	if(id < shortcut_count)
		return shortcuts[id];

	S s;
	return s;
}





#endif

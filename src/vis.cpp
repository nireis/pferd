#include "vis.h"

using namespace std;

vis::vis(Graph* g) : render() {
	
}

vis::vis(SCGraph* g) : render() {
	nodes = new openGL_Node_3d[g->getNodeCount()];
	edges = new openGL_Edge_Node[2 * ( g->getEdgeCount() )];
	shortcut_edges = new openGL_Node_3d[2 * ( g->getShortcutCount() )];

	NodeData* node_data = g->getNodeDataPointer();

	for(unsigned int i = 0; i < g->getNodeCount(); i++){
		nodes[i] = openGL_Node_3d( node_data[i].lon , node_data[i].lat , 0.0 );
	}

	unsigned int index = 0;
	unsigned int s_index = 0;
	float LOAD = 0.0; 
	// setze load in [0.0, 1.0] für färbung der kanten
	for(unsigned int i = 0; i < g->getNodeCount() ; i++){
		EdgesIterator it = g->getOutEdgesIt(i);
		while( it.hasNext() ){
			Edge e = * it.getNext();
				/*
				 * kante färben
				 */
				if(g->getEdgeData(e.id).load != 0){
					LOAD = 1.0;
				} else {
					LOAD = 0.0;
				}
			if( g->isShortcut( e.id )){
				shortcut_edges[s_index] = 
					openGL_Node_3d( (float) node_data[ i ].lon, (float) node_data[ i ].lat, LOAD );
				s_index++;
				shortcut_edges[s_index] = 
					openGL_Node_3d( (float) node_data[ e.other_node ].lon, (float) node_data[ e.other_node ].lat, LOAD );
				s_index++;
			} else {

				edges[index] = 
					openGL_Edge_Node( (float) node_data[ i ].lon, (float) node_data[ i ].lat, LOAD, (float)-(node_data[ i ].lat - node_data[ e.other_node ].lat),(float) (node_data[ i ].lon - node_data[ e.other_node ].lon),0.0  );
				index++;
				edges[index] = 
					openGL_Edge_Node( (float) node_data[ e.other_node ].lon, (float) node_data[ e.other_node ].lat, LOAD, (float)-(node_data[ i ].lat - node_data[ e.other_node ].lat),(float) (node_data[ i ].lon - node_data[ e.other_node ].lon),0.0 );
				index++;
			}
		}
	}

	render.setNodeCount(g->getNodeCount());
	render.setNodeArray(nodes);
	render.setEdgeCount((g->getEdgeCount()) *2);
	render.setEdgeArray(edges);
	render.setShortcutEdgeCount((g->getShortcutCount()) *2);
	render.setShortcutEdgeArray(shortcut_edges);
	render.setCamera((float) node_data[0].lon, (float)node_data[0].lat, 2.0);

	node_data = 0;
}

vis::~vis(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
	delete[] shortcut_edges; shortcut_edges = 0;
}

bool vis::start(){
	return render.start(0,0);
}


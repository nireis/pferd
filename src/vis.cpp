#include "vis.h"

using namespace std;

vis::vis(Graph* g) : render(), p(g){
	init();
}

vis::vis(SCGraph* g) : render(), p(g) {
	init();
}

vis::~vis(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
	delete[] shortcut_edges; shortcut_edges = 0;
}

void vis::init(){
	nodes = new openGL_Node_3d[p.getNodeCount()];
	edges = new openGL_Edge_Node[2 * ( p.getEdgeCount() )];
	shortcut_edges = new openGL_Node_3d[2 * ( p.getShortcutCount() )];

	NodeData* node_data = p.getNodeDataPointer();

	for(unsigned int i = 0; i < p.getNodeCount(); i++){
		nodes[i] = openGL_Node_3d( node_data[i].lon , node_data[i].lat , 0.0 );
	}

	unsigned int index = 0;
	unsigned int s_index = 0;
	float COLOUR = 0.0; 
	for(unsigned int i = 0; i < p.getNodeCount() ; i++){
		EdgesIterator it = p.getOutEdgesIt(i);
		while( it.hasNext() ){
			Edge e = * it.getNext();
				/*
				 * kante färben
				 */
				COLOUR = p.getEdgeData(e.id).colour;
			if( p.isShortcut( e.id )){
				shortcut_edges[s_index] = 
					openGL_Node_3d( (float) node_data[ i ].lon, (float) node_data[ i ].lat, COLOUR );
				s_index++;
				shortcut_edges[s_index] = 
					openGL_Node_3d( (float) node_data[ e.other_node ].lon, (float) node_data[ e.other_node ].lat, COLOUR );
				s_index++;
			} else {

				edges[index] = 
					openGL_Edge_Node( (float) node_data[ i ].lon, (float) node_data[ i ].lat, COLOUR, (float)-(node_data[ i ].lat - node_data[ e.other_node ].lat),(float) (node_data[ i ].lon - node_data[ e.other_node ].lon),0.0  );
				index++;
				edges[index] = 
					openGL_Edge_Node( (float) node_data[ e.other_node ].lon, (float) node_data[ e.other_node ].lat, COLOUR, (float)-(node_data[ i ].lat - node_data[ e.other_node ].lat),(float) (node_data[ i ].lon - node_data[ e.other_node ].lon),0.0 );
				index++;
			}
		}
	}

	render.setNodeCount(p.getNodeCount());
	render.setNodeArray(nodes);
	render.setEdgeCount((p.getEdgeCount()) *2);
	render.setEdgeArray(edges);
	render.setShortcutEdgeCount((p.getShortcutCount()) *2);
	render.setShortcutEdgeArray(shortcut_edges);
	render.setCamera((float) node_data[0].lon, (float)node_data[0].lat, 2.0);

	node_data = 0;
}

bool vis::start(){
	return render.start(0,0);
}


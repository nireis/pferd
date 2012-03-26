#include "vis.h"

using namespace std;

vis::vis(Graph* g) : render() {
	
}

vis::vis(SCGraph* g) : render() {
	nodes = new openGL_Node_3d[g->getNodeCount()];
	edges = new openGL_Node_3d[2 * ( g->getEdgeCount() + g->getShortcutCount() )];

	NodeData* node_data = g->getNodeDataPointer();

	for(unsigned int i = 0; i < g->getNodeCount(); i++){
		nodes[i] = openGL_Node_3d( node_data[i].lon , node_data[i].lat , 0.0 );
	}

	unsigned int index = 0;
	for(unsigned int i = 0; i < g->getNodeCount() ; i++){
		EdgesIterator it = g->getOutEdgesIt(i);
		while( it.hasNext() ){
			Edge e = * it.getNext();
			edges[index] = 
				openGL_Node_3d( (float) node_data[ i ].lon, (float) node_data[ i ].lat, (float) g->isShortcut( e.id) );
			index++;
			edges[index] = 
				openGL_Node_3d( (float) node_data[ e.other_node ].lon, (float) node_data[ e.other_node ].lat, (float) g->isShortcut( e.id) );
			index++;
		}
	}

	render.setNodeCount(g->getNodeCount());
	render.setNodeArray(nodes);
	render.setEdgeCount((g->getEdgeCount() + g->getShortcutCount()) *2);
	render.setEdgeArray(edges);
	render.setCamera((float) node_data[0].lon, (float)node_data[0].lat, 2.0);

	node_data = 0;
}

vis::~vis(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
}

bool vis::start(){
	return render.start(0,0);
}


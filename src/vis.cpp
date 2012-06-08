#include "vis.h"

using namespace std;

vis::vis(Graph* g, std::list<openGL_Cluster>* circs) : render(), nodes(0), edges(0), shortcut_edges(0), circles(0), p(g){
	init(circs);
}
vis::vis(SCGraph* g, std::list<openGL_Cluster>* circs) : render(), nodes(0), edges(0), shortcut_edges(0), circles(0), p(g){
	init(circs);
}
vis::vis(SCGraph* g) : render(), nodes(0), edges(0), shortcut_edges(0), circles(0), p(g){
	init();
}
vis::vis(Graph* g) : render(), nodes(0), edges(0), shortcut_edges(0), circles(0), p(g){
	init();
}

vis::~vis(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
	delete[] shortcut_edges; shortcut_edges = 0;
	delete[] circles; circles = 0;
}


void vis::init(){
	init(0);
}

void vis::init(std::list<openGL_Cluster>* circs){
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
	// setze load in [0.0, 1.0] für färbung der kanten
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

	/* anlegen der cirlce strukturen für renderer */
	unsigned int circles_count = 0;
	if( circs )
	{
		circles_count = circs->size();
		circles = new openGL_Cluster[circles_count];
		unsigned int counter = 0;
		while( ! circs->empty() )
		{
			circles[ counter ] = circs->front();
			circs->pop_front();
			counter++;
		}
	}
	render.setClusterCount(circles_count);
	render.setClusterArray(circles);

	node_data = 0;
}

bool vis::start(){
	return render.start(0,0);
}


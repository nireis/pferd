#include "vis.h"

using namespace std;

vis::vis(int pargc, char** pargv, Graph* gr, std::list<openGL_Cluster>* circs) : render(), nodes(0), edges(0), shortcut_edges(0), circles(0), g(gr){
	pferd_argc = pargc;
	pferd_argv = pargv;
	initRenderer(circs, 0);
}
vis::vis(int pargc, char** pargv, Graph* gr) : render(), nodes(0), edges(0), shortcut_edges(0), circles(0), g(gr){
	pferd_argc = pargc;
	pferd_argv = pargv;
	init();
}
vis::vis(int pargc, char** pargv) : render(), nodes(0), edges(0), shortcut_edges(0), circles(0), g(0){
	pferd_argc = pargc;
	pferd_argv = pargv;
}

vis::~vis(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
	delete[] shortcut_edges; shortcut_edges = 0;
	delete[] circles; circles = 0;
}

float vis::merkatorX(float in_x)
{
	float pi = 3.141592;
	float x_mercator = (pi/180.0)*in_x;

	return x_mercator;
}

float vis::merkatorY(float in_y)
{
	float pi = 3.141592;
	float pos_radian = (pi/180.0)*in_y;
	float y_mercator = log((1.0 + sin(pos_radian))/cos(pos_radian));

	return y_mercator;
}

void vis::initVis(Graph* gr, std::list<openGL_Cluster>* circs)
{
	g = gr;
	initRenderer(circs, 0);
}

void vis::initVis(Graph* gr)
{
	g = gr;
	initRenderer(0, 0);
}

void vis::initVis(Graph* gr, std::list<Edge>* shortcuts)
{
	g = gr;
	initRenderer(0, shortcuts);
}

void vis::init(){
	initRenderer(0, 0);
}

void vis::initRenderer(std::list<openGL_Cluster>* circs, std::list<Edge>* shortcuts)
{

	unsigned int NodeCount = 0;
	unsigned int EdgeCount = 0;
	unsigned int ShortcutCount = 0;
	NodeData* node_data = 0;

	NodeCount = g->getNodeCount();
	EdgeCount = g->getEdgeCount();
	node_data = g->getNodeDataPointer();
	if(shortcuts){
		ShortcutCount = shortcuts->size();
	} else {
		ShortcutCount = 0;
	}

	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
	delete[] shortcut_edges; shortcut_edges = 0;
	delete[] circles; circles = 0;

	nodes = new openGL_Node_3d[ NodeCount ];
	edges = new openGL_Edge_Node[2 * ( EdgeCount )];
	shortcut_edges = new openGL_Node_3d[ 2 * ( ShortcutCount )];

	for(unsigned int i = 0; i < NodeCount; i++)
	{
		nodes[i] = openGL_Node_3d(merkatorX(node_data[i].lon), merkatorY(node_data[i].lat), 0.0 );
	}

	unsigned int index = 0;
	unsigned int s_index = 0;
	float COLOUR = 0.0; 
	// setze load in [0.0, 1.0] für färbung der kanten
	for(unsigned int i = 0; i < NodeCount ; i++){
		EdgesIterator it = g->getOutEdgesIt(i);
		while( it.hasNext() ){
			Edge e = * it.getNext();
				/*
				 * kante färben
				 */
			COLOUR = g->getEdgeData(e.id).colour;
			if( 0 )
			{
				shortcut_edges[s_index] = openGL_Node_3d(	merkatorX((float) node_data[ i ].lon),
															merkatorY((float) node_data[ i ].lat),
															COLOUR );
				s_index++;
				shortcut_edges[s_index] = openGL_Node_3d(	merkatorX((float) node_data[ e.other_node ].lon),
															merkatorY((float) node_data[ e.other_node ].lat),
															COLOUR );
				s_index++;
			}
			else
			{
				edges[index] = 
					openGL_Edge_Node(	merkatorX((float) node_data[ i ].lon),
										merkatorY((float) node_data[ i ].lat),
										COLOUR,
										((float)-(node_data[ i ].lat - node_data[ e.other_node ].lat)),
										((float) (node_data[ i ].lon - node_data[ e.other_node ].lon)),
										0.0  );
				index++;
				edges[index] = 
					openGL_Edge_Node(	merkatorX((float) node_data[ e.other_node ].lon),
										merkatorY((float) node_data[ e.other_node ].lat), 
										COLOUR,
										((float)-(node_data[ i ].lat - node_data[ e.other_node ].lat)),
										((float) (node_data[ i ].lon - node_data[ e.other_node ].lon)),
										0.0 );
				index++;
			}
		}
	}

	//add shortcuts vom shortcut list
	s_index = 0;
	while( shortcuts && !(shortcuts->empty()) ){
		Edge e = shortcuts->front();
		shortcuts->pop_front();
				// shortcut.id ist source knoten
				shortcut_edges[s_index] = openGL_Node_3d(	merkatorX((float) node_data[ e.id ].lon),
															merkatorY((float) node_data[ e.id ].lat),
															COLOUR );
				s_index++;
				shortcut_edges[s_index] = openGL_Node_3d(	merkatorX((float) node_data[ e.other_node ].lon),
															merkatorY((float) node_data[ e.other_node ].lat),
															COLOUR );
				s_index++;
	}

	render.setNodeCount(NodeCount);
	render.setNodeArray(nodes);
	render.setEdgeCount((EdgeCount) *2);
	render.setEdgeArray(edges);
	render.setCamera((float) node_data[0].lon, (float)node_data[0].lat, 2.0);


	render.setShortcutEdgeCount((ShortcutCount) *2);
	render.setShortcutEdgeArray(shortcut_edges);

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
			circles[ counter ].xCenter = merkatorX(circles[ counter ].xCenter);
			circles[ counter ].yCenter = merkatorY(circles[ counter ].yCenter);
			counter++;
		}
	}
	render.setClusterCount(circles_count);
	render.setClusterArray(circles);

	node_data = 0;
}

bool vis::start(volatile bool* active, bool render_mode)
{
	render.setActivePointer(active);
	return render.start(pferd_argc, pferd_argv, render_mode);
}


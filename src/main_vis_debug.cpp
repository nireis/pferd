#include "openGLrender.h"
#include "parser.h"
#include <iostream>

int main(int argc, char* argv[])
{
	ParserNode* pNodes;
	ParserEdge* pEdges;
	parser testParser("1500K.txt");

	float camX = 0.0;
	float camY = 0.0;

	unsigned int node_count = testParser.getNodeCount();
	unsigned int edge_count = testParser.getEdgeCount();

	openGL_Node_3d* node_data = new openGL_Node_3d[node_count];
	openGL_Node_3d* ogledge = new openGL_Node_3d[edge_count*2];

	ParserNode* nodes = new ParserNode[node_count];
	ParserEdge* edges = new ParserEdge[edge_count];
	testParser.getNodesAndEdges(nodes, edges);

	for(unsigned int i = 0; i < node_count; i++){
		node_data[i] = openGL_Node_3d(nodes[i].lon,nodes[i].lat,nodes[i].elevation);
		camX = camX + node_data[i].lon;
		camY = camY + node_data[i].lat;
	}

	unsigned int index = 0;
	for(unsigned int i = 0; i < edge_count; i++){
		ogledge[index] = openGL_Node_3d(node_data[ edges[i].source ].lon, node_data[ edges[i].source ].lat, 
			edges[i].distance);
		index++;
		ogledge[index] = openGL_Node_3d(node_data[ edges[i].target ].lon, node_data[ edges[i].target ].lat,
			edges[i].distance);
		index++;
	}

	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;

	openGLrender *testRender = new openGLrender();
	testRender->setNodeCount(node_count);
	testRender->setNodeArray(node_data);
	testRender->setEdgeCount(edge_count*2);
	testRender->setEdgeArray(ogledge);
	testRender->setCamera((camX/float(node_count))*0.6649,(camY/float(node_count)),2.0);
	testRender->start(argc,argv);

	delete[] node_data; node_data = 0;
	delete[] ogledge; ogledge = 0;
	return 0;
}
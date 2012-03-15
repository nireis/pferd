#include "openGLrender.h"
#include <iostream>

int main(int argc, char* argv[])
{
	ParserNode* pNodes;
	ParserEdge* pEdges;
	parser testParser("1500K.txt");

	unsigned int node_count = testParser.getNodeCount();
	unsigned int edge_count = testParser.getEdgeCount();

	NodeData* node_data = new NodeData[node_count];
	openGL_Edge_Node* ogledge = new openGL_Edge_Node[edge_count*2];

	ParserNode* nodes = new ParserNode[node_count];
	ParserEdge* edges = new ParserEdge[edge_count];
	testParser.getNodesAndEdges(nodes, edges);

	for(unsigned int i = 0; i < node_count; i++){
		node_data[i] = NodeData(nodes[i]);
	}

	unsigned int index = 0;
	for(unsigned int i = 0; i < edge_count; i++){
		ogledge[index] = openGL_Edge_Node(node_data[ edges[i].source ].lat, node_data[ edges[i].source ].lon, 
			edges[i].distance);
		index++;
		ogledge[index] = openGL_Edge_Node(node_data[ edges[i].target ].lat, node_data[ edges[i].target ].lon,
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
	testRender->start(argc,argv);

	delete[] node_data; node_data = 0;
	delete[] ogledge; ogledge = 0;
	return 0;
}
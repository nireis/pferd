#ifndef rlparser_h
#define rlparser_h

class RlParser{
	private:
		int cs;
		int fd;

		ParserNode* nodes;
		ParserEdge* edges;

		unsigned int current_node;
		unsigned int current_edge;

		unsigned int node_count;
		unsigned int edge_count;

		int tmpint;
		unsigned int tmpsint;
		double tmpdouble;
		double tmpexp;
	public:
		RlParser(std::string filename);
		unsigned int getNodeCount();
		unsigned int getEdgeCount();
		void getNodesAndEdges(ParserNode* n, ParserEdge* e);
}

#endif

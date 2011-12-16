struct ParseContext{
	ParseContext()
		:cs(0),node_count(0),edge_count(0){}

	int cs;

	unsigned int node_count;
	unsigned int edge_count;
};

%%{
	machine graphparser;
	variable cs pc.cs;

	action NodeCount{
		
	}

	action EdgeCount{

	}

	action NodeID{

	}

	action NodeLat{

	}

	action NodeLon{

	}

	action NodeEle{

	}

	action NodesCheckEnd{

	}

	action EdgeSrc{

	}

	action EdgeTgt{

	}

	action EdgeDist{

	}

	action EdgeType{

	}

	action EdgesCheckEnd{

	}

	NL = '\n' | '\r\n';
	WS = ' '+;

	unit = ;
	snit = ;
	float = ;

	fileend := (NL | SP)*;

	edge = (uint %EdgeSrc WS uint %EdgeTgt WS uint %EdgeDist WS uint %EdgeType) %EdgesCheckEnd NL;
	edges := (edge)*;

	node = (uint %NodeID WS float %NodeLat WS float %NodeLon WS sint %NodeEle) %NodesCheckEnd NL;
	data = (node)*;
	
	main := uint %NodeCount NL uint %EdgeCount NL data;
}%%

void initParser(){

}

unsigned int getNodeCount(){

}

unsigned int getEdgeCount(){

}

void getNodesAndEdges(Node* n, Edge* e){

}

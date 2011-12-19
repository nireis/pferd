struct ParseContext{
	ParseContext()
		:cs(0),node_count(0),edge_count(0){}

	int cs;

	unsigned int node_count;
	unsigned int edge_count;

	int tmpint;
	unsigned int tmpsint;
	double tmpdouble;
	double tmpexp;
};

%%{
	machine graphparser;
	variable cs pc.cs;

	action StartInt{
		pc.tmpint = 0;
	}

	action ReadInt{
		pc.tmpint = pc.tmpint*10 + (fc - '0');
	}

	unit = '+'? [0-9]+ >StartInt $ReadInt;

	action NegSint{
		pc.tmpsint = -pc.tmpsint
	}

	action StartSint{
		pc.tmpsint = 0;
	}

	action ReadSint{
		pc.tmpsint = pc.tmpsint*10 + (fc - '0');
	}

	snit = ('-' [0-9]+ $ReadSint %NegSint | '+'? [0-9]+ $ReadSInt) >StartSint;

	action NetDouble{
		pc.tmpdouble = -pc.tmpdouble
	}

	action StartDouble{
		tmpdouble = 0;
		tmpexp = 0.1;
	}

	action ReadPreDouble{
		pc.tmpdouble = pc.tmpdouble*10 + (fc - '0');
	}

	action ReadPostDouble{
		pc.tmpdouble += pc.tmpexp * (fc - '0');
		pc.tmpexp /= 10.0;
	}

	double = ('-' [0-9]* $ReadPreDouble ('.' [0-9]* $ReadPostDouble)? %NegDouble | '+'? [0-9]* $ReadPreDouble ('.' [0-9]* $ReadPostDouble)?) >StartDouble;

	action NodeCount{
		pc.node_count = pc.tmpint;
	}

	action EdgeCount{
		pc.edge_count = pc.tmpint;
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

	fileend := (NL | SP)*;

	edge = (uint %EdgeSrc WS uint %EdgeTgt WS uint %EdgeDist WS uint %EdgeType) %EdgesCheckEnd NL;
	edges := (edge)*;

	node = (uint %NodeID WS double %NodeLat WS double %NodeLon WS sint %NodeEle) %NodesCheckEnd NL;
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

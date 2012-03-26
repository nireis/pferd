#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "structs.h"
#include "rlparser.h"

%%{
	machine graphparser;
	variable cs cs;

	action StartInt{
		tmpint = 0;
	}

	action ReadInt{
		tmpint = tmpint*10 + (fc - '0');
	}

	uint = '+'? [0-9]+ >StartInt $ReadInt;

	action NegSint{
		tmpsint = -tmpsint;
	}

	action StartSint{
		tmpsint = 0;
	}

	action ReadSint{
		tmpsint = tmpsint*10 + (fc - '0');
	}

	sint = ('-' [0-9]+ $ReadSint %NegSint | '+'? [0-9]+ $ReadSint) >StartSint;

	action NegDouble{
		tmpdouble = -tmpdouble;
	}

	action StartDouble{
		tmpdouble = 0;
		tmpexp = 0.1;
	}

	action ReadPreDouble{
		tmpdouble = tmpdouble*10 + (fc - '0');
	}

	action ReadPostDouble{
		tmpdouble += tmpexp * (fc - '0');
		tmpexp /= 10.0;
	}

	double = ('-' [0-9]* $ReadPreDouble ('.' [0-9]* $ReadPostDouble)? %NegDouble | '+'? [0-9]* $ReadPreDouble ('.' [0-9]* $ReadPostDouble)?) >StartDouble;

	action NodeCount{
		node_count = tmpint;
	}

	action EdgeCount{
		edge_count = tmpint;
	}

	action NodeID{
		nodes[current_node].id = tmpint;
	}

	action NodeLat{
		nodes[current_node].lat = tmpdouble;
	}

	action NodeLon{
		nodes[current_node].lon = tmpdouble;
	}

	action NodeEle{
		nodes[current_node].elevation = tmpsint;
	}

	action NodesCheckEnd{
		current_node++;
		if(current_node == node_count){
			fgoto edges;
		}
	}

	action EdgeSrc{
		edges[current_edge].source = tmpint;
	}

	action EdgeTgt{
		edges[current_edge].target = tmpint;
	}

	action EdgeDist{
		edges[current_edge].distance = tmpint;
	}

	action EdgeType{
		edges[current_edge].type = tmpint;
	}

	action EdgesCheckEnd{
		current_edge++;
		if(current_edge == edge_count){
			fgoto fileend;
		} 
	}

	NL = '\n' | '\r\n';
	WS = ' '+;

	fileend := (NL | WS)*;

	edge = (uint %EdgeSrc WS uint %EdgeTgt WS uint %EdgeDist WS uint %EdgeType) %EdgesCheckEnd NL;
	edges := (edge)*;

	node = (uint %NodeID WS double %NodeLat WS double %NodeLon WS sint %NodeEle) %NodesCheckEnd NL;
	data = (node)*;
	
	main := uint %NodeCount NL uint %EdgeCount NL data;
}%%

%%write data;

RlParser::RlParser(const char* filename){
	cs = 0;
	current_node = 0;
	current_edge = 0;
	f = fopen(filename, "r");
	%%write init;
}

unsigned int RlParser::getNodeCount(){
	char buf[1];
	do{
		buf[0] = fgetc(f);
		const char *p = buf;
		const char *pe = buf + 1;
		%% write exec;
	}while(buf[0] != '\n');
	return node_count;
}

unsigned int RlParser::getEdgeCount(){
	char buf[1];
	do{
		buf[0] = fgetc(f);
		const char *p = buf;
		const char *pe = buf + 1;
		%% write exec;
	}while(buf[0] != '\n');
	return edge_count;
}

void RlParser::getNodesAndEdges(ParserNode* n, ParserEdge* e){
	nodes = n;
	edges = e;
	char buf[128*1024];
	int r;

	while(0 < (r = fread(buf, sizeof(buf), 1, f))) {
		const char *p = buf;
		const char *pe = buf + r;
		%% write exec;
	}

	fclose(f);
}

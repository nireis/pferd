#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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
		std::cout << "Vor node_count" << std::endl;
		node_count = tmpint;
		nodes = new ParserNode[node_count];
	}

	action EdgeCount{
		std::cout << "Vor edge_count" << std::endl;
		edge_count = tmpint;
		edges = new ParserEdge[edge_count];
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
	fd = open(filename, O_RDONLY);
	std::cout << "Vor write init." << std::endl;
	%%write init;
	std::cout << "Nach write init." << std::endl;
}

void RlParser::run(ParserNode** n, ParserEdge** e){
	std::cout << "Anfang run()" << std::endl;
	char buf[4*1024*1024];
	int r;

	while(0 < (r = read(fd, buf, sizeof(buf)))) {
		const char *p = buf;
		const char *pe = buf + r;
		const char *eof = 0;
	
		%% write exec;
	}

	n = &nodes;
	e = &edges;

	close(fd);
}

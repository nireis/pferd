
#line 1 "rlparser.rl"
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


#line 128 "rlparser.rl"



#line 22 "rlparser.c"
static const char _graphparser_actions[] = {
	0, 1, 1, 1, 3, 1, 4, 1, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 1, 11, 1, 12, 1, 13, 1, 
	16, 1, 17, 1, 18, 2, 0, 1, 
	2, 3, 4, 2, 5, 12, 2, 5, 
	13, 2, 6, 3, 2, 6, 7, 2, 
	6, 12, 2, 6, 13, 2, 7, 4, 
	2, 12, 13, 2, 14, 15, 2, 19, 
	20, 3, 2, 14, 15, 3, 5, 12, 
	13, 3, 6, 12, 13, 4, 6, 7, 
	3, 4
};

static const unsigned char _graphparser_key_offsets[] = {
	0, 0, 3, 5, 9, 12, 14, 18, 
	20, 23, 29, 35, 41, 45, 51, 55, 
	60, 62, 66, 67, 69, 73, 76, 82, 
	86, 89, 95, 98, 104, 108, 111, 117, 
	121, 125, 129, 135, 139, 143, 146, 150, 
	153, 154, 155, 157, 160, 164, 166, 169, 
	173, 175, 178, 182, 184, 188, 189, 192, 
	195
};

static const char _graphparser_trans_keys[] = {
	43, 48, 57, 48, 57, 10, 13, 48, 
	57, 43, 48, 57, 48, 57, 10, 13, 
	48, 57, 48, 57, 32, 48, 57, 32, 
	43, 45, 46, 48, 57, 32, 43, 45, 
	46, 48, 57, 32, 43, 45, 46, 48, 
	57, 32, 46, 48, 57, 32, 43, 45, 
	46, 48, 57, 32, 46, 48, 57, 32, 
	43, 45, 48, 57, 48, 57, 10, 13, 
	48, 57, 10, 48, 57, 10, 13, 48, 
	57, 32, 48, 57, 10, 13, 32, 46, 
	48, 57, 32, 46, 48, 57, 32, 48, 
	57, 10, 13, 32, 46, 48, 57, 32, 
	48, 57, 10, 13, 32, 46, 48, 57, 
	32, 46, 48, 57, 32, 48, 57, 10, 
	13, 32, 46, 48, 57, 32, 46, 48, 
	57, 32, 46, 48, 57, 32, 46, 48, 
	57, 32, 43, 45, 46, 48, 57, 32, 
	46, 48, 57, 32, 46, 48, 57, 32, 
	48, 57, 32, 46, 48, 57, 32, 48, 
	57, 10, 10, 48, 57, 32, 48, 57, 
	32, 43, 48, 57, 48, 57, 32, 48, 
	57, 32, 43, 48, 57, 48, 57, 32, 
	48, 57, 32, 43, 48, 57, 48, 57, 
	10, 13, 48, 57, 10, 43, 48, 57, 
	10, 13, 32, 43, 48, 57, 0
};

static const char _graphparser_single_lengths[] = {
	0, 1, 0, 2, 1, 0, 2, 0, 
	1, 4, 4, 4, 2, 4, 2, 3, 
	0, 2, 1, 0, 2, 1, 4, 2, 
	1, 4, 1, 4, 2, 1, 4, 2, 
	2, 2, 4, 2, 2, 1, 2, 1, 
	1, 1, 0, 1, 2, 0, 1, 2, 
	0, 1, 2, 0, 2, 1, 1, 3, 
	1
};

static const char _graphparser_range_lengths[] = {
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	0, 0, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 0, 1, 0, 
	1
};

static const short _graphparser_index_offsets[] = {
	0, 0, 3, 5, 9, 12, 14, 18, 
	20, 23, 29, 35, 41, 45, 51, 55, 
	60, 62, 66, 68, 70, 74, 77, 83, 
	87, 90, 96, 99, 105, 109, 112, 118, 
	122, 126, 130, 136, 140, 144, 147, 151, 
	154, 156, 158, 160, 163, 167, 169, 172, 
	176, 178, 181, 185, 187, 191, 193, 196, 
	200
};

static const char _graphparser_indicies[] = {
	0, 2, 1, 2, 1, 3, 4, 5, 
	1, 6, 7, 1, 7, 1, 8, 9, 
	10, 1, 11, 1, 12, 13, 1, 14, 
	15, 16, 17, 18, 1, 19, 20, 21, 
	22, 23, 1, 19, 24, 25, 22, 26, 
	1, 27, 28, 29, 1, 30, 31, 32, 
	33, 34, 1, 35, 36, 37, 1, 38, 
	39, 40, 41, 1, 42, 1, 43, 44, 
	42, 1, 45, 1, 46, 1, 47, 48, 
	46, 1, 35, 49, 1, 43, 44, 35, 
	36, 37, 1, 50, 51, 52, 1, 50, 
	53, 1, 47, 48, 50, 51, 52, 1, 
	27, 54, 1, 43, 44, 27, 28, 29, 
	1, 55, 56, 57, 1, 55, 58, 1, 
	47, 48, 55, 56, 57, 1, 27, 28, 
	59, 1, 55, 56, 60, 1, 61, 62, 
	63, 1, 30, 64, 65, 33, 66, 1, 
	35, 36, 67, 1, 50, 51, 68, 1, 
	61, 69, 1, 70, 71, 72, 1, 70, 
	73, 1, 74, 1, 75, 1, 76, 1, 
	77, 78, 1, 79, 80, 81, 1, 81, 
	1, 82, 83, 1, 84, 85, 86, 1, 
	86, 1, 87, 88, 1, 89, 90, 91, 
	1, 91, 1, 92, 93, 94, 1, 95, 
	1, 96, 11, 1, 75, 97, 75, 1, 
	98, 76, 1, 0
};

static const char _graphparser_trans_targs[] = {
	2, 0, 3, 4, 40, 3, 5, 6, 
	54, 18, 6, 8, 9, 8, 10, 33, 
	38, 37, 33, 11, 31, 32, 26, 31, 
	12, 28, 27, 13, 26, 27, 13, 14, 
	23, 21, 22, 15, 21, 22, 15, 16, 
	19, 17, 17, 54, 18, 54, 20, 54, 
	18, 21, 15, 24, 25, 24, 26, 13, 
	29, 30, 29, 31, 32, 34, 37, 33, 
	35, 36, 35, 35, 36, 37, 34, 39, 
	38, 39, 4, 55, 43, 44, 43, 44, 
	45, 46, 47, 46, 47, 48, 49, 50, 
	49, 50, 51, 52, 56, 53, 52, 56, 
	7, 41, 42
};

static const char _graphparser_trans_actions[] = {
	0, 0, 29, 13, 13, 1, 0, 29, 
	15, 15, 1, 29, 17, 1, 47, 7, 
	7, 7, 44, 73, 7, 7, 7, 44, 
	41, 41, 77, 56, 0, 53, 50, 41, 
	41, 7, 77, 21, 0, 53, 0, 3, 
	3, 32, 5, 59, 59, 0, 5, 65, 
	65, 11, 38, 0, 53, 11, 11, 69, 
	0, 53, 11, 9, 9, 19, 0, 9, 
	7, 7, 44, 9, 9, 11, 35, 0, 
	9, 11, 0, 0, 29, 23, 1, 0, 
	0, 29, 25, 1, 0, 0, 29, 27, 
	1, 0, 0, 29, 62, 62, 1, 0, 
	0, 0, 0
};

static const int graphparser_start = 1;
static const int graphparser_first_final = 54;
static const int graphparser_error = 0;

static const int graphparser_en_fileend = 55;
static const int graphparser_en_edges = 56;
static const int graphparser_en_main = 1;


#line 131 "rlparser.rl"

RlParser::RlParser(const char* filename){
	cs = 0;
	current_node = 0;
	current_edge = 0;
	fd = open(filename, O_RDONLY);
	
#line 187 "rlparser.c"
	{
	( cs) = graphparser_start;
	}

#line 138 "rlparser.rl"
}

unsigned int RlParser::getNodeCount(){
	char buf[1];
	int r;
	do{
		r = read(fd, buf, sizeof(buf));
		const char *p = buf;
		const char *pe = buf + 1;
		const char *eof = 0;
		
#line 204 "rlparser.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( ( cs) == 0 )
		goto _out;
_resume:
	_keys = _graphparser_trans_keys + _graphparser_key_offsets[( cs)];
	_trans = _graphparser_index_offsets[( cs)];

	_klen = _graphparser_single_lengths[( cs)];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _graphparser_range_lengths[( cs)];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _graphparser_indicies[_trans];
	( cs) = _graphparser_trans_targs[_trans];

	if ( _graphparser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _graphparser_actions + _graphparser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
	break;
	case 1:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	break;
	case 2:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
	break;
	case 3:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	break;
	case 4:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	break;
	case 5:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
	break;
	case 6:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	break;
	case 7:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	break;
	case 8:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	break;
	case 9:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	break;
	case 10:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	break;
	case 11:
#line 70 "rlparser.rl"
	{
		nodes[current_node].id = tmpint;
	}
	break;
	case 12:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	break;
	case 13:
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	break;
	case 14:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
	break;
	case 15:
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{( cs) = 56; goto _again;}
		}
	}
	break;
	case 16:
#line 93 "rlparser.rl"
	{
		edges[current_edge].source = tmpint;
	}
	break;
	case 17:
#line 97 "rlparser.rl"
	{
		edges[current_edge].target = tmpint;
	}
	break;
	case 18:
#line 101 "rlparser.rl"
	{
		edges[current_edge].distance = tmpint;
	}
	break;
	case 19:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
	break;
	case 20:
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{( cs) = 55; goto _again;}
		} 
	}
	break;
#line 412 "rlparser.c"
		}
	}

_again:
	if ( ( cs) == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 149 "rlparser.rl"
	}while(buf[0] != '\n' && r > 0);
	return node_count;
}

unsigned int RlParser::getEdgeCount(){
	char buf[1];
	int r;
	do{
		r = read(fd, buf, sizeof(buf));
		const char *p = buf;
		const char *pe = buf + 1;
		const char *eof = 0;
		
#line 439 "rlparser.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( ( cs) == 0 )
		goto _out;
_resume:
	_keys = _graphparser_trans_keys + _graphparser_key_offsets[( cs)];
	_trans = _graphparser_index_offsets[( cs)];

	_klen = _graphparser_single_lengths[( cs)];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _graphparser_range_lengths[( cs)];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _graphparser_indicies[_trans];
	( cs) = _graphparser_trans_targs[_trans];

	if ( _graphparser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _graphparser_actions + _graphparser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
	break;
	case 1:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	break;
	case 2:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
	break;
	case 3:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	break;
	case 4:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	break;
	case 5:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
	break;
	case 6:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	break;
	case 7:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	break;
	case 8:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	break;
	case 9:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	break;
	case 10:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	break;
	case 11:
#line 70 "rlparser.rl"
	{
		nodes[current_node].id = tmpint;
	}
	break;
	case 12:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	break;
	case 13:
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	break;
	case 14:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
	break;
	case 15:
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{( cs) = 56; goto _again;}
		}
	}
	break;
	case 16:
#line 93 "rlparser.rl"
	{
		edges[current_edge].source = tmpint;
	}
	break;
	case 17:
#line 97 "rlparser.rl"
	{
		edges[current_edge].target = tmpint;
	}
	break;
	case 18:
#line 101 "rlparser.rl"
	{
		edges[current_edge].distance = tmpint;
	}
	break;
	case 19:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
	break;
	case 20:
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{( cs) = 55; goto _again;}
		} 
	}
	break;
#line 647 "rlparser.c"
		}
	}

_again:
	if ( ( cs) == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 162 "rlparser.rl"
	}while(buf[0] != '\n' && r > 0);
	return edge_count;
}

void RlParser::getNodesAndEdges(ParserNode* n, ParserEdge* e){
	nodes = n;
	edges = e;
	char buf[128*1024];
	int r;

	while(0 < (r = read(fd, buf, sizeof(buf)))) {
		const char *p = buf;
		const char *pe = buf + r;
		const char *eof = 0;
		
#line 676 "rlparser.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( ( cs) == 0 )
		goto _out;
_resume:
	_keys = _graphparser_trans_keys + _graphparser_key_offsets[( cs)];
	_trans = _graphparser_index_offsets[( cs)];

	_klen = _graphparser_single_lengths[( cs)];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _graphparser_range_lengths[( cs)];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _graphparser_indicies[_trans];
	( cs) = _graphparser_trans_targs[_trans];

	if ( _graphparser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _graphparser_actions + _graphparser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
	break;
	case 1:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	break;
	case 2:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
	break;
	case 3:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	break;
	case 4:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	break;
	case 5:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
	break;
	case 6:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	break;
	case 7:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	break;
	case 8:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	break;
	case 9:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	break;
	case 10:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	break;
	case 11:
#line 70 "rlparser.rl"
	{
		nodes[current_node].id = tmpint;
	}
	break;
	case 12:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	break;
	case 13:
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	break;
	case 14:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
	break;
	case 15:
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{( cs) = 56; goto _again;}
		}
	}
	break;
	case 16:
#line 93 "rlparser.rl"
	{
		edges[current_edge].source = tmpint;
	}
	break;
	case 17:
#line 97 "rlparser.rl"
	{
		edges[current_edge].target = tmpint;
	}
	break;
	case 18:
#line 101 "rlparser.rl"
	{
		edges[current_edge].distance = tmpint;
	}
	break;
	case 19:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
	break;
	case 20:
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{( cs) = 55; goto _again;}
		} 
	}
	break;
#line 884 "rlparser.c"
		}
	}

_again:
	if ( ( cs) == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 177 "rlparser.rl"
	}

	close(fd);
}

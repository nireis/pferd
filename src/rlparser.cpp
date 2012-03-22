
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



#line 22 "rlparser.cpp"
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
	
#line 40 "rlparser.cpp"
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
		
#line 56 "rlparser.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( ( cs) )
	{
case 1:
	if ( (*p) == 43 )
		goto st2;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
st0:
( cs) = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st3;
tr5:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 98 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr3;
		case 13: goto tr4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr3:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 116 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st5;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st6;
tr10:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 149 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr8;
		case 13: goto tr9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr10;
	goto st0;
tr8:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	goto st54;
tr43:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st54;
tr47:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 197 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
tr11:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st8;
tr13:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 230 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr12:
#line 70 "rlparser.rl"
	{
		nodes[current_node].id = tmpint;
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 246 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr14;
		case 43: goto tr15;
		case 45: goto tr16;
		case 46: goto tr17;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr18;
	goto st0;
tr14:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 271 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr19;
		case 43: goto tr20;
		case 45: goto tr21;
		case 46: goto tr22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr23;
	goto st0;
tr19:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 300 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr19;
		case 43: goto tr24;
		case 45: goto tr25;
		case 46: goto tr22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr24:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 325 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr27:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
tr30:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
tr55:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 372 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr30;
		case 43: goto tr31;
		case 45: goto tr32;
		case 46: goto tr33;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr34;
	goto st0;
tr31:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 397 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
tr35:
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st15;
tr50:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 425 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st15;
		case 43: goto tr39;
		case 45: goto tr40;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr41;
	goto st0;
tr39:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 444 "rlparser.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr41:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st17;
tr42:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 468 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr9:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	goto st18;
tr44:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st18;
tr48:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 516 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st54;
	goto st0;
tr40:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 530 "rlparser.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr46:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 544 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr33:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st21;
tr49:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 570 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr35;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr34:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st22;
tr37:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 609 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
tr32:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 634 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr53:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 653 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr50;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr52:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 673 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr22:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st26;
tr54:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 701 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr27;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr26:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st27;
tr29:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 740 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr25:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 765 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr58:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 784 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr55;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr58;
	goto st0;
tr57:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 804 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr20:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st31;
tr23:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st31;
tr59:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 842 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr59;
	goto st0;
tr21:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st32;
tr60:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 867 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr60;
	goto st0;
tr15:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st33;
tr18:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st33;
tr63:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 903 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr61;
		case 46: goto st37;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr63;
	goto st0;
tr61:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st34;
tr70:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 931 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr30;
		case 43: goto tr64;
		case 45: goto tr65;
		case 46: goto tr33;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr66;
	goto st0;
tr64:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st35;
tr66:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st35;
tr67:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 969 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr67;
	goto st0;
tr65:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st36;
tr68:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 994 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr68;
	goto st0;
tr17:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st37;
tr69:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1020 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr61;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr16:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st38;
tr72:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1043 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr70;
		case 46: goto st39;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr72;
	goto st0;
tr73:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1062 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr70;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr4:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1078 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st4;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 10: goto st55;
		case 13: goto st41;
		case 32: goto st55;
	}
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 10 )
		goto st55;
	goto st0;
tr92:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{goto st55;}
		} 
	}
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 1116 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st42;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
tr76:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st43;
tr78:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 1149 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr77;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr78;
	goto st0;
tr77:
#line 93 "rlparser.rl"
	{
		edges[current_edge].source = tmpint;
	}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 1165 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st44;
		case 43: goto st45;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
tr81:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st46;
tr83:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 1200 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr82;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto st0;
tr82:
#line 97 "rlparser.rl"
	{
		edges[current_edge].target = tmpint;
	}
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 1216 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st47;
		case 43: goto st48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr86;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr86;
	goto st0;
tr86:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st49;
tr88:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 1251 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr87;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr88;
	goto st0;
tr87:
#line 101 "rlparser.rl"
	{
		edges[current_edge].distance = tmpint;
	}
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 1267 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st50;
		case 43: goto st51;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr91;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr91;
	goto st0;
tr91:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st52;
tr94:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 1302 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr92;
		case 13: goto tr93;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto st0;
tr93:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{goto st55;}
		} 
	}
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 1327 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st56;
	goto st0;
	}
	_test_eof2: ( cs) = 2; goto _test_eof; 
	_test_eof3: ( cs) = 3; goto _test_eof; 
	_test_eof4: ( cs) = 4; goto _test_eof; 
	_test_eof5: ( cs) = 5; goto _test_eof; 
	_test_eof6: ( cs) = 6; goto _test_eof; 
	_test_eof54: ( cs) = 54; goto _test_eof; 
	_test_eof7: ( cs) = 7; goto _test_eof; 
	_test_eof8: ( cs) = 8; goto _test_eof; 
	_test_eof9: ( cs) = 9; goto _test_eof; 
	_test_eof10: ( cs) = 10; goto _test_eof; 
	_test_eof11: ( cs) = 11; goto _test_eof; 
	_test_eof12: ( cs) = 12; goto _test_eof; 
	_test_eof13: ( cs) = 13; goto _test_eof; 
	_test_eof14: ( cs) = 14; goto _test_eof; 
	_test_eof15: ( cs) = 15; goto _test_eof; 
	_test_eof16: ( cs) = 16; goto _test_eof; 
	_test_eof17: ( cs) = 17; goto _test_eof; 
	_test_eof18: ( cs) = 18; goto _test_eof; 
	_test_eof19: ( cs) = 19; goto _test_eof; 
	_test_eof20: ( cs) = 20; goto _test_eof; 
	_test_eof21: ( cs) = 21; goto _test_eof; 
	_test_eof22: ( cs) = 22; goto _test_eof; 
	_test_eof23: ( cs) = 23; goto _test_eof; 
	_test_eof24: ( cs) = 24; goto _test_eof; 
	_test_eof25: ( cs) = 25; goto _test_eof; 
	_test_eof26: ( cs) = 26; goto _test_eof; 
	_test_eof27: ( cs) = 27; goto _test_eof; 
	_test_eof28: ( cs) = 28; goto _test_eof; 
	_test_eof29: ( cs) = 29; goto _test_eof; 
	_test_eof30: ( cs) = 30; goto _test_eof; 
	_test_eof31: ( cs) = 31; goto _test_eof; 
	_test_eof32: ( cs) = 32; goto _test_eof; 
	_test_eof33: ( cs) = 33; goto _test_eof; 
	_test_eof34: ( cs) = 34; goto _test_eof; 
	_test_eof35: ( cs) = 35; goto _test_eof; 
	_test_eof36: ( cs) = 36; goto _test_eof; 
	_test_eof37: ( cs) = 37; goto _test_eof; 
	_test_eof38: ( cs) = 38; goto _test_eof; 
	_test_eof39: ( cs) = 39; goto _test_eof; 
	_test_eof40: ( cs) = 40; goto _test_eof; 
	_test_eof55: ( cs) = 55; goto _test_eof; 
	_test_eof41: ( cs) = 41; goto _test_eof; 
	_test_eof56: ( cs) = 56; goto _test_eof; 
	_test_eof42: ( cs) = 42; goto _test_eof; 
	_test_eof43: ( cs) = 43; goto _test_eof; 
	_test_eof44: ( cs) = 44; goto _test_eof; 
	_test_eof45: ( cs) = 45; goto _test_eof; 
	_test_eof46: ( cs) = 46; goto _test_eof; 
	_test_eof47: ( cs) = 47; goto _test_eof; 
	_test_eof48: ( cs) = 48; goto _test_eof; 
	_test_eof49: ( cs) = 49; goto _test_eof; 
	_test_eof50: ( cs) = 50; goto _test_eof; 
	_test_eof51: ( cs) = 51; goto _test_eof; 
	_test_eof52: ( cs) = 52; goto _test_eof; 
	_test_eof53: ( cs) = 53; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 148 "rlparser.rl"
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
		
#line 1405 "rlparser.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( ( cs) )
	{
case 1:
	if ( (*p) == 43 )
		goto st2;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
st0:
( cs) = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st3;
tr5:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1447 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr3;
		case 13: goto tr4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr3:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1465 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st5;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st6;
tr10:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1498 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr8;
		case 13: goto tr9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr10;
	goto st0;
tr8:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	goto st54;
tr43:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st54;
tr47:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 1546 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
tr11:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st8;
tr13:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 1579 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr12:
#line 70 "rlparser.rl"
	{
		nodes[current_node].id = tmpint;
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 1595 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr14;
		case 43: goto tr15;
		case 45: goto tr16;
		case 46: goto tr17;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr18;
	goto st0;
tr14:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 1620 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr19;
		case 43: goto tr20;
		case 45: goto tr21;
		case 46: goto tr22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr23;
	goto st0;
tr19:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 1649 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr19;
		case 43: goto tr24;
		case 45: goto tr25;
		case 46: goto tr22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr24:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 1674 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr27:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
tr30:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
tr55:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 1721 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr30;
		case 43: goto tr31;
		case 45: goto tr32;
		case 46: goto tr33;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr34;
	goto st0;
tr31:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 1746 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
tr35:
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st15;
tr50:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 1774 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st15;
		case 43: goto tr39;
		case 45: goto tr40;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr41;
	goto st0;
tr39:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 1793 "rlparser.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr41:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st17;
tr42:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 1817 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr9:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	goto st18;
tr44:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st18;
tr48:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 1865 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st54;
	goto st0;
tr40:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 1879 "rlparser.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr46:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 1893 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr33:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st21;
tr49:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 1919 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr35;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr34:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st22;
tr37:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 1958 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
tr32:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 1983 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr53:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 2002 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr50;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr52:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 2022 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr22:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st26;
tr54:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 2050 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr27;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr26:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st27;
tr29:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 2089 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr25:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 2114 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr58:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 2133 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr55;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr58;
	goto st0;
tr57:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 2153 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr20:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st31;
tr23:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st31;
tr59:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 2191 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr59;
	goto st0;
tr21:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st32;
tr60:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 2216 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr60;
	goto st0;
tr15:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st33;
tr18:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st33;
tr63:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 2252 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr61;
		case 46: goto st37;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr63;
	goto st0;
tr61:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st34;
tr70:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 2280 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr30;
		case 43: goto tr64;
		case 45: goto tr65;
		case 46: goto tr33;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr66;
	goto st0;
tr64:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st35;
tr66:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st35;
tr67:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 2318 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr67;
	goto st0;
tr65:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st36;
tr68:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 2343 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr68;
	goto st0;
tr17:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st37;
tr69:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 2369 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr61;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr16:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st38;
tr72:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 2392 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr70;
		case 46: goto st39;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr72;
	goto st0;
tr73:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 2411 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr70;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr4:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 2427 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st4;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 10: goto st55;
		case 13: goto st41;
		case 32: goto st55;
	}
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 10 )
		goto st55;
	goto st0;
tr92:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{goto st55;}
		} 
	}
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 2465 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st42;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
tr76:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st43;
tr78:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 2498 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr77;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr78;
	goto st0;
tr77:
#line 93 "rlparser.rl"
	{
		edges[current_edge].source = tmpint;
	}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 2514 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st44;
		case 43: goto st45;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
tr81:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st46;
tr83:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 2549 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr82;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto st0;
tr82:
#line 97 "rlparser.rl"
	{
		edges[current_edge].target = tmpint;
	}
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 2565 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st47;
		case 43: goto st48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr86;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr86;
	goto st0;
tr86:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st49;
tr88:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 2600 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr87;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr88;
	goto st0;
tr87:
#line 101 "rlparser.rl"
	{
		edges[current_edge].distance = tmpint;
	}
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 2616 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st50;
		case 43: goto st51;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr91;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr91;
	goto st0;
tr91:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st52;
tr94:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 2651 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr92;
		case 13: goto tr93;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto st0;
tr93:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{goto st55;}
		} 
	}
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 2676 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st56;
	goto st0;
	}
	_test_eof2: ( cs) = 2; goto _test_eof; 
	_test_eof3: ( cs) = 3; goto _test_eof; 
	_test_eof4: ( cs) = 4; goto _test_eof; 
	_test_eof5: ( cs) = 5; goto _test_eof; 
	_test_eof6: ( cs) = 6; goto _test_eof; 
	_test_eof54: ( cs) = 54; goto _test_eof; 
	_test_eof7: ( cs) = 7; goto _test_eof; 
	_test_eof8: ( cs) = 8; goto _test_eof; 
	_test_eof9: ( cs) = 9; goto _test_eof; 
	_test_eof10: ( cs) = 10; goto _test_eof; 
	_test_eof11: ( cs) = 11; goto _test_eof; 
	_test_eof12: ( cs) = 12; goto _test_eof; 
	_test_eof13: ( cs) = 13; goto _test_eof; 
	_test_eof14: ( cs) = 14; goto _test_eof; 
	_test_eof15: ( cs) = 15; goto _test_eof; 
	_test_eof16: ( cs) = 16; goto _test_eof; 
	_test_eof17: ( cs) = 17; goto _test_eof; 
	_test_eof18: ( cs) = 18; goto _test_eof; 
	_test_eof19: ( cs) = 19; goto _test_eof; 
	_test_eof20: ( cs) = 20; goto _test_eof; 
	_test_eof21: ( cs) = 21; goto _test_eof; 
	_test_eof22: ( cs) = 22; goto _test_eof; 
	_test_eof23: ( cs) = 23; goto _test_eof; 
	_test_eof24: ( cs) = 24; goto _test_eof; 
	_test_eof25: ( cs) = 25; goto _test_eof; 
	_test_eof26: ( cs) = 26; goto _test_eof; 
	_test_eof27: ( cs) = 27; goto _test_eof; 
	_test_eof28: ( cs) = 28; goto _test_eof; 
	_test_eof29: ( cs) = 29; goto _test_eof; 
	_test_eof30: ( cs) = 30; goto _test_eof; 
	_test_eof31: ( cs) = 31; goto _test_eof; 
	_test_eof32: ( cs) = 32; goto _test_eof; 
	_test_eof33: ( cs) = 33; goto _test_eof; 
	_test_eof34: ( cs) = 34; goto _test_eof; 
	_test_eof35: ( cs) = 35; goto _test_eof; 
	_test_eof36: ( cs) = 36; goto _test_eof; 
	_test_eof37: ( cs) = 37; goto _test_eof; 
	_test_eof38: ( cs) = 38; goto _test_eof; 
	_test_eof39: ( cs) = 39; goto _test_eof; 
	_test_eof40: ( cs) = 40; goto _test_eof; 
	_test_eof55: ( cs) = 55; goto _test_eof; 
	_test_eof41: ( cs) = 41; goto _test_eof; 
	_test_eof56: ( cs) = 56; goto _test_eof; 
	_test_eof42: ( cs) = 42; goto _test_eof; 
	_test_eof43: ( cs) = 43; goto _test_eof; 
	_test_eof44: ( cs) = 44; goto _test_eof; 
	_test_eof45: ( cs) = 45; goto _test_eof; 
	_test_eof46: ( cs) = 46; goto _test_eof; 
	_test_eof47: ( cs) = 47; goto _test_eof; 
	_test_eof48: ( cs) = 48; goto _test_eof; 
	_test_eof49: ( cs) = 49; goto _test_eof; 
	_test_eof50: ( cs) = 50; goto _test_eof; 
	_test_eof51: ( cs) = 51; goto _test_eof; 
	_test_eof52: ( cs) = 52; goto _test_eof; 
	_test_eof53: ( cs) = 53; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 160 "rlparser.rl"
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
		
#line 2756 "rlparser.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( ( cs) )
	{
case 1:
	if ( (*p) == 43 )
		goto st2;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
st0:
( cs) = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st3;
tr5:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 2798 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr3;
		case 13: goto tr4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr3:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2816 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st5;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st6;
tr10:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2849 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr8;
		case 13: goto tr9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr10;
	goto st0;
tr8:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	goto st54;
tr43:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st54;
tr47:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 2897 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
tr11:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st8;
tr13:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 2930 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr12:
#line 70 "rlparser.rl"
	{
		nodes[current_node].id = tmpint;
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 2946 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr14;
		case 43: goto tr15;
		case 45: goto tr16;
		case 46: goto tr17;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr18;
	goto st0;
tr14:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 2971 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr19;
		case 43: goto tr20;
		case 45: goto tr21;
		case 46: goto tr22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr23;
	goto st0;
tr19:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 3000 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr19;
		case 43: goto tr24;
		case 45: goto tr25;
		case 46: goto tr22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr24:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 3025 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr27:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
tr30:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
tr55:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 3072 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr30;
		case 43: goto tr31;
		case 45: goto tr32;
		case 46: goto tr33;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr34;
	goto st0;
tr31:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 3097 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
tr35:
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st15;
tr50:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 78 "rlparser.rl"
	{
		nodes[current_node].lon = tmpdouble;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 3125 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st15;
		case 43: goto tr39;
		case 45: goto tr40;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr41;
	goto st0;
tr39:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 3144 "rlparser.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr41:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st17;
tr42:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 3168 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr9:
#line 66 "rlparser.rl"
	{
		edge_count = tmpint;
	}
	goto st18;
tr44:
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st18;
tr48:
#line 28 "rlparser.rl"
	{
		tmpsint = -tmpsint;
	}
#line 82 "rlparser.rl"
	{
		nodes[current_node].elevation = tmpsint;
	}
#line 86 "rlparser.rl"
	{
		current_node++;
		if(current_node == node_count){
			{goto st56;}
		}
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 3216 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st54;
	goto st0;
tr40:
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 3230 "rlparser.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr46:
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 3244 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr33:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st21;
tr49:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 3270 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr35;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr34:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st22;
tr37:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 3309 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
tr32:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 3334 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr53:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 3353 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr50;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr52:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 3373 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr22:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st26;
tr54:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 3401 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr27;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr26:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st27;
tr29:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 3440 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr43;
		case 13: goto tr44;
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr25:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 32 "rlparser.rl"
	{
		tmpsint = 0;
	}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 3465 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr58:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 3484 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr55;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr58;
	goto st0;
tr57:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
#line 36 "rlparser.rl"
	{
		tmpsint = tmpsint*10 + ((*p) - '0');
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 3504 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto tr48;
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr20:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st31;
tr23:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st31;
tr59:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 3542 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr27;
		case 46: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr59;
	goto st0;
tr21:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st32;
tr60:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 3567 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr55;
		case 46: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr60;
	goto st0;
tr15:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st33;
tr18:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st33;
tr63:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 3603 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr61;
		case 46: goto st37;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr63;
	goto st0;
tr61:
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st34;
tr70:
#line 42 "rlparser.rl"
	{
		tmpdouble = -tmpdouble;
	}
#line 74 "rlparser.rl"
	{
		nodes[current_node].lat = tmpdouble;
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 3631 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr30;
		case 43: goto tr64;
		case 45: goto tr65;
		case 46: goto tr33;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr66;
	goto st0;
tr64:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st35;
tr66:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st35;
tr67:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 3669 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr35;
		case 46: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr67;
	goto st0;
tr65:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st36;
tr68:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 3694 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr50;
		case 46: goto st24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr68;
	goto st0;
tr17:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st37;
tr69:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 3720 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr61;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr16:
#line 46 "rlparser.rl"
	{
		tmpdouble = 0;
		tmpexp = 0.1;
	}
	goto st38;
tr72:
#line 51 "rlparser.rl"
	{
		tmpdouble = tmpdouble*10 + ((*p) - '0');
	}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 3743 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto tr70;
		case 46: goto st39;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr72;
	goto st0;
tr73:
#line 55 "rlparser.rl"
	{
		tmpdouble += tmpexp * ((*p) - '0');
		tmpexp /= 10.0;
	}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 3762 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr70;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr4:
#line 62 "rlparser.rl"
	{
		node_count = tmpint;
	}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 3778 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st4;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 10: goto st55;
		case 13: goto st41;
		case 32: goto st55;
	}
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 10 )
		goto st55;
	goto st0;
tr92:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{goto st55;}
		} 
	}
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 3816 "rlparser.cpp"
	if ( (*p) == 43 )
		goto st42;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
tr76:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st43;
tr78:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 3849 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr77;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr78;
	goto st0;
tr77:
#line 93 "rlparser.rl"
	{
		edges[current_edge].source = tmpint;
	}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 3865 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st44;
		case 43: goto st45;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
tr81:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st46;
tr83:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 3900 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr82;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto st0;
tr82:
#line 97 "rlparser.rl"
	{
		edges[current_edge].target = tmpint;
	}
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 3916 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st47;
		case 43: goto st48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr86;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr86;
	goto st0;
tr86:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st49;
tr88:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 3951 "rlparser.cpp"
	if ( (*p) == 32 )
		goto tr87;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr88;
	goto st0;
tr87:
#line 101 "rlparser.rl"
	{
		edges[current_edge].distance = tmpint;
	}
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 3967 "rlparser.cpp"
	switch( (*p) ) {
		case 32: goto st50;
		case 43: goto st51;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr91;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr91;
	goto st0;
tr91:
#line 18 "rlparser.rl"
	{
		tmpint = 0;
	}
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st52;
tr94:
#line 22 "rlparser.rl"
	{
		tmpint = tmpint*10 + ((*p) - '0');
	}
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 4002 "rlparser.cpp"
	switch( (*p) ) {
		case 10: goto tr92;
		case 13: goto tr93;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto st0;
tr93:
#line 105 "rlparser.rl"
	{
		edges[current_edge].type = tmpint;
	}
#line 109 "rlparser.rl"
	{
		current_edge++;
		if(current_edge == edge_count){
			{goto st55;}
		} 
	}
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 4027 "rlparser.cpp"
	if ( (*p) == 10 )
		goto st56;
	goto st0;
	}
	_test_eof2: ( cs) = 2; goto _test_eof; 
	_test_eof3: ( cs) = 3; goto _test_eof; 
	_test_eof4: ( cs) = 4; goto _test_eof; 
	_test_eof5: ( cs) = 5; goto _test_eof; 
	_test_eof6: ( cs) = 6; goto _test_eof; 
	_test_eof54: ( cs) = 54; goto _test_eof; 
	_test_eof7: ( cs) = 7; goto _test_eof; 
	_test_eof8: ( cs) = 8; goto _test_eof; 
	_test_eof9: ( cs) = 9; goto _test_eof; 
	_test_eof10: ( cs) = 10; goto _test_eof; 
	_test_eof11: ( cs) = 11; goto _test_eof; 
	_test_eof12: ( cs) = 12; goto _test_eof; 
	_test_eof13: ( cs) = 13; goto _test_eof; 
	_test_eof14: ( cs) = 14; goto _test_eof; 
	_test_eof15: ( cs) = 15; goto _test_eof; 
	_test_eof16: ( cs) = 16; goto _test_eof; 
	_test_eof17: ( cs) = 17; goto _test_eof; 
	_test_eof18: ( cs) = 18; goto _test_eof; 
	_test_eof19: ( cs) = 19; goto _test_eof; 
	_test_eof20: ( cs) = 20; goto _test_eof; 
	_test_eof21: ( cs) = 21; goto _test_eof; 
	_test_eof22: ( cs) = 22; goto _test_eof; 
	_test_eof23: ( cs) = 23; goto _test_eof; 
	_test_eof24: ( cs) = 24; goto _test_eof; 
	_test_eof25: ( cs) = 25; goto _test_eof; 
	_test_eof26: ( cs) = 26; goto _test_eof; 
	_test_eof27: ( cs) = 27; goto _test_eof; 
	_test_eof28: ( cs) = 28; goto _test_eof; 
	_test_eof29: ( cs) = 29; goto _test_eof; 
	_test_eof30: ( cs) = 30; goto _test_eof; 
	_test_eof31: ( cs) = 31; goto _test_eof; 
	_test_eof32: ( cs) = 32; goto _test_eof; 
	_test_eof33: ( cs) = 33; goto _test_eof; 
	_test_eof34: ( cs) = 34; goto _test_eof; 
	_test_eof35: ( cs) = 35; goto _test_eof; 
	_test_eof36: ( cs) = 36; goto _test_eof; 
	_test_eof37: ( cs) = 37; goto _test_eof; 
	_test_eof38: ( cs) = 38; goto _test_eof; 
	_test_eof39: ( cs) = 39; goto _test_eof; 
	_test_eof40: ( cs) = 40; goto _test_eof; 
	_test_eof55: ( cs) = 55; goto _test_eof; 
	_test_eof41: ( cs) = 41; goto _test_eof; 
	_test_eof56: ( cs) = 56; goto _test_eof; 
	_test_eof42: ( cs) = 42; goto _test_eof; 
	_test_eof43: ( cs) = 43; goto _test_eof; 
	_test_eof44: ( cs) = 44; goto _test_eof; 
	_test_eof45: ( cs) = 45; goto _test_eof; 
	_test_eof46: ( cs) = 46; goto _test_eof; 
	_test_eof47: ( cs) = 47; goto _test_eof; 
	_test_eof48: ( cs) = 48; goto _test_eof; 
	_test_eof49: ( cs) = 49; goto _test_eof; 
	_test_eof50: ( cs) = 50; goto _test_eof; 
	_test_eof51: ( cs) = 51; goto _test_eof; 
	_test_eof52: ( cs) = 52; goto _test_eof; 
	_test_eof53: ( cs) = 53; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 174 "rlparser.rl"
	}

	close(fd);
}

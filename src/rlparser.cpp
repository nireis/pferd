
#line 1 "rlparser.rl"
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
	f = fopen(filename, "r");
	
#line 40 "rlparser.cpp"
	{
	( cs) = graphparser_start;
	}

#line 138 "rlparser.rl"
}

unsigned int RlParser::getNodeCount(){
	char buf[1];
	do{
		buf[0] = fgetc(f);
		const char *p = buf;
		const char *pe = buf + 1;
		
#line 55 "rlparser.cpp"
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
#line 97 "rlparser.cpp"
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
#line 115 "rlparser.cpp"
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
#line 148 "rlparser.cpp"
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
#line 196 "rlparser.cpp"
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
#line 229 "rlparser.cpp"
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
#line 245 "rlparser.cpp"
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
#line 270 "rlparser.cpp"
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
#line 299 "rlparser.cpp"
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
#line 324 "rlparser.cpp"
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
#line 371 "rlparser.cpp"
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
#line 396 "rlparser.cpp"
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
#line 424 "rlparser.cpp"
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
#line 443 "rlparser.cpp"
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
#line 467 "rlparser.cpp"
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
#line 515 "rlparser.cpp"
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
#line 529 "rlparser.cpp"
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
#line 543 "rlparser.cpp"
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
#line 569 "rlparser.cpp"
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
#line 608 "rlparser.cpp"
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
#line 633 "rlparser.cpp"
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
#line 652 "rlparser.cpp"
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
#line 672 "rlparser.cpp"
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
#line 700 "rlparser.cpp"
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
#line 739 "rlparser.cpp"
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
#line 764 "rlparser.cpp"
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
#line 783 "rlparser.cpp"
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
#line 803 "rlparser.cpp"
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
#line 841 "rlparser.cpp"
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
#line 866 "rlparser.cpp"
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
#line 902 "rlparser.cpp"
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
#line 930 "rlparser.cpp"
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
#line 968 "rlparser.cpp"
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
#line 993 "rlparser.cpp"
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
#line 1019 "rlparser.cpp"
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
#line 1042 "rlparser.cpp"
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
#line 1061 "rlparser.cpp"
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
#line 1077 "rlparser.cpp"
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
#line 1115 "rlparser.cpp"
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
#line 1148 "rlparser.cpp"
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
#line 1164 "rlparser.cpp"
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
#line 1199 "rlparser.cpp"
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
#line 1215 "rlparser.cpp"
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
#line 1250 "rlparser.cpp"
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
#line 1266 "rlparser.cpp"
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
#line 1301 "rlparser.cpp"
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
#line 1326 "rlparser.cpp"
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

#line 147 "rlparser.rl"
	}while(buf[0] != '\n');
	return node_count;
}

unsigned int RlParser::getEdgeCount(){
	char buf[1];
	do{
		buf[0] = fgetc(f);
		const char *p = buf;
		const char *pe = buf + 1;
		
#line 1403 "rlparser.cpp"
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
#line 1445 "rlparser.cpp"
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
#line 1463 "rlparser.cpp"
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
#line 1496 "rlparser.cpp"
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
#line 1544 "rlparser.cpp"
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
#line 1577 "rlparser.cpp"
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
#line 1593 "rlparser.cpp"
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
#line 1618 "rlparser.cpp"
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
#line 1647 "rlparser.cpp"
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
#line 1672 "rlparser.cpp"
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
#line 1719 "rlparser.cpp"
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
#line 1744 "rlparser.cpp"
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
#line 1772 "rlparser.cpp"
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
#line 1791 "rlparser.cpp"
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
#line 1815 "rlparser.cpp"
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
#line 1863 "rlparser.cpp"
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
#line 1877 "rlparser.cpp"
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
#line 1891 "rlparser.cpp"
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
#line 1917 "rlparser.cpp"
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
#line 1956 "rlparser.cpp"
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
#line 1981 "rlparser.cpp"
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
#line 2000 "rlparser.cpp"
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
#line 2020 "rlparser.cpp"
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
#line 2048 "rlparser.cpp"
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
#line 2087 "rlparser.cpp"
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
#line 2112 "rlparser.cpp"
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
#line 2131 "rlparser.cpp"
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
#line 2151 "rlparser.cpp"
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
#line 2189 "rlparser.cpp"
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
#line 2214 "rlparser.cpp"
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
#line 2250 "rlparser.cpp"
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
#line 2278 "rlparser.cpp"
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
#line 2316 "rlparser.cpp"
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
#line 2341 "rlparser.cpp"
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
#line 2367 "rlparser.cpp"
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
#line 2390 "rlparser.cpp"
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
#line 2409 "rlparser.cpp"
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
#line 2425 "rlparser.cpp"
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
#line 2463 "rlparser.cpp"
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
#line 2496 "rlparser.cpp"
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
#line 2512 "rlparser.cpp"
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
#line 2547 "rlparser.cpp"
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
#line 2563 "rlparser.cpp"
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
#line 2598 "rlparser.cpp"
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
#line 2614 "rlparser.cpp"
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
#line 2649 "rlparser.cpp"
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
#line 2674 "rlparser.cpp"
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

#line 158 "rlparser.rl"
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
		
#line 2754 "rlparser.cpp"
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
#line 2796 "rlparser.cpp"
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
#line 2814 "rlparser.cpp"
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
#line 2847 "rlparser.cpp"
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
#line 2895 "rlparser.cpp"
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
#line 2928 "rlparser.cpp"
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
#line 2944 "rlparser.cpp"
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
#line 2969 "rlparser.cpp"
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
#line 2998 "rlparser.cpp"
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
#line 3023 "rlparser.cpp"
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
#line 3070 "rlparser.cpp"
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
#line 3095 "rlparser.cpp"
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
#line 3123 "rlparser.cpp"
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
#line 3142 "rlparser.cpp"
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
#line 3166 "rlparser.cpp"
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
#line 3214 "rlparser.cpp"
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
#line 3228 "rlparser.cpp"
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
#line 3242 "rlparser.cpp"
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
#line 3268 "rlparser.cpp"
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
#line 3307 "rlparser.cpp"
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
#line 3332 "rlparser.cpp"
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
#line 3351 "rlparser.cpp"
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
#line 3371 "rlparser.cpp"
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
#line 3399 "rlparser.cpp"
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
#line 3438 "rlparser.cpp"
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
#line 3463 "rlparser.cpp"
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
#line 3482 "rlparser.cpp"
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
#line 3502 "rlparser.cpp"
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
#line 3540 "rlparser.cpp"
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
#line 3565 "rlparser.cpp"
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
#line 3601 "rlparser.cpp"
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
#line 3629 "rlparser.cpp"
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
#line 3667 "rlparser.cpp"
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
#line 3692 "rlparser.cpp"
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
#line 3718 "rlparser.cpp"
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
#line 3741 "rlparser.cpp"
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
#line 3760 "rlparser.cpp"
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
#line 3776 "rlparser.cpp"
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
#line 3814 "rlparser.cpp"
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
#line 3847 "rlparser.cpp"
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
#line 3863 "rlparser.cpp"
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
#line 3898 "rlparser.cpp"
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
#line 3914 "rlparser.cpp"
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
#line 3949 "rlparser.cpp"
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
#line 3965 "rlparser.cpp"
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
#line 4000 "rlparser.cpp"
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
#line 4025 "rlparser.cpp"
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

#line 172 "rlparser.rl"
	}

	fclose(f);
}

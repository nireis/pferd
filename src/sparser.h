/* 
 * Parser zum Einlesen einer Graphdatei
 */

#ifndef parser_h
#define parser_h

#include "structs.h"
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

class parser
{
private:
	unsigned int NodeCount, EdgeCount;
	ParserNode* graphNodes;
	ParserEdge* graphEdges;
	std::string graphfile;

	/*
	 * createNode - erstellt einen neuen Knoten
	 * @variables
	 * string - aus datei eingelesene Zeile
	 * Node* - Zeiger auf den Knoten dessen Werte gesetzt werden (in/out varible !)
	 */
	void createNode(std::string inputString, ParserNode* rNode);

	/*
	 * createEdge - erstellt eine neue Kante
	 * @variables
	 * string - aus datei eingelesene Zeile
	 * unsinged int - Indexstelle des Arrays, KantenID
	 * Edge* - Zeiger auf die Kante dessen Werte gesetzt werden (in/out varible !)
	 */
	void createEdge(std::string inputString,unsigned int, ParserEdge* rEdge);
public:
	/*
	 *default constructer
	 */
	parser();
	parser(std::string graphdata);
	void getNodesAndEdges(ParserNode* n,ParserEdge* e );
	/*
	 *default desctucter
	 */
	~parser();

	/*
	 * getNodeCount - Gibt Knotenanzahl zur�ck
	 */
	unsigned int getNodeCount();
	/*
	 * getEdgeCount - Gibt Kantenanzahl zur�ck
	 */
	unsigned int getEdgeCount();
	/*
	 * getNodes - Gibt Knotenarray zur�ck
	 */
	ParserNode* getNodes();
	/*
	 * getEdges - Gibt Kantenarray zur�ck
	 */
	ParserEdge* getEdges();
};

#endif


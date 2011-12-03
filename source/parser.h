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
using namespace std;

class parser
{
private:
	unsigned int NodeCount, EdgeCount;
	Node* graphNodes;
	Edge* graphEdges;

	/*
	 * createNode - erstellt einen neuen Knoten
	 */
	Node* createNode(string, Node*);
	/*
	 * createEdge - erstellt eine neue Kante
	 */
	Edge* createEdge(string,unsigned int, Edge*);
public:
	/*
	 *default constructer
	 */
	parser(void);
	/*
	 *default desctucter
	 */
	~parser(void);

	/*
	 * readFile - bekommt den Pfad einer Datei als String übergeben und liest sie ein
	 */
	bool readFile(string);
	/*
	 * getNodeCount - Gibt Knotenanzahl zurück
	 */
	int getNodeCount();
	/*
	 * getEdgeCount - Gibt Kantenanzahl zurück
	 */
	int getEdgeCount();
	/*
	 * getNodes - Gibt Knotenarray zurück
	 */
	Node* getNodes();
	/*
	 * getEdges - Gibt Kantenarray zurück
	 */
	Edge* getEdges();
};

#endif


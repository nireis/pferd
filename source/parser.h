/* 
 * Parser zum Einlesen einer Graphdatei
 */

#ifndef parser_h
#define parser_h

#include "structs.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class parser
{
private:
	int NodeCount, EdgeCount;
	Node* graphNodes;
	Edge* graphEdges;

	/*
	 * createNode - erstellt einen neuen Knoten
	 */
	Node* createNode(string);
	/*
	 * createEdge - erstellt eine neue Kante
	 */
	Edge* createEdge(string,int);
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


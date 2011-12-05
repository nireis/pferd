/* 
 * Parser zum Einlesen einer Graphdatei
 */

#ifndef simple_parser_h
#define simple_parser_h

#include "structs.h"
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class simple_parser
{
private:
	unsigned int NodeCount, EdgeCount;
	Simple_Node* graphNodes;
	Simple_Edge* graphEdges;

	/*
	 * createSimple_Node - erstellt einen neuen Knoten
	 * @variables
	 * string - aus datei eingelesene Zeile
	 * Simple_Node* - Zeiger auf den Knoten dessen Werte gesetzt werden (in/out varible !)
	 */
	void createSimple_Node(string, Simple_Node*);

	/*
	 * createSimple_Edge - erstellt eine neue Kante
	 * @variables
	 * string - aus datei eingelesene Zeile
	 * unsinged int - Indexstelle des Arrays, KantenID
	 * Simple_Edge* - Zeiger auf die Kante dessen Werte gesetzt werden (in/out varible !)
	 */
	void createSimple_Edge(string,unsigned int, Simple_Edge*);
public:
	/*
	 *default constructer
	 */
	simple_parser(void);
	/*
	 *default desctucter
	 */
	~simple_parser(void);

	/*
	 * readFile - bekommt den Pfad einer Datei als String übergeben und liest sie ein
	 */
	bool readFile(string);
	/*
	 * getSimple_NodeCount - Gibt Knotenanzahl zurück
	 */
	unsigned int getNodeCount();
	/*
	 * getSimple_EdgeCount - Gibt Kantenanzahl zurück
	 */
	unsigned int getEdgeCount();
	/*
	 * getSimple_Nodes - Gibt Knotenarray zurück
	 */
	Simple_Node* getNodes();
	/*
	 * getSimple_Edges - Gibt Kantenarray zurück
	 */
	Simple_Edge* getEdges();
};

#endif


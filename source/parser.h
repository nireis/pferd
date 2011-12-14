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

class parser
{
private:
	unsigned int NodeCount, EdgeCount;
	Node* graphNodes;
	Edge* graphEdges;

	/*
	 * createNode - erstellt einen neuen Knoten
	 * @variables
	 * string - aus datei eingelesene Zeile
	 * Node* - Zeiger auf den Knoten dessen Werte gesetzt werden (in/out varible !)
	 */
	void createNode(std::string, Node*);

	/*
	 * createEdge - erstellt eine neue Kante
	 * @variables
	 * string - aus datei eingelesene Zeile
	 * unsinged int - Indexstelle des Arrays, KantenID
	 * Edge* - Zeiger auf die Kante dessen Werte gesetzt werden (in/out varible !)
	 */
	void createEdge(std::string,unsigned int, Edge*);
public:
	/*
	 *default constructer
	 */
	parser();
	/*
	 *default desctucter
	 */
	~parser();

	/*
	 * readFile - bekommt den Pfad einer Datei als String �bergeben und liest sie ein
	 */
	bool readFile(std::string);
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
	Node* getNodes();
	/*
	 * getEdges - Gibt Kantenarray zur�ck
	 */
	Edge* getEdges();

	/*
	 * schreibt geparsten Graph
	 * in eine Datei, die sp�ter
	 * schneller einlesbar sein soll
	 *
	 * liest besagte datei wieder ein
	 *
	 * gibt zur�ck, ob schreiben/lesen erfolgreich war
	 */
	bool writeGraphFile(std::string);
	bool readGraphFile(std::string);
};

#endif


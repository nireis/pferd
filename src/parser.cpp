/* 
 * Parser zum Einlesen einer Graphdatei
 * Kommentare in der parser.h
 * Zu Details befragen sie bitte den Autor, da er zu faul war jede Zeile
 * der Implementierung zu kommentieren
 */

#include "parser.h"
using namespace std;

parser::parser()
{
}

parser::~parser()
{
}

void parser::createNode(string inputString, ParserNode* rNode)
{
	//Variablen zum iterieren über den Eingabestring
	string::iterator itr1;
	string::iterator itr2 = inputString.begin();
	//Zahlvaribale für die Zuweisung der einzelnen Node-Variablen
	unsigned int currentComponent = 1;
	//Temporäre Variable vom Typ String, weil ich gerade zu blöd bin es ohne zu machen
	string tempStr;

	for (itr1=inputString.begin(); itr1 != inputString.end(); itr1++)
    {
	    if(*itr1 == ' ')
	    {
			if(currentComponent == 1)
			{
				tempStr.assign(itr2, itr1);
				rNode->id = atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 2)
			{
				tempStr.assign(itr2, itr1);
				rNode->lat = atof(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.assign(itr2, itr1);
				rNode->lon = atof(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
		}
	}

	/*
	 * Die Schleife wird verlassen sobald itr1 das ende der Zeile erreicht hat,
	 * und somit kann das letzte Element der Zeile nicht innerhalb der Schleife eingelesen werde
	 */
	tempStr.assign(itr2, itr1);
	rNode->elevation = atoi(tempStr.c_str());
}

void parser::createEdge(string inputString, unsigned int edgeID, ParserEdge* rEdge)
{
	//Variablen zum iterieren über den Eingabestring
	string::iterator itr1;
	string::iterator itr2 = inputString.begin();
	//Zahlvaribale für die Zuweisung der einzelnen Node-Variablen
	unsigned int currentComponent = 1;
	//Temporaere Variable vom Typ String, weil ich gerade zu blöd bin es ohne zu machen
	string tempStr;

	for (itr1=inputString.begin(); itr1 != inputString.end(); itr1++)
    {
	    if(*itr1 == ' ')
	    {
			if(currentComponent == 1)
			{
				tempStr.assign(itr2, itr1);
				rEdge->source = (unsigned int)atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 2)
			{
				tempStr.assign(itr2, itr1);
				rEdge->target = (unsigned int)atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.assign(itr2, itr1);
				rEdge->distance = (unsigned int)atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
		}
	}
	/*
	 * Die Schleife wird verlassen sobald itr1 das ende der Zeile erreicht hat,
	 * und somit kann das letzte Element der Zeile nicht innerhalb der Schleife eingelesen werde
	 */
	tempStr.assign(itr2, itr1);
	// Vorsicht! Die Doku der Quelldatei spricht hier zwar von integer, aber nirei hielt char* für eine gute Idee
	
	rEdge->type =(unsigned int) atoi(tempStr.c_str());
}

parser::parser(string graphdata)
{
	string buffer;
//	unsigned int currentline = 1;
	graphfile = graphdata;
	ifstream file;
	file.open(graphdata.c_str(), ios::in);

	if( file.is_open())
	{
		file.seekg(0, ios::beg);
		
		getline(file,buffer,'\n');
		NodeCount = (unsigned int)atoi(buffer.c_str());

		getline(file,buffer,'\n');
		EdgeCount = (unsigned int)atoi(buffer.c_str());
		file.close();
	}
}

void parser::getNodesAndEdges(ParserNode* n, ParserEdge* e)
{
	string buffer;
//	unsigned int currentline = 1;

	ifstream file;
	file.open(graphfile.c_str(), ios::in);

	if( file.is_open())
	{
		file.seekg(0, ios::beg);
		
		getline(file,buffer,'\n');
		NodeCount = (unsigned int)atoi(buffer.c_str());
		graphNodes = n;


		getline(file,buffer,'\n');
		EdgeCount = (unsigned int)atoi(buffer.c_str());
		graphEdges = e;

		for(unsigned int i = 0; i < NodeCount; i++){
			getline(file,buffer,'\n');
			createNode(buffer, &graphNodes[i]);
		}
		unsigned int j = 0;
		while(j < EdgeCount ){
			getline(file,buffer,'\n');
			createEdge(buffer, j, &graphEdges[j]);
			j++;
		}

		file.close();
	}
	else
	{
		cout << "Oh no!";
	}
	cout << "Fertig";
}

unsigned int parser::getNodeCount()
{
	return NodeCount;
}

unsigned int parser::getEdgeCount()
{
	return EdgeCount;
}

ParserNode* parser::getNodes()
{
	return graphNodes;
}

ParserEdge* parser::getEdges()
{
	return graphEdges;
}


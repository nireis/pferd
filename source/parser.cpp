/* 
 * Parser zum Einlesen einer Graphdatei
 * Kommentare in der parser.h
 * Zu Details befragen sie bitte den Autor, da er zu faul war jede Zeile
 * der Implementierung zu kommentieren
 */

#include "parser.h"
using namespace std;

parser::parser(void)
{
}

parser::~parser(void)
{
}

void parser::createNode(string inputString, Node* rNode)
{
	rNode->in_edge_offset = 0;
	rNode->out_edge_offset = 0;
	rNode->in_shortcut_offset = 0;
	rNode->out_shortcut_offset = 0;

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
				//Node ID - Varibale im Node jedoch nicht enthalten
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

void parser::createEdge(string inputString, unsigned int edgeID, Edge* rEdge)
{
	rEdge->id = edgeID;
	rEdge->load = 0;

	//Variablen zum iterieren über den Eingabestring
	string::iterator itr1;
	string::iterator itr2 = inputString.begin();
	//Zahlvaribale für die Zuweisung der einzelnen Node-Variablen
	unsigned int currentComponent = 1;
	//Temporaere Variable vom Typ String, weil ich gerade zu blöd bin es ohne zu machen
	string tempStr;
	//Temporaere Varibale vom Typ char* für die sehr ekelhaft, aber hoffentlich platzsparende type Zuweisung der Kanten
	// TODO
//	unsigned char tempChar = new unsigned char[2];

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
//	strcpy(tempChar, tempStr.c_str());
//	
//	rEdge->type = tempChar;
}

bool parser::readFile(string filename)
{
	string buffer;
//	unsigned int currentline = 1;

	ifstream file;
	file.open(filename.c_str(), ios::in);

	if( file.is_open())
	{
		file.seekg(0, ios::beg);
		
		getline(file,buffer,'\n');
		NodeCount = (unsigned int)atoi(buffer.c_str());
		graphNodes = new Node[NodeCount+1];


		getline(file,buffer,'\n');
		EdgeCount = (unsigned int)atoi(buffer.c_str());
		graphEdges = new Edge[EdgeCount];

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
		
		/*
		 * Sorry Michi, 
		 * der Parser gab teilweise nicht initialisierte
		 * Objekte zurück :(
		 */

		/*while( !file.eof())
		{
			getline(file,buffer,'\n');

			if(currentline == 1 && buffer.size() > 1)
			{
				NodeCount = (unsigned int)atoi(buffer.c_str());
				graphNodes = new Node[NodeCount+1];
			}
			else if(currentline == 2 && buffer.size() > 1)
			{
				EdgeCount = (unsigned int)atoi(buffer.c_str());
				graphEdges = new Edge[EdgeCount];
			}
			else if(2 < currentline && currentline < (NodeCount+3) && buffer.size() > 1)
			{
				createNode(buffer, &graphNodes[currentline -3]);
			}
			else if((NodeCount+2) < currentline && currentline < (NodeCount+EdgeCount+3) && buffer.size() > 1)
			{
				createEdge(buffer, (currentline -NodeCount -3), &graphEdges[currentline -NodeCount -3]);
			}
			else
			{
				return false;
			}

			currentline = currentline + 1;

		}*/

		file.close();
		//Dummy Node als letzten Eintrag im Knotenarray
		Node DummyNode;
		DummyNode.lat=0;
		DummyNode.lon=0;
		DummyNode.elevation=0;
		DummyNode.in_edge_offset=0;
		DummyNode.out_edge_offset=0;
		graphNodes[NodeCount] = DummyNode;
		
		return true;
	}
	else
	{
		return false;
	}
}

unsigned int parser::getNodeCount()
{
	return NodeCount;
}

unsigned int parser::getEdgeCount()
{
	return EdgeCount;
}

Node* parser::getNodes()
{
	return graphNodes;
}

Edge* parser::getEdges()
{
	return graphEdges;
}

bool parser::writeGraphFile(string filename){
	return false;
}

bool parser::readGraphFile(string filename){
	return false;
}

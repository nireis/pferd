/* 
 * Parser zum Einlesen einer Graphdatei
 * Kommentare in der parser.h
 * Zu Details befragen sie bitte den Autor, da er zu faul war jede Zeile
 * der Implementierung zu kommentieren
 */

#include "parser.h"


parser::parser(void)
{
}

parser::~parser(void)
{
}

Node* parser::createNode(string inputString)
{
	//Rückgabewert
	Node* rNode;
	rNode->in_edge_offset = 0;
	rNode->out_edge_offset = 0;

	//Variablen zum iterieren über den Eingabestring
	string::iterator itr1;
	string::iterator itr2 = inputString.begin();
	//Zahlvaribale für die Zuweisung der einzelnen Node-Variablen
	int currentComponent = 1;
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
				tempStr.append(itr2, itr1);
				rNode->lat = atof(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.append(itr2, itr1);
				rNode->lon = atof(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 4)
			{
				tempStr.append(itr2, itr1);
				rNode->elevation = atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
		}
	}

	return rNode;
}

Edge* parser::createEdge(string inputString, int nodeID)
{
	//Rückgabewert
	Edge* rEdge;
	rEdge->id = nodeID;

	//Variablen zum iterieren über den Eingabestring
	string::iterator itr1;
	string::iterator itr2 = inputString.begin();
	//Zahlvaribale für die Zuweisung der einzelnen Node-Variablen
	int currentComponent = 1;
	//Temporäre Variable vom Typ String, weil ich gerade zu blöd bin es ohne zu machen
	string tempStr;

	for (itr1=inputString.begin(); itr1 != inputString.end(); itr1++)
    {
	    if(*itr1 == ' ')
	    {
			if(currentComponent == 1)
			{
				tempStr.append(itr2, itr1);
				rEdge->source = atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 2)
			{
				tempStr.append(itr2, itr1);
				rEdge->target = atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.append(itr2, itr1);
				rEdge->distance = atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 4)
			{
				tempStr.append(itr2, itr1);
				rEdge->type = atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
		}
	}

	return rEdge;
}

bool parser::readFile(string filename)
{
	string buffer;
	int currentline = 1;

	ifstream file;
	file.open(filename, ios::in);

	if( file.is_open())
	{
		file.seekg(0, ios::beg);
		while( !file.eof())
		{
			getline(file,buffer,'\n');

			if(currentline == 1 && buffer.size() > 1)
			{
				NodeCount = atoi(buffer.c_str());
				graphNodes = new Node[NodeCount+1];
			}
			else if(currentline == 2 && buffer.size() > 1)
			{
				EdgeCount = atoi(buffer.c_str());
				graphEdges = new Edge[EdgeCount];
			}
			else if(2 < currentline < (NodeCount+3) && buffer.size() > 1)
			{
				graphNodes[currentline -3] = *(createNode(buffer));
			}
			else if((NodeCount+2) < currentline < (NodeCount+EdgeCount+3) && buffer.size() > 1)
			{
				graphEdges[currentline -NodeCount -3] = *(createEdge(buffer,(currentline -NodeCount -3)));
			}
			else
			{
				return false;
			}

			currentline = currentline + 1;

		}

		file.close();
		//Dummy Node als letzten Eintrag im Knotenarray
		graphNodes[NodeCount] = Node();
		
		return true;
	}
	else
	{
		return false;
	}

	file.close();
}

int parser::getNodeCount()
{
	return NodeCount;
}

int parser::getEdgeCount()
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
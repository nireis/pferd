/* 
 * Parser zum Einlesen einer Graphdatei
 * Kommentare in der parser.h
 * Zu Details befragen sie bitte den Autor, da er zu faul war jede Zeile
 * der Implementierung zu kommentieren
 */

#include "simple_parser.h"


simple_parser::simple_parser(void)
{
}

simple_parser::~simple_parser(void)
{
}

void simple_parser::createSimple_Node(string inputString, Simple_Node* rSimple_Node)
{
	rSimple_Node->in_edge_offset = 0;
	rSimple_Node->out_edge_offset = 0;

	//Variablen zum iterieren über den Eingabestring
	string::iterator itr1;
	string::iterator itr2 = inputString.begin();
	//Zahlvaribale für die Zuweisung der einzelnen Simple_Node-Variablen
	unsigned int currentComponent = 1;
	//Temporäre Variable vom Typ String, weil ich gerade zu blöd bin es ohne zu machen
	string tempStr;

	for (itr1=inputString.begin(); itr1 != inputString.end(); itr1++)
    {
	    if(*itr1 == ' ')
	    {
			if(currentComponent == 1)
			{
				//Simple_Node ID - Varibale im Node jedoch nicht enthalten
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 2)
			{
				tempStr.assign(itr2, itr1);
			//	rSimple_Node->lat = atof(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.assign(itr2, itr1);
			//	rSimple_Node->lon = atof(tempStr.c_str());
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
	// rSimple_Node->elevation = atoi(tempStr.c_str());
}

void simple_parser::createSimple_Edge(string inputString, unsigned int edgeID, Simple_Edge* rSimple_Edge)
{
	//rSimple_Edge->id = edgeID;

	//Variablen zum iterieren über den Eingabestring
	string::iterator itr1;
	string::iterator itr2 = inputString.begin();
	//Zahlvaribale für die Zuweisung der einzelnen Simple_Node-Variablen
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
				rSimple_Edge->source = (unsigned int)atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 2)
			{
				tempStr.assign(itr2, itr1);
				rSimple_Edge->target = (unsigned int)atoi(tempStr.c_str());
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.assign(itr2, itr1);
				rSimple_Edge->distance = (unsigned int)atoi(tempStr.c_str());
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
	//rSimple_Edge->type = (unsigned char) *(tempStr.c_str());
}

bool simple_parser::readFile(string filename)
{
	string buffer;
	unsigned int currentline = 1;

	ifstream file;
	file.open(filename.c_str(), ios::in);

	if( file.is_open())
	{
		file.seekg(0, ios::beg);
		while( !file.eof())
		{
			getline(file,buffer,'\n');

			if(currentline == 1 && buffer.size() > 1)
			{
				NodeCount = (unsigned int)atoi(buffer.c_str());
				graphNodes = new Simple_Node[NodeCount+1];
			}
			else if(currentline == 2 && buffer.size() > 1)
			{
				EdgeCount = (unsigned int)atoi(buffer.c_str());
				graphEdges = new Simple_Edge[EdgeCount];
			}
			else if(2 < currentline && currentline < (NodeCount+3) && buffer.size() > 1)
			{
				createSimple_Node(buffer, &graphNodes[currentline -3]);
			}
			else if((NodeCount+2) < currentline && currentline < (NodeCount+EdgeCount+3) && buffer.size() > 1)
			{
				createSimple_Edge(buffer, (currentline -NodeCount -3), &graphEdges[currentline -NodeCount -3]);
			}
			else
			{
				return false;
			}

			currentline = currentline + 1;

		}

		file.close();
		//Dummy Simple_Node als letzten Eintrag im Knotenarray
		Simple_Node DummyNode;
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

unsigned int simple_parser::getNodeCount()
{
	return NodeCount;
}

unsigned int simple_parser::getEdgeCount()
{
	return EdgeCount;
}

Simple_Node* simple_parser::getNodes()
{
	return graphNodes;
}

Simple_Edge* simple_parser::getEdges()
{
	return graphEdges;
}

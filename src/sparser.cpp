/* 
 * Parser zum Einlesen einer Graphdatei
 * Kommentare in der sparser.h
 * Zu Details befragen sie bitte den Autor, da er zu faul war jede Zeile
 * der Implementierung zu kommentieren
 */

/*
 * !
 * !
 * ! liest den Stuttgart Graph ein
 * ! und schreibt ihn wieder nach cout
 * ! mi angepassten typen
 * !
 */

#include "sparser.h"
using namespace std;

sparser::sparser()
{
}

sparser::~sparser()
{
}

void sparser::createNode(string inputString, ParserNode* rNode)
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
				// rNode->id = 
					cout << atoi(tempStr.c_str()) << " ";
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 2)
			{
				tempStr.assign(itr2, itr1);
				/* rNode->OSMid =  atoi(tempStr.c_str()); */
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.assign(itr2, itr1);
				//rNode->lat = 
					cout << /* setprecision(8) <<  atof( */ tempStr/*.c_str() )*/ << " ";
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 4)
			{
				tempStr.assign(itr2, itr1);
				//rNode->lon = 
					cout << /* setprecision(8) <<  atof( */ tempStr/* .c_str() )*/ << " ";
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
	//rNode->elevation = 
		cout << atoi(tempStr.c_str()) << endl;
}

void sparser::createEdge(string inputString, unsigned int edgeID, ParserEdge* rEdge)
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
				//rEdge->source = 
					cout << (unsigned int)atoi(tempStr.c_str()) << " ";
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 2)
			{
				tempStr.assign(itr2, itr1);
				//rEdge->target = 
					cout << (unsigned int)atoi(tempStr.c_str()) << " ";
				currentComponent++;
				itr2 = (itr1 + 1);
			}
			else if(currentComponent == 3)
			{
				tempStr.assign(itr2, itr1);
				//rEdge->distance = 
					cout << (unsigned int)atoi(tempStr.c_str()) << " ";
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
	
	//rEdge->type =
		cout << (unsigned int) ((atof(tempStr.c_str()))*100) << endl; 
}

sparser::sparser(string graphdata)
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

void sparser::getNodesAndEdges(ParserNode* n, ParserEdge* e)
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
	}
}

unsigned int sparser::getNodeCount()
{
	cout << NodeCount << endl;
	return NodeCount;
}

unsigned int sparser::getEdgeCount()
{
	cout << EdgeCount << endl;
	return EdgeCount;
}

ParserNode* sparser::getNodes()
{
	return graphNodes;
}

ParserEdge* sparser::getEdges()
{
	return graphEdges;
}

int main(int argc, char* argv[]){

	sparser p = sparser(string(argv[1]));

	p.getNodeCount();
	p.getEdgeCount();
	p.getNodesAndEdges(0, 0);



	return 0;
}

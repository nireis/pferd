/*
#include "graph.h"
#include "graphalgs.h"
#include "parser.h"
#include "simple_parser.h"
#include <ctime>

int main(int argc, char *argv[]){
	
	
	// Test des Graphen: instanziieren und alles mal ausprobieren 

cout << "            _|\\__/|, " << endl;
cout << "          ,((\\````\\\\_" << endl;
cout << "        ,(())     `))\\ "<< endl;
cout << "      ,(()))       ,_ \\ "<< endl;
cout << "     ((())'   |        \\ "<< endl;
cout << "     )))))     >.__     \\ "<< endl;
cout << "     ((('     /    `-. .c|" << endl;
cout << "hjw          /        `-`'" << endl;
cout << "( http://www.asciiworld.com/-Horses-.html )" << endl;
cout << " " << endl;
cout << " " << endl;

	string file = argv[1];

	ifstream checkfile(file.c_str());
	if(!checkfile){
		cout << "-> angegebene Datei existiert nicht." << endl;
		return 0;
	}

	clock_t start,finish;
	double time;
	cout << "Hallo! willkommen im Parser-Debugger!" << endl 
		<< "Starte parsen der Datei '" << file << "'" << endl;
	start = clock();
	parser p = parser();
	p.readFile(file);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Parsen fertig. Gebrauchte Sekunden: "<< time << endl << "Debbuging Ausgabe:" << endl;

	cout << "Taste drücken zum Ausgeben aller! geparster Elemente."; 
	cin.get();

	cout << "Knotenanzahl: " << p.getNodeCount() << endl;
	cout << "Kantenanzahl: " << p.getEdgeCount() << endl;
	Node* tNodeArray = p.getNodes();
	Edge* tEdgeArray = p.getEdges();
	for(int i = 0; i < p.getNodeCount() + 1; i++)
	{
		cout << tNodeArray[i].lat << " " << tNodeArray[i].lon << " " << tNodeArray[i].elevation << endl;
	}
	for(int i = 0; i < p.getEdgeCount(); i++)
	{
		cout << tEdgeArray[i].id << " " << tEdgeArray[i].source << " " << tEdgeArray[i].target << " " << tEdgeArray[i].distance << " " << tEdgeArray[i].type << endl;
	}

	cout << "Taste drücken zum beenden ... "; 
	cin.get(); 

	return 0;
}*/

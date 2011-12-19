#include "graph.h"
//#include "graphalgs.h"
#include "parser.h"
#include <ctime>

using namespace std;

int main(int argc, char *argv[]){

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

	if(argc != 2){
		cout << "---" << endl 
				<< "-- Aufruf der Binärdatei wie folgt: " << argv[0] << " Graphendatei " << endl
				<< "-- Graphdatei: Pfad zu einer Datei, die als Graphdatei gelesen werden kann." << endl
				<< "---" << endl;
		return 0;
	} 

	string file = argv[1];

	ifstream checkfile(file.c_str());
	if(!checkfile){
		cout << "-> angegebene Datei existiert nicht." << endl;
		return 0;
	}


	clock_t start,finish;
	double time;

Graph g = Graph();

start = clock();

g.setGraph(file);

finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
cout << "Zeit zum initialisieren des Graphen: " << time << endl;
start = clock();


unsigned int node = 3;
for(unsigned int i = 0; i <= node; i++){
	
	NodeData nd = g.getNodeData(i);
	cout.precision(15);

	cout << "node: " << i << endl;
	cout << "ID: " << nd.id << endl;
	cout << "Lat: " << nd.lat << endl;
	cout << "Lon: " << nd.lon << endl;
	cout << "El: " << nd.elevation << endl << endl;
	
	Graph::OutEdgesIterator it = g.getOutEdgesIt(i);
	while(it.hasNext()){
		Edge* e = it.getNext();
		cout << "other_node: " << e->other_node << endl;
				Graph::InEdgesIterator ieit = g.getInEdgesIt(e->other_node);
		cout << "value: " << e->value << endl;
		cout << "type: " << g.getEdgeData(e->id).type << endl;
		cout << "id: " << e->id << endl << endl;
				while(ieit.hasNext()){
					Edge* ee = ieit.getNext();
					cout << "   v: " << ee->value << endl;
					cout << "   other: " << ee->other_node << endl;
					cout << "   id: " << ee->id << endl;
				}
	}
	cout << " === === " << endl;
}


//	g.print(20);

time = (double(finish)-double(start))/CLOCKS_PER_SEC;

cin.get();
cout << "zeit: "<< time << endl;
	return 0;
}

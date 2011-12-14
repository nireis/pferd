#include "graph.h"
#include "graphalgs.h"
#include "parser.h"
#include <ctime>

using namespace std;

int main(int argc, char *argv[]){
	
	
	/***  Hallo Michi
	 ***  dies ist unser akteller Testlauf
	 ***  Er besteht im groben aus:
	 ***  
	 ***  - Graphdatei parsen
	 ***  - Graphobjekt erstellen und initialisieren
	 ***  - ein paar Kanten zum ersten Knoten des Graphen ausgeben
	 ***  - Dijkstra auf dem Graphen laufen lassen
	 ***  - Shortcuts erfinden und dem Graphen hinzufügen
	 ***  - Shortcuts im Graph initialisieren lassen
	 *** 
	 ***  Obwohl ein paar Funktionalitäten noch hinukommen/sich ändern
	 ***  werden, ist das erstmal eine gute Grundlage.
	 ***
	 ***  Viel Spaß!
	 */

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

	cout << "Hallo! willkommen im Testlauf!" << endl 
		<< "Starte parsen der Datei '" << file << "'" << endl;
start = clock();
	parser p = parser();
	p.readFile(file);
finish = clock();
time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Parsen fertig. Gebrauchte Sekunden: "<< time << endl 
		<< "Erstelle und initialisiere Graph." << endl;
start = clock();
	Graph g = 
		Graph(p.getNodeCount(), p.getEdgeCount(), 
				p.getNodes(), p.getEdges());

//	cout << "Taste drücken, damit   GRAPH   initialisiert wird. "; 
//	cin.get(); 

		g.initOffsets();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Graph initialisiert! Benötigte Sekunden: "<<time << endl
			<< "Hier die Informationen zur den Kanten des Knotens 0:" << endl;
	start = clock();
	Graph::OutEdgesIterator it = g.getOutEdgesIt(0);
	Graph::InEdgesIterator iit = g.getInEdgesIt(0);
	finish = clock();
		cout << "-CPU-Clocks, um alle Kanten eines Knotens zu bestimmen: " << (finish-start) << endl;
	
	cout << "Nun mit einem OutEdgesIterator "<< endl 
		<< "   die AUSgehenden Kanten von Node 0:" << endl;
	start = clock();
	while(it.hasNext()){
		//Simple_Edge &e(it.getNext());
		Edge* e = it.getNext();
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e->source << endl;
		cout <<" target: " << e->target<< endl;
		cout <<" distance: " << e->distance << endl;
		cout << " type : " << e->type << endl;
		cout << " id : "<< e->id << endl;
		
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Betnötigte Zeit zum iterieren über die Kanten: " << time << endl;
		
	cout << "Nun mit einem InEdgesIterator "<< endl 
		<< "   die EINgehenden Kanten von Node 0:" << endl;
	start = clock();
	while(iit.hasNext()){
		//Simple_Edge &e(it.getNext());
		Edge *e = iit.getNext();
		cout <<" -- next edge --" << endl;
		cout <<" source: " << e->source << endl;
		cout <<" target: " << e->target<< endl;
		cout <<" distance: " << e->distance << endl;
		cout << " type : " << e->type << endl;
		cout << " id : "<< e->id << endl;
		
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Betnötigte Zeit zum iterieren über die Kanten: " << time << endl;

//	cout << "Taste drücken, damit   DIJKSTRA   beginnt. "; 
//	cin.get(); 

	/*
	 * Test der Graphalgs Klasse!
	 */
	cout << "Dijkstra angefangen." << endl;
	Dijkstra(&g, 0);
	cout << "Wechsle auf den Dijkstra, welcher direkt auf den Graphenstrukturen arbeitet." << endl;
	DirectDijkstra(&g, 0);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Dijkstra beendet. Zeit: " << time <<  endl;
	cout << "Fange Erfinden der Shortcuts an. " << endl;
	cout << "Sollen die Shortcuts in einer Liste(1) oder einem Array(2) angelegt werden?" << endl;
	cout << "Eingabe: (1 oder 2) ";
	int mode;
	cin >> mode;
if(mode == 2){
	start = clock();
	unsigned int scc = g.getEdgeCount()*2;
	Edge* sc = new Edge[scc];
	//Edge* s = new Edge[g.getEdgeCount()];
	for(unsigned int i = 0; i < scc; i++){
		Edge s;
		s.distance = ((int)( i*i*3.141592)) % 100;
		s.source = i%g.getNodeCount();
		s.target = i*(i+1)%(g.getNodeCount());
		s.id = 0;
		s.type = 0;
		s.load = 0;
		sc[i] = s;
		//g.addShortcut(s);
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Shortcuts angelegt. Nun initialisieren. Benötigte Zeit:" << time << endl;
	cout << "Taste drücken zum Fortfahren! " << endl;
	cin.get();

	start = clock();
	g.initShortcutOffsets(sc, scc);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit, 2m Shortcuts zu initialisieren: " << time << endl;
	cout << "Taste drücken zum Fortfahren. " << endl;
	cin.get(); 
} else if(mode == 1){

	start = clock();
	unsigned int scc = g.getEdgeCount()*2;
	//Shortcut* s = new Shortcut[g.getEdgeCount()];
	for(unsigned int i = 0; i < scc; i++){
		Edge s;
		s.distance = ((int)( i*i*3.141592)) % 100;
		s.source = i%g.getNodeCount();
		s.target = i*(i+1)%(g.getNodeCount());
		s.id = 0;
		s.type = 0;
		s.load = 0;
		g.addShortcut(s);
	}
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Shortcuts angelegt. Nun initialisieren. Benötigte Zeit:" << time << endl;
	cout << "Taste drücken zum Fortfahren! " << endl;
	cin.get();

	start = clock();
	g.initShortcutOffsets();
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit, 2m Shortcuts zu initialisieren: " << time << endl;
	cout << "Taste drücken zum Fortfahren. " << endl;
	cin.get(); 

}

	cout << "Taste drücken zum beenden ... " << endl; 
	cin.get(); 

	return 0;
}

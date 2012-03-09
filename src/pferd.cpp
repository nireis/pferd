#include "graph.h"
#include "graphalgs.h"
#include "CHConstruction.h"
#include "parser.h"
#include <ctime>
#include "structs.h"
#include "ch.h"
#include "rlparser.h"
#include "vis.h"

using namespace std;



//	void CallSCGraph(Graph* g){
//		clock_t start,finish;
//		double time;
//		
//		SCGraph scg = SCGraph(g);
//		scg.setShortcuts();
//		
//		for(int i=0; i<10; i++){
//			start = clock();
//			Dijkstra(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für normalen Dijkstra von " << i << " aus: "<< time << endl;
//		}
//		for(int i=0; i<10; i++){
//			start = clock();
//			TDijkstra<SCGraph>(&scg, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für Template-Dijkstra mit statischer CH von " << i << " aus: "<< time << endl;
//		}
//		
//		cout << "Taste drücken zum Erstellen von m Shortcuts. " << endl;
//		cin.get();
//		
//		start = clock();
//			unsigned int scc = g->getEdgeCount();
//				for(unsigned int i = 0; i < scc; i++){
//				Shortcut s;
//				s.value = ((int)( i*i*3.141592)) % 100;
//				s.source = i%g->getNodeCount();
//				s.target = i*(i+1)%(g->getNodeCount());
//		      s.papa_edge = i*i % g->getEdgeCount();
//		      s.mama_edge = i*(i+1) % g->getEdgeCount();
//				scg.addShortcut(s);
//			}
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Shortcuts bereitgelegt. Benötigte Zeit:" << time << endl;
//			cout << "Nun initialisieren der Shortcuts.Taste drücken. " << endl;
//			cin.get();
//		
//			start = clock();
//			scg.setShortcuts();
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit, m Shortcuts und m vorhandene Kanten zu initialisieren: " << time << endl;
//			cout << "Nun löschen der Shortcuts. Taste drücken. " << endl;
//			cin.get(); 
//		
//			scg.clearShortcutlist();
//			scg.clearShortcuts();
//		
//		//	start = clock();
//		//	unsigned int is = scg.calcIndepSet();
//		//	finish = clock();
//		//	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//		//
//		//	cout << "Habe Independent Set der Größe > " << is << " < aufgebaut in: " << time << endl;
//	}
//	
//	void dijkstra_tests(Graph* g){
//	
//		clock_t start,finish;
//		double time;
//	
//		cout << "Dijkstra angefangen." << endl;
//		for(int i=0; i<10; i++){
//			start = clock();
//			DirectDijkstra(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für Dijkstra direkt auf dem Graphen von " << i << " aus: "<< time << endl;
//		}
//		for(int i=0; i<10; i++){
//			start = clock();
//			Dijkstra(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für normalen Dijkstra von " << i << " aus: "<< time << endl;
//		}
//		for(int i=0; i<10; i++){
//			start = clock();
//			Dijkstra_plus(g, i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für 'optimierten' Dijkstra und eigenem Bitvector von " << i << " aus: "<< time << endl;
//		}
//		
//			start = clock();
//			DijkstraC<Graph> dc = DijkstraC<Graph>( g );
//			finish = clock();
//			cout << "Zeit zum Erstellen des Dijkstra-Objektes: "<< time << endl;
//		for(int i=0; i<10; i++){
//			start = clock();
//			dc.run(i);
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit für Dijkstra-Klasse normal von " << i << " aus: "<< time << endl;
//		}
//		
//		 cout << " -- ab hier frisst das ganze >3GB Speicher --" << endl;
//		 cout << "Taste drücken zum Erstellen zweier CHs (statisch und dynamisch). " << endl;
//		 cin.get();
//		 CallSCGraph(g);
//	
//	/* DIJKSTRA TESTS
//	 *
//		for(unsigned int i=5010; i<5011; i++){
//			cout << "Wechsle auf den bidirektionalen Dijkstra." << endl;
//			start = clock();
//			cout << "Distanz: " << BiDijkstra(&g,0,i) << endl;
//			finish = clock();
//			cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
//	
//			cout << "Wechsle auf den Dijkstra, welcher direkt auf den Graphenstrukturen arbeitet." << endl;
//			start = clock();
//			cout << "Distanz: " << DirectDijkstra(&g,0,i) << endl;
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
//	
//			cout << "Wechsle auf den normalen Dijkstra." << endl;
//			start = clock();
//			cout << "Distanz: " << Dijkstra(&g,0,i) << endl;
//			finish = clock();
//			time = (double(finish)-double(start))/CLOCKS_PER_SEC;
//			cout << "Zeit: " << (double(finish)-double(start))/CLOCKS_PER_SEC << endl;
//		}
//	*/
//	}



void testSCGraph(Graph* g){
	clock_t start,finish;
	double time;
	
	SCGraph scg = SCGraph(g);
	
	cout << scg.mergeRound(1) << endl;
	start = clock();
	cout << scg.mergeShortcutsAndGraph(1) << endl;
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit um Graph zu kopieren: " << time << endl;
	
	cout << scg.isShortcut(0) << " " << scg.isShortcut( scg.getEdgeCount() ) << endl;
	
}

void berechneVis(vector<vis::text>* txt, vector<vis::textsc>* txtsc, list<Shortcut>* sclist,
		vector<vis::linesc>* lnsc, Graph* g){
	vector<unsigned int> edgePainted(g->getEdgeCount(),false);
	vector<unsigned int> nodeSeen(g->getNodeCount(),false);
	list<unsigned int> seenNodes;
	EdgesIterator it;
	for(unsigned int i=0; i<g->getNodeCount(); i++){
		// Die normalen Kanten einfügen
		Edge* tmpedge;
		double lon;
		double lat;
		string val;
		it = g->getOutEdgesIt(i);
		while(it.hasNext()){
			tmpedge = it.getNext();
			if(!edgePainted[tmpedge->id]){
				// Die Kante einfügen
				lon = (g->getNodeData(i).lon+g->getNodeData(tmpedge->other_node).lon)/2;
				lat = (g->getNodeData(i).lat+g->getNodeData(tmpedge->other_node).lat)/2;
				stringstream sstr;
				sstr << tmpedge->value;
				val = sstr.str();
				txt->push_back(vis::text(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
				edgePainted[tmpedge->id] = true;
			}
			nodeSeen[tmpedge->other_node] = true;
			seenNodes.push_back(tmpedge->other_node);
		}
		it = g->getInEdgesIt(i);
		while(it.hasNext()){
			tmpedge = it.getNext();
			// Die Kante wurde schon eingefügt
			if(nodeSeen[tmpedge->other_node]){
				edgePainted[tmpedge->id] = true;
			}
			else{
				// Die Kante einfügen
				lon = (g->getNodeData(i).lon+g->getNodeData(tmpedge->other_node).lon)/2;
				lat = (g->getNodeData(i).lat+g->getNodeData(tmpedge->other_node).lat)/2;
				stringstream sstr;
				sstr << tmpedge->value;
				val = sstr.str();
				txt->push_back(vis::text(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
			}
		}
	}
	double lon;
	double lat;
	ND nds;
	ND ndt;
	string val;
	Shortcut sc;
	// Shortcuts einzeichnen
	for(list<Shortcut>::iterator i=sclist->begin(); i!=sclist->end(); i++){
		sc = *i;
		// Die Kante einfügen
		nds = g->getNodeData(sc.source);
		ndt = g->getNodeData(sc.target);
		lnsc->push_back(vis::linesc(GeoDataCoordinates(nds.lon,nds.lat,0.0,GeoDataCoordinates::Degree),
					GeoDataCoordinates(ndt.lon,ndt.lat,0.0,GeoDataCoordinates::Degree)));
		// Die Werte einfügen
		lon = (nds.lon+ndt.lon)/2;
		lat = (nds.lat+ndt.lat)/2;
		stringstream sstr;
		sstr << sc.value;
		val = sstr.str();
		txtsc->push_back(vis::textsc(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
	}
}

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
	string file = "../data/15K.txt";

	cout << "Erstelle Graph mit Datei " << file << endl;
	Graph g = Graph();

	g.setGraph(file, true);

	clock_t start,finish;
	double time;
	start = clock();

	// Shortcuts berechnen und die Liste zurückgeben
	list<Shortcut>* sclist = new list<Shortcut>;
	list<unsigned int>* nodelist = new list<unsigned int>;
	CHConstruction<Graph>(&g).calcOneRound(sclist, nodelist);

	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit für erste Runde der CH-Berechnung: " << time << endl;

	// vis test
//	vector<vis::text>* txt = new vector<vis::text>;
//	vector<vis::textsc>* txtsc = new vector<vis::textsc>;
//	vector<vis::linesc>* lnsc = new vector<vis::linesc>;
//	berechneVis(txt, txtsc, sclist, lnsc, &g);
//	QApplication app(argc,argv);
//	vis *mapWidget = new vis(&g, txt, txtsc, nodelist, lnsc);
//	mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
//	mapWidget->setProjection(Mercator);
//	mapWidget->centerOn(GeoDataCoordinates(9.07, 48.45, 0.0, GeoDataCoordinates::Degree));
//	mapWidget->show();
//	app.exec();
//	delete mapWidget;
//	delete sclist;
//	delete nodelist;
	return 0;
}

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

void testGraphs(){
	Graph g = Graph();
	Graph g2 = Graph();
	g.setGraph("../data/15K.txt",false);
	g2.setGraph("../data/15K.txt.grp",false);

	if(g.getNodeCount() != g2.getNodeCount())
		cout << "nodecounts ungleich" << endl;

	if( g.getEdgeCount() != g2.getEdgeCount() )
		cout << "edgecoutns ungleich" << endl;

	for(unsigned int i = 0; i < g.getNodeCount(); i++){
		if( g.getNodeData(i).id != g2.getNodeData(i).id || g.getNodeData(i).lat != g2.getNodeData(i).lat || g.getNodeData(i).lon != g2.getNodeData(i).lon || g.getNodeData(i).elevation != g2.getNodeData(i).elevation )
			cout << "node-data ungleich" << endl;

		if( g.getLowerInEdgeBound(i) != g2.getLowerInEdgeBound(i) )
			cout << "edge-in-offsets nicht ok" << endl;
		if( g.getLowerOutEdgeBound(i) != g2.getLowerOutEdgeBound(i) )
			cout << "edge-out-offsets nicht ok" << endl;
	}
		if( g.getLowerInEdgeBound(g.getNodeCount() ) != g2.getLowerInEdgeBound(g.getNodeCount() ) )
			cout << "edge-in-offsets nicht ok" << endl;
		if( g.getLowerOutEdgeBound(g.getNodeCount() ) != g2.getLowerOutEdgeBound(g.getNodeCount() ) )
			cout << "edge-out-offsets nicht ok" << endl;

		Edge* goe = g.getOutEdge(0);
		Edge* g2oe = g2.getOutEdge(0);
		Edge* gie = g.getInEdge(0);
		Edge* g2ie = g2.getInEdge(0);
	for(unsigned int i = 0; i < g.getEdgeCount(); i++){	
		if( g.getEdgeData(i).in_index != g2.getEdgeData(i).in_index ||  g.getEdgeData(i).out_index != g2.getEdgeData(i).out_index ||  g.getEdgeData(i).distance != g2.getEdgeData(i).distance ||   g.getEdgeData(i).type != g2.getEdgeData(i).type ||  g.getEdgeData(i).load != g2.getEdgeData(i).load  )
			cout << "edge-data ungleich" << endl;

		if((goe+i)->other_lvl != (g2oe+i)->other_lvl || (goe+i)->other_node != (g2oe+i)->other_node || (goe+i)->id != (g2oe+i)->id || (goe+i)->value != (g2oe+i)->value )
			cout << "out-edges ungleich" << endl;

		if((gie+i)->other_lvl != (g2ie+i)->other_lvl || (gie+i)->other_node != (g2ie+i)->other_node || (gie+i)->id != (g2ie+i)->id || (gie+i)->value != (g2ie+i)->value )
			cout << "in-edges ungleich" << endl;
	}

//	if( g. != g2. ){
//		cout << " ungleich" << endl;

}



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
	
	cout << scg.mergeRoundNegative(1) << endl;
	start = clock();
	cout << scg.mergeShortcutsAndGraph(1) << endl;
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit um Graph zu kopieren: " << time << endl;
	
	cout << scg.isShortcut(0) << " " << scg.isShortcut( scg.getEdgeCount() ) << endl;
	
}

void berechneVis(vector<vis::text>* txt, vector<vis::textsc>* txtsc, list<Shortcut>* sclist,
		vector<vis::linesc>* lnsc, SCGraph* g){
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
		it = g->getOutEdgesIt_Round(i);
		while(it.hasNext()){
			tmpedge = it.getNext();
			//if(!edgePainted[tmpedge->id]){
				// Die Kante einfügen
				stringstream sstr;
		if(tmpedge->value == 999999)
			cout << "   VALUE = 999999 ! " << "out von : " <<  i << endl;
				sstr << tmpedge->value ;//<< " " << i ;//<< " (" << tmpedge->id << ")";
				if(tmpedge -> id >= g->getEdgeCount())
					sstr << " (SC)";
				val = sstr.str();
				lon = (g->getNodeData(i).lon+g->getNodeData(tmpedge->other_node).lon)/2;
				lat = (g->getNodeData(i).lat+g->getNodeData(tmpedge->other_node).lat)/2;
				txt->push_back(vis::text(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
				//edgePainted[tmpedge->id] = true;
			//}
			//nodeSeen[tmpedge->other_node] = true;
			seenNodes.push_back(tmpedge->other_node);
		}
		it = g->getInEdgesIt_Round(i);
		while(it.hasNext()){
			tmpedge = it.getNext();
			// Die Kante wurde schon eingefügt
			//if(nodeSeen[tmpedge->other_node]){
				//edgePainted[tmpedge->id] = true;
			//}
			//else{
				// Die Kante einfügen
				lon = (g->getNodeData(i).lon+g->getNodeData(tmpedge->other_node).lon)/2;
				lat = (g->getNodeData(i).lat+g->getNodeData(tmpedge->other_node).lat)/2;
				stringstream sstr;
		if(tmpedge->value == 999999)
			cout << "   VALUE = 999999 ! " << " in von : " << tmpedge->other_node << endl;
				sstr << tmpedge->value ;//<< "      " << i ;//<< " (" << tmpedge->id << ")";
				if(tmpedge -> id >= g->getEdgeCount())
					sstr << " (SC)";
				val = sstr.str();
				txt->push_back(vis::text(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
			//}
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


//	testGraphs();
//
//	cout << "tests fertig" << endl;
//	cin.get();

	string file = "../data/15K.txt.grp";

	clock_t start,finish;
	double time;

	cout << "Erstelle Graph mit Datei " << file << endl;
	start = clock();
	Graph g = Graph();

	g.setGraph(file, true);

	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit zum Initialisieren des Graphen: " << time << endl;

	cout << "Erstelle neuen Graph: " << endl;
	start = clock();
	SCGraph scg = SCGraph(&g);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "SCGraph erstellt in zeit: : " << time << endl;
	

	for(int i=0; i<1; i++){
		start = clock();
		TDijkstra<SCGraph>(&scg, i);
		finish = clock();
		time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Zeit für Template-Dijkstra mit SCGraph von " << i << " aus: "<< time << endl;
	}


	
	start = clock();
	// Shortcuts berechnen und die Liste zurückgeben
	
	list<Shortcut>* sclist = scg.getShortcutListPointer();
	list<unsigned int>* nodelist = scg.getBlackNodesListPointer();
	list<Shortcut> drawSClist = list<Shortcut>();
	unsigned int max_rounds = 3;

	for(unsigned int j = 1; j < max_rounds; j++){
		cout << "Berechne Shortcuts für Runde " <<  j ;
		start = clock();
		CHConstruction<SCGraph>(&scg).calcOneRound(sclist, nodelist);
		time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << ", Zeit: " << time << endl;

		for(list<Shortcut>::iterator it = sclist->begin(); it != sclist->end(); it++){
			drawSClist.push_front( Shortcut( *it ) );
		}

		cout << "Merge Shortcuts in SCGraph. ";
		start = clock();
		scg.mergeRoundNegative(j);
		finish = clock();
		time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Zeit: " << time << endl;
		cout << " => Runde " << j << " fertig."  << endl;
	}
	// scg.mergeShortcutsAndGraph(max_rounds);

	cout << "Runden fertig. Eingabetaste:" << endl;
	cin.get();

	// vis test
	vector<vis::text>* txt = new vector<vis::text>;
	vector<vis::textsc>* txtsc = new vector<vis::textsc>;
	vector<vis::linesc>* lnsc = new vector<vis::linesc>;
	berechneVis(txt, txtsc, &drawSClist, lnsc, &scg);
	QApplication app(argc,argv);
	vis *mapWidget = new vis(&scg, txt, txtsc, nodelist, lnsc);
	mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
	mapWidget->setProjection(Mercator);
	mapWidget->centerOn(GeoDataCoordinates(9.07, 48.45, 0.0, GeoDataCoordinates::Degree));
	mapWidget->show();
	app.exec();
	delete mapWidget;
	return 0;
}

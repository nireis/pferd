#include "graph.h"
#include "graphalgs.h"
#include "CHConstruction.h"
#include "parser.h"
#include <ctime>
#include "structs.h"
#include "ch.h"
#include "rlparser.h"
#include "marble_vis.h"

using namespace std;




void berechneVis(vector<marble_vis::text>* txt, vector<marble_vis::textsc>* txtsc, list<Shortcut>* sclist,
		vector<marble_vis::linesc>* lnsc, SCGraph* g){
	vector<unsigned int> edgePainted(g->getEdgeCount(),false);
	vector<unsigned int> edgeSeen(g->getEdgeCount() + g->getShortcutCount(), 0);
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
			/*
			 * Edges doppelt anzeigen? dann immer in das if gehen:
			 * if( 1 || ... )
			 */
			if(edgeSeen[tmpedge->id] == 0){
					stringstream sstr;
					sstr << tmpedge->value ;
					if(tmpedge -> id >= g->getEdgeCount())
						sstr << " (SC)";
					/*
					 * Edge IDs angeben?
					 */
					//sstr << "(id: " << tmpedge->id << ")" ;
					val = sstr.str();
					lon = (g->getNodeData(i).lon+g->getNodeData(tmpedge->other_node).lon)/2;
					lat = (g->getNodeData(i).lat+g->getNodeData(tmpedge->other_node).lat)/2;
					txt->push_back(marble_vis::text(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
				edgeSeen[tmpedge->id] = 1;
			} 
		}
	}
	double lon;
	double lat;
	ND nds;
	ND ndt;
	string val;
	Shortcut sc;
	/*
	// Shortcuts einzeichnen
	for(list<Shortcut>::iterator i=sclist->begin(); i!=sclist->end(); i++){
		sc = *i;
		// Die Kante einfügen
		nds = g->getNodeData(sc.source);
		ndt = g->getNodeData(sc.target);
		lnsc->push_back(marble_vis::linesc(GeoDataCoordinates(nds.lon,nds.lat,0.0,GeoDataCoordinates::Degree),
					GeoDataCoordinates(ndt.lon,ndt.lat,0.0,GeoDataCoordinates::Degree)));
		// Die Werte einfügen
		lon = (nds.lon+ndt.lon)/2;
		lat = (nds.lat+ndt.lat)/2;
		stringstream sstr;
		sstr << sc.value;
		val = sstr.str();
		txtsc->push_back(marble_vis::textsc(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
	} */
	// Kanten zeichnen
	for(unsigned int i = 0; i < g->getNodeCount(); i++){
		EdgesIterator ei = g->getOutEdgesIt(i);
		while(ei.hasNext()){
			Edge* e = ei.getNext();
			/* 
			 * einstellen, ob 
			 * Shortcut angezeigt werden
			 */
			if(1 || (! g->isShortcut(e->id))){
				// Die Kante einfügen
				nds = g->getNodeData(i);
				ndt = g->getNodeData(e->other_node);
				lnsc->push_back(marble_vis::linesc(GeoDataCoordinates(nds.lon,nds.lat,0.0,GeoDataCoordinates::Degree),
							GeoDataCoordinates(ndt.lon,ndt.lat,0.0,GeoDataCoordinates::Degree)));
				// Die Werte einfügen
				lon = (nds.lon+ndt.lon)/2;
				lat = (nds.lat+ndt.lat)/2;
				stringstream sstr;
				sstr << sc.value;
				val = sstr.str();
				txtsc->push_back(marble_vis::textsc(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val));
			}
		}
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

	CH hy(&g, &scg);
	//hy.calcCHverbose();
	hy.calcCH();

	// vis test
	list<Shortcut> drawSClist = list<Shortcut>();
	vector<marble_vis::text>* txt = new vector<marble_vis::text>;
	vector<marble_vis::textsc>* txtsc = new vector<marble_vis::textsc>;
	vector<marble_vis::linesc>* lnsc = new vector<marble_vis::linesc>;
	berechneVis(txt, txtsc, &drawSClist, lnsc, &scg);
	QApplication app(argc,argv);
	marble_vis *mapWidget = new marble_vis(&scg, txt, txtsc, lnsc);
	mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
	mapWidget->setProjection(Mercator);
	mapWidget->centerOn(GeoDataCoordinates(scg.getNodeData(0).lon , scg.getNodeData(0).lat , 0.0, GeoDataCoordinates::Degree));
	mapWidget->setDistance(0.75);
	mapWidget->show();
	app.exec();
	delete mapWidget;

	return 0;
}

#include "vis.h"

using namespace std;

vis::vis(Graph* g){
	this->g = g;
}

void vis::customPaint(GeoPainter* painter)
{
	ND nd;
   painter->setPen(Qt::black);
	vector<unsigned int> edgePainted(g->getEdgeCount(),false);
	vector<unsigned int> nodeSeen(g->getNodeCount(),false);
	list<unsigned int> seenNodes;
	EdgesIterator it;
	for(unsigned int i=0; i<g->getNodeCount(); i++){
		// Die Knoten einzeichnen
		nd = g->getNodeData(i);
		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
		// Die normalen Kanten einzeichnen
		Edge* tmpedge;
		double lon;
		double lat;
		const char* val;
		it = g->getOutEdgesIt(i);
		while(it.hasNext()){
			tmpedge = it.getNext();
			if(!edgePainted[tmpedge->id]){
				// Die Kante einzeichnen
				lon = (g->getNodeData(i).lon+g->getNodeData(tmpedge->other_node).lon)/2;
				lat = (g->getNodeData(i).lat+g->getNodeData(tmpedge->other_node).lat)/2;
				stringstream sstr;
				sstr << tmpedge->value;
				val = sstr.str().c_str();
				painter->drawText(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val);
				edgePainted[tmpedge->id] = true;
			}
			nodeSeen[tmpedge->other_node] = true;
			seenNodes.push_front(tmpedge->other_node);
		}
		it = g->getInEdgesIt(i);
		while(it.hasNext()){
			tmpedge = it.getNext();
			// Die Kante wurde schon eingezeichnet
			if(nodeSeen[tmpedge->other_node]){
				edgePainted[tmpedge->id] = true;
			}
			else{
				// Die Kante einzeichnen
				lon = (g->getNodeData(i).lon+g->getNodeData(tmpedge->other_node).lon)/2;
				lat = (g->getNodeData(i).lat+g->getNodeData(tmpedge->other_node).lat)/2;
				stringstream sstr;
				sstr << tmpedge->value;
				val = sstr.str().c_str();
				painter->drawText(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val);
			}
		}
	}
	// Shortcuts berechnen und die Liste zurückgeben
   painter->setPen(Qt::red);
	list<Shortcut>** sclist = new list<Shortcut>*;
	list<unsigned int>** nodelist = new list<unsigned int>*;
	CHConstruction<Graph>(g).calcOneRound(sclist, nodelist);
	double lon;
	double lat;
	const char* val;
	Shortcut sc;
	// Shortcuts einzeichnen
	while(!(*sclist)->empty()){
		sc = ((*sclist)->front());
		// Die Kante einzeichnen
		lon = (g->getNodeData(sc.source).lon+g->getNodeData(sc.target).lon)/2;
		lat = (g->getNodeData(sc.source).lat+g->getNodeData(sc.target).lat)/2;
		stringstream sstr;
		sstr << sc.value;
		val = sstr.str().c_str();
		painter->drawText(GeoDataCoordinates(lon,lat,0.0,GeoDataCoordinates::Degree),val);
	}
}

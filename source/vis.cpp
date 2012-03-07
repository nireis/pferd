#include "vis.h"

using namespace std;

vis::vis(Graph* g, vector<text>* txt, vector<textsc>* txtsc, list<unsigned int>** blackn){
	this->g = g;
	this->txt = txt;
	this->txtsc = txtsc;
	this->blackn = blackn;
}

void vis::customPaint(GeoPainter* painter)
{
	ND nd;
   painter->setPen(Qt::black);
	// Die Knoten einzeichnen
	for(unsigned int i=0; i<g->getNodeCount(); i++){
		nd = g->getNodeData(i);
		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
	}
	// Die normalen Kanten einzeichnen
	for(vector<text>::iterator it = txt->begin(); it!=txt->end(); it++){
		painter->drawText((*it).gdc, (*it).val.c_str());
	}
   painter->setPen(Qt::red);
	// Shortcuts einzeichnen
	for(vector<textsc>::iterator it = txtsc->begin(); it!=txtsc->end(); it++){
		painter->drawText((*it).gdc, (*it).val.c_str());
	}
	// Kontrahierte Knoten markieren
	for(list<unsigned int>::iterator i=(*blackn)->begin(); i!=(*blackn)->end(); i++){
		nd = g->getNodeData(*i);
		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
	}
}

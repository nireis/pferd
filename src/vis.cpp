#include "vis.h"

using namespace std;

vis::vis(SCGraph* g, vector<text>* txt, vector<textsc>* txtsc, list<unsigned int>* blackn,
		vector<linesc>* lnsc){
	this->g = g;
	this->txt = txt;
	this->txtsc = txtsc;
	this->blackn = blackn;
	this->lnsc = lnsc;
}

void vis::customPaint(GeoPainter* painter)
{
	ND nd;
   painter->setPen(Qt::black);
	// Die Knoten einzeichnen
	for(unsigned int i=0; i<g->getNodeCount(); i++){
		nd = g->getNodeData(i);
		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
		stringstream sstr;
		sstr << nd.elevation << " ("<< nd.id << ")";
		painter->drawText(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), sstr.str().c_str());
		/*stringstream sstr;
		sstr << i;
		painter->drawText(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), sstr.str().c_str());*/
	}
	// Die normalen Kanten einzeichnen
	for(vector<text>::iterator it = txt->begin(); it!=txt->end(); it++){
		painter->drawText(it->gdc, it->val.c_str());
	}
   painter->setPen(Qt::red);
	// Shortcuts einzeichnen
	for(vector<textsc>::iterator it = txtsc->begin(); it!=txtsc->end(); it++){
		painter->drawText(it->gdc, it->val.c_str());
	}
	// Kontrahierte Knoten markieren
	for(list<unsigned int>::iterator i=blackn->begin(); i!=blackn->end(); i++){
		nd = g->getNodeData(*i);
		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
	}
	for(vector<linesc>::iterator it = lnsc->begin(); it!=lnsc->end(); it++){
		painter->drawLine(it->gdc1, it->gdc2);
	}
}

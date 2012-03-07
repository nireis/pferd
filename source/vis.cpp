#include "vis.h"

using namespace std;

vis::vis(Graph* g, vector<text>* txt, vector<textsc>* txtsc){
	this->g = g;
	this->txt = txt;
	this->txtsc = txtsc;
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
	for(vector<text>::iterator it = txt->begin(); it!=txt->end(); it++){
		painter->drawText((*it).gdc, (*it).val.c_str());
	}
   painter->setPen(Qt::red);
	for(vector<textsc>::iterator it = txtsc->begin(); it!=txtsc->end(); it++){
		painter->drawText((*it).gdc, (*it).val.c_str());
	}
}

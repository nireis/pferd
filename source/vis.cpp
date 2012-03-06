#include "vis.h"
#include <iostream>

using namespace std;

vis::vis(Graph* g){
	this->g = g;
}

void vis::customPaint(GeoPainter* painter)
{
	ND nd;
   painter->setPen(Qt::black);
	for(unsigned int i=0; i<g->getNodeCount(); i++){
		nd = g->getNodeData(i);
		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
	}
   /*GeoDataCoordinates home(8.4, 49.0, 0.0, GeoDataCoordinates::Degree);
   painter->setPen(Qt::green);
   painter->drawEllipse(home, 7, 7);
   painter->setPen(Qt::black);
   painter->drawText(home, "Hello Marble!");*/
}

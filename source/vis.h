#ifndef vis_h
#define vis_h

#include <QtGui/QApplication>
#include <marble/MarbleWidget.h>
#include <marble/GeoPainter.h>
#include <iostream>
#include <sstream>
#include <list>
#include "graph.h"
#include "CHConstruction.h"
 
using namespace Marble;
 
class vis: public MarbleWidget
{
	public:
		struct text{
			GeoDataCoordinates gdc;
			string val;

			text(GeoDataCoordinates g, string v):gdc(g),val(v){}
		};
		struct textsc{
			GeoDataCoordinates gdc;
			string val;

			textsc(GeoDataCoordinates g, string v):gdc(g),val(v){}
		};
		struct linesc{
			GeoDataCoordinates gdc1;
			GeoDataCoordinates gdc2;

			linesc(GeoDataCoordinates g1, GeoDataCoordinates g2):gdc1(g1),gdc2(g2){}
		};
	private:
		Graph* g;
		vector<text>* txt;
		vector<textsc>* txtsc;
		list<unsigned int>** blackn;
		vector<linesc>* lnsc;
	public:
		vis(Graph* g, vector<text>* txt, vector<textsc>* txtsc, list<unsigned int>** blackn,
				vector<linesc>* lnsc);
		virtual void customPaint(GeoPainter* painter);
};

#endif

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
	private:
		Graph* g;
		vector<text>* txt;
		vector<textsc>* txtsc;
	public:
		vis(Graph* g, vector<text>* txt, vector<textsc>* txtsc);
		virtual void customPaint(GeoPainter* painter);
};

#endif

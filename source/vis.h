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
	private:
		Graph* g;
	public:
		vis(Graph* g);
		virtual void customPaint(GeoPainter* painter);
};

#endif

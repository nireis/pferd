#ifndef vis_h
#define vis_h

#include <QtGui/QApplication>
#include <marble/MarbleWidget.h>
#include <marble/GeoPainter.h>
#include "graph.h"
 
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

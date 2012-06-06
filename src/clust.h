#ifndef clust_h
#define clust_h

#include "structs.h"
#include "graph.h"
#include <list>

class cluster {
	private:
		Graph* g;
		double minX, maxX, minY, maxY;
		double dX, dY;

		/*
		 *	sinnvoller Wert etwa 
		 */
		double step;
		unsigned int Xcells, Ycells;

		struct cell {
			std::list<unsigned int> nodes;
		};

		cell** cells;

		struct point {
			double x;
			double y;

			point() : x(0), y(0) {}
			point(double a, double b) : x(a), y(b) {}
			point(NodeData nd) {
				//double pi = 3.141592;
				double p = (3.141592/180.0);
				double pos_radian = p* nd.lat;
				y = log((1.0 + sin(pos_radian))/cos(pos_radian));
				x = p*nd.lon;
			}
		};

		point* mostPop;
		unsigned int mostPopSize;
		
		cluster(){}

		void setXY();
		void run();

	public:
		cluster(Graph* gr, double s);
		~cluster();

		void setMostPopulatedCells(unsigned int count);

};

#endif

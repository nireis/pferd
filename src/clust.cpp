#include "clust.h"

cluster::cluster(Graph* gr, double s) : g(gr), step(s) {
	mostPop = 0;
	mostPopSize = 0;

	setXY();

	/*
	 *  Y
	 *  ^ ^ ^ ^ ^
	 *  | | | | |
	 *  ^ ^ ^ ^ ^
	 *  | | | | |
	 * ->->->->-> X
	 *
	 *  [X][Y]
	 */
	cells = new cell*[Xcells];
	for(unsigned int i = 0; i < Xcells; i++)
		cells[i] = new cell[Ycells];

	run();
}
cluster::~cluster(){
	for(unsigned int i = 0; i < Xcells; i++){
		delete[] cells[i];
		cells[i] = 0;
	}
	delete[] cells;
	cells = 0;

	delete[] mostPop; mostPop = 0;
}

void cluster::setXY(){
	point p = point( g->getNodeData(0) );
	minX = p.x;
	maxX = p.x;
	minY = p.y;
	maxY = p.y;
	for(unsigned int i = 0; i < g->getNodeCount(); i++){
		p = point( g->getNodeData(i) );
		if( p.x < minX ){
			minX = p.x;
		} else 
		if( p.x > maxX ){
			maxX = p.x;
		}
		if( p.y < minY ){
			minY = p.y;
		} else 
		if( p.y > maxY ){
			maxY = p.y;
		}
	}
	dX = maxX - minX;
	dY = maxY - minY;
	Xcells = (unsigned int)(dX / step) +1;
	Ycells = (unsigned int)(dY / step) +1;
	std::cout << " Cluster: " << std::endl << "  dX " << dX << std::endl;
	std::cout << "  dY " << dY << std::endl;
	std::cout << "  Xcells " << Xcells << std::endl
		<< "  Ycells " << Ycells << std::endl;
}
void cluster::run(){
	for(unsigned int i = 0; i < g->getNodeCount(); i++){
		point p = point( g->getNodeData(i) );
		cells[
		(unsigned int)((p.x - minX)/step)
		][
		(unsigned int)((p.y - minY)/step)
		].nodes.push_back(i);

	}
}

void cluster::setMostPopulatedCells(unsigned int count){
	mostPopSize = count;
	delete[] mostPop;
	mostPop = new point[count];
	
	for(unsigned int i = 0; i < Xcells; i++)
		for(unsigned int j = 0; j < Ycells; j++){
		}
		
}

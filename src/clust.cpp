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
	std::cout << "Cluster: " << std::endl << " dX " << dX << std::endl;
	std::cout << " dY " << dY << std::endl;
	std::cout << " Xcells " << Xcells << std::endl
		<< " Ycells " << Ycells << std::endl;
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
	unsigned int min = 0;
	unsigned int max = g->getNodeCount();

	unsigned int k = 0;
	while( k < count ){
		for(unsigned int i = 0; i < Xcells; i++)
			for(unsigned int j = 0; j < Ycells; j++){
				if(cells[i][j].nodes.size() > min 
						&& cells[i][j].nodes.size() < max){
					mostPop[k].x = i;
					mostPop[k].y = j;
					min = cells[i][j].nodes.size();
					/* Zellen drumherum null setzen */
					{
						unsigned int im = i-1;
						unsigned int ip = i+1;
						unsigned int jm = j-1;
						unsigned int jp = j+1;

						bool imv = (im < Xcells);
						bool ipv = (ip < Xcells);
						bool jmv = (jm < Ycells);
						bool jpv = (jp < Ycells);

						if(imv && jmv)
						cells[im][jm].nodes.clear();
						if(jmv)
						cells[i][jm].nodes.clear();
						if(ipv && jmv)
						cells[ip][jm].nodes.clear();
						if(imv)
						cells[im][j].nodes.clear();
						if(ipv)
						cells[ip][j].nodes.clear();
						if(imv && jpv)
						cells[im][jp].nodes.clear();
						if(jpv)
						cells[i][jp].nodes.clear();
						if(ipv && jpv)
						cells[ip][jp].nodes.clear();

						//unsigned int im2 = i-2;
						//unsigned int ip2 = i+2;
						//unsigned int jm2 = j-2;
						//unsigned int jp2 = j+2;

						//bool imv2 = (im2 < Xcells);
						//bool ipv2 = (ip2 < Xcells);
						//bool jmv2 = (jm2 < Ycells);
						//bool jpv2 = (jp2 < Ycells);

						//if(imv2 && jmv2)
						//cells[im2][jm2].nodes.clear();
						//if(jmv2)
						//cells[i][jm2].nodes.clear();
						//if(ipv2 && jmv2)
						//cells[ip2][jm2].nodes.clear();
						//if(imv2)
						//cells[im2][j].nodes.clear();
						//if(ipv2)
						//cells[ip2][j].nodes.clear();
						//if(imv2 && jpv2)
						//cells[im2][jp2].nodes.clear();
						//if(jpv2)
						//cells[i][jp2].nodes.clear();
						//if(ipv2 && jpv2)
						//cells[ip2][jp2].nodes.clear();
					}
				}
			}
		max = min;
		min = 0;
		k++;
	}
	
}

void cluster::getNodes(unsigned int count, std::list<unsigned int>* nodes){
	for(unsigned int i = 0; i < mostPopSize; i++){
		unsigned int x = (unsigned int)mostPop[i].x;
		unsigned int y = (unsigned int)mostPop[i].y;
		std::list<unsigned int>::iterator it = cells[x][y].nodes.begin();
		unsigned int counter = 0;
		while(counter < count && it != cells[x][y].nodes.end()){
			nodes->push_front(*it);
			it++;
			counter++;
		}
	}
}
void cluster::getNodes(unsigned int count, 
		std::vector< std::vector<unsigned int> >* nodes)
{
	if(!nodes){
		delete nodes;
		nodes = new std::vector< std::vector<unsigned int> >;
		nodes->reserve(mostPopSize);
	}
	for(unsigned int i = 0; i < mostPopSize; i++){
		unsigned int x = (unsigned int)mostPop[i].x;
		unsigned int y = (unsigned int)mostPop[i].y;
		std::list<unsigned int>::iterator it = cells[x][y].nodes.begin();
		unsigned int counter = 0;
		while(counter < count && it != cells[x][y].nodes.end()){
			//(*nodes)[i].push_back(*it);
			nodes->operator[](i).push_back(*it);
			it++;
			counter++;
		}
	}
}

void cluster::getNodesLower(unsigned int count, 
		unsigned int lower,
		std::list<unsigned int>* nodes,
		std::list< openGL_Cluster >* cluster)
{
	for(unsigned int i = mostPopSize-lower; i < mostPopSize; i++){
		unsigned int x = (unsigned int)mostPop[i].x;
		unsigned int y = (unsigned int)mostPop[i].y;
		std::list<unsigned int>::iterator it = cells[x][y].nodes.begin();
		unsigned int counter = 0;
		while(counter < count && it != cells[x][y].nodes.end()){
			nodes->push_front(*it);
			it++;
			counter++;
		}
		if(! nodes->empty()){
			float x = g->getNodeData(nodes->front()).lon ;
			float y = g->getNodeData(nodes->front()).lat ;
			float r = step/1.0 ;
			float c = 0.25 + (0.0) ;
			cluster->push_front( openGL_Cluster(x, y, r, c) );
		}
	}
}

void cluster::getNodesUpper(unsigned int count, 
		unsigned int upper,
		std::list<unsigned int>* nodes,
		std::list< openGL_Cluster >* cluster)
{
	for(unsigned int i = 0; i < (upper); i++){
		unsigned int x = (unsigned int)mostPop[i].x;
		unsigned int y = (unsigned int)mostPop[i].y;
		std::list<unsigned int>::iterator it = cells[x][y].nodes.begin();
		unsigned int counter = 0;
		while(counter < count && it != cells[x][y].nodes.end()){
			nodes->push_front(*it);
			it++;
			counter++;
		}
		if(! nodes->empty()){
			float x = g->getNodeData(nodes->front()).lon ;
			float y = g->getNodeData(nodes->front()).lat ;
			float r = step/1.0 ;
			float c = 0.75 + (0.0) ;
			cluster->push_front( openGL_Cluster(x, y, r, c) );
		}
	}
}


#include "vis.h"

using namespace std;

vis::vis(Graph* g) : render() {
	
}

vis::vis(SCGraph* g) : render() {
	nodes = new openGL_Node_3d[g->getNodeCount()];
	edges = new openGL_Node_3d[2 * ( g->getEdgeCount() + g->getShortcutCount() )];

	NodeData* node_data = g->getNodeDataPointer();

	for(unsigned int i = 0; i < g->getNodeCount(); i++){
		nodes[i] = openGL_Node_3d( node_data[i].lon , node_data[i].lat , 0.0 );
	}

	unsigned int index = 0;
	for(unsigned int i = 0; i < g->getNodeCount() ; i++){
		EdgesIterator it = g->getOutEdgesIt(i);
		while( it.hasNext() ){
			Edge e = * it.getNext();
			edges[index] = 
				openGL_Node_3d( (float) node_data[ i ].lon, (float) node_data[ i ].lat, (float) g->isShortcut( e.id) );
			index++;
			edges[index] = 
				openGL_Node_3d( (float) node_data[ e.other_node ].lon, (float) node_data[ e.other_node ].lat, (float) g->isShortcut( e.id) );
			index++;
		}
	}

	render.setNodeCount(g->getNodeCount());
	render.setNodeArray(nodes);
	render.setEdgeCount((g->getEdgeCount() + g->getShortcutCount()) *2);
	render.setEdgeArray(edges);
	render.setCamera((float) node_data[0].lon, (float)node_data[0].lat, 2.0);

	node_data = 0;
}

vis::~vis(){
	delete[] nodes; nodes = 0;
	delete[] edges; edges = 0;
}

bool vis::start(){
	return render.start(0,0);
}

// vis::vis(SCGraph* g, vector<text>* txt, vector<textsc>* txtsc, list<unsigned int>* blackn,
// 		vector<linesc>* lnsc){
// 	this->g = g;
// 	this->txt = txt;
// 	this->txtsc = txtsc;
// 	this->blackn = blackn;
// 	this->lnsc = lnsc;
// }
// 
// void vis::customPaint(GeoPainter* painter)
// {
// 	ND nd;
//    painter->setPen(Qt::black);
// 	// Die Knoten einzeichnen
// 	for(unsigned int i=0; i<g->getNodeCount(); i++){
// 		nd = g->getNodeData(i);
// 		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
// 		stringstream sstr;
// 		sstr << nd.elevation << " ("<< nd.id << ") " ;
// 		if(nd.id == 1998)
// 			sstr << " ================================================";
// 		painter->drawText(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), sstr.str().c_str());
// 		/*stringstream sstr;
// 		sstr << i;
// 		painter->drawText(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), sstr.str().c_str());*/
// 	}
// 	// Die normalen Kanten einzeichnen
// 	for(vector<text>::iterator it = txt->begin(); it!=txt->end(); it++){
// 		painter->drawText(it->gdc, it->val.c_str());
// 	}
//    painter->setPen(Qt::red);
// 	// Shortcuts einzeichnen
// 	for(vector<textsc>::iterator it = txtsc->begin(); it!=txtsc->end(); it++){
// 		painter->drawText(it->gdc, it->val.c_str());
// 	}
// 	// Kontrahierte Knoten markieren
// 	for(list<unsigned int>::iterator i=blackn->begin(); i!=blackn->end(); i++){
// 		nd = g->getNodeData(*i);
// 		painter->drawEllipse(GeoDataCoordinates(nd.lon, nd.lat, nd.elevation, GeoDataCoordinates::Degree), 5, 5);
// 	}
// 	for(vector<linesc>::iterator it = lnsc->begin(); it!=lnsc->end(); it++){
// 		painter->drawLine(it->gdc1, it->gdc2);
// 	}
// }

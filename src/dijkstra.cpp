#include "dijkstra.h"

Dijkstra::Dijkstra(Graph* g):
	nr_of_nodes(g->getNodeCount()),
	dist(nr_of_nodes),
	o_found_by(2,vector<int> (nr_of_nodes,-1)),
	m_found_by(nr_of_nodes,-1),
	o_reset_found_by(2){
	this->g = g;
	o_reset_found_by[0].reserve(nr_of_nodes);
	o_reset_found_by[1].reserve(nr_of_nodes);
	m_reset_found_by.reserve(nr_of_nodes);
}

unsigned int Dijkstra::oneToOne(unsigned int node_id0, unsigned int node_id1,
		unsigned int weight){
	// Wegen Codeschönheit: Array für Source und Target anlegen.
	vector<unsigned int> node_id(2);
	node_id[0] = node_id0;
	node_id[1] = node_id1;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element_bi, std::vector<U_element_bi>, Compare_U_element_bi> U;
	unsigned int min_path_length = numeric_limits<unsigned int>::max();
	unsigned int min_path_center;

	//unsigned int numEdges = 0;
	//unsigned int numNodes = 2;

	// Die ersten Knoten abarbeiten
	if(node_id0 == node_id1){
		min_path_center = node_id0;
		return 0;
	}
	// Die Kanten der ersten beiden Knoten anschauen und die targets zu
	// U hinzufügen, wenn das Level größer ist.
	for(int i=0; i<2; i++){
		o_found_by[i][node_id[i]] = (int)node_id[i];
		o_reset_found_by[i].push_back(node_id[i]);
		EdgesIterator it = g->getEdgesIt(i, node_id[i]);
		while(it.hasNext()){
			Edge* currentEdge = it.getNext();
			unsigned int currentEdgeTarget = currentEdge->other_node;
			// Wenn wir schon den anderen Knoten gefunden haben...
			if(currentEdgeTarget == node_id[(i+1)%2]){
				min_path_length = currentEdge->value;
				// Die Werte für das Backtracing setzen.
				min_path_center = node_id[(i+1)%2];
				o_found_by[i][node_id[(i+1)%2]] = (int)currentEdge->id;
				o_reset_found_by[i].push_back(node_id[(i+1)%2]);
			}
			else{
				// ...sonst tu sie in U
				U.push(U_element_bi(
					currentEdge->value,currentEdge->other_node,currentEdge->id,i));
			}
		}
	}
	dist[node_id0] = 0;
	dist[node_id1] = 0;

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid = node_id0;
	while(!U.empty() && min_path_length > dist[tmpid]){
		int i = U.top().found_by;
		tmpid = U.top().id;
		if(o_found_by[i][tmpid] == -1){
			o_found_by[i][tmpid] = (int)U.top().eid;
			o_reset_found_by[i].push_back(tmpid);
			unsigned int tmp_min_path_length = dist[tmpid] + U.top().distance;
			// Wir prüfen ob ein minimaler Pfad gefunden wurde.
			if(o_found_by[(i+1)%2][tmpid] != (-1) && tmp_min_path_length < min_path_length){
				min_path_length = tmp_min_path_length;
				min_path_center = tmpid;
			}
			dist[tmpid] = U.top().distance;
			EdgesIterator it = g->getEdgesIt(i, tmpid);
			while(it.hasNext()){
				Edge* currentEdge = it.getNext();
				unsigned int currentEdgeTarget = currentEdge->other_node;
				// Nur Knoten, die wir noch nicht gesettled haben.
				if(o_found_by[i][currentEdgeTarget] == -1){
					U.push(U_element_bi(
						currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id,i));
				}
			}
		}
		U.pop();
	}

	// Backtracing.
	// Nur, wenn ein Pfad existiert...
	if(min_path_length != numeric_limits<unsigned int>::max()){
		// ...gehe von dem Treffpunkt jeweils zurück zum Anfangsknoten
		// und speichere die Kanten-IDs.
		for(int i=0; i<2; i++){
			unsigned int tmpid = min_path_center;
			while(tmpid != node_id[i]){
				int takenedge = o_found_by[i][tmpid];
				g->addEdgeLoad(takenedge, weight);
				tmpid = g->getEdge(i, (unsigned int)takenedge)->other_node;
			}
		}
	}

	resetOne();
	return min_path_length;
}

void Dijkstra::resetOne(){
	for(int i=0; i<2; i++){
		while(!o_reset_found_by[i].empty()){
			o_found_by[i][o_reset_found_by[i].back()] = -1;
			o_reset_found_by[i].pop_back();
		}
	}
}

void Dijkstra::oneToMany(unsigned int node_id0, vector<unsigned int>* targets,
		unsigned int weight){
	// Von node_id0 aus einen "normalen" Dijkstra machen und dabei aufsteigende
	// und markierte Kanten benutzen. Sobald wir alle targets gefunden haben
	// brechen wir ab.
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	unsigned int tmpnode;

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	for(unsigned int i=0; i<targets->size(); i++){
		unsigned int target = (*targets)[i];
		if(m_found_by[target] == -1){
			while(!U.empty() && U.top().id != target){
				// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
				tmpnode = U.top().id;
				if(m_found_by[tmpnode] == -1){
					dist[tmpnode] = U.top().distance;
					m_found_by[tmpnode] = (int)U.top().eid;
					m_reset_found_by.push_back(tmpnode);
					// Die ausgehenden Kanten durchgehen und wenn sie aufwärts gehen oder
					// markiert sind auf ihnen weitersuchen.
					EdgesIterator it = g->getOutEdgesIt(tmpnode);
					while(it.hasNext()){
						Edge* tmpedge = it.getNext();
						// Wenn der Knoten noch nicht gefunden wurde UND (sein Level größer ist
						// ODER die Kante markiert ist).
						if(m_found_by[tmpedge->other_node] == -1){
							// ...tu sie in U
							U.push(U_element(
										tmpedge->value+U.top().distance,tmpedge->other_node,tmpedge->id));
						}
					}
				}
				U.pop();
			}
		}
		int tmpfoundby = -1;
		// Die Distanz des Knotens setzen, je nachdem ob er...
		if(m_found_by[target] != -1){
			//...schon gefunden wurde...
			(*targets)[i] = dist[target];
			tmpfoundby = m_found_by[target];
		}
		else{
			if(!U.empty() && U.top().id == target){
				(*targets)[i] = U.top().distance;
				tmpfoundby = (int)U.top().eid;
				//...erst gerade gefunden wurde...
				if(target == targets->back()){
					dist[target] = U.top().distance;
					m_found_by[target] = (int)U.top().eid;
					m_reset_found_by.push_back(target);
				}
			}
			else{
				//...oder garnicht gefunden wurde.
				(*targets)[i] = numeric_limits<unsigned int>::max();
				dist[target] = numeric_limits<unsigned int>::max();
			}
		}
		// Backtracing der Knoten.
		if((*targets)[i] != numeric_limits<unsigned int>::max()){
			// cout << "=== Node " << target << " ===" << endl;
			// ersten Knoten
			tmpnode = target;
			if(tmpnode != node_id0){
				// cout << tmpnode << endl;
				int takenedge = tmpfoundby;
				// path->push_front(takenedge);
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
			// restlichen Knoten
			while(tmpnode != node_id0){
				// cout << tmpnode << endl;
				int takenedge = m_found_by[tmpnode];
				// path->push_front(takenedge);
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
		}
	}
	resetMany();
}

void Dijkstra::manyToOne(vector<unsigned int>* sources, unsigned int node_id0,
		unsigned int weight){
	// Von node_id0 aus einen "normalen" Dijkstra machen und dabei aufsteigende
	// und markierte Kanten benutzen. Sobald wir alle sources gefunden haben
	// brechen wir ab.
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	unsigned int tmpnode;

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	for(unsigned int i=0; i<sources->size(); i++){
		unsigned int target = (*sources)[i];
		if(m_found_by[target] == -1){
			while(!U.empty() && U.top().id != target){
				// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
				tmpnode = U.top().id;
				if(m_found_by[tmpnode] == -1){
					dist[tmpnode] = U.top().distance;
					m_found_by[tmpnode] = (int)U.top().eid;
					m_reset_found_by.push_back(tmpnode);
					// Die ausgehenden Kanten durchgehen und wenn sie aufwärts gehen oder
					// markiert sind auf ihnen weitersuchen.
					EdgesIterator it = g->getInEdgesIt(tmpnode);
					while(it.hasNext()){
						Edge* tmpedge = it.getNext();
						// Wenn der Knoten noch nicht gefunden wurde UND (sein Level größer ist
						// ODER die Kante markiert ist).
						if(m_found_by[tmpedge->other_node] == -1){
							// ...tu sie in U
							U.push(U_element(
										tmpedge->value+U.top().distance,tmpedge->other_node,tmpedge->id));
						}
					}
				}
				U.pop();
			}
		}
		int tmpfoundby = -1;
		// Die Distanz des Knotens setzen, je nachdem ob er...
		if(m_found_by[target] != -1){
			//...schon gefunden wurde...
			(*sources)[i] = dist[target];
			tmpfoundby = m_found_by[target];
		}
		else{
			if(!U.empty() && U.top().id == target){
				(*sources)[i] = U.top().distance;
				tmpfoundby = (int)U.top().eid;
				//...erst gerade gefunden wurde...
				if(target == sources->back()){
					dist[target] = U.top().distance;
					m_found_by[target] = (int)U.top().eid;
					m_reset_found_by.push_back(target);
				}
			}
			else{
				//...oder garnicht gefunden wurde.
				(*sources)[i] = numeric_limits<unsigned int>::max();
				dist[target] = numeric_limits<unsigned int>::max();
			}
		}
		// Backtracing der Knoten.
		if((*sources)[i] != numeric_limits<unsigned int>::max()){
			// ersten Knoten
			tmpnode = target;
			if(tmpnode != node_id0){
				int takenedge = tmpfoundby;
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getOutEdge((unsigned int)takenedge)->other_node;
			}
			// restlichen Knoten
			while(tmpnode != node_id0){
				int takenedge = m_found_by[tmpnode];
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getOutEdge((unsigned int)takenedge)->other_node;
			}
		}
	}
	resetMany();
}

void Dijkstra::resetMany(){
	while(!m_reset_found_by.empty()){
		m_found_by[m_reset_found_by.back()] = -1;
		m_reset_found_by.pop_back();
	}
}

void Dijkstra::getNeighbours(Graph* g, unsigned int node_id, unsigned int radius,
		vector<unsigned int>* nbrs){
	// Iterator für die ausgehenden Kanten eines Knotens
	EdgesIterator it = g->getOutEdgesIt(node_id);
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	Edge* currentEdge;
	vector<bool> n_found(nr_of_nodes,false);

	// Den ersten Knoten abarbeiten
	n_found[node_id] = true;
	while(it.hasNext()){
		currentEdge = it.getNext();
		// Die Knoten mit ihrer Distanz in U stecken
		U.push(U_element(currentEdge->value,currentEdge->other_node,currentEdge->id));
	}

	// Die restlichen Knoten abarbeiten
	unsigned int tmpid;
	unsigned int tmpdist;
	while(!U.empty() && U.top().distance <= radius){
		// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
		tmpid = U.top().id;
		if(!n_found[tmpid]){
			nbrs->push_back(tmpid);
			tmpdist = U.top().distance;
			n_found[tmpid] = true;
			// Die ausgehenden Kanten durchgehen und in U werfen
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				currentEdge = it.getNext();
				// Wenn sie noch nicht gefunden wurde...
				if(!n_found[currentEdge->other_node]){
					// ...tu sie in U
					U.push(U_element(
								currentEdge->value+tmpdist,currentEdge->other_node,currentEdge->id));
				}
			}
		}
		U.pop();
	}
	resetNeigh(nbrs, node_id);
}

void Dijkstra::resetNeigh(vector<unsigned int>* n_reset_found, unsigned int node_id){
	n_found[node_id] = false;
	for(unsigned int i=0; i<n_reset_found->size(); i++){
		n_found[(*n_reset_found)[i]] = false;
	}
}

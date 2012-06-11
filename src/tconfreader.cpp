#include "tconfreader.h"

#include <stdexcept>

tconfreader::tconfreader(string fname){
	filename = fname;
	buffer = "";
	working = false;
}

bool tconfreader::readLine(){
	
	if(f.eof()){
		buffer = "";
		working = false;
		value.clear();
		return false;
	}
	else{
		getline(f,buffer,'\n');
		if(buffer== ""){
			readLine();
		}
		else{
			string::iterator it_begin = buffer.begin();
			string::iterator it_pos = buffer.begin()-1;
			value.clear();
			
			do{
				it_pos++;
				if(*it_pos == ' ' || it_pos == buffer.end()){
					string tmp_str;
					tmp_str.assign(it_begin, it_pos);
					value.push_back(tmp_str);
					it_begin = it_pos+1;
				}
			}
			while(it_pos != buffer.end());
		}
		return true;
	}
}

vector<travelconf>* tconfreader::readConf(){
	working = true;
	
	vector<travelconf>* trconf = new vector<travelconf>;
	
	f.open(filename.c_str(), ios::in);
	if(f.is_open()){
		f.seekg(0, ios::beg);
		
		if(!readLine()){
			cout << "Fehler: traffic-Konfigurationsdatei leer" << endl;
			f.close();
			throw std::runtime_error("file does not meet requirements");
		}
		while(working){
			if(value[0] == "{"){
				travelconf tmp;
				tmp.mode = 0;
				tmp.max_travelers = 1;
				tmp.source_count = 1;
				tmp.target_count = 1;
				tmp.weight_lower_bound = 1;
				tmp.weight_upper_bound = 1;
				
				if(!readLine()){
					cout << "Error while parsing traffic-config: \"}\" expected." << endl;
					f.close();
					throw std::runtime_error("file does not meet requirements");
				}
				while(value[0] != "}"){
					if(value[0] == "mode"){
						if(value.size() == 2){
							tmp.mode = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						  
						}
					}
					else if(value[0] == "count"){
						if(value.size() == 2){
							tmp.max_travelers = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "traveler_min"){
						if(value.size() == 2){
							tmp.weight_lower_bound = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "traveler_max"){
						if(value.size() == 2){
							tmp.weight_upper_bound = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "source_count"){
						if(value.size() == 2){
							tmp.source_count = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "target_count"){
						if(value.size() == 2){
							tmp.target_count = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "clust_step"){
						if(value.size() == 2){
							tmp.clust_step = atof(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "clust_top_percentage"){
						if(value.size() == 2){
							tmp.clust_top_percentage = atof(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "clust_count_top_clusters"){
						if(value.size() == 2){
							tmp.clust_count_top_clusters = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "clust_top_uppers"){
						if(value.size() == 2){
							tmp.clust_top_uppers = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "clust_top_lowers"){
						if(value.size() == 2){
							tmp.clust_top_lowers = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "clust_top_upper_nodecount_per_cluster"){
						if(value.size() == 2){
							tmp.clust_top_upper_nodecount_per_cluster = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "clust_top_lower_nodecount_per_cluster"){
						if(value.size() == 2){
							tmp.clust_top_lower_nodecount_per_cluster = atoi(value[1].c_str());
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					else if(value[0] == "nodes"){
						if(value.size() > 1){
							tmp.nodes.reserve(value.size() - 1);
							for(int i = 1; i < value.size(); i++){
								tmp.nodes.push_back(atoi(value[1].c_str()));
							}
						}
						else{
							cout << "Error while parsing traffic-config: Illegal number of arguments for \"" << value[0] <<  "\"" << endl;
							f.close();
							throw std::runtime_error("file does not meet requirements");
						}
					}
					
					else{
						cout << "Error while parsing traffic-config: Unknown argument: \"" << value[0] << "\"" << endl;
						f.close();
						throw std::runtime_error("file does not meet requirements");
					}
					
					if(!readLine()){
						cout << "Error while parsing traffic-config: \"}\" expected." << endl;
						f.close();
						throw std::runtime_error("file does not meet requirements");
					}
				}
				
				trconf->push_back(tmp);
				
			}
			else{
				cout << "Error while parsing traffic-config: \"{\" expected." << endl;
				f.close();
				throw std::runtime_error("file does not meet requirements");
			}
		}
		
	}
	else{
		cout << "Fehler: Öffnen der traffic-Konfigurationsdatei \"" << filename << "\" fehlgeschlagen." << endl;
		throw std::runtime_error("file could not be opened");
	}
	f.close();
	return trconf;
}

#include "graph.h"
#include "CHConstruction.h"
#include "conf.h"
#include "clust.h"
#include "structs.h"
#include "sim.h"
#include "parser.h"
#include "ch.h"

#include <ctime> 
#include <fstream> 
#include <stdio.h> 
#include <iostream> 
 
using namespace std;

	/*
	 * mit Widmung an Benjamin Prölß
	 */

Graph* g;
SCGraph* scg;
CH* ch;
CHDijkstra* chd;
Dijkstra* d;

const char* folder = "../benchdata";

const char* fileending = ".txt.grp";
const char* graphs[] = 
{	"../data/15K", // Reutlingen oder so
	"../data/150K", // Pforzheim ?
	"../data/500K", // Stuttgart und VVS-Gebiet
	"../data/1500K", // AUsschnitt von BW
	"../data/15000" // Deutschland
};
const unsigned int graphs_count = 5;

clock_t start, finish;

void CH_Times(unsigned int runden){
	string dataname = "ch_constr_times.dat";
	cout << "Starte " << dataname << endl;
	ofstream file;
	file.open(string(folder)+"/"+dataname);
	file << "# " << dataname << " \n";
	file << "# Zeiten gemittelt über " << runden << " Runden \n"; 
	file << "# " 
		<< "Graph NodeCount"
		<<	", EdgeCount"
		<<	", ShortcutCount"
		<<	", graph_init_time" 
		<<	", scgr_init_time"
		<<	", ch_init_time"
		<<	", ch_calc_time"
		<<	", cleanup_time"
		<<	", times_sum"
		<<	" \n";


	for(unsigned int i = 0; i < graphs_count ; i++){
		double graph_init_time=0.0;
		double scgr_init_time=0.0; 
		double ch_init_time=0.0; 
		double ch_calc_time=0.0;
		double cleanup_time=0.0;
		unsigned int NodeCount=0;
		unsigned int EdgeCount=0;
		unsigned int ShortcutCount=0;
	for(unsigned int wiederholung = 0; wiederholung < runden; wiederholung++){

			start = clock();
		g = new Graph();
		g->setGraph(string(graphs[i])+string(fileending), true);
			finish = clock();
			graph_init_time += (double(finish)-double(start))/CLOCKS_PER_SEC;

		sim s = sim(g, 0, 0);

			NodeCount = g->getNodeCount();
			EdgeCount = g->getEdgeCount();


			start = clock();
		scg = new SCGraph(g);
			finish = clock();
			scgr_init_time += (double(finish)-double(start))/CLOCKS_PER_SEC;


			start = clock();
		ch = new CH(g, scg, 1); // starte CH mit einem Thread
			finish = clock();
			ch_init_time += (double(finish)-double(start))/CLOCKS_PER_SEC;


			start = clock();
		ch->calcCH(false);
			finish = clock();
			ch_calc_time += (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Graph fertig mit " << scg->getShortcutCount() << " Shortcuts"<< endl;
			ShortcutCount = scg->getShortcutCount();

			start = clock();
		delete ch; ch = 0;
		delete scg; scg = 0;
		delete g; g = 0;
			finish = clock();
			cleanup_time += (double(finish)-double(start))/CLOCKS_PER_SEC;

	}
		file << NodeCount << " "
			<< EdgeCount << " "
			<< ShortcutCount << " "
			<< graph_init_time/double(runden) << " " 
			<< scgr_init_time/double(runden) << " "
			<< ch_init_time/double(runden) << " "
			<< ch_calc_time/double(runden) << " "
			<< cleanup_time/double(runden) << " "
			<< (graph_init_time + scgr_init_time + ch_init_time + ch_calc_time + cleanup_time)/double(runden) 
			<< " \n";
	}

	file.close();
	cout << dataname << " Berechnung fertig. " << endl;
}

void All2AllTimes(unsigned int runden){
	string dataname = "all_to_all_times.dat";
	cout << "Starte " << dataname << endl;
	ofstream file;
	file.open(string(folder)+"/"+dataname);
	file << "# " << dataname << " \n";
	file << "# Zeiten gemittelt über " << runden << " Runden \n"; 
	file << "# " 
		<< "Graph NodeCount"
		<<	", EdgeCount"
		<<	", ShortcutCount"
		<< ", ch_constr_time"
		//<<	", o2o" 
		//<<	", o2och"
		<<	", o2m"
		<<	", o2mch"
		<< ", init_time_chd"
		<< ", init_time_d"
		<<	", cleanup_time_chd"
		<<	", cleanup_time_d"
		<<	" \n";

	for(unsigned int i = 0; i < /* nur der erste machbar */ 1; i++){
		double ch_constr_time=0.0;
		//double o2o=0.0;
		//double o2och=0.0; 
		double o2m=0.0; 
		double o2mch=0.0;
		double chd_init_time=0.0;
		double d_init_time=0.0;
		double chd_cleanup=0.0;
		double d_cleanup=0.0;
		unsigned int NodeCount=0;
		unsigned int EdgeCount=0;
		unsigned int ShortcutCount=0;

		g = new Graph();
		g->setGraph(string(graphs[i])+string(fileending), true);

		sim s = sim(g, 0, 0);

			NodeCount = g->getNodeCount();
			EdgeCount = g->getEdgeCount();

			start = clock();
		scg = new SCGraph(g);
		ch = new CH(g, scg, 1); // starte CH mit einem Thread
		ch->calcCH(false);
			finish = clock();
			ch_constr_time += (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Graph fertig mit " 
			<< scg->getShortcutCount() << " Shortcuts"<< endl;
			ShortcutCount = scg->getShortcutCount();

		vector<unsigned int> source(g->getNodeCount());
		vector<unsigned int> target(g->getNodeCount());
		for(unsigned int i = 0; i < g->getNodeCount(); i++)
		{
			source[i] = i;
			target[i] = i;
		}
	
	for(unsigned int wiederholung = 0; wiederholung < runden; wiederholung++){

		// starte Dijkstras ab hier

			start = clock();
		chd = new CHDijkstra(scg);
			finish = clock();
			chd_init_time += (double(finish)-double(start))/CLOCKS_PER_SEC;
		{
			//start = clock();
			//for(unsigned int i = 0; i < source.size(); i++)
			//for(unsigned int j = 0; j < target.size(); j++)
			//{
			//chd->oneToOne(source[i], target[j], 1);
			//}
			//finish = clock();
			//o2och += (double(finish)-double(start))/CLOCKS_PER_SEC;
			//cout << o2och/double(NodeCount*NodeCount) << " Zeit o2och" << endl;

			start = clock();
			for(unsigned int i = 0; i < source.size(); i++)
			{
			chd->oneToMany(source[i], &target, 1);
			}
			finish = clock();
			o2mch += (double(finish)-double(start))/CLOCKS_PER_SEC;
			cout << o2mch/double(NodeCount) << " Zeit o2mch" << endl;
		}
			start = clock();
		delete chd; chd = 0;
			finish = clock();
			chd_cleanup += (double(finish)-double(start))/CLOCKS_PER_SEC;

		cout << "CHDijkstras fertig. Starte langsame Dijkstras. " << endl;
		
			start = clock();
		d = new Dijkstra(g);
			finish = clock();
			d_init_time += (double(finish)-double(start))/CLOCKS_PER_SEC;
		{
			//start = clock();
			//for(unsigned int i = 0; i < source.size(); i++)
			//for(unsigned int j = 0; j < target.size(); j++)
			//{
			//d->oneToOne(source[i], target[j], 1);
			//}
			//finish = clock();
			//o2o += (double(finish)-double(start))/CLOCKS_PER_SEC;
			//cout << o2o/double(NodeCount*NodeCount) << " Zeit o2o" << endl;
			
			start = clock();
			for(unsigned int i = 0; i < source.size(); i++)
			{
			d->oneToMany(source[i], &target, 1);
			}
			finish = clock();
			o2m += (double(finish)-double(start))/CLOCKS_PER_SEC;
			cout << o2m/double(NodeCount) << " Zeit o2m" << endl;
		}
			start = clock();
		delete d; d = 0;
			finish = clock();
			d_cleanup += (double(finish)-double(start))/CLOCKS_PER_SEC;


		delete ch; ch = 0;
		delete scg; scg = 0;
		delete g; g = 0;
	}

		file << NodeCount << " "
			<< EdgeCount << " "
			<< ShortcutCount << " "
			<< ch_constr_time/double(runden) << " "
			//<< o2o/double(runden) << " "
		 	//<< o2och/double(runden) << " " 
		 	<< o2m/double(runden)/double(NodeCount) << " " 
		 	<< o2mch/double(runden)/double(NodeCount) << " " 
		 	<< chd_init_time/double(runden) << " "
		 	<< d_init_time/double(runden) << " "
		 	<< chd_cleanup/double(runden) << " "
		 	<< d_cleanup/double(runden) << " "
			<< " \n";
	}

	file.close();
	cout << dataname << " Berechnung fertig. " << endl;
}

void One2_All_Times(unsigned int i, unsigned int up_counter, unsigned int sources_count){
	string dataname = string(graphs[i]).replace(0,8,"")+"-"+"one_to_All_times.dat";
	cout << "Starte " << dataname << endl;
	ofstream file;
	file.open(string(folder)+"/"+dataname);
	file << "# " << dataname << " \n";
	file << "# " << graphs[i] << " \n";
	file << "# Sources pro N Targets (gemittelte Dijkstras pro Wert): "  << sources_count << " \n";

		double ch_constr_time;
		double o2m_time; 
		double o2mch_time;
		double chd_init_time;
		double d_init_time;
		double chd_cleanup;
		double d_cleanup;
		unsigned int current_N;
		unsigned int NodeCount;
		unsigned int EdgeCount;
		unsigned int ShortcutCount;

		g = new Graph();
		g->setGraph(string(graphs[i])+string(fileending), true);

		sim s = sim(g, 0, 0);

			NodeCount = g->getNodeCount();
			EdgeCount = g->getEdgeCount();

			start = clock();
		scg = new SCGraph(g);
		ch = new CH(g, scg, 1); // starte CH mit einem Thread
		ch->calcCH(false);
			finish = clock();
			ch_constr_time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		cout << "Graph fertig mit " 
			<< scg->getShortcutCount() << " Shortcuts"<< endl;
			ShortcutCount = scg->getShortcutCount();

		file << "# " 
			<< "Graph NodeCount"
			<<	", EdgeCount"
			<<	", ShortcutCount"
			<< ", ch_constr_time"
			<< " \n";

		file << "# " 
			<< NodeCount << " "
			<< EdgeCount << " "
			<< ShortcutCount << " "
			<< ch_constr_time << " "
			<< " \n";

		file << "# " 
			<< ", current_N"
			<< ", o2m_time"
			<< ", o2mch_time"
			//<< ", chd_init_time"
			//<< ", d_init_time"
			//<< ", chd_cleanup"
			//<< ", d_cleanup"
			<<	" \n";

		// starte Dijkstras ab hier
		// initialisiere source / target vektor random

		std::srand((unsigned)std::time(0));
		unsigned int max_rand = RAND_MAX - (RAND_MAX % NodeCount);

		// lege zufällig permutierte arrays an
		vector<unsigned int> target(g->getNodeCount());
		vector<unsigned int> source(sources_count);
		for(unsigned int j=0; j < sources_count; j++){
			unsigned int ran = rand();
			while( ran > max_rand )
				ran = rand();
			ran = ran % NodeCount;
			source[j] = ran;
		}
		for(unsigned int j = 0; j < g->getNodeCount(); j++)
		{
			target[j] = j;
		}
		for(unsigned int j = 0; j < g->getNodeCount(); j++)
		{
			unsigned int randIndex1 = rand();
			while( randIndex1 > max_rand )
				randIndex1 = rand();
			randIndex1 = randIndex1 % NodeCount;

			unsigned int randIndex2 = rand();
			while( randIndex2 > max_rand )
				randIndex2 = rand();
			randIndex2 = randIndex2 % NodeCount;

			unsigned int tmp = target[randIndex1];
			target[randIndex1] = target[randIndex2];
			target[randIndex2] = tmp;
		}
		vector<unsigned int> target_plate;
		current_N = 1;
		target_plate.push_back( target[0] );

			start = clock();
		chd = new CHDijkstra(scg);
			finish = clock();
			chd_init_time = (double(finish)-double(start))/CLOCKS_PER_SEC;
		
			start = clock();
		d = new Dijkstra(g);
			finish = clock();
			d_init_time = (double(finish)-double(start))/CLOCKS_PER_SEC;

		while(current_N < NodeCount)
		{

			{
				start = clock();
				for(unsigned int j = 0; j < source.size(); j++)
				{
				chd->oneToMany(source[j], &target, 1);
				}
				finish = clock();
				o2mch_time = (double(finish)-double(start))/CLOCKS_PER_SEC;
				o2mch_time = o2mch_time / double(sources_count);
				cout << o2mch_time/double(sources_count) << " Zeit o2mch" << endl;
			}
			{
				start = clock();
				for(unsigned int j = 0; j < source.size(); j++)
				{
				d->oneToMany(source[j], &target, 1);
				}
				finish = clock();
				o2m_time = (double(finish)-double(start))/CLOCKS_PER_SEC;
				o2m_time = o2m_time / double(sources_count);
				cout << o2m_time/double(sources_count) << " Zeit o2m" << endl;
			}

			file << current_N << " "
				<< o2m_time << " " 
				<< o2mch_time << " " 
				//<< chd_init_time << " "
				//<< d_init_time << " "
				//<< chd_cleanup << " "
				//<< d_cleanup << " "
				<< " \n";

			// fülle neue targets nach
			current_N += up_counter;
			target_plate.resize(current_N);
			for(unsigned int i = current_N-up_counter; i < current_N; i++)
			{
				target_plate[i] = target[i];
			}
		}
			start = clock();
		delete chd; chd = 0;
			finish = clock();
			chd_cleanup = (double(finish)-double(start))/CLOCKS_PER_SEC;

			start = clock();
		delete d; d = 0;
			finish = clock();
			d_cleanup = (double(finish)-double(start))/CLOCKS_PER_SEC;

		file << "# " 
			<< ", chd_init_time"
			<< ", d_init_time"
			<< ", chd_cleanup"
			<< ", d_cleanup"
			<< " \n";
		file << "# " 
			<< chd_init_time << " "
			<< d_init_time << " "
			<< chd_cleanup << " "
			<< d_cleanup << " "
			<< " \n";

		delete ch; ch = 0;
		delete scg; scg = 0;
		delete g; g = 0;


	file.close();
	cout << dataname << " Berechnung fertig. " << endl;
}

void O2O_Times(unsigned int runden){
	string dataname = "one_to_one_times.dat";
	cout << "Starte " << dataname << endl;
	ofstream file;
	file.open(string(folder)+"/"+dataname);
	file << "# " << dataname << " \n";
	file << "# Zeiten gemittelt über " << runden << " einzelne random Dijkstras \n"; 

	file << "# " 
		<< "Graph NodeCount"
		<<	", EdgeCount"
		<<	", ShortcutCount"
		<< ", o2o_time"
		<< ", o2och_time"
		<< ", o2o_one_time"
		<< ", o2och_one_time"
		<<	" \n";


	for(unsigned int i = 0; i < graphs_count ; i++){
		double o2o_time=0.0;
		double o2och_time=0.0;
		double o2o_one_time=0.0;
		double o2och_one_time=0.0;
		unsigned int NodeCount=0;
		unsigned int EdgeCount=0;
		unsigned int ShortcutCount=0;

		g = new Graph();
		g->setGraph(string(graphs[i])+string(fileending), true);

		sim s = sim(g, 0, 0);

			NodeCount = g->getNodeCount();
			EdgeCount = g->getEdgeCount();

		scg = new SCGraph(g);

		ch = new CH(g, scg, 1); // starte CH mit einem Thread

		ch->calcCH(false);
		cout << "Graph fertig mit " 
			<< scg->getShortcutCount() << " Shortcuts"<< endl;

			ShortcutCount = scg->getShortcutCount();

		chd = new CHDijkstra(scg);
		d = new Dijkstra(g);

		// random pairs erzeugen
		vector<unsigned int> pairs(runden * 2);
		std::srand((unsigned)std::time(0));
		unsigned int max_rand = RAND_MAX - (RAND_MAX % NodeCount);

		for(unsigned int j=0; j < pairs.size(); j++){
			unsigned int ran = rand();
			while( ran > max_rand )
				ran = rand();
			ran = ran % NodeCount;

			pairs[j] = ran;
		}

		// starte Dijksrtas
		
		cout << "Starte CHDijkstras " << runden << endl;

			start = clock();
		for(unsigned int k=0; k < pairs.size(); k+=2)
		{
			chd->oneToOne(pairs[k], pairs[k+1], 1);
		}
			finish = clock();
			o2och_time = (double(finish)-double(start))/CLOCKS_PER_SEC;
			o2och_one_time = o2och_time/double(runden);

		cout << "Starte Dijkstras " << runden << endl;

			start = clock();
		for(unsigned int k=0; k < pairs.size(); k+=2)
		{
			d->oneToOne(pairs[k], pairs[k+1], 1);
		}
			finish = clock();
			o2o_time = (double(finish)-double(start))/CLOCKS_PER_SEC;
			o2o_one_time = o2o_time/double(runden);

			//start = clock();
			//finish = clock();
			//= (double(finish)-double(start))/CLOCKS_PER_SEC;
			//\= double(runden);

		file << NodeCount << " "
			<< EdgeCount << " "
			<< ShortcutCount << " "
			<< o2o_time << " " 
			<< o2och_time << " " 
			<< o2o_one_time << " " 
			<< o2och_one_time << " " 
			<< " \n";

		delete d; d = 0;
		delete chd; chd = 0;
		delete ch; ch = 0;
		delete scg; scg = 0;
		delete g; g = 0;
	}

	file.close();
	cout << dataname << " Berechnung fertig. " << endl;
}

void O2M_Times_N_Targets(unsigned int i, unsigned int runden){
}

void O2O_evolution(unsigned int i, unsigned int runden, unsigned int up_count){
	string dataname = string(graphs[i]).replace(0,8,"")+"-"+"one_to_one_evolution.dat";
	cout << "Starte " << dataname << endl;
	ofstream file;
	file.open(string(folder)+"/"+dataname);
	file << "# " << dataname << " \n";
	file << "# " << graphs[i] << " \n";
	file << "# "  << runden 
		<< " maximale Start/Ziel Paare mit schrittweiser Erhöhung um " 
		<< up_count << " \n"
		<< "# durchschnittliche Zeit pro Dijkstra bei N hintereinander bearbeiteten" << " \n";


		double o2o_time=0.0;
		double o2och_time=0.0;
		unsigned int NodeCount=0;
		unsigned int EdgeCount=0;
		unsigned int ShortcutCount=0;

		unsigned int current_Count = 1;

		g = new Graph();
		g->setGraph(string(graphs[i])+string(fileending), true);

		sim s = sim(g, 0, 0);

			NodeCount = g->getNodeCount();
			EdgeCount = g->getEdgeCount();

		scg = new SCGraph(g);

		ch = new CH(g, scg, 1); // starte CH mit einem Thread

		ch->calcCH(false);
		cout << "Graph fertig mit " 
			<< scg->getShortcutCount() << " Shortcuts"<< endl;

			ShortcutCount = scg->getShortcutCount();

		chd = new CHDijkstra(scg);
		d = new Dijkstra(g);


	file << "# " 
		<< "Graph NodeCount"
		<<	", EdgeCount"
		<<	", ShortcutCount"
		<<	" \n";
	file << "# " 
		<< NodeCount <<" "
		<<	EdgeCount <<" "
		<<	ShortcutCount <<" "
		<<	" \n";


		// random pairs erzeugen
		vector<unsigned int> pairs(runden * 2);
		std::srand((unsigned)std::time(0));
		unsigned int max_rand = RAND_MAX - (RAND_MAX % NodeCount);

		for(unsigned int j=0; j < pairs.size(); j++){
			unsigned int ran = rand();
			while( ran > max_rand )
				ran = rand();
			ran = ran % NodeCount;

			pairs[j] = ran;
		}

		// starte Dijksrtas
		while( current_Count < runden)
		{

		cout << "Starte CHDijkstras " << runden << endl;

			start = clock();
		for(unsigned int k=0; k < current_Count; k+=2)
		{
			chd->oneToOne(pairs[k], pairs[k+1], 1);
		}
			finish = clock();
			o2och_time = (double(finish)-double(start))/CLOCKS_PER_SEC;

		cout << "Starte Dijkstras " << runden << endl;

			start = clock();
		for(unsigned int k=0; k < current_Count; k+=2)
		{
			d->oneToOne(pairs[k], pairs[k+1], 1);
		}
			finish = clock();
			o2o_time = (double(finish)-double(start))/CLOCKS_PER_SEC;

			//start = clock();
			//finish = clock();
			//= (double(finish)-double(start))/CLOCKS_PER_SEC;
			//\= double(runden);
			

		file << current_Count << " "
			<< o2o_time/double(current_Count) << " " 
			<< o2och_time/double(current_Count) << " " 
			<< " \n";

		current_Count += up_count;

		}

		delete d; d = 0;
		delete chd; chd = 0;
		delete ch; ch = 0;
		delete scg; scg = 0;
		delete g; g = 0;

	file.close();
	cout << dataname << " Berechnung fertig. " << endl;
}

int main(){
	cout << "            _|\\__/|, " << endl;
	cout << "          ,((\\````\\\\_" << endl;
	cout << "        ,(())     `))\\ "<< endl;
	cout << "      ,(()))       ,_ \\ "<< endl;
	cout << "     ((())'   |        \\ "<< endl;
	cout << "     )))))     >.__     \\ "<< endl;
	cout << "     ((('     /    `-. .c|" << endl;
	cout << "hjw          /        `-`'" << endl;
	cout << "( http://www.asciiworld.com/-Horses-.html )" << endl;
	cout << " " << endl;
	cout << " " << endl;
	
	// === === === 
	

CH_Times(1);

All2AllTimes(1);
	
	//One2_All_Times(0, 100, 50);
	//One2_All_Times(1, 500, 1); // 15 * 4/3 min
	//One2_All_Times(2, ,1000 1); // 25 * 4/3 min
	//One2_All_Times(3, 1000, 1); // > 75 * 4/3 min
	//One2_All_Times(4, 10000, 1); // > 75 * 4/3 min
//One2_All_Times(0, 100, 50);
//One2_All_Times(1, 700, 1); // 15 * 4/3 min
//One2_All_Times(2, 1500, 1); // 25 * 4/3 min
//One2_All_Times(3, 1500, 1); // > 75 * 4/3 min
//One2_All_Times(4, 25000, 1); // > 75 * 4/3 min

//O2O_Times(2000);

//O2O_evolution(0, 1000, 100);
//O2O_evolution(1, 1000, 100);
//O2O_evolution(2, 1000, 200);
//O2O_evolution(3, 1000, 200);
//O2O_evolution(4, 1000, 250);

	return 0;
}


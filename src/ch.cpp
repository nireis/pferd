#include "ch.h"

using namespace std;

CH::CH(Graph* gr, SCGraph* scgr, unsigned int ThreadsCount) : 
	isDone(false), g(gr), scg(scgr), algos(scgr, ThreadsCount), rounds(1),
	sclistpointer(scgr->getShortcutListPointer()), 
	bnlistpointer(scgr->getBlackNodesListPointer())
{}
CH::CH(Graph* gr, SCGraph* scgr) : 
	isDone(false), g(gr), scg(scgr), algos(scgr), rounds(1),
	sclistpointer(scgr->getShortcutListPointer()), 
	bnlistpointer(scgr->getBlackNodesListPointer())
{}

CH::~CH(){
	g = 0;
	scg = 0;
	sclistpointer = 0;
	bnlistpointer = 0;
}

void CH::calcCH(bool verbose){
	if( isDone )
		return;

	while(algos.calcOneRound(sclistpointer, bnlistpointer, verbose)){
		scg->mergeRound(rounds, verbose);
		rounds++;
	}
	scg->mergeShortcutsAndGraph(rounds, verbose);
	isDone = true;
}

void CH::calcCHverbose(){
	if( isDone )
		return;

	clock_t start,finish;
	double alltime = 0.0;
	double time;
	bool run = true;

	unsigned int j = 1;

	while(run){
		cout << "Berechne Shortcuts" << endl;
		start = clock();
		run = algos.calcOneRound(sclistpointer, bnlistpointer, true);
		finish = clock();
		time = (double(finish)-double(start));
		alltime += time;
		time = time /CLOCKS_PER_SEC;
		cout << "Zeit: " << time << endl;

		cout << "Merge Shortcuts in SCGraph. " << endl;
		start = clock();
		scg->mergeRound(j, true);
		finish = clock();
		time = (double(finish)-double(start));
		alltime += time;
		time = time /CLOCKS_PER_SEC;
		cout << "Zeit: " << time << endl;
		cout << " => Runde " << j << " fertig."  << endl;
		j++;
	}
	cout << "Insgesamt gebrauchte Zeit für Runden: " << (alltime/CLOCKS_PER_SEC) / 60.0 << " Minuten " << endl;
	cout << "Merge Shortcuts und original-Graph. " << endl;
	start = clock();
	scg->mergeShortcutsAndGraph(j, true);
	finish = clock();
	time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "Zeit: " << time << endl;

	cout << "Runden fertig." << endl;
	isDone = true;
}




#include "main.h"

void createColony(std::vector<Ant> &colony) {
	for (int i = 0; i < populationSize; i++) {		
		colony.push_back(Ant());
		colony[i].solutionRating = 0;
	}
}

void resetAnt(int i, std::vector<Ant> &colony) {
	colony[i].solutionRating = 0;
	colony[i].jobList.clear();
}

void resetColony(std::vector<Ant> &colony) {
	for (int i = 0; i < populationSize; i++) {
		resetAnt(i, colony);
	}
}
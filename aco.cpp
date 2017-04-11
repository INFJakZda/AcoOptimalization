#include "main.h"
#include <string>

bool compareMachin(const Machine* a, const Machine* b) { return (a[0].totalTime < b[0].totalTime); }

void generatingAntPath(int a, std::vector<Ant> &colony, Job* job, int moveit) {
	std::vector<float> propability;
	std::vector<int> helpArray;
	
	colony[a].jobList.push_back(std::rand()%jobsNumber);
	job[colony[a].jobList[0]].visited = true;
	bool endBigLoop = false;
	while (!endBigLoop) {
		if (moveit == 0) {
			int tem = std::rand() % jobsNumber;
			while (job[tem].visited == true) {
				tem = std::rand() % jobsNumber;
			}
			colony[a].jobList.push_back(tem);
			job[tem].visited = true;
		}
		else {
			float t_ij, fi_ij;
			for (int j = 0; j < jobsNumber; j++) {
				if (!job[j].visited) {
					t_ij = pow(jobsGraph[colony[a].jobList[colony[a].jobList.size() - 1]][j], alfa);
					fi_ij = pow((100/(job[j].op0.totalTime + job[j].op1.totalTime)), beta);
					float sum = 0;
					for (int k = 0; k < jobsNumber; k++) {
						if (!job[k].visited) {
							sum += (pow(jobsGraph[colony[a].jobList[colony[a].jobList.size() - 1]][k], alfa) *
								pow((100/(job[k].op0.totalTime + job[k].op1.totalTime)), beta));
						}
					}
					propability.push_back((t_ij * fi_ij) / sum);

				}
				else {
					propability.push_back(0);
				}
				
			}
			bool endLoop = false;
			while (!endLoop) {
				float temp;
				temp = (((float)((std::rand() % 3) + a) / (float)populationSize) / (float)(change_range_probability));
				for (int i = 0; i < propability.size(); i++) {
					if (propability[i] > temp) {
						helpArray.push_back(i);
					}
				}
				if (helpArray.size() == 0) {
					int temp_prop;
					if (propability.size() != 0) {
						temp_prop = std::rand() % propability.size();
					}
					else {
						temp_prop = std::rand() % jobsNumber;
					}
					bool endLoop2 = false;
					while (!endLoop2) {
						if (job[temp_prop].visited) {
							if (propability.size() != 0) {
								temp_prop = std::rand() % propability.size();
							}
							else {
								temp_prop = std::rand() % jobsNumber;
							}
						}
						else {
							endLoop = endLoop2 = true;
							colony[a].jobList.push_back(temp_prop);
						}
					}
				}
				else {
					int t_emp = helpArray[rand() % helpArray.size()];
					colony[a].jobList.push_back(t_emp);
					helpArray.clear();
					endLoop = true;
				}
			}
		} //END else
		job[colony[a].jobList[colony[a].jobList.size() - 1]].visited = true;
		propability.clear();
		if (colony[a].jobList.size() == jobsNumber) {
			endBigLoop = true;
		}

	}
}

void updatePenal(std::vector<Machine*> &allSolution_org, int &penal) {
	std::vector<Machine*> allSolution = allSolution_org;

	std::sort(allSolution.begin(), allSolution.end(), compareMachin);
	if (allSolution[0][0].totalTime == allSolution[1][0].totalTime) {
		if (allSolution[1][0].totalTime == allSolution[2][0].totalTime) {
			++penal;
		}
	}
}

void printJobsGraph(void) {
	std::cout << std::fixed;
	std::cout << std::setprecision(2);
	for (int i = 0; i < jobsNumber; i++) {
		for (int j = 0; j < jobsNumber; j++) {					
				std::cout << jobsGraph[i][j] << " ";			
		}
		std::cout << std::endl;
	}
}

void updatePhermonon(std::vector<Machine*> &allSolution, int &penal) {
	for (int i = 0; i < jobsNumber; i++) {
		for (int j = 0; j < jobsNumber; j++) {
			if (i != j) {
				jobsGraph[i][j] *= (1.0 - q_evaporation_rate);
			}
		}
	}	
	for(int i = 0; i < allSolution.size(); i++){
		for (int j = 1; j < allSolution[i][0].jobList.size(); j++) {
			jobsGraph[allSolution[i][0].jobList[j-1]][allSolution[i][0].jobList[j]] += (float)((float)q_value / (float)allSolution[i][0].totalTime);
		}
	}
	updatePenal(allSolution, penal);
}

void resetVistedJobs(Job *jobs) {
	for (int i = 0; i < jobsNumber; i++) {
		jobs[i].visited = false;
	}
}

void saveAllBestSolution(std::vector<Machine*> &allBestSolution) {
	std::ofstream myFile;
	char fileName[] = "bestTimes";

	myFile.open(fileName);
	for (int i = 0; i < allBestSolution.size(); i++) {
		myFile << allBestSolution[i][0].totalTime << std::endl;
	}
	myFile.close();
}

Machine* aco(Job *jobs, TechnicBreak *tb) {
	Machine *bestJobsLineUp = NULL;
	std::vector<Machine*> allSolution;
	std::vector<Machine*> allBestSolution;

	int bestSolution;
	std::vector<Ant> colony;
	int moveIt = 0;
	int penal = 0;

	bestSolution = -1;
	createColony(colony);
	resetVistedJobs(jobs);
	do {
		for (int i = 0; i<populationSize; i++) {
			generatingAntPath(i, colony, jobs, moveIt);
			allSolution.push_back(simulation(jobs, i, colony, tb));
			resetVistedJobs(jobs);
		}
		updatePhermonon(allSolution, penal);
		++moveIt;
		resetColony(colony);
		createColony(colony);
		if (moveIt == 1) {
			std::sort(allSolution.begin(), allSolution.end(), compareMachin);
			bestTimeInFirstColonyMove = allSolution[0][0].totalTime;
			bestJobsLineUp = allSolution[0];
			allBestSolution.push_back(bestJobsLineUp);
			allSolution.clear();
		}

		if((moveIt== goNestNumber) || (penal==3)){
			std::sort(allSolution.begin(), allSolution.end(), compareMachin);
			bestJobsLineUp = allSolution[0];
			allBestSolution.push_back(bestJobsLineUp);
			allSolution.clear();
		}
		else{
			std::sort(allSolution.begin(), allSolution.end(), compareMachin);
			bestJobsLineUp = allSolution[0];
			allBestSolution.push_back(bestJobsLineUp);
			allSolution.clear();
		}
	} while ((moveIt<goNestNumber) && (penal<3));
	//saveAllBestSolution(allBestSolution);
	std::sort(allBestSolution.begin(), allBestSolution.end(), compareMachin);
	bestJobsLineUp = allBestSolution[0];
	allBestSolution.clear();
	return bestJobsLineUp;
}

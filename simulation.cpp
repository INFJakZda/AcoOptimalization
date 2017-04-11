#include "main.h"

void resetJobsDone(Job *jobs) {
	for (int i = 0; i < jobsNumber; i++) {
		jobs[i].op0.doneJob = false;
		jobs[i].op1.doneJob = false;
	}
}

Machine* simulation(Job *jobs, int a, std::vector<Ant> &colony, TechnicBreak *tb) {
	Machine *machin = new Machine[2];	
	machin[0].jobList = machin[1].jobList = colony[a].jobList;
	machin[0].totalTechnicBreakTime = 0;
	resetJobsDone(jobs);

	unsigned long long time = 0;
	int it_0 = 0, it_1 = 0, it_tb = 0;		   //indeks wykonywanego zadania 
	int temp0 = 0, temp1 = 0;				   //ile czasu sie wykonuje operacja na maszynie
	int pom0 = 0, pom1 = 0, pomtb = 0;         //zapisanie wartosci czasu trwania operacji
	bool gate0 = 0, gate1 = 0;				   //bramy true-zamknieta
	bool oper0 = 0, oper1 = 0;				   //ktora operacja jest na maszynie wykonywana jak 0 to operacja pierwsza
	bool endLoop = false;
	while (!endLoop) {
		//WARUNEK KONCA PETLI
		for (int kl = 0; kl < jobsNumber; kl++) {
			if ((jobs[kl].op0.doneJob == false) || (jobs[kl].op1.doneJob == false)) {
				break;
			}
			else {
				if (kl == jobsNumber - 1) {
					endLoop = true;
				}
			}
		}
		//MASZYNA 0 START
		if (!gate0) {
			if (jobs[machin[0].jobList[it_0]].op0.machineId == 0) {
				if (!jobs[machin[0].jobList[it_0]].op0.doneJob) {
					if (jobs[machin[0].jobList[it_0]].delayTime <= time) {
						gate0 = true;
						jobs[machin[0].jobList[it_0]].op0.startTime = time;						
						pom0 = (jobs[machin[0].jobList[it_0]].op0.totalTime - 1);
						oper0 = 0;
					}
				}
			}
			else {
				if (!jobs[machin[0].jobList[it_0]].op1.doneJob) {
					if (jobs[machin[0].jobList[it_0]].op0.doneJob) {
						gate0 = true;
						jobs[machin[0].jobList[it_0]].op1.startTime = time;
						pom0 = (jobs[machin[0].jobList[it_0]].op1.totalTime - 1);
						oper0 = 1;
					}
				}
			}
		}
		else {
			if (pom0 > 1) {
				--pom0;
			}
			else {
				if (oper0 == 0) {
					jobs[machin[0].jobList[it_0]].op0.doneJob = true;
					jobs[machin[0].jobList[it_0]].op0.endTime = time;
					machin[0].operationList.push_back(jobs[machin[0].jobList[it_0]].op0);
				}
				else {
					jobs[machin[0].jobList[it_0]].op1.doneJob = true;
					jobs[machin[0].jobList[it_0]].op1.endTime = time;
					machin[0].operationList.push_back(jobs[machin[0].jobList[it_0]].op1);
				}
				if (it_0 < machin[0].jobList.size()-1) {
					++it_0;
				}
				pom0 = 0;
				gate0 = false;
			}
		}
		//MASZYNA 0 KONIEC

		//MASZYNA 1 START
		if (tb[it_tb].startTime == time) {
			pomtb = tb[it_tb].time;
			if (gate1) {
				if (oper1 == 0) {
					jobs[machin[1].jobList[it_1]].op0.startTime = time + tb[it_tb].time -1;
					pom1 = tb[it_tb].time + jobs[machin[1].jobList[it_1]].op0.totalTime;
				}
				else {
					jobs[machin[1].jobList[it_1]].op1.startTime = time + tb[it_tb].time -1;
					pom1 = tb[it_tb].time + jobs[machin[1].jobList[it_1]].op1.totalTime;
				}				
			}
			if (it_tb < K - 1) {
				++it_tb;				
			}

		}
		if (pomtb >= 0) {
			--pomtb;
			if (pom1 > 0) {
				--pom1;
			}
		}
		else {
			if (!gate1) {
				if (jobs[machin[1].jobList[it_1]].op0.machineId == 1) {
					if (!jobs[machin[1].jobList[it_1]].op0.doneJob) {
						if (jobs[machin[1].jobList[it_1]].delayTime <= time) {
							gate1 = true;
							jobs[machin[1].jobList[it_1]].op0.startTime = time;
							pom1 = (jobs[machin[1].jobList[it_1]].op0.totalTime - 1);
							oper1 = 0;
						}
					}
				}
				else {
					if (!jobs[machin[1].jobList[it_1]].op1.doneJob) {
						if (jobs[machin[1].jobList[it_1]].op0.doneJob) {
							gate1 = true;
							jobs[machin[1].jobList[it_1]].op1.startTime = time;
							pom1 = (jobs[machin[1].jobList[it_1]].op1.totalTime - 1);
							oper1 = 1;
						}
					}

				}
			}
			else {
				if (pom1 > 1) {
					--pom1;
				}
				else {
					if (oper1 == 0) {
						jobs[machin[1].jobList[it_1]].op0.doneJob = true;
						jobs[machin[1].jobList[it_1]].op0.endTime = time;
						machin[1].operationList.push_back(jobs[machin[1].jobList[it_1]].op0);
					}
					else {
						jobs[machin[1].jobList[it_1]].op1.doneJob = true;
						jobs[machin[1].jobList[it_1]].op1.endTime = time;
						machin[1].operationList.push_back(jobs[machin[1].jobList[it_1]].op1);
					}
					if (it_1 < machin[1].jobList.size() - 1) {
						++it_1;
					}
					pom1 = 0;
					gate1 = false;
				}
			}
		}
		//MASZYNA 1 KONIEC
		++time;    //AKTUALIZACJA CZASU
	} // endloop
	machin[0].totalTime = machin[1].totalTime = time;
	std::cout << a << " TIME: " << time << std::endl;
	return machin;
}
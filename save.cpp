#include "main.h"
#include <string>

void idleCounter(Machine *machin, TechnicBreak *tb) {
	int time = 0;					//zmienna iteracyjna czasowa
	//ZMIENNE DLA MASZYNY 0 ********
	Idle idle_0;         				//pomocnicza struktura typu Idle do wpisywania na wektor 'idlów'
	int it_idle_0 = 0, it_op_0 = 0;     //indeksy dla idle timów i operacji
	bool gate_idle_0 = 0;				  //bramy - gdy 0 to otwarte, gdy 1 to wykonywana jest operacja / idle
	int pom_op_0 = 0;					//iterator zamykajacy, dekremenujemy go w kazdej chwili czasu gdy > 0
	//********************************
	//ZMIENNE DLA MASZYNY 1 ++++++++++
	Idle idle_1;         						//pomocnicza struktura typu Idle do wpisywania na wektor 'idlów'
	int it_idle_1 = 0, it_op_1 = 0, it_tb = 0;     //indeksy dla idle timów i operacji
	bool  gate_idle_1 = 0;				 //bramy - gdy 0 to otwarte, gdy 1 to wykonywana jest operacja / idle
	int pom_op_1 = 0, pom_tb = 0;			//iterator zamykajacy, dekremenujemy go w kazdej chwili czasu gdy > 0
	//++++++++++++++++++++++++++++++++
	while(time < machin[0].totalTime) {
		//MASZYNA 0 START ******************************
		if (pom_op_0 == 0) {
			if (machin[0].operationList[it_op_0].startTime == time) {
				pom_op_0 = machin[0].operationList[it_op_0].totalTime - 1;
				if(it_op_0 < machin[0].operationList.size() - 1)
					++it_op_0;
				if (gate_idle_0 == 1) {
					idle_0.endTime = time;
					idle_0.totalTime = idle_0.endTime - idle_0.startTime;
					++it_idle_0;
					machin[0].idleList.push_back(idle_0);
					gate_idle_0 = 0;
				}
			}
			else if(gate_idle_0 == 0) {
				idle_0.startTime = time;
				idle_0.idIdle = it_idle_0;				
				gate_idle_0 = 1;
			}
		}
		else {
			--pom_op_0;
		}
		//MASZYNA 0 KONIEC ********************************

		//MASZYNA 1 START  ++++++++++++++++++++++++++++++
		if (pom_op_1 == 0) {
			if (pom_tb == 0) {
				if (machin[1].operationList[it_op_1].startTime == time) {
					pom_op_1 = machin[1].operationList[it_op_1].totalTime - 1;
					if (it_op_1 < machin[1].operationList.size() - 1)
						++it_op_1;
					if (gate_idle_1 == 1) {
						idle_1.endTime = time;
						idle_1.totalTime = idle_1.endTime - idle_1.startTime;
						++it_idle_1;
						machin[1].idleList.push_back(idle_1);
						gate_idle_1 = 0;
					}
				}
				else if (tb[it_tb].startTime == time) {
					pom_tb = tb[it_tb].time - 1;
					++it_tb;
					if (gate_idle_1 == 1) {
						idle_1.endTime = time;
						idle_1.totalTime = idle_1.endTime - idle_1.startTime;
						++it_idle_1;
						machin[1].idleList.push_back(idle_1);
						gate_idle_1 = 0;
					}

				}
				else if(gate_idle_1 == 0) {
					idle_1.startTime = time;
					idle_1.idIdle = it_idle_1;
					gate_idle_1 = 1;
				}
			}
			else {
				--pom_tb;
			}
		}
		else {
			--pom_op_1;
		}
		//MASZYNA 1 KONIEC  ++++++++++++++++++++++++++++++++

		++time;
	}

	//SUMY IDLE 
	machin[0].totalIdleTime = machin[1].totalIdleTime = machin[1].totalTechnicBreakTime = 0;
	for (int i = 0; i < machin[0].idleList.size(); i++) {
		machin[0].totalIdleTime += machin[0].idleList[i].totalTime;
	}
	for (int i = 0; i < machin[1].idleList.size(); i++) {
		machin[1].totalIdleTime += machin[1].idleList[i].totalTime;
	}
	//SUMY MAINTENCÓW
	for (int i = 0; i < K; i++) {
		machin[1].totalTechnicBreakTime += tb[i].time;
	}
}

void saveSolution(Job *jobs, Machine *machin, TechnicBreak *tb, int argc) {
	//std::cout << "SAVE START" << std::endl;
	
	std::ofstream myFileInstance, myFileOutput;
	int processingNumber = rand()%3000 + rand()%3000 + rand() % 3000 + rand() % 3000;

	struct tm now;
	time_t nowGet = time(0);
	localtime_s(&now, &nowGet);

	char instanceNameFile[20];
	char outputNameFile[20];

	std::string tmp1 = std::to_string(processingNumber);
	std::string tmp2 = "o"+ std::to_string(processingNumber);

	strncpy(instanceNameFile, tmp1.c_str(), sizeof(instanceNameFile));
	instanceNameFile[sizeof(instanceNameFile) - 1] = 0;
	strncpy(outputNameFile, tmp2.c_str(), sizeof(outputNameFile));
	outputNameFile[sizeof(outputNameFile) - 1] = 0;

	if (argc != 14) {
		myFileInstance.open(instanceNameFile);
		myFileInstance << jobsNumber << std::endl;
		for (int i = 0; i < jobsNumber; i++) {
			myFileInstance << jobs[i].op0.totalTime << ";" << jobs[i].op1.totalTime
				<< ";" << jobs[i].op0.machineId << ";" << jobs[i].op1.machineId << ";" << jobs[i].delayTime << std::endl;
		}
		for (int i = 0; i < K; i++) {
			myFileInstance << tb[i].idTB << ";" << "1" << ";" << tb[i].time << ";" << tb[i].startTime << std::endl;
		}
	}

	myFileOutput.open(outputNameFile);
	idleCounter(machin, tb);

	if (argc != 14) {
		myFileOutput << "**** " << processingNumber << " ****" << std::endl;
	}
	else {
		myFileOutput << "**** " << loadInstanceNumber << " ****" << std::endl;
	}
	myFileOutput << machin[0].totalTime << "," << bestTimeInFirstColonyMove << std::endl;
	myFileOutput << "M0: ";
	int it_op = 0, it_idle = 0, it_tb;
	for (int time = 0; time < machin[0].totalTime; time++) {
		if (machin[0].idleList[it_idle].startTime == time) {
			myFileOutput << "idle" << it_idle << "_M0, " << machin[0].idleList[it_idle].startTime <<
							", " << machin[0].idleList[it_idle].totalTime << ";";
			if (it_idle < machin[0].idleList.size() - 1) {
				++it_idle;
			}
		}
		else if (machin[0].operationList[it_op].startTime == time) {
			myFileOutput << "op" << machin[0].operationList[it_op].operationId << "_" << machin[0].operationList[it_op].jobId << ", "
				<< machin[0].operationList[it_op].startTime << ", " << machin[0].operationList[it_op].totalTime << ";";
			if (it_op < machin[0].operationList.size() - 1) {
				++it_op;
			}
		}
	}
	myFileOutput << std::endl;
	myFileOutput << "M1: ";
	it_op = 0, it_idle = 0, it_tb = 0;
	for (int time = 0; time < machin[1].totalTime; time++) {
		if (machin[0].idleList[it_idle].startTime == time) {
			myFileOutput << "idle" << it_idle << "_M1, " << machin[1].idleList[it_idle].startTime <<
				", " << machin[1].idleList[it_idle].totalTime << ";";
			if (it_idle < machin[1].idleList.size() - 1) {
				++it_idle;
			}
		}
		else if (machin[0].operationList[it_op].startTime == time) {
			myFileOutput << "op" << machin[1].operationList[it_op].operationId << "_" << machin[1].operationList[it_op].jobId << ", "
				<< machin[1].operationList[it_op].startTime << ", " << machin[1].operationList[it_op].totalTime << ";";
			if (it_op < machin[1].operationList.size() - 1) {
				++it_op;
			}
		}
		else if (tb[it_tb].startTime == time) {
			myFileOutput << "maint" << it_tb << "_M1, " << tb[it_tb].startTime << ", " << tb[it_tb].time << ";";
			if (it_tb < K-1) {
				++it_tb;
			}
		}
	}
	myFileOutput << std::endl;
	myFileOutput << "0" << std::endl;
	myFileOutput << machin[1].totalTechnicBreakTime << std::endl;
	myFileOutput << machin[0].totalIdleTime << std::endl;
	myFileOutput << machin[1].totalIdleTime << std::endl;

	myFileInstance.close();
	myFileOutput.close();
}
#include "main.h"

void debugTechnicBreak(TechnicBreak *tb) {
	for (int i = 0; i < K; i++) {
		std::cout << i << ": " << " time = " << tb[i].time << " start = " << tb[i].startTime << " end = " << tb[i].endTime << std::endl;
	}
}
void debugJobs(Job *tb) {
	for (int i = 0; i < jobsNumber; i++) {
		std::cout << i << ": " << "1 time = " << tb[i].op0.totalTime << " machineId = " << tb[i].op0.machineId << " operationId = " << tb[i].op0.operationId << " delay = " << tb[i].delayTime << std::endl;
		std::cout << i << ": " << "2 time = " << tb[i].op1.totalTime << " machineId = " << tb[i].op1.machineId << " operationId = " << tb[i].op1.operationId << std::endl;
		std::cout << "----------------------------------------------------------" << std::endl;
	}
}

//GENERATOR INSTANCJI
void generator(Job *jobs, TechnicBreak *tb){
	//Ustawianie chwil rozpoczecia i czasów trwania Przerw technicznych
	tb[0].idTB = 0;
	tb[0].time = (std::rand() % (breakLengthMax - breakLengthMin)) + breakLengthMin;
	tb[0].startTime = (std::rand() % 51) +200;
	tb[0].endTime = tb[0].startTime + tb[0].time;
	for (int i = 1; i < K; i++) {
		tb[i].idTB = tb[i-1].idTB + 1;
		tb[i].time = (std::rand() % (breakLengthMax - breakLengthMin)) + breakLengthMin;
		tb[i].startTime = (std::rand() % 51) + 200 + tb[i-1].endTime;
		tb[i].endTime = tb[i].startTime + tb[i].time;
	}
    //Ustawianie czasów trwania operacji
	for (int i = 0; i < jobsNumber; i++) {		
		bool zakres;
		zakres = rand() % 2;
		switch (zakres)
		{
		case 1:
			jobs[i].op0.totalTime = (std::rand() % 20) + 2;
			jobs[i].op1.totalTime = (std::rand() % 20) + 2;
			break;
		case 0:
			jobs[i].op0.totalTime = (std::rand() % 200) + 2;
			jobs[i].op1.totalTime = (std::rand() % 200) + 2;
			break;
		default:
			break;
		}		
		//Ustawianie Kolejnosci wykonania operacji (Job shop)
		jobs[i].op0.jobId = i;
		jobs[i].op1.jobId = i;
		jobs[i].op0.machineId = (std::rand() % 2);
		jobs[i].op0.operationId = 0;
		jobs[i].op1.operationId = 1;
		if (jobs[i].op0.machineId == 0) {
			jobs[i].op1.machineId = 1;
		}
		else {
			jobs[i].op1.machineId = 0;
		}
	}
	//ustawianie "ready time"
	int genDelay;
	int temp = 0;
	for (int i = 0; i < jobsNumber; i++) {
		temp += jobs[i].op0.totalTime;		
	}
	genDelay = int(temp / 8);
	//genDelay = 1;
	for (int i = 0; i < jobsNumber; i++) {		
			jobs[i].delayTime = (rand() % genDelay);		
	}

#define TEST
#ifdef TEST
	debugTechnicBreak(tb);
	debugJobs(jobs);
#endif // TEST
}
#ifndef _MAIN_H
#define _MAIN_H

#define M$_VS2015
#ifdef M$_VS2015
	#pragma comment(linker, "/STACK:500000000")
	#pragma comment(linker, "/HEAP:500000000")
#endif // M$_VS2015

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

//ZMIENNE GLOBALNE
extern int jobsNumber;					//Liczba zada� do przetworzenia
extern float **jobsGraph;				//Graf po kt�rym przechodzi ka�da mr�wka
extern int K;							//Okresy przestoju dla drugiej maszyny
extern int breakLengthMax;				//D�ugo�� przerwy technicznej (G�rny zakres)
extern int breakLengthMin;				//D�ugo�� przerwy technicznej (Dolny zakres)
//MR�WKA
extern int populationSize;				//Wielko�� populacji
extern float initialPheromoneLevel;		//Wielko�� startowa fermonu  
extern float alfa;						//Sta�a we wzorze do obliczenia pozostawionego feromonu
extern float q_evaporation_rate;		//Ilosc wyparowania feromonu
extern float beta;						//Sta�a we wzorze do obliczenia pozostawionego feromonu
extern int q_value;						//Sta�a we wzorze do obliczenia pozostawionego feromonu
extern int bestTimeInFirstColonyMove;   
extern int loadInstanceNumber;
extern float change_range_probability;	
extern int goNestNumber;				//Liczba przej�� mrowiska


//STRUKTURA OPERACJA
struct Operation {
	//DLA GENERATORA
	int totalTime;		//Czas wykonania operacji
	int machineId;		//Numer maszyny na kt�rym ma zosta� wykonany
	//DLA MASZYN
	int startTime;		//Czas rozpocz�cia
	int endTime;		//Czas zako�czenia
	int operationId;	//Identyfikator operacji (0 lub 1)
	int jobId;			//Identyfikator zadania
	bool doneJob;		//Wykonanie zadania.
};
//Przerwa techniczna
struct TechnicBreak {
	int idTB;			//Numeracja wyst�pie�
	int time;
	int startTime;
	int endTime;
};
//STRUKTURA IDLE
struct Idle {
	int idIdle;
	int totalTime;
	int startTime;
	int endTime;
};
//STRUKTURA ZADANIA
struct Job {
	int delayTime;		//Czas op�nienia zadania
	Operation op0;		//Operacja pierwsza zadania
	Operation op1;		//Operacja druga zadania
	//DLA MR�WKI
	bool visited;		//Czy zosta� odwiedzony
};
//STRUKTURA MASZYNA
struct Machine {
	int totalTime;
	//int totalTimeForOne;
	std::vector <Operation> operationList;
	std::vector<int> jobList;
	std::vector <Idle> idleList;
	int totalTechnicBreakTime;
	int totalIdleTime;
};
//STRUKTURA MR�WKA
struct Ant {
	int solutionRating;
	std::vector<int> jobList;
};

//FUNKCJE
void generator(Job *jobs, TechnicBreak *tb);
void createColony(std::vector<Ant> &colony);
void resetAnt(int i, std::vector<Ant> &colony);
void resetColony(std::vector<Ant> &colony);
Machine* aco(Job *jobs, TechnicBreak *tb);
Machine* simulation(Job *jobs, int i, std::vector<Ant> &colony, TechnicBreak *tb);
void saveSolution(Job *jobs, Machine *machin, TechnicBreak *tb, int argc);

#endif

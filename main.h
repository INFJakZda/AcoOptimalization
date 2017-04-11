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
extern int jobsNumber;					//Liczba zadañ do przetworzenia
extern float **jobsGraph;				//Graf po którym przechodzi ka¿da mrówka
extern int K;							//Okresy przestoju dla drugiej maszyny
extern int breakLengthMax;				//D³ugoœæ przerwy technicznej (Górny zakres)
extern int breakLengthMin;				//D³ugoœæ przerwy technicznej (Dolny zakres)
//MRÓWKA
extern int populationSize;				//Wielkoœæ populacji
extern float initialPheromoneLevel;		//Wielkoœæ startowa fermonu  
extern float alfa;						//Sta³a we wzorze do obliczenia pozostawionego feromonu
extern float q_evaporation_rate;		//Ilosc wyparowania feromonu
extern float beta;						//Sta³a we wzorze do obliczenia pozostawionego feromonu
extern int q_value;						//Sta³a we wzorze do obliczenia pozostawionego feromonu
extern int bestTimeInFirstColonyMove;   
extern int loadInstanceNumber;
extern float change_range_probability;	
extern int goNestNumber;				//Liczba przejœæ mrowiska


//STRUKTURA OPERACJA
struct Operation {
	//DLA GENERATORA
	int totalTime;		//Czas wykonania operacji
	int machineId;		//Numer maszyny na którym ma zostaæ wykonany
	//DLA MASZYN
	int startTime;		//Czas rozpoczêcia
	int endTime;		//Czas zakoñczenia
	int operationId;	//Identyfikator operacji (0 lub 1)
	int jobId;			//Identyfikator zadania
	bool doneJob;		//Wykonanie zadania.
};
//Przerwa techniczna
struct TechnicBreak {
	int idTB;			//Numeracja wyst¹pieñ
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
	int delayTime;		//Czas opóŸnienia zadania
	Operation op0;		//Operacja pierwsza zadania
	Operation op1;		//Operacja druga zadania
	//DLA MRÓWKI
	bool visited;		//Czy zosta³ odwiedzony
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
//STRUKTURA MRÓWKA
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

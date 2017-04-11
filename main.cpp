#include "main.h"

//ZMIENNE GLOBALNE
int jobsNumber;
float **jobsGraph;
int K;						//Okresy przestoju dla drugiej maszyny
int breakLengthMax;         //Długość przerwy technicznej (Górny zakres)
int breakLengthMin;         //Długość przerwy technicznej (Dolny zakres)
//MRÓWKA
int populationSize;
float initialPheromoneLevel;
float alfa;
float q_evaporation_rate;
float beta;
int q_value;
int bestTimeInFirstColonyMove;
int loadInstanceNumber;
float change_range_probability;
int goNestNumber;

int main(int argc, char *argv[]) {	//jobsNumber, populationSize, goNestNumber, stopValue,  breakLengthMin, breakLengthMax, change_range_probability,  initialPheromoneLevel, q_evaporation_rate, q_value, alfa, beta [, load_instance_file_name]
	srand((unsigned int)time(NULL));
	Job *jobs;
	TechnicBreak *tb;
	Machine *machine;
	int stopValue;

	if (argc == 13) {
		jobsNumber = atoi(argv[1]);
		populationSize = atoi(argv[2]);
		goNestNumber = atoi(argv[3]);
		stopValue = atoi(argv[4]);
		breakLengthMin = atoi(argv[5]);
		breakLengthMax = atoi(argv[6]);
		change_range_probability = (float)atof(argv[7]);
		initialPheromoneLevel = (float)atof(argv[8]);
		q_evaporation_rate = (float)atof(argv[9]);
		q_value = atoi(argv[10]);
		alfa = (float)atof(argv[11]);
		beta = (float)atof(argv[12]);
		
		K = (int)jobsNumber / stopValue;
		jobs = new Job[jobsNumber];
		tb = new TechnicBreak[K];
		
		generator(jobs, tb);
	}
	else if (argc == 14) {
		populationSize = atoi(argv[2]);
		goNestNumber = atoi(argv[3]);
		stopValue = atoi(argv[4]);
		breakLengthMin = atoi(argv[5]);
		breakLengthMax = atoi(argv[6]);
		change_range_probability = (float)atof(argv[7]);
		initialPheromoneLevel = (float)atof(argv[8]);
		q_evaporation_rate = (float)atof(argv[9]);
		q_value = atoi(argv[10]);
		alfa = (float)atof(argv[11]);
		beta = (float)atof(argv[12]);
		loadInstanceNumber = atoi(argv[13]);

		std::ifstream fileRead;
		int temp;
		fileRead.open(argv[13]);
		fileRead >> jobsNumber;

		jobs = new Job[jobsNumber];
		K = (int)jobsNumber / stopValue;
		tb = new TechnicBreak[K];

		for (int i = 0; i < jobsNumber; i++) {
			fileRead >> jobs[i].op0.totalTime;
			fileRead.get();
			fileRead >> jobs[i].op1.totalTime;
			fileRead.get();
			fileRead >> jobs[i].op0.machineId;
			fileRead.get();
			fileRead >> jobs[i].op1.machineId;
			fileRead.get();
			fileRead >> jobs[i].delayTime;
			jobs[i].visited = false;
		}

		for (int i = 0; i < K; i++) {
			fileRead >> tb[i].idTB;
			fileRead.get();
			fileRead >> temp;
			fileRead.get();
			fileRead >> tb[i].time;
			fileRead.get();
			fileRead >> tb[i].startTime;
		}

		fileRead.close();
	}
	else {
		std::cout << "Za malo argumentow, badz za duzo!!!!" << std::endl;
		std::cout << "[plik.exe] jobsNumber populationSize stopValue  breakLengthMin breakLengthMax change_range_probability initialPheromoneLevel q_evaporation_rate q_value alfa beta load_instance_file_name_opcjonalne" << std::endl;
		std::cout << "Jesli wczytanie z pliku to w parametrze - jobsNumber prosze podac jaka kolwiek wartpsc." << std::endl;
		std::cout << "argc = " << argc << std::endl;
		system("pause");
		return 0;
	}

	jobsGraph = new float*[jobsNumber];
	for (int i = 0; i < jobsNumber; i++) {
		jobsGraph[i] = new float[jobsNumber];
	}
	for (int i = 0; i < jobsNumber; i++) {
		jobsGraph[i][i] = -1.0;		//Przekatna
	}
	for (int i = 0; i < jobsNumber; i++) {
		for (int j = 0; j < jobsNumber; j++) {
			if (i != j) {
				jobsGraph[i][j] = initialPheromoneLevel;
			}
		}
	}

	machine = aco(jobs, tb);

	if (machine != NULL) {
		saveSolution(jobs, machine, tb, argc);
		delete[] machine;
	}

	for (int i = 0; i < jobsNumber; i++) {
		delete[] jobsGraph[i];
	}
	delete[] jobsGraph;
	delete[] jobs;
	delete[] tb;


	std::cout << std::endl;

	return 0;
}

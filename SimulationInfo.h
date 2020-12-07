#pragma once
class SimulationInfo
{
public:

	int CurrentNumberofSimulations;
	int GoalNumberofSimulations;
	int ThinksPerSimulation;
	int BestNumberofPorkchops;
	int BestAchievedAtIteration;

	int CurrentAverageIteration;
	int SimulationTotalPorkchops;
	int AverageAlgorithmTries;

	float MinimumThinkTime;

	SimulationInfo();
	~SimulationInfo();
};


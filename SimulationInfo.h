#pragma once
class SimulationInfo
{
public:

	//float WorstPossibleCost;
	//float CurrentBestCost;
	int CurrentNumberofSimulations;
	int GoalNumberofSimulations;
	int ThinksPerSimulation;
	int BestNumberofPorkchops;
	int BestAchievedAtIteration;

	int CurrentAverageIteration;
	int SimulationTotalPorkchops;
	int AverageAlgorithmTries;
	//float CurrentCost;

	float MinimumThinkTime;
	float TweakChance = 1.0f;

	SimulationInfo();
	~SimulationInfo();
};


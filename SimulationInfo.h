#pragma once
class SimulationInfo
{
public:

	float WorstPossibleCost;
	float CurrentBestCost;
	int CurrentNumberofSimulations;
	int GoalNumberofSimulations;
	int ThinksPerSimulation;
	int CurrentThink;
	float BestNumberofPorkchops;
	int BestAchievedAtIteration;

	int CurrentAverageIteration;
	int AverageModeTotalPorkchops;
	int AverageAlgorithmTries;
	int CurrentCost;

	float MinimumThinkTime;
	float TweakChance = 1.0f;

	SimulationInfo();
	~SimulationInfo();
};


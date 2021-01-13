#pragma once

string consoleadditives;

extern float thinktime;
extern int failsinarow;
extern int currenthink;
extern float TweakChance;

extern int GetProgressDebt();

void PushConsoleLine(string line)
{
	consoleadditives = consoleadditives + (line + "\n");
}

void ClearConsole()
{
	consoleadditives = "";
}

void PrintConsole() // Print a fresh console with all the bells and whistles
{
	system("CLS");

	cout << "Simulation " << OurSimulation.CurrentNumberofSimulations << "/" << OurSimulation.GoalNumberofSimulations << ". Think " << currenthink << "/" << OurSimulation.ThinksPerSimulation << " (" << thinktime << "s). Average Iteration " << (OurSimulation.CurrentAverageIteration+1) << "/" << OurSimulation.AverageAlgorithmTries << ". Max TweakChance = " << (1.0 * pow(0.96f, OurSimulation.BestNumberofPorkchops)) *100.0f << "%" << endl;
	cout << "Layer1FilterSum: " << GetFilterWeightsSum(1) << endl;
	cout << "Current simulation porkchops: " << OurSimulation.SimulationTotalPorkchops << +"(+" << HowMuchUncookedPork() << + ")" << ". Best number: " << OurSimulation.BestNumberofPorkchops << " (achieved at simulation " << OurSimulation.BestAchievedAtIteration << ")" << endl;
	cout << "Progress debt: " << GetProgressDebt() << endl;
	cout << consoleadditives;
}
#pragma once

string consoleadditives;

extern float thinktime;
extern int failsinarow;

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

	cout << "Simulation " << OurSimulation.CurrentNumberofSimulations << "/" << OurSimulation.GoalNumberofSimulations << ". Think " << OurSimulation.CurrentThink << "/" << OurSimulation.ThinksPerSimulation << " (" << thinktime << "s). Average Iteration " << OurSimulation.CurrentAverageIteration << "/" << OurSimulation.AverageAlgorithmTries << ". TweakChance = " << OurSimulation.TweakChance*100.0f << "%" << endl;
	cout << "Current average porkchops: " << ((OurSimulation.AverageModeTotalPorkchops+HowMuchUncookedPork())/(float)(OurSimulation.CurrentAverageIteration+1)) << ". Best average: " << OurSimulation.BestNumberofPorkchops << " (achived at simulation " << OurSimulation.BestAchievedAtIteration << ")" << endl;

	cout << consoleadditives;
}
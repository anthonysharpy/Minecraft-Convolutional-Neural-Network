#pragma once

string consoleadditives;

extern float thinktime;
extern int failsinarow;
extern int currenthink;

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

	cout << "Simulation " << OurSimulation.CurrentNumberofSimulations << "/" << OurSimulation.GoalNumberofSimulations << ". Think " << currenthink << "/" << OurSimulation.ThinksPerSimulation << " (" << thinktime << "s). Average Iteration " << OurSimulation.CurrentAverageIteration << "/" << OurSimulation.AverageAlgorithmTries << ". TweakChance = " << OurSimulation.TweakChance*100.0f << "%" << endl;
	cout << "Current simulation porkchops: " << OurSimulation.SimulationTotalPorkchops << +"(+" << HowMuchUncookedPork() << + ")" << ". Best number: " << OurSimulation.BestNumberofPorkchops << " (achieved at simulation " << OurSimulation.BestAchievedAtIteration << ")" << endl;

	cout << consoleadditives;
}
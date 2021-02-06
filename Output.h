#pragma once

#include "Enums.h"

string consoleadditives;

extern float thinktime;
extern int failsinarow;
extern int currentthink;
extern float TweakChance;
extern int CloseAttempts[5];
extern int simulationsdonethisrun;
extern double runtime;
extern float GetProgressDebt();
extern float getpixelstime;
extern float pixelconverttime;
extern float networkruntime;
extern float processmessagetime;
extern float drawimagestime;
extern float drawtexttime;
extern float iterationtime;
extern RunMode CurrentRunMode;
extern int currenttweakmode;
extern int currenttweakfilter;

void PushConsoleLine(string line)
{
	consoleadditives = consoleadditives + (line + "\n");
}

void ClearConsole()
{
	consoleadditives = "";
}

double GetSimulationsPerHour()
{
	return (simulationsdonethisrun / runtime) * 3600.0;
}

string GetTweakModeString()
{
	switch (currenttweakmode)
	{
	case 1:
		return "Tweaking filter " + to_string(currenttweakfilter + 1);
		break;
	case 2:
		return "Tweaking connected layer";
		break;
	case 3:
		return "Tweaking output layer";
		break;
	}

	return "ERROR";
}

void PrintConsole() // Print a fresh console with all the bells and whistles
{
	system("CLS");

	if (CurrentRunMode == RunMode::Learning)
	{
		cout << "Simulation " << OurSimulation.CurrentNumberofSimulations << "/" << OurSimulation.GoalNumberofSimulations << ". Think " << currentthink << "/" << OurSimulation.ThinksPerSimulation << ". Average Iteration " << (OurSimulation.CurrentAverageIteration + 1) << "/" << OurSimulation.AverageAlgorithmTries << ". Max TweakChance = " << TweakChance * 100.0f << "%" << endl;
		cout << GetTweakModeString() << endl;
		cout << "Current simulation porkchops: " << OurSimulation.SimulationTotalPorkchops << +"(+" << HowMuchUncookedPork() << +")" << ". Best number: " << OurSimulation.BestNumberofPorkchops << " (achieved at simulation " << OurSimulation.BestAchievedAtIteration << ")" << endl;
		cout << simulationsdonethisrun << " simulations done (" << GetSimulationsPerHour() << "/hr)" << endl;
		cout << "Progress debt: " << GetProgressDebt() << endl;
		cout << "---------------------" << endl;
		cout << OurSimulation.BestNumberofPorkchops - 1 << ": " << CloseAttempts[4] << "\n";
		cout << OurSimulation.BestNumberofPorkchops - 2 << ": " << CloseAttempts[3] << "\n";
		cout << OurSimulation.BestNumberofPorkchops - 3 << ": " << CloseAttempts[2] << "\n";
		cout << OurSimulation.BestNumberofPorkchops - 4 << ": " << CloseAttempts[1] << "\n";
		cout << OurSimulation.BestNumberofPorkchops - 5 << ": " << CloseAttempts[0] << "\n";
		cout << "---------------------" << endl;
		cout << "Get pixels time:		" << getpixelstime << "s" << endl;
		cout << "Pixel convert time:		" << pixelconverttime << "s" << endl;
		cout << "Network run time:		" << networkruntime << "s" << endl;
		cout << "Process message time:		" << processmessagetime << "s" << endl;
		cout << "	Draw images time:	" << drawimagestime << "s" << endl;
		cout << "	Draw text time:		" << drawtexttime << "s" << endl;
		cout << "TOTAL THINK TIME:		" << thinktime << "s" << endl;
		cout << "ITERATION TIME:			" << iterationtime << "s" << endl;
		cout << "---------------------" << endl;
		if (iterationtime > (OurSimulation.MinimumThinkTime * (currentthink + 1)) + 0.1f)
		{
			cout << "WARNING: ITERATION TIME OVER-RUNNING!" << endl;
			cout << "---------------------" << endl;
		}
		cout << consoleadditives;
	}
	else
	{
		cout << "PERFORMANCE MODE\n";
		cout << "Think " << currentthink << "/" << OurSimulation.ThinksPerSimulation << ". Average Iteration " << (OurSimulation.CurrentAverageIteration + 1) << "/" << OurSimulation.AverageAlgorithmTries << endl;
		cout << "Current simulation porkchops: " << OurSimulation.SimulationTotalPorkchops << +"(+" << HowMuchUncookedPork() << +")" << ". Best number: " << OurSimulation.BestNumberofPorkchops << " (achieved at simulation " << OurSimulation.BestAchievedAtIteration << ")" << endl;
		cout << "Progress debt: " << GetProgressDebt() << endl;
		cout << "---------------------" << endl;
		cout << "Get pixels time:		" << getpixelstime << "s" << endl;
		cout << "Pixel convert time:		" << pixelconverttime << "s" << endl;
		cout << "Network run time:		" << networkruntime << "s" << endl;
		cout << "Process message time:		" << processmessagetime << "s" << endl;
		cout << "	Draw images time:	" << drawimagestime << "s" << endl;
		cout << "	Draw text time:		" << drawtexttime << "s" << endl;
		cout << "TOTAL THINK TIME:		" << thinktime << "s" << endl;
		cout << "ITERATION TIME:			" << iterationtime << "s" << endl;
		cout << "---------------------" << endl;
		if (iterationtime > (OurSimulation.MinimumThinkTime * (currentthink + 1)) + 0.1f)
		{
			cout << "WARNING: ITERATION TIME OVER-RUNNING!" << endl;
			cout << "---------------------" << endl;
		}
		cout << consoleadditives;
	}
}
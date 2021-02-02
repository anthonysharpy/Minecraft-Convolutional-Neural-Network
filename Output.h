#pragma once

#include "Enums.h"

string consoleadditives;

extern float thinktime;
extern int failsinarow;
extern int currentthink;
extern float TweakChance;
extern int CloseAttempts[5];
extern int simulationsdonethisrun;
extern float runtime;
extern float GetProgressDebt();
extern float getpixelstime;
extern float pixelconverttime;
extern float networkruntime;
extern float processmessagetime;
extern float drawimagestime;
extern float drawtexttime;
extern float iterationtime;
extern RunMode CurrentRunMode;

void PushConsoleLine(string line)
{
	consoleadditives = consoleadditives + (line + "\n");
}

void ClearConsole()
{
	consoleadditives = "";
}

float GetSimulationsPerHour()
{
	return (simulationsdonethisrun / runtime) * 60.0f * 60.0f;
}

void PrintConsole() // Print a fresh console with all the bells and whistles
{
	system("CLS");

	if (CurrentRunMode == RunMode::Learning)
	{
		cout << "Simulation " << OurSimulation.CurrentNumberofSimulations << "/" << OurSimulation.GoalNumberofSimulations << ". Think " << currentthink << "/" << OurSimulation.ThinksPerSimulation << ". Average Iteration " << (OurSimulation.CurrentAverageIteration + 1) << "/" << OurSimulation.AverageAlgorithmTries << ". Max TweakChance = " << (1.0 * pow(0.86f, OurSimulation.BestNumberofPorkchops)) * 100.0f << "%" << endl;
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
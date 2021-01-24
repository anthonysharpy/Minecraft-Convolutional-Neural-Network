#pragma once

string consoleadditives;

extern float thinktime;
extern int failsinarow;
extern int currentthink;
extern float TweakChance;
extern int CloseAttempts[5];
extern int GetProgressDebt();
extern float getpixelstime;
extern float pixelconverttime;
extern float networkruntime;
extern float processmessagetime;
extern float drawimagestime;
extern float drawtexttime;
extern float iterationtime;

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

	cout << "Simulation " << OurSimulation.CurrentNumberofSimulations << "/" << OurSimulation.GoalNumberofSimulations << ". Think " << currentthink << "/" << OurSimulation.ThinksPerSimulation << ". Average Iteration " << (OurSimulation.CurrentAverageIteration+1) << "/" << OurSimulation.AverageAlgorithmTries << ". Max TweakChance = " << (1.0 * pow(0.86f, OurSimulation.BestNumberofPorkchops)) *100.0f << "%" << endl;
	cout << "Layer1FilterSum: " << GetFilterWeightsSum(1) << endl;
	cout << "Current simulation porkchops: " << OurSimulation.SimulationTotalPorkchops << +"(+" << HowMuchUncookedPork() << + ")" << ". Best number: " << OurSimulation.BestNumberofPorkchops << " (achieved at simulation " << OurSimulation.BestAchievedAtIteration << ")" << endl;
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
	if (iterationtime > (OurSimulation.MinimumThinkTime * (currentthink+1))+0.1f)
	{
		cout << "WARNING: ITERATION TIME OVER-RUNNING!" << endl;
		cout << "---------------------" << endl;
	}
	cout << consoleadditives;
}
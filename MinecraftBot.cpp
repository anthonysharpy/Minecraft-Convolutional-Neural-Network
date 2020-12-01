// VERSION 1: repeats simulation. if get more than before, learns.
// problem: hits roadblock very early

// NEW VERSION: 
// average learning

// MinecraftBot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// res is 300 x 260 but we combine each 4 pixels into a single square soo

#pragma once

#include "pch.h"
#include "Network.h"
#include "PixelRecognition.h"
#include "Input.h"
#include "File IO.h"
#include "Macros.h"
#include "EasyBMP.h"
#include "Screenshots.h"
#include "SimulationInfo.h"
#include "Output.h"
#include "HelperWindow.h"

using namespace std;

SimulationInfo OurSimulation;

const int BotViewScreenWidth = 960;//GetSystemMetrics(SM_CXSCREEN);
const int BotViewScreenHeight = 600;//GetSystemMetrics(SM_CYSCREEN);

RGBQUAD *Pixels = new RGBQUAD[BotViewScreenWidth * BotViewScreenHeight];

HWND hWnd;

void GotoMinecraftWindow()
{
	SetForegroundWindow(hWnd);
	SetActiveWindow(hWnd);
	SetWindowPos(hWnd, HWND_TOP, 950, 50, (BotViewScreenWidth) + 16, (BotViewScreenHeight) + 39, NULL);
}

long GetTime()
{
	const auto p1 = std::chrono::system_clock::now();
	return (long)std::chrono::duration_cast<std::chrono::microseconds>(p1.time_since_epoch()).count();
}

void GetPixels()
{
	hWnd = FindWindowA(NULL, "Minecraft 1.16.4 - Singleplayer");

	HDC hdcScreen;
	HDC hdcWindow;
	HDC hdcMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	RECT rcClient;
	BITMAPINFOHEADER   bi;

	if (!hWnd)
	{
		MessageBox(hWnd, L"Window doesn't exist", L"Failed", MB_OK);
		system("Pause");
		return;
	}

	hdcScreen = GetDC(NULL);
	hdcWindow = GetDC(hWnd);

	if (hdcWindow == NULL)
	{
		cout << "GetDC(hWnd) failed." << endl;
		system("Pause");
	}

	hdcMemDC = CreateCompatibleDC(hdcWindow);

	if (!hdcMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
	}

	if (GetClientRect(hWnd, &rcClient) == 0)
	{
		cout << "GetClientRect() failed ... error was " + to_string(GetLastError()) << endl;
	}

	//This is the best stretch mode
	SetStretchBltMode(hdcWindow, HALFTONE);

	//The source DC is the entire screen and the destination DC is the current window (HWND)
	if (!StretchBlt(hdcWindow,
		0, 0,
		rcClient.right, rcClient.bottom,
		hdcScreen,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		SRCCOPY))
	{
		MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
	}

	// Create a compatible bitmap from the Window DC
	hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	if (hbmScreen == NULL)
	{
		cout << "CreateCompatibleBitmap() returned null ... rcClient.right was " + to_string(rcClient.right) << endl;
		system("Pause");
	}

	if (!hbmScreen)
	{
		MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
	}

	// Select the compatible bitmap into the compatible memory DC.
	SelectObject(hdcMemDC, hbmScreen);

	// Bit block transfer into our compatible memory DC.
	if (!BitBlt(hdcMemDC,
		0, 0,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		hdcWindow,
		0, 0,
		SRCCOPY))
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
	}

	// Get the BITMAP from the HBITMAP
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	//BITMAPFILEHEADER   bmfHeader;

	if (bmpScreen.bmHeight != BotViewScreenHeight)
	{
		cout << "ERROR: bitmap height (" + to_string(bmpScreen.bmHeight) + ") does not match BotViewScreenHeight (" + to_string(BotViewScreenHeight) + ")" << endl;
		system("Pause");
		GotoMinecraftWindow();
		return;
	}
	if (bmpScreen.bmWidth != BotViewScreenWidth)
	{
		cout << "ERROR: bitmap width (" + to_string(bmpScreen.bmWidth) + ") does not match BotViewScreenWidth (" + to_string(BotViewScreenWidth) + ")" << endl;
		system("Pause");
		GotoMinecraftWindow();
		return;
	}

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	//HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	//Pixels = (GlobalLock(hDIB);

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		Pixels,
		(BITMAPINFO *)&bi, DIB_RGB_COLORS);

	//Clean up

	SelectObject(hdcMemDC, hbmScreen);

	DeleteObject(hbmScreen);
	DeleteDC(hdcMemDC);

	ReleaseDC(hWnd, hdcWindow);
	ReleaseDC(NULL, hdcScreen);
}

int numtimesattacked = 0;
bool havemoved = false;

float ActivationNumber = LAYER4SIZE * 0.25;

void PerformOutputs()
{
	if (OutputNeurons[0] >= ActivationNumber)
	{
		havemoved = true;
		KeyDown("w");
	}
	else KeyUp("w");

	if (OutputNeurons[1] >= ActivationNumber)
	{
		havemoved = true;
		KeyDown("s");
	}
	else KeyUp("s");

	if (OutputNeurons[2] >= ActivationNumber)
	{
		havemoved = true;
		KeyDown("a");
	}
	else KeyUp("a");

	if (OutputNeurons[3] >= ActivationNumber)
	{
		havemoved = true;
		KeyDown("d");
	}
	else KeyUp("d");

	if (OutputNeurons[4] >= ActivationNumber)
	{
		havemoved = true;
		KeyDown("space");
	}
	else KeyUp("space");

	if (OutputNeurons[5] >= ActivationNumber)
	{
		numtimesattacked++;
		LeftClickDown();
	}
	else LeftClickUp();

	if (OutputNeurons[6] >= ActivationNumber)
	{
		havemoved = true;
		MoveMouseLeft();
	}

	if (OutputNeurons[7] >= ActivationNumber)
	{
		havemoved = true;
		MoveMouseRight();
	}

	if (OutputNeurons[8] >= ActivationNumber)
	{
		havemoved = true;
		MoveMouseDown();
	}

	if (OutputNeurons[9] >= ActivationNumber)
	{
		havemoved = true;
		MoveMouseUp();
	}
}

void InitialiseDefaultSimulationInfo()
{
	OurSimulation.MinimumThinkTime = 0.1f;
	OurSimulation.ThinksPerSimulation = 200;
	//OurSimulation.WorstPossibleCost = (float)pow(100, 2);
	OurSimulation.CurrentAverageIteration = 0;
	//OurSimulation.CurrentBestCost = OurSimulation.WorstPossibleCost;
	OurSimulation.BestNumberofPorkchops = 0;
	OurSimulation.GoalNumberofSimulations = 90000000;
	OurSimulation.AverageAlgorithmTries = 4;
	OurSimulation.SimulationTotalPorkchops = 0;
	OurSimulation.TweakChance = 1.0f;
}

float thinktime = 0;

int failsinarow = 0;

void ProcessMessages()
{
	MSG msg = { };

	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) > 0) // if there's a message
	{
		int res = GetMessage(&msg, NULL, 0, 0); // get it

		if (res > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (res == 0)
		{
			MessageBox(NULL, L"WM_QUIT CALLED", L"ERROR", MB_OK);
		}
		if (res == -1)
		{
			MessageBox(NULL, L"ERROR. HELPER WINDOW ERROR", L"ERROR", MB_OK);
		}
	}
}

void TakeScreenshots()
{
	PushConsoleLine("Taking screenshots.");
	PrintConsole();

	// input

	ScreenshotInput("sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_inputpixels");

	// layer 1

	for (int fil = 0; fil < LAYER1NUMFILTERS; fil++) // screenshot all activation maps on layer 1
	{
		Screenshot1DArray(Layer1ActivationMaps[fil], LAYER1WIDTH, LAYER1HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer1_activationmap_filter" + to_string(fil), 1.0f);
	}

	Screenshot1DArray(Layer1PrePoolOutput, LAYER1WIDTH, LAYER1HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer1_aPREPOOLEDOUTPUT", 1.0f);
	Screenshot1DArray(Layer1PooledOutput, LAYER1POOLEDWIDTH, LAYER1POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer1_POOLEDOUTPUT", 1.0f);

	// layer 2

	for (int fil = 0; fil < LAYER2NUMFILTERS; fil++) // screenshot all activation maps on layer 1
	{
		Screenshot1DArray(Layer2ActivationMaps[fil], LAYER2WIDTH, LAYER2HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer2_activationmap_filter" + to_string(fil), 1.0f);
	}

	Screenshot1DArray(Layer2PrePoolOutput, LAYER2WIDTH, LAYER2HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer2_aPREPOOLEDOUTPUT", 1.0f);
	Screenshot1DArray(Layer2PooledOutput, LAYER2POOLEDWIDTH, LAYER2POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer2_POOLEDOUTPUT", 1.0f);

	// layer 3

	for (int fil = 0; fil < LAYER3NUMFILTERS; fil++) // screenshot all activation maps on layer 3
	{
		Screenshot1DArray(Layer3ActivationMaps[fil], LAYER3WIDTH, LAYER3HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer3_activationmap_filter" + to_string(fil), 1.0f);
	}

	for (int fil = 0; fil < LAYER3NUMFILTERS; fil++) // screenshot all pooled activation maps on layer 3
	{
		Screenshot1DArray(Layer3PooledActivationMaps[fil], LAYER3POOLEDWIDTH, LAYER3POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer3_POOLEDactivationmap_filter" + to_string(fil), 1.0f);
	}
}

int currenthink = 0;

int main()
{
	hWnd = FindWindowA(NULL, "Minecraft 1.16.4 - Singleplayer");

	CreateHelperWindow();

	GotoMinecraftWindow();

	InitialiseInput();

	cout << "Starting in 3 seconds" << endl << endl;
	Sleep(3000);

	LoadArraysFromFile();
	//OurSimulation.CurrentBestCost = pow(100 - OurSimulation.BestNumberofPorkchops, 2);
	OurSimulation.TweakChance = 1.0f * pow(0.87f, OurSimulation.BestNumberofPorkchops);
	SaveNetwork();

	GetPixels();
	ScreenshotInput("START");

	bool screenshotted = false;

	for (; OurSimulation.CurrentNumberofSimulations < OurSimulation.GoalNumberofSimulations; OurSimulation.CurrentNumberofSimulations++)
	{
		timestweaked = 0;
		timesnottweaked = 0;

		TweakStuff(OurSimulation.TweakChance, 1.1f);

		//OurSimulation.CurrentCost = 0;
		OurSimulation.SimulationTotalPorkchops = 0;

		for (OurSimulation.CurrentAverageIteration = 0; OurSimulation.CurrentAverageIteration < OurSimulation.AverageAlgorithmTries; OurSimulation.CurrentAverageIteration++)
		{
			numtimesattacked = 0;
			havemoved = false;

			ClearInventory();
			Suicide();
			Sleep(2000);

			screenshotted = false;

			ClearConsole();

			int porknow = 0;

			for (currenthink = 0; currenthink < OurSimulation.ThinksPerSimulation; currenthink++)
			{
				long a = GetTime();

				ProcessMessages();

				if (HowMuchUncookedPork() > porknow) porknow = HowMuchUncookedPork(); // because it seems sometimes the function returns zero when it shouldnt; this protects the variable

				if (currenthink >= 150 && porknow == 0 && OurSimulation.CurrentAverageIteration == 0)
				{
					PushConsoleLine("Not getting anything. Suiciding.");
					PrintConsole();
					goto end_simulation;
				}
				if (currenthink >= 50 && numtimesattacked == 0 && OurSimulation.CurrentAverageIteration == 0)
				{
					PushConsoleLine("Failed to attack. Suiciding.");
					PrintConsole();
					goto end_simulation;
				}
				if (currenthink >= 40 && havemoved == false && OurSimulation.CurrentAverageIteration == 0)
				{
					PushConsoleLine("Failed to move. Suiciding.");
					PrintConsole();
					goto end_simulation;
				}

				if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					AllKeysUp();
					std::cout << "Paused. Press UP to unpause." << endl;

					while (!(GetAsyncKeyState(VK_UP) & 0x8000))
					{
					}
				}

				GetPixels();
				CalculateLayer1();
				CalculateLayer2();
				CalculateLayer3();
				CalculateLayer4();
				CalculateOutputLayer();

				if (!screenshotted)
				{
					TakeScreenshots();
					screenshotted = true;
				}

				PerformOutputs();

				long b = GetTime();

				thinktime = (b - a) / 1000000.0f;

				if (thinktime < OurSimulation.MinimumThinkTime)
				{
					Sleep((int)((OurSimulation.MinimumThinkTime - thinktime) * 1000));
				}

				if ((currenthink % 5) == 0) PrintConsole();
			}

			PushConsoleLine("Iteration finished.");
			PrintConsole();

			AllKeysUp();

			//float cost = (float)pow(100 - porknow, 2);

			//OurSimulation.CurrentCost += cost;
			OurSimulation.SimulationTotalPorkchops += porknow;
		}

	end_simulation:

		PushConsoleLine("Simulation finished.");

		AllKeysUp();

		//float averagecost = OurSimulation.CurrentCost / (float)OurSimulation.AverageAlgorithmTries;

		//if (averagecost <= OurSimulation.CurrentBestCost)
		if(OurSimulation.SimulationTotalPorkchops >= OurSimulation.BestNumberofPorkchops)
		{
			PushConsoleLine("Better or same cost; keeping");

			//OurSimulation.CurrentBestCost = averagecost;
			OurSimulation.BestNumberofPorkchops = OurSimulation.SimulationTotalPorkchops;
			OurSimulation.BestAchievedAtIteration = OurSimulation.CurrentNumberofSimulations;
			OurSimulation.TweakChance = 1.0f * pow(0.87f, OurSimulation.SimulationTotalPorkchops);

			SaveNetwork();
		}
		else
		{
			PushConsoleLine("Worse cost; reverting network");
			RevertTweaks();
		}

		PrintConsole();

		SaveArraysToFile();
	}

	cout << "Program finished."; // Best cost was " + to_string(OurSimulation.CurrentBestCost) << endl;

	system("Pause");
}


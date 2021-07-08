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
#include "Enums.h"

using namespace std;

SimulationInfo OurSimulation;

const int BotViewScreenWidth = 960;//GetSystemMetrics(SM_CXSCREEN);
const int BotViewScreenHeight = 600;//GetSystemMetrics(SM_CYSCREEN);

RGBQUAD* Pixels = new RGBQUAD[BotViewScreenWidth * BotViewScreenHeight];
RGBQUADFLOAT* PixelsFloat = new RGBQUADFLOAT[BotViewScreenWidth * BotViewScreenHeight];

HWND hWnd;

int CloseAttempts[5];

HWND GUIWindowHwnd;

float getpixelstime;
float pixelconverttime;
float networkruntime;
float processmessagetime;
float drawimagestime;
float drawtexttime;
float iterationtime;

bool imagedrawdirty;
bool textdrawdirty;
bool otherdrawdirty;

double runtime;
int simulationsdonethisrun;

void GotoMinecraftWindow()
{
	SetForegroundWindow(hWnd);
	SetActiveWindow(hWnd);
	SetWindowPos(hWnd, HWND_TOP, 950, 0, (BotViewScreenWidth) + 16, (BotViewScreenHeight) + 39, NULL);
}

long GetTime()
{
	const auto p1 = std::chrono::system_clock::now();
	return (long)std::chrono::duration_cast<std::chrono::microseconds>(p1.time_since_epoch()).count();
}

// Apparently this really should be called from WM_PAINT in a Windows procedure. lol.
void GetPixels()
{
	long prepixels = GetTime();
	
	hWnd = FindWindowA(NULL, "Minecraft 1.16.5 - Singleplayer");

	HDC hdcScreen;
	HDC hdcWindow;
	HDC hdcMemDC;
	_Post_ _Notnull_ HBITMAP hbmScreen;
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
		return;
	}
	if (hdcScreen == NULL)
	{
		cout << "GetDC(NULL) failed." << endl;
		system("Pause");
		return;
	}

	hdcMemDC = CreateCompatibleDC(hdcWindow);

	if (!hdcMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		system("Pause");
		return;
	}

	if (GetClientRect(hWnd, &rcClient) == 0)
	{
		cout << "GetClientRect() failed ... error was " + to_string(GetLastError()) << endl;
		system("Pause");
	}

	//This is the best stretch mode
	if (!SetStretchBltMode(hdcWindow, HALFTONE))
	{
		cout << "Unable to SetStretchBltMode()\n";
		system("Pause");
	}

	if (rcClient.right != BotViewScreenWidth)
	{
		cout << "rcClient right not equal to BotViewScreenWidth\n";
		system("Pause");
		return;
	}
	if (rcClient.bottom != BotViewScreenHeight)
	{
		cout << "rcClient bottom not equal to BotViewScreenHeight\n";
		system("Pause");
		return;
	}

	//The source DC is the entire screen and the destination DC is the current window (HWND)
	if (!StretchBlt(hdcWindow,
		0, 0,
		rcClient.right, rcClient.bottom,
		hdcScreen,
		0, 0,
		1920,
		1080,
		SRCCOPY))
	{
		MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
		system("Pause");
	}

	// Create a compatible bitmap from the Window DC
	hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	if (hbmScreen == NULL)
	{
		cout << "CreateCompatibleBitmap() returned null ... rcClient.right was " + to_string(rcClient.right) << endl;
		system("Pause");
		return;
	}
	else
	{
		// Select the compatible bitmap into the compatible memory DC.
		SelectObject(hdcMemDC, hbmScreen);
	}

	// Bit block transfer into our compatible memory DC.
	if (!BitBlt(hdcMemDC,
		0, 0,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		hdcWindow,
		0, 0,
		SRCCOPY))
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		system("Pause");
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

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	if (!GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		Pixels,
		(BITMAPINFO*)&bi, DIB_RGB_COLORS))
	{
		cout << "GetDIBits error.\n";
		system("Pause");
	}

	//Clean up

	DeleteObject(hbmScreen);
	DeleteDC(hdcMemDC);
	if (!ReleaseDC(NULL, hdcScreen))
	{
		cout << "Couldn't ReleaseDC() hdcScreen\n";
		system("Pause");
	}
	if (!ReleaseDC(hWnd, hdcWindow))
	{
		cout << "Couldn't ReleaseDC() hdcWindow\n";
		system("Pause");
	}

	long postpixels = GetTime();
	getpixelstime = (postpixels - prepixels) / 1000000.0f;
}

int numtimesattacked = 0;
bool havemoved = false;
bool havemovedmouse = false;

float ActivationNumber = LAYER10SIZE * 0.25;

void PerformOutputs()
{
	if (OutputNeurons[0] >= ActivationNumber)
	{
		if(OutputNeurons[1] < ActivationNumber) havemoved = true; // s key not pressed
		KeyDown("w");
	}
	else KeyUp("w");

	if (OutputNeurons[1] >= ActivationNumber)
	{
		if (OutputNeurons[0] < ActivationNumber) havemoved = true; // w key not pressed
		KeyDown("s");
	}
	else KeyUp("s");

	if (OutputNeurons[2] >= ActivationNumber)
	{
		if (OutputNeurons[3] < ActivationNumber) havemoved = true; // d key not pressed
		KeyDown("a");
	}
	else KeyUp("a");

	if (OutputNeurons[3] >= ActivationNumber)
	{
		if (OutputNeurons[2] < ActivationNumber) havemoved = true; // a key not pressed
		KeyDown("d");
	}
	else KeyUp("d");

	if (OutputNeurons[4] >= ActivationNumber)
	{
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
		havemovedmouse = true;
		MoveMouseLeft();
	}

	if (OutputNeurons[7] >= ActivationNumber)
	{
		havemovedmouse = true;
		MoveMouseRight();
	}

	if (OutputNeurons[8] >= ActivationNumber)
	{
		havemovedmouse = true;
		MoveMouseDown();
	}

	if (OutputNeurons[9] >= ActivationNumber)
	{
		havemovedmouse = true;
		MoveMouseUp();
	}
}

void InitialiseDefaultSimulationInfo()
{
	OurSimulation.MinimumThinkTime = 0.1f;
	OurSimulation.ThinksPerSimulation = 250;
	OurSimulation.CurrentAverageIteration = 0;
	OurSimulation.BestNumberofPorkchops = 0;
	OurSimulation.GoalNumberofSimulations = 90000000;
	OurSimulation.AverageAlgorithmTries = 4;
	OurSimulation.SimulationTotalPorkchops = 0;
}

float thinktime = 0;

int failsinarow = 0;

void ProcessMessages()
{
	long preprocessmessages = GetTime();

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

	long postprocessmessages = GetTime();
	processmessagetime = (postprocessmessages - preprocessmessages) / 1000000.0f;
}

void TakeScreenshots()
{
	PushConsoleLine("Taking screenshots.");
	PrintConsole();

	// input
	Screenshot1DArray(PixelsFloat, BotViewScreenWidth, BotViewScreenHeight, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_inputpixels");

	// layer 1
	Screenshot1DArray(Layer1ActivationMaps, LAYER1WIDTH, LAYER1HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer1_activationmap");
	Screenshot1DArray(Layer1PooledOutput, LAYER1POOLEDWIDTH, LAYER1POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer1_POOLEDOUTPUT");

	// layer 2
	Screenshot1DArray(Layer2ActivationMaps, LAYER2WIDTH, LAYER2HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer2_activationmap");
	Screenshot1DArray(Layer2PooledOutput, LAYER2POOLEDWIDTH, LAYER2POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer2_POOLEDOUTPUT");

	// layer 3
	Screenshot1DArray(Layer3ActivationMaps, LAYER3WIDTH, LAYER3HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer3_activationmap");
	Screenshot1DArray(Layer3PooledOutput, LAYER3POOLEDWIDTH, LAYER3POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer3_POOLEDOUTPUT");

	// layer 4
	Screenshot1DArray(Layer4ActivationMaps, LAYER4WIDTH, LAYER4HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer4_activationmap");
	Screenshot1DArray(Layer4PooledOutput, LAYER4POOLEDWIDTH, LAYER4POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer4_POOLEDOUTPUT");
	
	// layer 5
	Screenshot1DArray(Layer5ActivationMaps, LAYER5WIDTH, LAYER5HEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer5_activationmap");
	Screenshot1DArray(Layer5PooledOutput, LAYER5POOLEDWIDTH, LAYER5POOLEDHEIGHT, "sim" + to_string(OurSimulation.CurrentNumberofSimulations) + "_layer5_POOLEDOUTPUT");
}

int currentthink = 0;

float TweakChance = 0;

RunMode CurrentRunMode;

void GetRunMode()
{
	cout << "What mode should be run?\n1: learning.\n2: performance.\n";

	int mode = -5;

	cin >> mode;

	if (mode == 1) CurrentRunMode = RunMode::Learning;
	else if (mode == 2) CurrentRunMode = RunMode::Performance;
	else
	{
		cout << "ERROR: invalid mode.\n";
		system("Pause");
		exit(0);
	}
}

extern float GetRandomNumber(double, double);

int porknow = 0;

void ConvertRGBQUADtoRGBQUADFLOAT(RGBQUAD* inputarr, RGBQUADFLOAT* outputarr, int size)
{
	long preconvert = GetTime();

	for (int i = 0; i < size; i++)
	{
		outputarr[i].reserved = (float)inputarr[i].rgbReserved;
		outputarr[i].rgbGreen = (float)inputarr[i].rgbGreen;
		outputarr[i].rgbRed = (float)inputarr[i].rgbRed;
		outputarr[i].rgbBlue = (float)inputarr[i].rgbBlue;
	}

	long postconvert = GetTime();
	pixelconverttime = (postconvert - preconvert) / 1000000.0f;
}

void RunNetwork()
{
	long prerun = GetTime();

	CalculateCovolutionalLayer(Layer1Filter, BOTINPUTPIXELHEIGHT, BOTINPUTPIXELWIDTH, PixelsFloat, Layer1ActivationMaps, Layer1PooledOutput);
	CalculateCovolutionalLayer(Layer2Filter, LAYER1POOLEDHEIGHT, LAYER1POOLEDWIDTH, Layer1PooledOutput, Layer2ActivationMaps, Layer2PooledOutput);
	CalculateCovolutionalLayer(Layer3Filter, LAYER2POOLEDHEIGHT, LAYER2POOLEDWIDTH, Layer2PooledOutput, Layer3ActivationMaps, Layer3PooledOutput);
	CalculateCovolutionalLayer(Layer4Filter, LAYER3POOLEDHEIGHT, LAYER3POOLEDWIDTH, Layer3PooledOutput, Layer4ActivationMaps, Layer4PooledOutput);
	CalculateCovolutionalLayer(Layer5Filter, LAYER4POOLEDHEIGHT, LAYER4POOLEDWIDTH, Layer4PooledOutput, Layer5ActivationMaps, Layer5PooledOutput);

	CalculateLayer10();
	CalculateOutputLayer();

	long postrun = GetTime();
	networkruntime = (postrun - prerun) / 1000000.0f;
}

void InvalidateText()
{
	RECT r;
	r.left = 0;
	r.right = 860;
	r.top = 420;
	r.bottom = 0;

	// Re-draw text
	textdrawdirty = true;
	InvalidateRect(GUIWindowHwnd, &r, FALSE);
}

void InvalidateImages()
{
	RECT r;
	r.left = 860;
	r.right = 1900;
	r.top = 420;
	r.bottom = 0;
	// Re-draw images
	imagedrawdirty = true;
	InvalidateRect(GUIWindowHwnd, &r, FALSE);
}

float GetTweakChance()
{
	return GetRandomNumber(0.0, 1.0) * (float)(1.0 * pow(0.8575f, OurSimulation.BestNumberofPorkchops));
}

int main()
{
	long prestart = GetTime();

	hWnd = FindWindowA(NULL, "Minecraft 1.16.5 - Singleplayer");

	GUIWindowHwnd = CreateHelperWindow();

	GotoMinecraftWindow();

	InitialiseInput();

	GetRunMode();

	cout << "Starting in 3 seconds" << endl << endl;
	Sleep(3000);

	LoadArraysFromFile();
	SaveNetwork();

	GetPixels();

	bool screenshotted = false;

	for (; OurSimulation.CurrentNumberofSimulations < OurSimulation.GoalNumberofSimulations; OurSimulation.CurrentNumberofSimulations++)
	{
		timestweaked = 0;
		timesnottweaked = 0;

		if (CurrentRunMode == RunMode::Learning)
		{
			TweakChance = GetTweakChance();
			TweakStuff(TweakChance, 1.1f);
		}

		otherdrawdirty = true;

		OurSimulation.SimulationTotalPorkchops = 0;

		for (OurSimulation.CurrentAverageIteration = 0; OurSimulation.CurrentAverageIteration < OurSimulation.AverageAlgorithmTries; OurSimulation.CurrentAverageIteration++)
		{
			porknow = 0;
			iterationtime = 0;

			if (CurrentRunMode == RunMode::Learning && GetProgressDebt() > (OurSimulation.BestNumberofPorkchops*0.25f)*1.3f)
			{
				PushConsoleLine("Progress debt unrealistically high. Ending simulation.");
				PrintConsole();
				goto end_simulation;
			}

			numtimesattacked = 0;
			havemoved = false;
			havemovedmouse = false;

			ClearInventory();
			Suicide();
			Sleep(1500);

			screenshotted = false;

			ClearConsole();

			for (currentthink = 0; currentthink < OurSimulation.ThinksPerSimulation; currentthink++)
			{
				long poststart = GetTime();
				runtime = (poststart - prestart) / 1000000.0f;

				long a = GetTime();

				ProcessMessages();
				GetPixels();
				ConvertRGBQUADtoRGBQUADFLOAT(Pixels, PixelsFloat, BotViewScreenHeight * BotViewScreenWidth);

				long prerun = GetTime();
				RunNetwork();
				long postrun = GetTime();

				if (CurrentRunMode == RunMode::Learning && !screenshotted)
				{
					TakeScreenshots();
					screenshotted = true;
				}

				PerformOutputs();

				if (HowMuchUncookedPork() > porknow) porknow = HowMuchUncookedPork(); // because it seems sometimes the function returns zero when it shouldnt; this protects the variable

				if (CurrentRunMode == RunMode::Learning)
				{
					if (currentthink >= 150 && GetProgressDebt() > (OurSimulation.BestNumberofPorkchops / 4.0f) * 0.7f)
					{
						PushConsoleLine("Progress debt already unrealistically high. Ending simulation.");
						PrintConsole();
						OurSimulation.SimulationTotalPorkchops += porknow;
						goto end_simulation;
					}
					if (currentthink >= 100 && porknow == 0)
					{
						PushConsoleLine("Not getting anything. Ending simulation.");
						PrintConsole();
						OurSimulation.SimulationTotalPorkchops += porknow;
						goto end_simulation;
					}
					else if (currentthink >= 100 && numtimesattacked == 0)
					{
						PushConsoleLine("Failed to attack. Ending simulation.");
						PrintConsole();
						OurSimulation.SimulationTotalPorkchops += porknow;
						goto end_simulation;
					}
					else if (currentthink >= 50 && havemoved == false)
					{
						PushConsoleLine("Failed to move. Ending simulation.");
						PrintConsole();
						OurSimulation.SimulationTotalPorkchops += porknow;
						goto end_simulation;
					}
					else if (currentthink >= 50 && havemovedmouse == false)
					{
						PushConsoleLine("Failed to move mouse. Ending simulation.");
						PrintConsole();
						OurSimulation.SimulationTotalPorkchops += porknow;
						goto end_simulation;
					}
				}

				if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					AllKeysUp();

					std::cout << "Paused. Press UP to unpause." << endl;

					while (!(GetAsyncKeyState(VK_UP) & 0x8000)) {}
				}

				if ((currentthink % 5) == 0)
				{
					PrintConsole();
					InvalidateText();
					InvalidateImages();
				}
				else InvalidateImages();
					
				long b = GetTime();

				thinktime = (b - a) / 1000000.0f;
				iterationtime += thinktime;

				a = GetTime();

				if (iterationtime < OurSimulation.MinimumThinkTime * (currentthink+1))
				{
					Sleep((DWORD)(((OurSimulation.MinimumThinkTime * (currentthink + 1)) - iterationtime) * 1000.0f));
					b = GetTime();
					thinktime = (b - a) / 1000000.0f;
					iterationtime += thinktime;
				}
			}

			PushConsoleLine("Iteration finished.");
			PrintConsole();

			AllKeysUp();

			OurSimulation.SimulationTotalPorkchops += porknow;
		}

	end_simulation:

		AllKeysUp();
		PushConsoleLine("Simulation finished.");

		if (CurrentRunMode == RunMode::Learning)
		{
			simulationsdonethisrun++;

			if (OurSimulation.SimulationTotalPorkchops >= OurSimulation.BestNumberofPorkchops)
			{
				PushConsoleLine("Better or same cost; keeping");

				if (OurSimulation.SimulationTotalPorkchops > OurSimulation.BestNumberofPorkchops) memset(CloseAttempts, 0, sizeof(CloseAttempts));

				OurSimulation.BestNumberofPorkchops = OurSimulation.SimulationTotalPorkchops;
				OurSimulation.BestAchievedAtIteration = OurSimulation.CurrentNumberofSimulations;

				SaveNetwork();
			}
			else
			{
				// close attempt counter
				if (OurSimulation.SimulationTotalPorkchops >= OurSimulation.BestNumberofPorkchops - 5)
				{
					int n = OurSimulation.SimulationTotalPorkchops - OurSimulation.BestNumberofPorkchops + 5;

					CloseAttempts[n]++;
				}

				PushConsoleLine("Worse cost; reverting network");
				RevertTweaks();
			}

			SaveArraysToFile();
		}

		PrintConsole();
	}

	cout << "Program ended\n.";
	system("Pause");
}

float GetProgressDebt()
{
	return ((OurSimulation.BestNumberofPorkchops / 4.0f) * (OurSimulation.CurrentAverageIteration + 1.0f)) 
		- (OurSimulation.SimulationTotalPorkchops + porknow);
}
#pragma once

// TODO : calculate layer 4 takes too long ... as do other things probably
// todo: change optimisations

#include "Filter.h"
#include "FilterRGBGroup.h"

extern long GetTime();
extern const int BotViewScreenWidth;
extern const int BotViewScreenHeight;

const int BOTINPUTPIXELHEIGHT = BotViewScreenHeight; //200;
const int BOTINPUTPIXELWIDTH = BotViewScreenWidth; //320;

const int INPUTLAYERSIZE = BOTINPUTPIXELHEIGHT * BOTINPUTPIXELWIDTH;

struct RGBQUADFLOAT
{
	float rgbRed;
	float rgbGreen;
	float rgbBlue;
	float reserved;
};

// convolution + pooling
const int LAYER1NUMFILTERS = 1;
const int LAYER1HEIGHT = 598;
const int LAYER1WIDTH = 958;
const int LAYER1SIZE = LAYER1HEIGHT * LAYER1WIDTH;
FilterRGBGroup Layer1Filters[LAYER1NUMFILTERS];
RGBQUADFLOAT Layer1ActivationMaps[LAYER1NUMFILTERS][LAYER1SIZE];
FilterRGBGroup SavedLayer1Filters[LAYER1NUMFILTERS];
RGBQUADFLOAT Layer1PrePoolOutput[LAYER1SIZE];
const int LAYER1POOLEDWIDTH = LAYER1WIDTH / 2;
const int LAYER1POOLEDHEIGHT = LAYER1HEIGHT / 2;
const int LAYER1POOLEDMAPSIZE = LAYER1POOLEDWIDTH * LAYER1POOLEDHEIGHT;
RGBQUADFLOAT Layer1PooledOutput[LAYER1POOLEDMAPSIZE];

// convolution + pooling
const int LAYER2NUMFILTERS = 1;
const int LAYER2HEIGHT = LAYER1POOLEDHEIGHT-2;
const int LAYER2WIDTH = LAYER1POOLEDWIDTH-2;
const int LAYER2SIZE = LAYER2HEIGHT * LAYER2WIDTH;
FilterRGBGroup Layer2Filters[LAYER2NUMFILTERS];
RGBQUADFLOAT Layer2ActivationMaps[LAYER2NUMFILTERS][LAYER2SIZE];
FilterRGBGroup SavedLayer2Filters[LAYER2NUMFILTERS];
RGBQUADFLOAT Layer2PrePoolOutput[LAYER2SIZE];
const int LAYER2POOLEDWIDTH = LAYER2WIDTH / 2;
const int LAYER2POOLEDHEIGHT = LAYER2HEIGHT / 2;
const int LAYER2POOLEDMAPSIZE = LAYER2POOLEDHEIGHT * LAYER2POOLEDWIDTH;
RGBQUADFLOAT Layer2PooledOutput[LAYER2POOLEDMAPSIZE];

// convolution + pooling
const int LAYER3NUMFILTERS = 1;
const int LAYER3HEIGHT = LAYER2POOLEDHEIGHT - 2;
const int LAYER3WIDTH = LAYER2POOLEDWIDTH - 2;
const int LAYER3SIZE = LAYER3HEIGHT * LAYER3WIDTH;
FilterRGBGroup Layer3Filters[LAYER3NUMFILTERS];
RGBQUADFLOAT Layer3ActivationMaps[LAYER3NUMFILTERS][LAYER3SIZE];
FilterRGBGroup SavedLayer3Filters[LAYER3NUMFILTERS];
const int LAYER3POOLEDWIDTH = LAYER3WIDTH / 2;
const int LAYER3POOLEDHEIGHT = LAYER3HEIGHT / 2;
const int LAYER3POOLEDMAPSIZE = LAYER3POOLEDHEIGHT * LAYER3POOLEDWIDTH;
RGBQUADFLOAT Layer3PooledActivationMaps[LAYER3NUMFILTERS][LAYER3POOLEDMAPSIZE];

const int LAYER4SIZE = 30;
const int LAYER4NUMWEIGHTS = LAYER4SIZE * (LAYER3POOLEDMAPSIZE * LAYER3NUMFILTERS);
double Layer4Neurons[LAYER4SIZE];
float Layer4Weights[LAYER4NUMWEIGHTS];
float Layer4Biases[LAYER4SIZE];
float SavedLayer4Weights[LAYER4NUMWEIGHTS];
float SavedLayer4Biases[LAYER4SIZE];

const int OUTPUTLAYERSIZE = 10;
const int OUTPUTLAYERNUMWEIGHTS = OUTPUTLAYERSIZE * LAYER4SIZE;
double OutputNeurons[OUTPUTLAYERSIZE];
float OutputWeights[OUTPUTLAYERNUMWEIGHTS];
float OutputBiases[OUTPUTLAYERSIZE];
float SavedOutputWeights[OUTPUTLAYERNUMWEIGHTS];
float SavedOutputBiases[OUTPUTLAYERSIZE];

extern RGBQUAD* Pixels;

float GetFilterWeightsSum(int layer)
{
	if (layer == 1)
	{
		float total = 0;

		for (int i = 0; i < 9; i++)
		{
			total += Layer1Filters[1].OurFilter.Weights[i].blueWeight;
			total += Layer1Filters[1].OurFilter.Weights[i].greenWeight;
			total += Layer1Filters[1].OurFilter.Weights[i].redWeight;
		}

		return total;
	}

	exit(0);
}

float GetSuitableMaxBiasForLayer(int layer)
{
	switch (layer)
	{
	case 1: // layer1
		return 10;
		break;
	case 2:
		return 10;
		break;
	case 3: 
		return 10;
		break;
	case 4:
		return 100;
		break;
	case 5:  // outputlayer
		return 500;
		break;
	default:
		cout << "ERROR: UNKNOWN LAYER";
		system("Pause");
		break;
	}

	exit(0);
}

static unsigned long x = 113456789;
static unsigned long y = 362736069;
static unsigned long z = 521258629;

unsigned long xorshf96() 
{
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

/* Seed */
std::random_device rd;
/* Random number generator */
std::default_random_engine generator(rd());
/* Distribution on which to apply the generator */
std::uniform_int_distribution<long long unsigned> distribution(0, 0xFFFFFFFFFFFF);

float GetRandomNumber(double from, double to)
{
	unsigned long long range = (to - from)*100000.0;

	unsigned long long result = distribution(generator) % range;

	return (float)(((double)result / 100000.0)+from);
}

	//return 0;

	//long range = (long)(1000.0f*(to - from));
	//long res = xorshf96() % range;
	//return res / 1000.0f;
//}

inline float FastSigmoidFunction(float x)
{
	return x / (1 + abs(x));
}

inline int RectifiedLinearUnitFunction(int x)
{
	if (x > 0) return x;
	return 0;
}

inline float RectifiedLinearUnitFunction(float x)
{
	if (x > 0) return x;
	return 0;
}

inline double RectifiedLinearUnitFunction(double x)
{
	if (x > 0) return x;
	return 0;
}

inline RGBQUADFLOAT RectifiedLinearUnitFunctionRGB(RGBQUADFLOAT x)
{
	RGBQUADFLOAT returnv;

	if (x.rgbBlue > 0) returnv.rgbBlue = x.rgbBlue;
	if (x.rgbGreen > 0) returnv.rgbGreen = x.rgbGreen;
	if (x.rgbRed > 0) returnv.rgbRed = x.rgbRed;

	return returnv;
}

inline float Max(float a, float b, float c, float d)
{
	float* biggest = &a;

	if (b > * biggest) biggest = &b;
	if (c > * biggest) biggest = &c;
	if (d > * biggest) biggest = &d;

	return *biggest;
}

void NormaliseArray0to1(RGBQUADFLOAT* arr, int size)
{
	float highest = 0;
	float lowest = 0;

	for (int i = 0; i < size; i++)
	{
		if (arr[i].rgbBlue < lowest) lowest = arr[i].rgbBlue;
		if (arr[i].rgbGreen < lowest) lowest = arr[i].rgbGreen;
		if (arr[i].rgbRed < lowest) lowest = arr[i].rgbRed;

		if (arr[i].rgbBlue > highest) highest = arr[i].rgbBlue;
		if (arr[i].rgbGreen > highest) highest = arr[i].rgbGreen;
		if (arr[i].rgbRed > highest) highest = arr[i].rgbRed;
	}

	float range = highest - lowest;
	float offset = 0;

	if (lowest < 0) offset = abs(lowest); // we'll add this to the data so that there are no negative numbers

	for (int i = 0; i < size; i++)
	{
		arr[i].rgbBlue = (arr[i].rgbBlue + offset) / range;
		arr[i].rgbGreen = (arr[i].rgbGreen + offset) / range;
		arr[i].rgbRed = (arr[i].rgbRed + offset) / range;
	}
}

void NormaliseArray0to255(RGBQUADFLOAT* arr, int size)
{
	float highest = 0;
	float lowest = 0;

	for (int i = 0; i < size; i++)
	{
		if (arr[i].rgbBlue < lowest) lowest = arr[i].rgbBlue;
		if (arr[i].rgbGreen < lowest) lowest = arr[i].rgbGreen;
		if (arr[i].rgbRed < lowest) lowest = arr[i].rgbRed;

		if (arr[i].rgbBlue > highest) highest = arr[i].rgbBlue;
		if (arr[i].rgbGreen > highest) highest = arr[i].rgbGreen;
		if (arr[i].rgbRed > highest) highest = arr[i].rgbRed;
	}

	float range = highest - lowest;
	float offset = 0;

	if (lowest < 0) offset = abs(lowest); // we'll add this to the data so that there are no negative numbers

	for (int i = 0; i < size; i++)
	{
		arr[i].rgbBlue = (arr[i].rgbBlue + offset) / range * 255.0f;
		arr[i].rgbGreen = (arr[i].rgbGreen + offset) / range * 255.0f;
		arr[i].rgbRed = (arr[i].rgbRed + offset) / range * 255.0f;
	}
}

void CalculateLayer1()
{
	long a = GetTime();

	// calculate activation maps for each filter

	for (int f = 0; f < LAYER1NUMFILTERS; f++) // for each filter
	{
		for (int y = 1; y < BOTINPUTPIXELHEIGHT - 1; y++) // traverse the input image
		{
			for (int x = 1; x < BOTINPUTPIXELWIDTH - 1; x++)
			{
				float red = RectifiedLinearUnitFunction(
					Layer1Filters[f].OurFilter.Weights[0].redWeight * Pixels[(x - 1) + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[1].redWeight * Pixels[x + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[2].redWeight * Pixels[(x + 1) + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[3].redWeight * Pixels[(x - 1) + (y * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[4].redWeight * Pixels[x + (y * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[5].redWeight * Pixels[(x + 1) + (y * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[6].redWeight * Pixels[(x - 1) + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[7].redWeight * Pixels[x + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].OurFilter.Weights[8].redWeight * Pixels[(x + 1) + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbRed +
					Layer1Filters[f].Bias);

				float green = RectifiedLinearUnitFunction(
					Layer1Filters[f].OurFilter.Weights[0].greenWeight * Pixels[(x - 1) + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[1].greenWeight * Pixels[x + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[2].greenWeight * Pixels[(x + 1) + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[3].greenWeight * Pixels[(x - 1) + (y * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[4].greenWeight * Pixels[x + (y * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[5].greenWeight * Pixels[(x + 1) + (y * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[6].greenWeight * Pixels[(x - 1) + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[7].greenWeight * Pixels[x + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].OurFilter.Weights[8].greenWeight * Pixels[(x + 1) + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbGreen +
					Layer1Filters[f].Bias);

				float blue = RectifiedLinearUnitFunction(
					Layer1Filters[f].OurFilter.Weights[0].blueWeight * Pixels[(x - 1) + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[1].blueWeight * Pixels[x + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[2].blueWeight * Pixels[(x + 1) + ((y - 1) * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[3].blueWeight * Pixels[(x - 1) + (y * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[4].blueWeight * Pixels[x + (y * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[5].blueWeight * Pixels[(x + 1) + (y * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[6].blueWeight * Pixels[(x - 1) + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[7].blueWeight * Pixels[x + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].OurFilter.Weights[8].blueWeight * Pixels[(x + 1) + ((y + 1) * BOTINPUTPIXELWIDTH)].rgbBlue +
					Layer1Filters[f].Bias);

				Layer1ActivationMaps[f][(x - 1) + (LAYER1WIDTH * (y - 1))].rgbBlue = blue;
				Layer1ActivationMaps[f][(x - 1) + (LAYER1WIDTH * (y - 1))].rgbRed = red;
				Layer1ActivationMaps[f][(x - 1) + (LAYER1WIDTH * (y - 1))].rgbGreen = green;
			}
		}

		NormaliseArray0to255(Layer1ActivationMaps[f], LAYER1SIZE);
	}

	// wipe arrays

	for (int a = 0; a < LAYER1SIZE; a++)
	{
		Layer1PrePoolOutput[a].rgbBlue = 0;
		Layer1PrePoolOutput[a].rgbGreen = 0;
		Layer1PrePoolOutput[a].rgbRed = 0;
	}

	for (int a = 0; a < LAYER1POOLEDMAPSIZE; a++)
	{
		Layer1PooledOutput[a].rgbBlue = 0;
		Layer1PooledOutput[a].rgbGreen = 0;
		Layer1PooledOutput[a].rgbRed = 0;
	}

	// sum activation maps

	for (int y = 0; y < LAYER1HEIGHT; y++)
	{
		for (int x = 0; x < LAYER1WIDTH; x++)
		{
			for (int fil = 0; fil < LAYER1NUMFILTERS; fil++) // for each filter
			{
				Layer1PrePoolOutput[x + (y * LAYER1WIDTH)].rgbBlue += Layer1ActivationMaps[fil][x + (y * LAYER1WIDTH)].rgbBlue;
				Layer1PrePoolOutput[x + (y * LAYER1WIDTH)].rgbRed += Layer1ActivationMaps[fil][x + (y * LAYER1WIDTH)].rgbRed;
				Layer1PrePoolOutput[x + (y * LAYER1WIDTH)].rgbGreen += Layer1ActivationMaps[fil][x + (y * LAYER1WIDTH)].rgbGreen;
			}
		}
	}

	NormaliseArray0to255(Layer1PrePoolOutput, LAYER1SIZE);

	// pool output .. half size

	for (int y = 0; y < LAYER1POOLEDHEIGHT; y++)
	{
		for (int x = 0; x < LAYER1POOLEDWIDTH; x++)
		{
			float redmax = Max(Layer1PrePoolOutput[(x * 2) + ((y * 2) * LAYER1WIDTH)].rgbRed,
				Layer1PrePoolOutput[(x * 2) + 1 + ((y * 2) * LAYER1WIDTH)].rgbRed,
				Layer1PrePoolOutput[(x * 2) + (((y * 2) + 1) * LAYER1WIDTH)].rgbRed,
				Layer1PrePoolOutput[(x * 2) + 1 + (((y * 2) + 1) * LAYER1WIDTH)].rgbRed);

			float bluemax = Max(Layer1PrePoolOutput[(x * 2) + ((y * 2) * LAYER1WIDTH)].rgbBlue,
				Layer1PrePoolOutput[(x * 2) + 1 + ((y * 2) * LAYER1WIDTH)].rgbBlue,
				Layer1PrePoolOutput[(x * 2) + (((y * 2) + 1) * LAYER1WIDTH)].rgbBlue,
				Layer1PrePoolOutput[(x * 2) + 1 + (((y * 2) + 1) * LAYER1WIDTH)].rgbBlue);

			float greenmax = Max(Layer1PrePoolOutput[(x * 2) + ((y * 2) * LAYER1WIDTH)].rgbGreen,
				Layer1PrePoolOutput[(x * 2) + 1 + ((y * 2) * LAYER1WIDTH)].rgbGreen,
				Layer1PrePoolOutput[(x * 2) + (((y * 2) + 1) * LAYER1WIDTH)].rgbGreen,
				Layer1PrePoolOutput[(x * 2) + 1 + (((y * 2) + 1) * LAYER1WIDTH)].rgbGreen);

			Layer1PooledOutput[x + (y * LAYER1POOLEDWIDTH)].rgbBlue = bluemax;
			Layer1PooledOutput[x + (y * LAYER1POOLEDWIDTH)].rgbGreen = greenmax;
			Layer1PooledOutput[x + (y * LAYER1POOLEDWIDTH)].rgbRed = redmax;
		}
	}

	NormaliseArray0to255(Layer1PooledOutput, LAYER1POOLEDMAPSIZE);
}

void CalculateLayer2()
{
	long a = GetTime();

	// calculate activation maps for each filter

	for (int f = 0; f < LAYER2NUMFILTERS; f++) // for each filter
	{
		for (int y = 1; y < LAYER1POOLEDHEIGHT - 1; y++) // traverse the input image
		{
			for (int x = 1; x < LAYER1POOLEDWIDTH - 1; x++)
			{
				float red = RectifiedLinearUnitFunction(
					Layer2Filters[f].OurFilter.Weights[0].redWeight * Layer1PooledOutput[(x - 1) + ((y - 1) * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[1].redWeight * Layer1PooledOutput[x + ((y - 1) * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[2].redWeight * Layer1PooledOutput[(x + 1) + ((y - 1) * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[3].redWeight * Layer1PooledOutput[(x - 1) + (y * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[4].redWeight * Layer1PooledOutput[x + (y * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[5].redWeight * Layer1PooledOutput[(x + 1) + (y * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[6].redWeight * Layer1PooledOutput[(x - 1) + ((y + 1) * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[7].redWeight * Layer1PooledOutput[x + ((y + 1) * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].OurFilter.Weights[8].redWeight * Layer1PooledOutput[(x + 1) + ((y + 1) * LAYER1POOLEDWIDTH)].rgbRed +
					Layer2Filters[f].Bias);

				float green = RectifiedLinearUnitFunction(
					Layer2Filters[f].OurFilter.Weights[0].greenWeight * Layer1PooledOutput[(x - 1) + ((y - 1) * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[1].greenWeight * Layer1PooledOutput[x + ((y - 1) * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[2].greenWeight * Layer1PooledOutput[(x + 1) + ((y - 1) * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[3].greenWeight * Layer1PooledOutput[(x - 1) + (y * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[4].greenWeight * Layer1PooledOutput[x + (y * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[5].greenWeight * Layer1PooledOutput[(x + 1) + (y * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[6].greenWeight * Layer1PooledOutput[(x - 1) + ((y + 1) * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[7].greenWeight * Layer1PooledOutput[x + ((y + 1) * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].OurFilter.Weights[8].greenWeight * Layer1PooledOutput[(x + 1) + ((y + 1) * LAYER1POOLEDWIDTH)].rgbGreen +
					Layer2Filters[f].Bias);

				float blue = RectifiedLinearUnitFunction(
					Layer2Filters[f].OurFilter.Weights[0].blueWeight * Layer1PooledOutput[(x - 1) + ((y - 1) * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[1].blueWeight * Layer1PooledOutput[x + ((y - 1) * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[2].blueWeight * Layer1PooledOutput[(x + 1) + ((y - 1) * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[3].blueWeight * Layer1PooledOutput[(x - 1) + (y * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[4].blueWeight * Layer1PooledOutput[x + (y * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[5].blueWeight * Layer1PooledOutput[(x + 1) + (y * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[6].blueWeight * Layer1PooledOutput[(x - 1) + ((y + 1) * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[7].blueWeight * Layer1PooledOutput[x + ((y + 1) * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].OurFilter.Weights[8].blueWeight * Layer1PooledOutput[(x + 1) + ((y + 1) * LAYER1POOLEDWIDTH)].rgbBlue +
					Layer2Filters[f].Bias);

				Layer2ActivationMaps[f][(x - 1) + (LAYER2WIDTH * (y - 1))].rgbBlue = blue;
				Layer2ActivationMaps[f][(x - 1) + (LAYER2WIDTH * (y - 1))].rgbRed = red;
				Layer2ActivationMaps[f][(x - 1) + (LAYER2WIDTH * (y - 1))].rgbGreen = green;
			}
		}

		NormaliseArray0to255(Layer2ActivationMaps[f], LAYER2SIZE);
	}

	// wipe arrays

	for (int a = 0; a < LAYER2SIZE; a++)
	{
		Layer2PrePoolOutput[a].rgbBlue = 0;
		Layer2PrePoolOutput[a].rgbGreen = 0;
		Layer2PrePoolOutput[a].rgbRed = 0;
	}

	for (int a = 0; a < LAYER2POOLEDMAPSIZE; a++)
	{
		Layer2PooledOutput[a].rgbBlue = 0;
		Layer2PooledOutput[a].rgbGreen = 0;
		Layer2PooledOutput[a].rgbRed = 0;
	}

	// sum activation maps

	for (int y = 0; y < LAYER2HEIGHT; y++)
	{
		for (int x = 0; x < LAYER2WIDTH; x++)
		{
			for (int fil = 0; fil < LAYER2NUMFILTERS; fil++) // for each filter
			{
				Layer2PrePoolOutput[x + (y * LAYER2WIDTH)].rgbBlue += Layer2ActivationMaps[fil][x + (y * LAYER2WIDTH)].rgbBlue;
				Layer2PrePoolOutput[x + (y * LAYER2WIDTH)].rgbRed += Layer2ActivationMaps[fil][x + (y * LAYER2WIDTH)].rgbRed;
				Layer2PrePoolOutput[x + (y * LAYER2WIDTH)].rgbGreen += Layer2ActivationMaps[fil][x + (y * LAYER2WIDTH)].rgbGreen;
			}
		}
	}

	NormaliseArray0to255(Layer2PrePoolOutput, LAYER2SIZE);

	// pool output .. half size

	for (int y = 0; y < LAYER2POOLEDHEIGHT; y++)
	{
		for (int x = 0; x < LAYER2POOLEDWIDTH; x++)
		{
			float redmax = Max(Layer2PrePoolOutput[(x * 2) + ((y * 2) * LAYER2WIDTH)].rgbRed,
				Layer2PrePoolOutput[(x * 2) + 1 + ((y * 2) * LAYER2WIDTH)].rgbRed,
				Layer2PrePoolOutput[(x * 2) + (((y * 2) + 1) * LAYER2WIDTH)].rgbRed,
				Layer2PrePoolOutput[(x * 2) + 1 + (((y * 2) + 1) * LAYER2WIDTH)].rgbRed);

			float bluemax = Max(Layer2PrePoolOutput[(x * 2) + ((y * 2) * LAYER2WIDTH)].rgbBlue,
				Layer2PrePoolOutput[(x * 2) + 1 + ((y * 2) * LAYER2WIDTH)].rgbBlue,
				Layer2PrePoolOutput[(x * 2) + (((y * 2) + 1) * LAYER2WIDTH)].rgbBlue,
				Layer2PrePoolOutput[(x * 2) + 1 + (((y * 2) + 1) * LAYER2WIDTH)].rgbBlue);

			float greenmax = Max(Layer2PrePoolOutput[(x * 2) + ((y * 2) * LAYER2WIDTH)].rgbGreen,
				Layer2PrePoolOutput[(x * 2) + 1 + ((y * 2) * LAYER2WIDTH)].rgbGreen,
				Layer2PrePoolOutput[(x * 2) + (((y * 2) + 1) * LAYER2WIDTH)].rgbGreen,
				Layer2PrePoolOutput[(x * 2) + 1 + (((y * 2) + 1) * LAYER2WIDTH)].rgbGreen);

			Layer2PooledOutput[x + (y * LAYER2POOLEDWIDTH)].rgbBlue = bluemax;
			Layer2PooledOutput[x + (y * LAYER2POOLEDWIDTH)].rgbGreen = greenmax;
			Layer2PooledOutput[x + (y * LAYER2POOLEDWIDTH)].rgbRed = redmax;
		}
	}

	NormaliseArray0to255(Layer2PooledOutput, LAYER2POOLEDMAPSIZE);
}

void CalculateLayer3()
{
	long a = GetTime();

	// calculate activation maps for each filter

	for (int f = 0; f < LAYER3NUMFILTERS; f++) // for each filter
	{
		for (int y = 1; y < LAYER2POOLEDHEIGHT-1; y++) // traverse the input image
		{
			for (int x = 1; x < LAYER2POOLEDWIDTH-1; x++)
			{
				float red = RectifiedLinearUnitFunction(
					Layer3Filters[f].OurFilter.Weights[0].redWeight * Layer2PooledOutput[(x - 1) + ((y - 1) * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[1].redWeight * Layer2PooledOutput[x + ((y - 1) * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[2].redWeight * Layer2PooledOutput[(x + 1) + ((y - 1) * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[3].redWeight * Layer2PooledOutput[(x - 1) + (y * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[4].redWeight * Layer2PooledOutput[x + (y * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[5].redWeight * Layer2PooledOutput[(x + 1) + (y * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[6].redWeight * Layer2PooledOutput[(x - 1) + ((y + 1) * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[7].redWeight * Layer2PooledOutput[x + ((y + 1) * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].OurFilter.Weights[8].redWeight * Layer2PooledOutput[(x + 1) + ((y + 1) * LAYER2POOLEDWIDTH)].rgbRed +
					Layer3Filters[f].Bias);

				float green = RectifiedLinearUnitFunction(
					Layer3Filters[f].OurFilter.Weights[0].greenWeight * Layer2PooledOutput[(x - 1) + ((y - 1) * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[1].greenWeight * Layer2PooledOutput[x + ((y - 1) * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[2].greenWeight * Layer2PooledOutput[(x + 1) + ((y - 1) * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[3].greenWeight * Layer2PooledOutput[(x - 1) + (y * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[4].greenWeight * Layer2PooledOutput[x + (y * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[5].greenWeight * Layer2PooledOutput[(x + 1) + (y * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[6].greenWeight * Layer2PooledOutput[(x - 1) + ((y + 1) * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[7].greenWeight * Layer2PooledOutput[x + ((y + 1) * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].OurFilter.Weights[8].greenWeight * Layer2PooledOutput[(x + 1) + ((y + 1) * LAYER2POOLEDWIDTH)].rgbGreen +
					Layer3Filters[f].Bias);

				float blue = RectifiedLinearUnitFunction(
					Layer3Filters[f].OurFilter.Weights[0].blueWeight * Layer2PooledOutput[(x - 1) + ((y - 1) * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[1].blueWeight * Layer2PooledOutput[x + ((y - 1) * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[2].blueWeight * Layer2PooledOutput[(x + 1) + ((y - 1) * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[3].blueWeight * Layer2PooledOutput[(x - 1) + (y * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[4].blueWeight * Layer2PooledOutput[x + (y * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[5].blueWeight * Layer2PooledOutput[(x + 1) + (y * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[6].blueWeight * Layer2PooledOutput[(x - 1) + ((y + 1) * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[7].blueWeight * Layer2PooledOutput[x + ((y + 1) * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].OurFilter.Weights[8].blueWeight * Layer2PooledOutput[(x + 1) + ((y + 1) * LAYER2POOLEDWIDTH)].rgbBlue +
					Layer3Filters[f].Bias);

				Layer3ActivationMaps[f][(x - 1) + (LAYER3WIDTH * (y - 1))].rgbBlue = blue;
				Layer3ActivationMaps[f][(x - 1) + (LAYER3WIDTH * (y - 1))].rgbRed = red;
				Layer3ActivationMaps[f][(x - 1) + (LAYER3WIDTH * (y - 1))].rgbGreen = green;
			}
		}

		NormaliseArray0to255(Layer3ActivationMaps[f], LAYER3SIZE);
	}

	// pool

	for (int map = 0; map < LAYER3NUMFILTERS; map++) // for each filter
	{
		for (int y = 0; y < LAYER3POOLEDHEIGHT; y++) // traverse the input image
		{
			for (int x = 0; x < LAYER3POOLEDWIDTH; x++)
			{
				float redmax = Max(Layer3ActivationMaps[map][(x * 2) + ((y * 2) * LAYER3WIDTH)].rgbRed,
					Layer3ActivationMaps[map][(x * 2) + 1 + ((y * 2) * LAYER3WIDTH)].rgbRed,
					Layer3ActivationMaps[map][(x * 2) + (((y * 2) + 1) * LAYER3WIDTH)].rgbRed,
					Layer3ActivationMaps[map][(x * 2) + 1 + (((y * 2) + 1) * LAYER3WIDTH)].rgbRed);

				float bluemax = Max(Layer3ActivationMaps[map][(x * 2) + ((y * 2) * LAYER3WIDTH)].rgbBlue,
					Layer3ActivationMaps[map][(x * 2) + 1 + ((y * 2) * LAYER3WIDTH)].rgbBlue,
					Layer3ActivationMaps[map][(x * 2) + (((y * 2) + 1) * LAYER3WIDTH)].rgbBlue,
					Layer3ActivationMaps[map][(x * 2) + 1 + (((y * 2) + 1) * LAYER3WIDTH)].rgbBlue);

				float greenmax = Max(Layer3ActivationMaps[map][(x * 2) + ((y * 2) * LAYER3WIDTH)].rgbGreen,
					Layer3ActivationMaps[map][(x * 2) + 1 + ((y * 2) * LAYER3WIDTH)].rgbGreen,
					Layer3ActivationMaps[map][(x * 2) + (((y * 2) + 1) * LAYER3WIDTH)].rgbGreen,
					Layer3ActivationMaps[map][(x * 2) + 1 + (((y * 2) + 1) * LAYER3WIDTH)].rgbGreen);

				Layer3PooledActivationMaps[map][x + (y * LAYER3POOLEDWIDTH)].rgbBlue = bluemax;
				Layer3PooledActivationMaps[map][x + (y * LAYER3POOLEDWIDTH)].rgbGreen = greenmax;
				Layer3PooledActivationMaps[map][x + (y * LAYER3POOLEDWIDTH)].rgbRed = redmax;
			}
		}

		NormaliseArray0to255(Layer3PooledActivationMaps[map], LAYER3POOLEDMAPSIZE);
		//NormaliseArray0to1(Layer3PooledActivationMaps[map], LAYER3POOLEDMAPSIZE);
	}

	long b = GetTime();
}

void CalculateLayer4()
{
	for (int i = 0; i < LAYER4SIZE; i++) // for each neuron
	{
		Layer4Neurons[i] = Layer4Biases[i];

		for (int point = 0; point < LAYER3POOLEDMAPSIZE; point++) // each point (theres only one map lol)
		{
			Layer4Neurons[i] += ((double)Layer3PooledActivationMaps[0][point].rgbRed * Layer4Weights[(LAYER3POOLEDMAPSIZE * i) + point]);
			Layer4Neurons[i] += ((double)Layer3PooledActivationMaps[0][point].rgbBlue * Layer4Weights[(LAYER3POOLEDMAPSIZE * i) + point]);
			Layer4Neurons[i] += ((double)Layer3PooledActivationMaps[0][point].rgbGreen * Layer4Weights[(LAYER3POOLEDMAPSIZE * i) + point]);
		}

		Layer4Neurons[i] = RectifiedLinearUnitFunction(Layer4Neurons[i]);
	}
}

void CalculateOutputLayer()
{
	for (int i = 0; i < OUTPUTLAYERSIZE; i++) // for each neuron
	{
		OutputNeurons[i] = OutputBiases[i];

		for (int con = 0; con < LAYER4SIZE; con++) // for each connection
		{
			OutputNeurons[i] += Layer4Neurons[con] * OutputWeights[con];
		}

		//OutputNeurons[i] = RectifiedLinearUnitFunction(OutputNeurons[i]);
	}
}

void RevertTweaks()
{
	memcpy(Layer1Filters, SavedLayer1Filters, sizeof(SavedLayer1Filters));
	memcpy(Layer2Filters, SavedLayer2Filters, sizeof(SavedLayer2Filters));
	memcpy(Layer3Filters, SavedLayer3Filters, sizeof(SavedLayer3Filters));

	memcpy(Layer4Biases, SavedLayer4Biases, sizeof(SavedLayer4Biases));
	memcpy(Layer4Weights, SavedLayer4Weights, sizeof(SavedLayer4Weights));

	memcpy(OutputWeights, SavedOutputWeights, sizeof(SavedOutputWeights));
	memcpy(OutputBiases, SavedOutputBiases, sizeof(SavedOutputBiases));
}

void SaveNetwork()
{
	memcpy(SavedLayer1Filters, Layer1Filters, sizeof(Layer1Filters));
	memcpy(SavedLayer2Filters, Layer2Filters, sizeof(Layer2Filters));
	memcpy(SavedLayer3Filters, Layer3Filters, sizeof(Layer3Filters));

	memcpy(SavedLayer4Biases, Layer4Biases, sizeof(Layer4Biases));
	memcpy(SavedLayer4Weights, Layer4Weights, sizeof(Layer4Weights));

	memcpy(SavedOutputWeights, OutputWeights, sizeof(OutputWeights));
	memcpy(SavedOutputBiases, OutputBiases, sizeof(OutputBiases));
}

int timestweaked;
int timesnottweaked;

inline bool ShouldTweak(float chance)
{
	if (GetRandomNumber(1, 10000000) <= chance * 10000000.0f)
	{
		timestweaked++;
		return true;
	}
	
	timesnottweaked++;
	return false;
}

void TweakStuff(float chanceoftweaking, float maxscalefactor) // from 0 - 1
{
	// Tweak fully connected weights

	for (int i = 0; i < LAYER4NUMWEIGHTS; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer4Weights[i] = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	for (int i = 0; i < OUTPUTLAYERNUMWEIGHTS; i++)
	{
		if (ShouldTweak(chanceoftweaking)) OutputWeights[i] = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	// Tweak filter weights

	for (int f = 0; f < LAYER1NUMFILTERS; f++)
	{
		for (int i = 0; i < 9; i++)
		{
			if (ShouldTweak(chanceoftweaking)) Layer1Filters[f].OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
			if (ShouldTweak(chanceoftweaking)) Layer1Filters[f].OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
			if (ShouldTweak(chanceoftweaking)) Layer1Filters[f].OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		}
	}

	for (int f = 0; f < LAYER2NUMFILTERS; f++)
	{
		for (int i = 0; i < 9; i++)
		{
			if (ShouldTweak(chanceoftweaking)) Layer2Filters[f].OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
			if (ShouldTweak(chanceoftweaking)) Layer2Filters[f].OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
			if (ShouldTweak(chanceoftweaking)) Layer2Filters[f].OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		}
	}

	for (int f = 0; f < LAYER3NUMFILTERS; f++)
	{
		for (int i = 0; i < 9; i++)
		{
			if (ShouldTweak(chanceoftweaking)) Layer3Filters[f].OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
			if (ShouldTweak(chanceoftweaking)) Layer3Filters[f].OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
			if (ShouldTweak(chanceoftweaking)) Layer3Filters[f].OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		}
	}

	float layer1maxbias = GetSuitableMaxBiasForLayer(1);
	float layer2maxbias = GetSuitableMaxBiasForLayer(2);
	float layer3maxbias = GetSuitableMaxBiasForLayer(3);
	float layer4maxbias = GetSuitableMaxBiasForLayer(4);
	float outputmaxbias = GetSuitableMaxBiasForLayer(5);

	// Tweak fully connected biases

	for (int i = 0; i < LAYER4SIZE; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer4Biases[i] = GetRandomNumber(-layer4maxbias, layer4maxbias);
	}

	for (int i = 0; i < OUTPUTLAYERSIZE; i++)
	{
		if (ShouldTweak(chanceoftweaking)) OutputBiases[i] = GetRandomNumber(-outputmaxbias, outputmaxbias);
	}

	// Tweak filter biases;

	for (int f = 0; f < LAYER1NUMFILTERS; f++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer1Filters[f].Bias = GetRandomNumber(-layer1maxbias, layer1maxbias);
	}

	for (int f = 0; f < LAYER2NUMFILTERS; f++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer2Filters[f].Bias = GetRandomNumber(-layer2maxbias, layer2maxbias);
	}

	for (int f = 0; f < LAYER3NUMFILTERS; f++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer3Filters[f].Bias = GetRandomNumber(-layer3maxbias, layer3maxbias);
	}
}
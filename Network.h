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
const int LAYER1HEIGHT = 598;
const int LAYER1WIDTH = 958;
const int LAYER1SIZE = LAYER1HEIGHT * LAYER1WIDTH;
FilterRGBGroup Layer1Filter;
RGBQUADFLOAT Layer1ActivationMaps[LAYER1SIZE];
FilterRGBGroup SavedLayer1Filter;
const int LAYER1POOLEDWIDTH = LAYER1WIDTH / 2; 
const int LAYER1POOLEDHEIGHT = LAYER1HEIGHT / 2;
const int LAYER1POOLEDMAPSIZE = LAYER1POOLEDWIDTH * LAYER1POOLEDHEIGHT;
RGBQUADFLOAT Layer1PooledOutput[LAYER1POOLEDMAPSIZE];

// convolution + pooling
const int LAYER2HEIGHT = 297;
const int LAYER2WIDTH = 477;
const int LAYER2SIZE = LAYER2HEIGHT * LAYER2WIDTH;
FilterRGBGroup Layer2Filter;
RGBQUADFLOAT Layer2ActivationMaps[LAYER2SIZE];
FilterRGBGroup SavedLayer2Filter;
const int LAYER2POOLEDWIDTH = LAYER2WIDTH / 2;
const int LAYER2POOLEDHEIGHT = LAYER2HEIGHT / 2;
const int LAYER2POOLEDMAPSIZE = LAYER2POOLEDHEIGHT * LAYER2POOLEDWIDTH;
RGBQUADFLOAT Layer2PooledOutput[LAYER2POOLEDMAPSIZE];

// convolution + pooling
const int LAYER3HEIGHT = 146;
const int LAYER3WIDTH = 236;
const int LAYER3SIZE = LAYER3HEIGHT * LAYER3WIDTH;
FilterRGBGroup Layer3Filter;
RGBQUADFLOAT Layer3ActivationMaps[LAYER3SIZE];
FilterRGBGroup SavedLayer3Filter;
const int LAYER3POOLEDWIDTH = LAYER3WIDTH / 2;
const int LAYER3POOLEDHEIGHT = LAYER3HEIGHT / 2;
const int LAYER3POOLEDMAPSIZE = LAYER3POOLEDHEIGHT * LAYER3POOLEDWIDTH;
RGBQUADFLOAT Layer3PooledOutput[LAYER3POOLEDMAPSIZE];

// convolution + pooling
const int LAYER4HEIGHT = 71;
const int LAYER4WIDTH = 116;
const int LAYER4SIZE = LAYER4HEIGHT * LAYER4WIDTH;
FilterRGBGroup Layer4Filter;
RGBQUADFLOAT Layer4ActivationMaps[LAYER4SIZE];
FilterRGBGroup SavedLayer4Filter;
const int LAYER4POOLEDWIDTH = LAYER4WIDTH / 2;
const int LAYER4POOLEDHEIGHT = LAYER4HEIGHT / 2;
const int LAYER4POOLEDMAPSIZE = LAYER4POOLEDHEIGHT * LAYER4POOLEDWIDTH;
RGBQUADFLOAT Layer4PooledOutput[LAYER4POOLEDMAPSIZE];

// convolution + pooling
const int LAYER5HEIGHT = 33;
const int LAYER5WIDTH = 56;
const int LAYER5SIZE = LAYER5HEIGHT * LAYER5WIDTH;
FilterRGBGroup Layer5Filter;
RGBQUADFLOAT Layer5ActivationMaps[LAYER5SIZE];
FilterRGBGroup SavedLayer5Filter;
const int LAYER5POOLEDWIDTH = LAYER5WIDTH / 2;
const int LAYER5POOLEDHEIGHT = LAYER5HEIGHT / 2;
const int LAYER5POOLEDMAPSIZE = LAYER5POOLEDHEIGHT * LAYER5POOLEDWIDTH;
RGBQUADFLOAT Layer5PooledOutput[LAYER5POOLEDMAPSIZE];

const int LAYER10SIZE = 30;
const int LAYER10NUMWEIGHTS = LAYER10SIZE * LAYER5POOLEDMAPSIZE;
double Layer10Neurons[LAYER10SIZE];
float Layer10Weights[LAYER10NUMWEIGHTS];
float Layer10Biases[LAYER10SIZE];
float SavedLayer10Weights[LAYER10NUMWEIGHTS];
float SavedLayer10Biases[LAYER10SIZE];

const int OUTPUTLAYERSIZE = 10;
const int OUTPUTLAYERNUMWEIGHTS = OUTPUTLAYERSIZE * LAYER10SIZE;
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
			total += Layer1Filter.OurFilter.Weights[i].blueWeight;
			total += Layer1Filter.OurFilter.Weights[i].greenWeight;
			total += Layer1Filter.OurFilter.Weights[i].redWeight;
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
		return 10;
		break;
	case 5:
		return 10;
		break;
	case 10: // first fully connected layer
		return 100;
		break;
	case 11:  // outputlayer
		return 500;
		break;
	default:
		cout << "ERROR: UNKNOWN LAYER";
		system("Pause");
		break;
	}

	exit(0);
}

unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();

/* Seed */
std::random_device rd;
/* Random number generator */
std::default_random_engine generator(seed);
/* Distribution on which to apply the generator */
std::uniform_int_distribution<long long unsigned> distribution(0, 0xFFFFFFFFFFFF);

float GetRandomNumber(double from, double to)
{
	unsigned long long range = (unsigned long long)((to - from)*100000.0);

	unsigned long long result = distribution(generator) % range;

	return (float)(((double)result / 100000.0)+from);
}

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
	else returnv.rgbBlue = 0;
	if (x.rgbGreen > 0) returnv.rgbGreen = x.rgbGreen;
	else returnv.rgbGreen = 0; 
	if (x.rgbRed > 0) returnv.rgbRed = x.rgbRed;
	else returnv.rgbRed = 0;

	returnv.reserved = 0;

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

void DoMaxPoolingToArray(RGBQUADFLOAT* inputarray, int inputwidth,
	RGBQUADFLOAT* outputarray, int outputheight)
{
	int outputwidth = (int)floor((float)inputwidth * 0.5f);
	
	for (int y = 0; y < outputheight; y++)
	{
		for (int x = 0; x < outputwidth; x++)
		{
			float redmax = Max(inputarray[(x * 2) + ((y * 2) * inputwidth)].rgbRed,
				inputarray[(x * 2) + 1 + ((y * 2) * inputwidth)].rgbRed,
				inputarray[(x * 2) + (((y * 2) + 1) * inputwidth)].rgbRed,
				inputarray[(x * 2) + 1 + (((y * 2) + 1) * inputwidth)].rgbRed);

			float bluemax = Max(inputarray[(x * 2) + ((y * 2) * inputwidth)].rgbBlue,
				inputarray[(x * 2) + 1 + ((y * 2) * inputwidth)].rgbBlue,
				inputarray[(x * 2) + (((y * 2) + 1) * inputwidth)].rgbBlue,
				inputarray[(x * 2) + 1 + (((y * 2) + 1) * inputwidth)].rgbBlue);

			float greenmax = Max(inputarray[(x * 2) + ((y * 2) * inputwidth)].rgbGreen,
				inputarray[(x * 2) + 1 + ((y * 2) * inputwidth)].rgbGreen,
				inputarray[(x * 2) + (((y * 2) + 1) * inputwidth)].rgbGreen,
				inputarray[(x * 2) + 1 + (((y * 2) + 1) * inputwidth)].rgbGreen);

			outputarray[x + (y * outputwidth)].rgbBlue = bluemax;
			outputarray[x + (y * outputwidth)].rgbGreen = greenmax;
			outputarray[x + (y * outputwidth)].rgbRed = redmax;
		}
	}
}

void CalculateCovolutionalLayer(FilterRGBGroup filtersarray, int inputheight, int inputwidth, RGBQUADFLOAT* inputarray, 
	RGBQUADFLOAT* activationmaparray, RGBQUADFLOAT* pooledoutputarray)
{
	int activationmapheight = inputheight - 2;
	int activationmapwidth = inputwidth - 2;
	int outputwidth = (int)floor((float)inputwidth * 0.5f);
	int outputheight = (int)floor((float)inputheight * 0.5f);

	if (inputheight == 299)
	{
		Sleep(1);
	}

	// calculate activation map

	for (int y = 1; y < inputheight - 1; y++) // traverse the input image
	{
		for (int x = 1; x < inputwidth - 1; x++)
		{
			float red = RectifiedLinearUnitFunction(
				filtersarray.OurFilter.Weights[0].redWeight * inputarray[(x - 1) + ((y - 1) * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[1].redWeight * inputarray[x + ((y - 1) * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[2].redWeight * inputarray[(x + 1) + ((y - 1) * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[3].redWeight * inputarray[(x - 1) + (y * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[4].redWeight * inputarray[x + (y * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[5].redWeight * inputarray[(x + 1) + (y * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[6].redWeight * inputarray[(x - 1) + ((y + 1) * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[7].redWeight * inputarray[x + ((y + 1) * inputwidth)].rgbRed +
				filtersarray.OurFilter.Weights[8].redWeight * inputarray[(x + 1) + ((y + 1) * inputwidth)].rgbRed +
				filtersarray.Bias);

			float green = RectifiedLinearUnitFunction(
				filtersarray.OurFilter.Weights[0].greenWeight * inputarray[(x - 1) + ((y - 1) * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[1].greenWeight * inputarray[x + ((y - 1) * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[2].greenWeight * inputarray[(x + 1) + ((y - 1) * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[3].greenWeight * inputarray[(x - 1) + (y * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[4].greenWeight * inputarray[x + (y * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[5].greenWeight * inputarray[(x + 1) + (y * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[6].greenWeight * inputarray[(x - 1) + ((y + 1) * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[7].greenWeight * inputarray[x + ((y + 1) * inputwidth)].rgbGreen +
				filtersarray.OurFilter.Weights[8].greenWeight * inputarray[(x + 1) + ((y + 1) * inputwidth)].rgbGreen +
				filtersarray.Bias);

			float blue = RectifiedLinearUnitFunction(
				filtersarray.OurFilter.Weights[0].blueWeight * inputarray[(x - 1) + ((y - 1) * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[1].blueWeight * inputarray[x + ((y - 1) * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[2].blueWeight * inputarray[(x + 1) + ((y - 1) * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[3].blueWeight * inputarray[(x - 1) + (y * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[4].blueWeight * inputarray[x + (y * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[5].blueWeight * inputarray[(x + 1) + (y * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[6].blueWeight * inputarray[(x - 1) + ((y + 1) * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[7].blueWeight * inputarray[x + ((y + 1) * inputwidth)].rgbBlue +
				filtersarray.OurFilter.Weights[8].blueWeight * inputarray[(x + 1) + ((y + 1) * inputwidth)].rgbBlue +
				filtersarray.Bias);

			activationmaparray[(x - 1) + (activationmapwidth * (y - 1))].rgbBlue = blue;
			activationmaparray[(x - 1) + (activationmapwidth * (y - 1))].rgbRed = red;
			activationmaparray[(x - 1) + (activationmapwidth * (y - 1))].rgbGreen = green;
		}
	}

	NormaliseArray0to255(activationmaparray, activationmapheight * activationmapwidth);

	// pool output .. half size
	DoMaxPoolingToArray(activationmaparray, activationmapwidth, pooledoutputarray, outputheight);

	NormaliseArray0to255(pooledoutputarray, outputwidth*outputheight);
}

void CalculateLayer10()
{
	for (int i = 0; i < LAYER10SIZE; i++) // for each neuron
	{
		Layer10Neurons[i] = Layer10Biases[i];

		for (int point = 0; point < LAYER5POOLEDMAPSIZE; point++) // each point (theres only one map lol)
		{
			Layer10Neurons[i] += ((double)Layer5PooledOutput[point].rgbRed * Layer10Weights[(LAYER5POOLEDMAPSIZE * i) + point]);
			Layer10Neurons[i] += ((double)Layer5PooledOutput[point].rgbBlue * Layer10Weights[(LAYER5POOLEDMAPSIZE * i) + point]);
			Layer10Neurons[i] += ((double)Layer5PooledOutput[point].rgbGreen * Layer10Weights[(LAYER5POOLEDMAPSIZE * i) + point]);
		}

		Layer10Neurons[i] = RectifiedLinearUnitFunction(Layer10Neurons[i]);
	}
}

void CalculateOutputLayer()
{
	for (int i = 0; i < OUTPUTLAYERSIZE; i++) // for each neuron
	{
		OutputNeurons[i] = OutputBiases[i];

		for (int con = 0; con < LAYER10SIZE; con++) // for each connection
		{
			OutputNeurons[i] += Layer10Neurons[con] * OutputWeights[con];
		}

		//OutputNeurons[i] = RectifiedLinearUnitFunction(OutputNeurons[i]);
	}
}

void RevertTweaks()
{
	memcpy(&Layer1Filter, &SavedLayer1Filter, sizeof(FilterRGBGroup));
	memcpy(&Layer2Filter, &SavedLayer2Filter, sizeof(FilterRGBGroup));
	memcpy(&Layer3Filter, &SavedLayer3Filter, sizeof(FilterRGBGroup));
	memcpy(&Layer4Filter, &SavedLayer4Filter, sizeof(FilterRGBGroup));
	memcpy(&Layer5Filter, &SavedLayer5Filter, sizeof(FilterRGBGroup));

	memcpy(Layer10Biases, SavedLayer10Biases, sizeof(float) * LAYER10SIZE);
	memcpy(Layer10Weights, SavedLayer10Weights, sizeof(float) * LAYER10NUMWEIGHTS);

	memcpy(OutputWeights, SavedOutputWeights, sizeof(float) * OUTPUTLAYERNUMWEIGHTS);
	memcpy(OutputBiases, SavedOutputBiases, sizeof(float) * OUTPUTLAYERSIZE);
}

void SaveNetwork()
{
	memcpy(&SavedLayer1Filter, &Layer1Filter, sizeof(FilterRGBGroup));
	memcpy(&SavedLayer2Filter, &Layer2Filter, sizeof(FilterRGBGroup));
	memcpy(&SavedLayer3Filter, &Layer3Filter, sizeof(FilterRGBGroup));
	memcpy(&SavedLayer4Filter, &Layer4Filter, sizeof(FilterRGBGroup));
	memcpy(&SavedLayer5Filter, &Layer5Filter, sizeof(FilterRGBGroup));

	memcpy(SavedLayer10Biases, Layer10Biases, sizeof(float) * LAYER10SIZE);
	memcpy(SavedLayer10Weights, Layer10Weights, sizeof(float) * LAYER10NUMWEIGHTS);

	memcpy(SavedOutputWeights, OutputWeights, sizeof(float) * OUTPUTLAYERNUMWEIGHTS);
	memcpy(SavedOutputBiases, OutputBiases, sizeof(float) * OUTPUTLAYERSIZE);
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

	for (int i = 0; i < LAYER10NUMWEIGHTS; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer10Weights[i] = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	for (int i = 0; i < OUTPUTLAYERNUMWEIGHTS; i++)
	{
		if (ShouldTweak(chanceoftweaking)) OutputWeights[i] = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	// Tweak filter weights

	for (int i = 0; i < 9; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer1Filter.OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer1Filter.OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer1Filter.OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}
	
	for (int i = 0; i < 9; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer2Filter.OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer2Filter.OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer2Filter.OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	for (int i = 0; i < 9; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer3Filter.OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer3Filter.OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer3Filter.OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	for (int i = 0; i < 9; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer4Filter.OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer4Filter.OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer4Filter.OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	for (int i = 0; i < 9; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer5Filter.OurFilter.Weights[i].blueWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer5Filter.OurFilter.Weights[i].greenWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
		if (ShouldTweak(chanceoftweaking)) Layer5Filter.OurFilter.Weights[i].redWeight = GetRandomNumber(-maxscalefactor, maxscalefactor);
	}

	float layer1maxbias = GetSuitableMaxBiasForLayer(1);
	float layer2maxbias = GetSuitableMaxBiasForLayer(2);
	float layer3maxbias = GetSuitableMaxBiasForLayer(3);
	float layer4maxbias = GetSuitableMaxBiasForLayer(4);
	float layer5maxbias = GetSuitableMaxBiasForLayer(5);

	float layer10maxbias = GetSuitableMaxBiasForLayer(10);
	float outputmaxbias = GetSuitableMaxBiasForLayer(11);

	// Tweak fully connected biases

	for (int i = 0; i < LAYER10SIZE; i++)
	{
		if (ShouldTweak(chanceoftweaking)) Layer10Biases[i] = GetRandomNumber(-layer10maxbias, layer10maxbias);
	}

	for (int i = 0; i < OUTPUTLAYERSIZE; i++)
	{
		if (ShouldTweak(chanceoftweaking)) OutputBiases[i] = GetRandomNumber(-outputmaxbias, outputmaxbias);
	}

	// Tweak filter biases;

	if (ShouldTweak(chanceoftweaking)) Layer1Filter.Bias = GetRandomNumber(-layer1maxbias, layer1maxbias);
	if (ShouldTweak(chanceoftweaking)) Layer2Filter.Bias = GetRandomNumber(-layer2maxbias, layer2maxbias);
	if (ShouldTweak(chanceoftweaking)) Layer3Filter.Bias = GetRandomNumber(-layer3maxbias, layer3maxbias);
	if (ShouldTweak(chanceoftweaking)) Layer4Filter.Bias = GetRandomNumber(-layer4maxbias, layer4maxbias);
	if (ShouldTweak(chanceoftweaking)) Layer5Filter.Bias = GetRandomNumber(-layer5maxbias, layer5maxbias);
}
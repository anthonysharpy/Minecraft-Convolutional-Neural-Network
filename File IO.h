#pragma once

#include "SimulationInfo.h"

// Checked :)

extern SimulationInfo OurSimulation;
extern void InitialiseDefaultSimulationInfo();

void SaveArraysToFile()
{
	ofstream fsimulationinfo("data/simulationinfo", ios::binary);
	ofstream flayer1filters("data/layer1filters", ios::binary);
	ofstream flayer2filters("data/layer2filters", ios::binary);
	ofstream flayer3filters("data/layer3filters", ios::binary);
	ofstream flayer4weights("data/layer4weights", ios::binary);
	ofstream flayer4biases("data/layer4biases", ios::binary);
	ofstream foutputlayerweights("data/outputlayerweights", ios::binary);
	ofstream outputbiases("data/outputbiases", ios::binary);

	fsimulationinfo.write((char*)&OurSimulation, sizeof(SimulationInfo));

	flayer1filters.write((char*)&Layer1Filters, sizeof(FilterRGBGroup) * LAYER1NUMFILTERS);
	flayer2filters.write((char*)&Layer2Filters, sizeof(FilterRGBGroup) * LAYER2NUMFILTERS);
	flayer3filters.write((char*)&Layer3Filters, sizeof(FilterRGBGroup) * LAYER3NUMFILTERS);

	flayer4weights.write((char*)&Layer4Weights, sizeof(float) * LAYER4NUMWEIGHTS);
	flayer4biases.write((char*)&Layer4Biases, sizeof(float) * LAYER4SIZE);

	foutputlayerweights.write((char*)&OutputWeights, sizeof(float) * OUTPUTLAYERNUMWEIGHTS);
	outputbiases.write((char*)&OutputBiases, sizeof(float) * OUTPUTLAYERSIZE);

	fsimulationinfo.close();
	flayer1filters.close();
	flayer2filters.close();
	flayer3filters.close();
	flayer4biases.close();
	flayer4weights.close();
	foutputlayerweights.close();
	outputbiases.close();
}

void LoadArraysFromFile()
{
	ifstream fsimulationinfo("data/simulationinfo", ios::binary);
	ifstream flayer1filters("data/layer1filters", ios::binary);	
	ifstream flayer2filters("data/layer2filters", ios::binary);
	ifstream flayer3filters("data/layer3filters", ios::binary);
	ifstream flayer4weights("data/layer4weights", ios::binary);
	ifstream foutputlayerweights("data/outputlayerweights", ios::binary);
	ifstream flayer4biases("data/layer4biases", ios::binary);
	ifstream outputbiases("data/outputbiases", ios::binary);

	if (!fsimulationinfo.is_open())
	{
		cout << "No file data. Using randomised values" << endl << endl;
		InitialiseDefaultSimulationInfo();
		return;
	}

	fsimulationinfo.read((char*)&OurSimulation, sizeof(SimulationInfo));

	flayer1filters.read((char*)&Layer1Filters, sizeof(FilterRGBGroup) * LAYER1NUMFILTERS);
	flayer2filters.read((char*)&Layer2Filters, sizeof(FilterRGBGroup) * LAYER2NUMFILTERS);
	flayer3filters.read((char*)&Layer3Filters, sizeof(FilterRGBGroup) * LAYER3NUMFILTERS);

	foutputlayerweights.read((char*)&OutputWeights, sizeof(float) * OUTPUTLAYERNUMWEIGHTS);
	outputbiases.read((char*)&OutputBiases, sizeof(float) * OUTPUTLAYERSIZE);

	flayer4weights.read((char*)&Layer4Weights, sizeof(float) * LAYER4NUMWEIGHTS);
	flayer4biases.read((char*)&Layer4Biases, sizeof(float) * LAYER4SIZE);

	fsimulationinfo.close();
	flayer1filters.close();
	flayer2filters.close();
	flayer3filters.close();
	foutputlayerweights.close();
	outputbiases.close();
	flayer4biases.close();
	flayer4weights.close();

	cout << "Read arrays from file." << endl;
}
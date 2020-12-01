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

	fsimulationinfo.write((char*)&OurSimulation, sizeof(OurSimulation));

	flayer1filters.write((char*)&Layer1Filters, sizeof(Layer1Filters));
	flayer2filters.write((char*)&Layer2Filters, sizeof(Layer2Filters));
	flayer3filters.write((char*)&Layer3Filters, sizeof(Layer3Filters));

	flayer4weights.write((char*)&Layer4Weights, sizeof(Layer4Weights));
	flayer4biases.write((char*)&Layer4Biases, sizeof(Layer4Biases));

	foutputlayerweights.write((char*)&OutputWeights, sizeof(OutputWeights));
	outputbiases.write((char*)&OutputBiases, sizeof(OutputBiases));

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

	fsimulationinfo.read((char*)&OurSimulation, sizeof(OurSimulation));

	flayer1filters.read((char*)&Layer1Filters, sizeof(Layer1Filters));
	flayer2filters.read((char*)&Layer2Filters, sizeof(Layer2Filters));
	flayer3filters.read((char*)&Layer3Filters, sizeof(Layer3Filters));

	foutputlayerweights.read((char*)&OutputWeights, sizeof(OutputWeights));
	
	outputbiases.read((char*)&OutputBiases, sizeof(OutputBiases));

	flayer4weights.read((char*)&Layer4Weights, sizeof(Layer4Weights));
	flayer4biases.read((char*)&Layer4Biases, sizeof(Layer4Biases));

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
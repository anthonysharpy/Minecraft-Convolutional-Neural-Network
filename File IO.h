#pragma once

#include "SimulationInfo.h"

// Checked :)

extern SimulationInfo OurSimulation;
extern void InitialiseDefaultSimulationInfo();

void SaveArraysToFile()
{
	ofstream fsimulationinfo("data/simulationinfo", ios::binary);
	ofstream flayer1filters("data/layer1filters", ios::binary);
	ofstream fLayer2Filter("data/Layer2Filter", ios::binary);
	ofstream fLayer3Filter("data/Layer3Filter", ios::binary);
	ofstream fLayer4Filter("data/Layer4Filter", ios::binary);
	ofstream fLayer5Filter("data/Layer5Filter", ios::binary);
	ofstream flayer10weights("data/layer10weights", ios::binary);
	ofstream flayer10biases("data/layer10biases", ios::binary);
	ofstream foutputlayerweights("data/outputlayerweights", ios::binary);
	ofstream outputbiases("data/outputbiases", ios::binary);

	fsimulationinfo.write((char*)&OurSimulation, sizeof(SimulationInfo));

	flayer1filters.write((char*)&Layer1Filter, sizeof(FilterRGBGroup));
	fLayer2Filter.write((char*)&Layer2Filter, sizeof(FilterRGBGroup));
	fLayer3Filter.write((char*)&Layer3Filter, sizeof(FilterRGBGroup));
	fLayer4Filter.write((char*)&Layer4Filter, sizeof(FilterRGBGroup));
	fLayer5Filter.write((char*)&Layer5Filter, sizeof(FilterRGBGroup));

	flayer10weights.write((char*)&Layer10Weights, sizeof(float) * LAYER10NUMWEIGHTS);
	flayer10biases.write((char*)&Layer10Biases, sizeof(float) * LAYER10SIZE);

	foutputlayerweights.write((char*)&OutputWeights, sizeof(float) * OUTPUTLAYERNUMWEIGHTS);
	outputbiases.write((char*)&OutputBiases, sizeof(float) * OUTPUTLAYERSIZE);

	fsimulationinfo.close();
	flayer1filters.close();
	fLayer2Filter.close();
	fLayer3Filter.close();
	fLayer4Filter.close();
	fLayer5Filter.close();
	flayer10biases.close();
	flayer10weights.close();
	foutputlayerweights.close();
	outputbiases.close();
}

void LoadArraysFromFile()
{
	ifstream fsimulationinfo("data/simulationinfo", ios::binary);
	ifstream flayer1filters("data/layer1filters", ios::binary);	
	ifstream fLayer2Filter("data/Layer2Filter", ios::binary);
	ifstream fLayer3Filter("data/Layer3Filter", ios::binary);
	ifstream fLayer4Filter("data/Layer4Filter", ios::binary);
	ifstream fLayer5Filter("data/Layer5Filter", ios::binary);
	ifstream flayer10weights("data/layer10weights", ios::binary);
	ifstream foutputlayerweights("data/outputlayerweights", ios::binary);
	ifstream flayer10biases("data/layer10biases", ios::binary);
	ifstream outputbiases("data/outputbiases", ios::binary);

	if (!fsimulationinfo.is_open())
	{
		cout << "No file data. Using randomised values" << endl << endl;
		InitialiseDefaultSimulationInfo();
		return;
	}

	fsimulationinfo.read((char*)&OurSimulation, sizeof(SimulationInfo));

	flayer1filters.read((char*)&Layer1Filter, sizeof(FilterRGBGroup));
	fLayer2Filter.read((char*)&Layer2Filter, sizeof(FilterRGBGroup));
	fLayer3Filter.read((char*)&Layer3Filter, sizeof(FilterRGBGroup));
	fLayer4Filter.read((char*)&Layer4Filter, sizeof(FilterRGBGroup));
	fLayer5Filter.read((char*)&Layer5Filter, sizeof(FilterRGBGroup));

	foutputlayerweights.read((char*)&OutputWeights, sizeof(float) * OUTPUTLAYERNUMWEIGHTS);
	outputbiases.read((char*)&OutputBiases, sizeof(float) * OUTPUTLAYERSIZE);

	flayer10weights.read((char*)&Layer10Weights, sizeof(float) * LAYER10NUMWEIGHTS);
	flayer10biases.read((char*)&Layer10Biases, sizeof(float) * LAYER10SIZE);

	fsimulationinfo.close();
	flayer1filters.close();
	fLayer2Filter.close();
	fLayer3Filter.close();
	fLayer4Filter.close();
	fLayer5Filter.close();
	foutputlayerweights.close();
	outputbiases.close();
	flayer10biases.close();
	flayer10weights.close();

	cout << "Read arrays from file." << endl;
}
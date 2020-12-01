#pragma once

struct RGBQUADWeights
{
	float redWeight;
	float greenWeight;
	float blueWeight;
};

class Filter
{
public:
	Filter();
	~Filter();
	RGBQUADWeights Weights[9];
};




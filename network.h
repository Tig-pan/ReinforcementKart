#pragma once

#include "layer.h"

enum Loss
{
	MeanSquaredError
};

/*
A Neural Network is composed of an array of sequential layers, where each one is feed forward from the previous.
Is capable of prediction, and training. Trains using stochastic gradient descent.
*/
class Network
{
public:
	// A constructor for the network, takes an array of Layer pointers and the size of the array
	Network(Loss loss, float learningRate, float weightDecay, Layer** layers, int layerCount);

	// Feeds the inputs forward throughout the network, and returns the final outputs
	float* predict(float* inputs);

	/* First predicts using the inputs, then calculates the error (using the expectedOutputs) from 
	the latest predictionand backprogates it through the network, updating the layer weights after all gradients are calculated.

	Then returns the loss of the training. */
	float train(float* inputs, float* expectedOutputs);

private:
	Layer** layers;
	int layerCount;

	Loss loss;
	float learningRate;
	float weightDecay;
};
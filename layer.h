#pragma once

#include <random>
#include <iostream>

#include "activation.h"

/*
A Layer in a Neural Network. Allows for feeding forwarding, and backpropogation.
*/
class Layer
{
public:
	// Takes the input and feeds it through the layer to update the output nodes
	virtual void feedForward(float* inputs) = 0;

	// Backpropogates the error throughout the layer and updates the gradients
	virtual void backpropogate(float* inputs) = 0;

	// Updates the error for the input layer using the current error and weights stored in this layer
	virtual void calculateErrorFrom(float* inputError) = 0;

	// Applies the current gradients, uses the given learningRate and weightDecay
	virtual void apply(float learningRate, float weightDecay) = 0;

	float* nodes;
	float* error;

	int inputNodes;
	int outputNodes;
};

/*
A Dense layer. Composed of a line of input nodes and a line of output nodes, and a weight for each connection between an input and an output node.
*/
class DenseLayer : public Layer
{
public:
	DenseLayer(int inputNodes, int outputNodes, Activation* activation);

	void feedForward(float* inputs) override;
	void backpropogate(float* inputs) override;
	void calculateErrorFrom(float* inputError) override;
	void apply(float learningRate, float weightDecay) override;

protected:
	Activation* activation;

	float* weights;
	float* biases;

	float* weightGradients;
	float* biasGradients;
};
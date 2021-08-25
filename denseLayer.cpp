#include "layer.h"

DenseLayer::DenseLayer(int inputNodes, int outputNodes, Activation* activation)
	: activation(activation)
{
	Layer::inputNodes = inputNodes;
	Layer::outputNodes = outputNodes;

	nodes = new float[outputNodes];
	error = new float[outputNodes];

	// Initilize the network parameters using Xavier initilization
	std::default_random_engine generator;
	std::normal_distribution<float> distribution(0.0f, 1.0f / (float)inputNodes); // mean of 0, variance of initVariance

	weights = new float[inputNodes * outputNodes];
	biases = new float[outputNodes];

	weightGradients = new float[inputNodes * outputNodes];
	biasGradients = new float[outputNodes];

	for (int i = 0; i < inputNodes * outputNodes; i++)
	{
		weights[i] = distribution(generator);
		weightGradients[i] = 0.0f;
	}

	for (int i = 0; i < outputNodes; i++)
	{
		biases[i] = 0.0f;
		biasGradients[i] = 0.0f;
	}
}

void DenseLayer::feedForward(float* inputs)
{
	for (int i = 0; i < outputNodes; i++)
	{
		nodes[i] = biases[i];
		for (int j = 0; j < inputNodes; j++)
		{
			nodes[i] += inputs[j] * weights[i * inputNodes + j];
		}
	}
	activation->activate(nodes, outputNodes);
}

void DenseLayer::backpropogate(float* inputs)
{
	// Error needs to be set properly prior to this being called
	activation->applyDerivative(error, nodes, outputNodes);
	for (int i = 0; i < outputNodes; i++)
	{
		biasGradients[i] += error[i];

		for (int j = 0; j < inputNodes; j++)
		{
			weightGradients[i * inputNodes + j] += inputs[j] * error[i];
		}
	}
}

void DenseLayer::calculateErrorFrom(float* inputError)
{
	for (int j = 0; j < inputNodes; j++)
	{
		float total = 0.0f;
		for (int i = 0; i < outputNodes; ++i)
		{
			total += weights[i * inputNodes + j] * error[i];
		}
		inputError[j] = total;
	}
}

void DenseLayer::apply(float learningRate, float weightDecay)
{
	for (int i = 0; i < inputNodes * outputNodes; i++)
	{
		weights[i] += weightGradients[i] * learningRate - 2.0f * weights[i] * weightDecay;
		weightGradients[i] = 0.0f;
	}

	for (int i = 0; i < outputNodes; i++)
	{
		biases[i] += biasGradients[i] * learningRate - 2.0f * biases[i] * weightDecay;
		biasGradients[i] = 0.0f;
	}
}

#include "network.h"

Network::Network(Loss loss, float learningRate, float weightDecay, Layer** layers, int layerCount)
	: layers(layers)
	, layerCount(layerCount)

	, loss(loss)
	, learningRate(learningRate)
	, weightDecay(weightDecay)
{

}

float* Network::predict(float* inputs)
{
	layers[0]->feedForward(inputs);
	for (int i = 1; i < layerCount; i++)
	{
		layers[i]->feedForward(layers[i - 1]->nodes);
	}
	return layers[layerCount - 1]->nodes;
}

float Network::train(float* inputs, float* expectedOutputs)
{
	predict(inputs);

	Layer* lastLayer = layers[layerCount - 1];
	float batchLoss = 0.0f; // weight decay is not included in the loss to save computation time
	if (loss == Loss::MeanSquaredError)
	{
		for (int i = 0; i < lastLayer->outputNodes; i++)
		{
			if (std::isnan(expectedOutputs[i])) // if it is nan, then assume the error is 0. this is used to only train for certain outputs.
			{
				lastLayer->error[i] = 0.0f;
			}
			else
			{
				lastLayer->error[i] = expectedOutputs[i] - lastLayer->nodes[i];
			}
			batchLoss += lastLayer->error[i] * lastLayer->error[i];
		}
		batchLoss /= (float)lastLayer->outputNodes;
	}

	if (layerCount == 1) // if there is only one layer, then the last layers inputs are the network inputs
	{
		lastLayer->backpropogate(inputs);
	}
	else
	{
		lastLayer->backpropogate(layers[layerCount - 2]->nodes);
	}

	for (int i = layerCount - 2; i >= 0; i--)
	{
		layers[i + 1]->calculateErrorFrom(layers[i]->error); // must calcualte the error of the layer before backpropogating
		if (i == 0) // if this is the last layer, then its inputs are the network inputs
		{
			layers[i]->backpropogate(inputs);
		}
		else
		{
			layers[i]->backpropogate(layers[i - 1]->nodes);
		}
	}

	return batchLoss;
}

void Network::apply()
{
	for (int i = 0; i < layerCount; i++)
	{
		layers[i]->apply(learningRate, weightDecay);
	}
}

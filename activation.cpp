#include "activation.h"

void Sigmoid::activate(float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		nodes[i] = 1.0f / (1.0f + expf(-nodes[i]));
	}
}

void Sigmoid::applyDerivative(float* error, float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		error[i] *= nodes[i] * (1.0f - nodes[i]);
	}
}

void TanH::activate(float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		nodes[i] = tanhf(nodes[i]);
	}
}

void TanH::applyDerivative(float* error, float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		error[i] *= 1.0f - nodes[i] * nodes[i];
	}
}

void ReLU::activate(float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		nodes[i] = nodes[i] > 0.0f ? nodes[i] : 0.0f;
	}
}

void ReLU::applyDerivative(float* error, float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		error[i] *= nodes[i] = nodes[i] > 0.0f ? 1.0f : 0.0f;
	}
}

void LeakyReLU::activate(float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		nodes[i] = nodes[i] > 0.0f ? nodes[i] : a * nodes[i];
	}
}

void LeakyReLU::applyDerivative(float* error, float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		error[i] *= nodes[i] = nodes[i] > 0.0f ? 1.0f : a;
	}
}

void Softmax::activate(float* nodes, int size)
{
	float sum = 0.0f;
	for (int i = 0; i < size; i++)
	{
		nodes[i] = expf(nodes[i]);
		sum += nodes[i];
	}

	for (int i = 0; i < size; i++)
	{
		nodes[i] /= sum;
	}
}

void Softmax::applyDerivative(float* error, float* nodes, int size)
{
	for (int i = 0; i < size; i++)
	{
		error[i] *= (1.0f - nodes[i]) * nodes[i];
	}
}
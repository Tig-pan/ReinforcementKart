#pragma once

#include <math.h>

/*
Designates an Activation function to be used in a neural network
*/
class Activation
{
public:
	// Takes the given node array, and performs the activation function on each element
	virtual void activate(float* nodes, int size) = 0;

	// Takes the given error and multiplies it by the derivative of the node for each element of the array
	virtual void applyDerivative(float* error, float* nodes, int size) = 0;
};

// A Sigmoid activation function { Range: (0,1), 0 -> 0.5 }
class Sigmoid : public Activation
{
public:
	Sigmoid() {}

	void activate(float* nodes, int size) override;
	void applyDerivative(float* error, float* nodes, int size) override;
};

// A TanH activation function { Range: (-1,1), 0 -> 0 }
class TanH : public Activation
{
public:
	TanH() {}

	void activate(float* nodes, int size) override;
	void applyDerivative(float* error, float* nodes, int size) override;
};

// A Rectified Linear Unit activation function { Range: [0,inf), 0 -> 0 }
class ReLU : public Activation
{
public:
	ReLU() {}

	void activate(float* nodes, int size) override;
	void applyDerivative(float* error, float* nodes, int size) override;
};

// A Leaky Rectified Linear Unit activation function { Range: (-inf,inf), 0 -> 0 }
class LeakyReLU : public Activation
{
public:
	LeakyReLU(float a = 0.01) : a(a) {}

	void activate(float* nodes, int size) override;
	void applyDerivative(float* error, float* nodes, int size) override;
private:
	float a;
};

// A Softmax activation function { Range: (0, 1), 0 -> ? (this function is special as the activations rely on the entire set, not just individual nodes) }
class Softmax : public Activation
{
public:
	Softmax() {}

	void activate(float* nodes, int size) override;
	void applyDerivative(float* error, float* nodes, int size) override;
};

// A Linear activation function { Range: (-inf, inf), 0 -> 0 (this function has 0 effect, it is y = x) }
class Linear : public Activation
{
public:
	Linear() {}

	void activate(float* nodes, int size) override {}
	void applyDerivative(float* error, float* nodes, int size) override {}
};
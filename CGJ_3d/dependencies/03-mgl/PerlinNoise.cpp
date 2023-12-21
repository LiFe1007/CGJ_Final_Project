#include "PerlinNoise.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

// THIS IS A DIRECT TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

// Initialize with the reference values for the permutation vector
PerlinNoise::PerlinNoise() {}

float PerlinNoise::noise1(int x, int y)
{
	int n;
	n = x + y * 57;
	x = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);

}
float PerlinNoise::interpol(float a, float b, float x)
{
	float ft, f;
	ft = x * 3.1415927;
	f = (1 - cos(ft)) * .5;
	return  a * (1 - f) + b * f;
}
float PerlinNoise::SmoothedNoise1(float x, float y)
{
	float corners, sides, center;
	corners = (noise1(x - 1, y - 1) + noise1(x + 1, y - 1) + noise1(x - 1, y + 1) + noise1(x + 1, y + 1)) / 16;
	sides = (noise1(x - 1, y) + noise1(x + 1, y) + noise1(x, y - 1) + noise1(x, y + 1)) / 8;
	center = noise1(x, y) / 4;
	return corners + sides + center;
}

float PerlinNoise::interpolatedNoise(float x, float y)
{
	float fractional_X, fractional_Y, v1, v2, v3, v4, i1, i2;
	int integer_X, integer_Y;

	integer_X = (int)x;
	fractional_X = x - integer_X;

	integer_Y = (int)y;
	fractional_Y = y - integer_Y;

	v1 = SmoothedNoise1(integer_X, integer_Y);
	v2 = SmoothedNoise1(integer_X + 1, integer_Y);
	v3 = SmoothedNoise1(integer_X, integer_Y + 1);
	v4 = SmoothedNoise1(integer_X + 1, integer_Y + 1);

	i1 = interpol(v1, v2, fractional_X);
	i2 = interpol(v3, v4, fractional_X);

	return interpol(i1, i2, fractional_Y);

}

float PerlinNoise::perlin(float x, float y)
{
	float total, p, frequency, amplitude;
	int i, n;
	total = 0.0;
	p = 1.0;
	n = 8;


	for (i = 0; i <= n;i++)
	{
		frequency = powf(5.0, (float)i);
		amplitude = p / (pow(1.7, i));
		total = total + interpolatedNoise(x * frequency / 20, y * frequency / 20) * amplitude;
	}
	return total;
}
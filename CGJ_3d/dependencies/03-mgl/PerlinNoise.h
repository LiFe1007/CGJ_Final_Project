#include <vector>

#ifndef PERLINNOISE_H
#define PERLINNOISE_H

class PerlinNoise {
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise();
	// Generate a new permutation vector based on the value of seed
	float perlin(float x, float y);
private:
	float interpolatedNoise(float x, float y);
	float SmoothedNoise1(float x, float y);
	float interpol(float a, float b, float x);
	float noise1(int x, int y);
};

#endif

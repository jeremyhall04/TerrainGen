#ifndef PERLIN_H
#define PERLIN_H

#include <random>
#include <algorithm>

namespace noise {

	/*
			utility methods
	*/

	// fade function: f(t) = 6*t^5 - 15*t^4 + 10*t^3 (optimize for fewer multiplications)
	inline float fade(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	// linear interpolation
	inline float lerp(float t, float a, float b) {
		return a + t * (b - a);
	}

	// precise linear interpolation
	inline float lerp_precise(float t, float a, float b) {
		return (1 - t) * a + t * b;
	}

	// map function
	inline float map(float val, float ogMin, float ogMax, float newMin, float newMax) {
		// get proportion in original range
		float prop = (val - ogMin) / (ogMax - ogMin);

		return lerp(prop, newMin, newMax);
	}

	// gradient function
	// calculate the dot product between the gradient vector and distance vector
	inline float grad(unsigned char hash, float x, float y, float z) {
		// convert the last 4 bits of the hash into one of 12 possible gradients
		int h = hash & 0b1111; // = hash & 15 = hash % 16 = get last 4 bits

		// if first bit 1, set to x, otherwise set to y
		float u = h < 0b1000 ? x : y;

		// if first/second bits 0, set to y
		// if first/second bits 1, set to x
		// else set to z
		float v = h < 0b0100 ? y : h == 0b1100 || h == 0b1110 ? x : z;

		// make u, v negative based on the last 2 bits, then add results
		// adding results is like dot product because gradient components are 1,
		// so results of dot product is adding distance components
		return ((h & 0b0001) == 0 ? u : -u) + ((h & 0b0010) == 0 ? v : -v);
	}


	/*
		Perlin Noise
	*/

	class Perlin
	{
	private:
		unsigned char p[512];	// permutation table
		unsigned int seed;		// current seed

	public:
		Perlin();
		Perlin(unsigned int seed);
		~Perlin();

		void resetSeed(unsigned int seed);
		
		// Perlin
		float noise1D(float x);
		float noise2D(float x, float y);
		float noise3D(float x, float y, float z);

		// Fractal Brownian Motion
		float accumulatedNoise1D(float x, int octaves, float lacunarity, float gain);
		float accumulatedNoise2D(float x, float y, int octaves, float lacunarity, float gain);
		float accumulatedNoise3D(float x, float y, float z, int octaves, float lacunarity, float gain);
	};
};

#endif

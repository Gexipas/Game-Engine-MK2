#pragma once

#include<math.h>
#include<iostream>
#include<vector>

class Perlin
{
	/*-------------------Singleton----------------------*/
public:
	static Perlin& GetInstance()
	{
		static Perlin instance;
		return instance;
	}
	Perlin(Perlin const&) = delete;
	void operator= (Perlin const&) = delete;

private:
	Perlin() {};
	~Perlin() {};
	/*--------------------------------------------------*/

public:
	std::vector<std::vector<float>> createNoise(int lengthX, int lengthY, int _numOctaves = 10, float _persistence = 1.1f);

private:
	float Random(int x, int y);
	float Smooth(int x, int y);
	float Interpolate(float a, float b, float x);
	float InterpolatedNoise(float x, float y);
	float Noise2D(int x, int y);

	int numOctaves = 10;
	float persistence = 1.1f;
};

inline std::vector<std::vector<float>> Perlin::createNoise(int lengthX, int lengthY, int _numOctaves, float _persistence)
{
	numOctaves = _numOctaves;
	persistence = _persistence;
	std::vector<std::vector<float>> tempVec;
	for (int y = 0; y < lengthY; ++y)
	{
		std::vector<float> temp;
		for (int x = 0; x < lengthX; ++x)
		{
			temp.push_back(Noise2D(x, y));
		}
		tempVec.push_back(temp);
	}
	return tempVec;
}

inline float Perlin::Random(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0f - float(t) * 0.931322574615478515625e-9f;
}

inline float Perlin::Smooth(int x, int y)
{
	float corners;
	float sides;
	float center;
	corners = (Random(x - 1, y - 1) + Random(x + 1, y - 1) + Random(x - 1, y + 1) + Random(x + 1, y + 1)) / 16;
	sides = (Random(x - 1, y) + Random(x + 1, y) + Random(x, y - 1) + Random(x, y + 1)) / 8;
	center = Random(x, y) / 4;
	return corners + sides + center;
}

inline float Perlin::Interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927f;
	float f = (1 - cosf(ft)) * 0.5f;
	return  a * (1 - f) + b * f;
}

inline float Perlin::InterpolatedNoise(float x, float y)
{
	int intX = (int)x;
	float fracX = x - intX;
	int intY = (int)y;
	float fracY = y - intY;

	float v1 = Smooth(intX, intY);
	float v2 = Smooth(intX + 1, intY);
	float v3 = Smooth(intX, intY + 1);
	float v4 = Smooth(intX + 1, intY + 1);
	float i1 = Interpolate(v1, v2, fracX);
	float i2 = Interpolate(v3, v4, fracX);
	return Interpolate(i1, i2, fracY);
}

inline float Perlin::Noise2D(int x, int y)
{
	float total = 0;
	float frequency = powf(2, (float)numOctaves);
	float amplitude = 1;
	for (int i = 0; i < numOctaves; ++i) {
		frequency /= 2;
		amplitude *= persistence;
		total += InterpolatedNoise(x * frequency, y * frequency) * amplitude;
	}
	return total;
}
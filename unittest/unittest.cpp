#include <climits>

#include "gtest/include/gtest/gtest.h"
#include "bluemars.h"
#include "simpleAdditionLayer.h"
#include "perlin.h"
#include "miniz.c"
#include "elevationlayer.h"

// Protected to public for unit testing
// Perlin Noise
class NoisePublic : public Noise
{
public:
	NoisePublic(long pb_seed) : Noise(pb_seed) {}
	long discreteNoise(long x, long y, int octave) { return Noise::discreteNoise(x, y, octave); }
	double interpolatedNoise(long x, long y, int waveLength = BASE_WAVE_LENGTH) { return Noise::interpolatedNoise(x, y, waveLength); }
	double cosineInterpolate(long X1, long X2, double Z1, double Z2, long x) { return Noise::cosineInterpolate(X1, X2, Z1, Z2, x); }
};

// Perlin Noise Testing

TEST(PerlinTest, murmurHash_determisn)
{
	EXPECT_EQ(murmurHash2(5), murmurHash2(5)); // Check if we always have the same output
}
TEST(PerlinTest, murmurHash_uniformDistribution)
{
	std::vector<int> distribution = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (long i = 0; i < 10000; i++)
	{
		long result = murmurHash2(i) % 10;
		distribution.at(result) = distribution.at(result) + 1;
	}
	int minExpected = (int) ((10000 / 10) * 0.8); // Expected in 80%-120% range
	int maxExpected = (int) ((10000 / 10) * 1.2);
	for (int i = 0; i < 10; i++)
	{
		EXPECT_GE(distribution.at(i), minExpected);
		EXPECT_LE(distribution.at(i), maxExpected);
	}
}
TEST(PerlinTest, murmurHash_Randomness)
{
	long nb_floats = 1024*1024;
	const double BUFSIZE = (double)nb_floats*sizeof(long);
	unsigned char *unComp = (unsigned char *)malloc((size_t)BUFSIZE);
	//unsigned char unComp[BUFSIZE];
	long result;
	for (long i = 0; i<nb_floats; ++i)
	{
		result = murmurHash2(i);
		memcpy(unComp + i*sizeof(long), &result, sizeof(long));
	}
	unsigned char* comp = (unsigned char *)malloc((long)(1.1*BUFSIZE));
	unsigned long cmp_len = compressBound((long)(BUFSIZE*1.1));
	compress2(comp, &cmp_len, (const unsigned char *)unComp, (mz_ulong)BUFSIZE, 9);
	ASSERT_GT((float)cmp_len / BUFSIZE, 0.9);
}

TEST(PerlinTest, discreteNoise_determinism)
{
	NoisePublic noise(1);
	EXPECT_EQ(noise.discreteNoise(5, 10, 4), noise.discreteNoise(5, 10, 4));
}
TEST(PerlinTest, discreteNoise_Randomness)
{
	NoisePublic noise(456132);
	long nb_floats = 1000 * 1000;
	const long BUFSIZE = (long)nb_floats*sizeof(long);
	unsigned char *unComp = (unsigned char *)malloc((size_t)BUFSIZE);
	long result;
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			result = noise.discreteNoise(i, j, 1);
			memcpy(unComp + (i * 1000 + j)*sizeof(long), &result, sizeof(long));
		}
	}
	unsigned char* comp = (unsigned char *)malloc((long)(1.1*(double)BUFSIZE));
	unsigned long cmp_len = compressBound((long)((double)BUFSIZE*1.1));
	compress2(comp, &cmp_len, (const unsigned char *)unComp, (mz_ulong)BUFSIZE, 9);
	double maxPrecision = (double)7 / 32 * 0.75; // Because we modulo by amplitude, result is only random on 7 bits out of the 32 of a long, we have also loss of information from aggregating x,y,seed to long, so we accept a loss of 75%
	ASSERT_GT((float)cmp_len / (float)BUFSIZE, maxPrecision);
}

TEST(PerlinTest, octaveTest)
{
	NoisePublic noise(1);
	// First compute sum of both octave
	long sumOct1 = 0, sumOct4 = 0;
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			sumOct1 += noise.discreteNoise(i, j, 1);
			sumOct4 += noise.discreteNoise(i, j, 4);
		}
	}
	sumOct1 = sumOct1 / (1000 * 1000);
	sumOct4 = sumOct4 / (1000 * 1000);
	ASSERT_GT(sumOct1, sumOct4);
	// Then compute standard deviation
	long sdOct1 = 0, sdOct4 = 0;
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			sdOct1 += (long)pow((double)(noise.discreteNoise(i, j, 1) - sumOct1), 2);
			sdOct4 += (long)pow((double)(noise.discreteNoise(i, j, 4) - sumOct4), 2);
		}
	}
	sdOct1 = sdOct1 / (1000 * 1000);
	sdOct4 = sdOct4 / (1000 * 1000);
	ASSERT_GT(sdOct1, sdOct4);
}

TEST(PerlinTest, cosineInterpolate_at_boundaries)
{
	NoisePublic noise(1);
	long x1 = 5;
	long x2 = 768;
	double z1 = 0.8;
	double z2 = 0.3;
	ASSERT_EQ(noise.cosineInterpolate(x1, x2, z1, z2, x1), z1);
	ASSERT_EQ(noise.cosineInterpolate(x1, x2, z1, z2, x2), z2);
}

TEST(PerlinTest, cosineInterpolate_between_boundaries)
{
	NoisePublic noise(1);
	long x1 = 5;
	long x2 = 768;
	long x = 240;
	double z1 = 0.8;
	double z2 = 0.3;
	double z = noise.cosineInterpolate(x1, x2, z1, z2, x);
	ASSERT_TRUE(z2 <= z && z <= z1);
}

TEST(PerlinTest, cosineInterpolate_monotony)
{
	NoisePublic noise(1);
	long x1 = 5;
	long x2 = 705;
	long deltaX = (x2 - x1) / 100;
	double z1 = 0.24;
	double z2 = 0.68;
	double z = 0;
	double zPrime = 0;
	bool monotone = z2 >= z1;
	if (z2 >= z1) //we want the interpolate function to be increasing
	{
		for (long x = x1; x <= x2; x += deltaX)
		{
			z = noise.cosineInterpolate(x1, x2, z1, z2, x);
			zPrime = noise.cosineInterpolate(x1, x2, z1, z2, x + deltaX);
			monotone = monotone && (zPrime - z >= 0); //if at least one "zPrime-z" < 0, result switches to false
		}
	}
	else //we want the interpolate function to be decreasing
	{
		for (long x = x1; x <= x2; x += deltaX)
		{
			z = noise.cosineInterpolate(x1, x2, z1, z2, x);
			zPrime = noise.cosineInterpolate(x1, x2, z1, z2, x + deltaX);
			monotone = monotone && (zPrime - z <= 0);
		}
	}
}

TEST(PerlinTest, Noise_seedTtest)
{
	Noise noise1(1);
	Noise noise2(1);
	Noise noise3(2);

	ASSERT_EQ(noise1.outputValue(384, 117, 8), noise2.outputValue(384, 117, 8));
	ASSERT_EQ(noise1.outputValue(1000, 1000, 8), noise2.outputValue(1000, 1000, 8));
	ASSERT_EQ(noise1.outputValue(-100, -2000, 8), noise2.outputValue(-100, -2000, 8));
	ASSERT_EQ(noise1.outputValue(-1000, -1000, 8), noise2.outputValue(-1000, -1000, 8));
	ASSERT_NE(noise1.outputValue(5, 10, 8), noise3.outputValue(5, 10, 8));
}

TEST(BlueMarsTest, KnownCoord)
{
	BlueMarsMap map;
	map.addLayer("simpleAdditionLayer", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
	float value = map.get("simpleAdditionLayer", 0, 0);
	ASSERT_EQ(value,0); // 0+0
	value = map.get("simpleAdditionLayer", 10, 10);
	ASSERT_EQ(value,20); //10+10
	value = map.get("simpleAdditionLayer", 2000, 2000);
	ASSERT_EQ(value,1952);
	value = map.get("simpleAdditionLayer",1000,2000);
	ASSERT_EQ(value,1976);
	value = map.get("simpleAdditionLayer", -10, 5);
	ASSERT_EQ(value, 1019);
	value = map.get("simpleAdditionLayer", -10, -10);
	ASSERT_EQ(value, 2028);
	value = map.get("simpleAdditionLayer", -2000, -2000);
	ASSERT_EQ(value,96);
	value = map.get("simpleAdditionLayer", 2000, -2000);
	ASSERT_EQ(value,1024);
}

TEST(BlueMarsTest, NBChunks)
{
	BlueMarsMap map;
	map.addLayer("simpleAdditionLayer", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
	map.addLayer("simpleAdditionLayer2", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
	ASSERT_EQ(map.nbChunks(),0);
	map.get("simpleAdditionLayer",0,0);
	map.get("simpleAdditionLayer2",0,0);
	ASSERT_EQ(map.nbChunks(),2);
	map.get("simpleAdditionLayer",1000,1000);
	ASSERT_EQ(map.nbChunks(),2);
	map.get("simpleAdditionLayer",2000,2000);
	ASSERT_EQ(map.nbChunks(),3);
	map.get("simpleAdditionLayer", -10, -10);
	ASSERT_EQ(map.nbChunks(), 4);
	map.get("simpleAdditionLayer", -2000, -2000);
	ASSERT_EQ(map.nbChunks(), 5);
	map.get("simpleAdditionLayer", -2000, 2000);
	ASSERT_EQ(map.nbChunks(), 6);
	map.get("simpleAdditionLayer", -1000, -1000);
}

// TEST(ElevationLayerTest, Randomness)
// {
// 	BlueMarsMap map;
// 	map.addLayer("elevation", std::make_shared<elevation> (elevation()));
// 	//float value = map.get("elevation", 0, 0);
// 	long nb_floats = 1024*1024;
// 	const double BUFSIZE = (double)nb_floats*sizeof(float);
// 	unsigned char *unComp = (unsigned char *)malloc((size_t)BUFSIZE);
// 	float value;
// 	for(long x; x<1024; ++x)
// 		{
// 			for(long y; y<1024; ++y)
// 				{
// 					value = map.get("elevation", x, y);
// 					memcpy(unComp + x*1024+y*sizeof(float), &value, sizeof(float));
// 				}
// 		}
// 	unsigned char* comp = (unsigned char *)malloc((long)(1.1*BUFSIZE));
// 	unsigned long cmp_len = compressBound((long)(BUFSIZE*1.1));
// 	compress2(comp, &cmp_len, (const unsigned char *)unComp, (mz_ulong)BUFSIZE, 9);
// 	ASSERT_GT((float)cmp_len / BUFSIZE, 2);
// }
TEST(ElevationTest, Determinist)
{
	BlueMarsMap map;
	map.addLayer("elevation", std::make_shared<ElevationLayer>());
	EXPECT_EQ(map.get("elevation",0,0), map.get("elevation",0,0));
	EXPECT_EQ(map.get("elevation",100,100), map.get("elevation",100,100));
	EXPECT_EQ(map.get("elevation",-2048,2048), map.get("elevation",-2048,2048));
}

TEST(ElevationTest, MaxValue)
{
	BlueMarsMap map;
	map.addLayer("elevation", std::make_shared<ElevationLayer>());
	float maxValue = 0;
	float value = 0;
	for(long x=0; x<1024; ++x)
			{

				for(long y=0; y<1024; ++y)
					{
						value = map.get("elevation", x, y);
						maxValue = (maxValue<value)?value:maxValue ;
					}
			}
	ASSERT_LE(maxValue,1);
}

TEST(ElevationTest, MinValue)
{
	BlueMarsMap map;
	map.addLayer("elevation", std::make_shared<ElevationLayer>());
	float minValue = 0;
	float value = 0;
	for(long x=0; x<1024; ++x)
			{

				for(long y=0; y<1024; ++y)
					{
						value = map.get("elevation", x, y);
						minValue = (minValue>value)?value:minValue ;
					}
			}
	ASSERT_GE(minValue,0);
}

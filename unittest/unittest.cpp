#include <climits>

#include "gtest/include/gtest/gtest.h"
#include "bluemars.h"
#include "simpleAdditionLayer.h"
#include "perlin.h"
#include "miniz.c"

// Protected to public for unit testing
// Perlin Noise
class NoisePublic : public Noise
{
public:
	NoisePublic(long seed) : Noise(seed) {}
	double discreteNoise(long x, long y, int octave) { return Noise::discreteNoise(x, y, octave); }
	double interpolatedNoise(long x, long y, long waveLength = BASE_WAVE_LENGTH) { return Noise::interpolatedNoise(x, y, waveLength); }
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
// No pattern test

TEST(PerlinTest, discreteNoise_determinism)
{
	NoisePublic noise(1);
	EXPECT_EQ(noise.discreteNoise(5, 10, 4), noise.discreteNoise(5, 10, 4));
}

// No pattern test

TEST(PerlinTest, octaveTest)
{
	NoisePublic noise(1);
	// First compute sum of both octave
	double sumOct1 = 0, sumOct4 = 0;
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
	double sdOct1 = 0, sdOct4 = 0;
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			sdOct1 += pow(noise.discreteNoise(i, j, 1) - sumOct1, 2);
			sdOct4 += pow(noise.discreteNoise(i, j, 4) - sumOct4, 2);
		}
	}
	sdOct1 = sdOct1 / (1000 * 1000);
	sdOct4 = sdOct4 / (1000 * 1000);
	ASSERT_GT(sdOct1, sdOct4);
}

TEST(PerlinTest, Noise_seedTtest)
{
	Noise noise1(1);
	Noise noise2(1);
	Noise noise3(2);

	ASSERT_EQ(noise1.outputValue(5, 10, 8), noise2.outputValue(5, 10, 8));
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

TEST(PerlinTest, DeterministicNoise)
{
    Noise noise1(1);
    Noise noise2(1);
    long X = 384;
    long Y = 117;
    double noise1ForXY = noise1.outputValue(X,Y,5);
    double noise2ForXY = noise2.outputValue(X,Y,5);
    ASSERT_EQ(noise1ForXY, noise2ForXY);
	  X=1000;
    Y=1000;
    noise1ForXY = noise1.outputValue(X,Y,5);
    noise2ForXY = noise2.outputValue(X,Y,5);
    ASSERT_EQ(noise1ForXY, noise2ForXY);
    X=-100;
    Y=-2000;
    noise1ForXY = noise1.outputValue(X,Y,5);
    noise2ForXY = noise2.outputValue(X,Y,5);
    ASSERT_EQ(noise1ForXY, noise2ForXY);
    X=-1000;
    Y=-1000;
    noise1ForXY = noise1.outputValue(X,Y,5);
    noise2ForXY = noise2.outputValue(X,Y,5);
    ASSERT_EQ(noise1ForXY, noise2ForXY);

}

TEST(PerlinTest, cosineInterpolate_at_boundaries)
{
    NoisePublic noise(1);
    long x1 = 5;
    long x2 = 768;
    double z1 = 0.8;
    double z2 = 0.3;
    ASSERT_EQ(noise.cosineInterpolate(x1,x2,z1,z2,x1), z1);
    ASSERT_EQ(noise.cosineInterpolate(x1,x2,z1,z2,x2), z2);
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
    ASSERT_TRUE(z2<=z && z<=z1);
}

TEST(PerlinTest, consineInterpolate_monotony)
{
    NoisePublic noise(1);
    long x1 = 5;
    long x2 = 705;
    long deltaX = (x2-x1)/100;
    double z1 = 0.24;
    double z2 = 0.68;
    double z = 0;
    double zPrime = 0;
    bool monotone = z2>=z1;
    if (z2 >= z1) //we want the interpolate function to be increasing
    {
        for (long x=x1; x<=x2; x+=deltaX)
        {
            z = noise.cosineInterpolate(x1, x2, z1, z2, x);
            zPrime = noise.cosineInterpolate(x1, x2, z1, z2, x+deltaX);
            monotone = monotone && (zPrime - z >= 0); //if at least one "zPrime-z" < 0, result switches to false
        }
    }
    else //we want the interpolate function to be decreasing
    {
        for (long x=x1; x<=x2; x+=deltaX)
        {
            z = noise.cosineInterpolate(x1, x2, z1, z2, x);
            zPrime = noise.cosineInterpolate(x1, x2, z1, z2, x+deltaX);
            monotone = monotone && (zPrime - z <= 0);
        }
    }
}

TEST(PerlinTest, Murmur_Randomness)
{
		long nb_floats = 5048576;
		const double BUFSIZE = (double)nb_floats*sizeof(long);
		unsigned char *unComp = (unsigned char *)malloc((size_t)BUFSIZE);
		//unsigned char unComp[BUFSIZE];
		long result;
		for(long i = 0; i<nb_floats; ++i)
			{
				result = murmurHash2(i);
				memcpy(unComp + i*sizeof(long), &result, sizeof(long));
			}
		unsigned char* comp = (unsigned char *)malloc((long)(1.1*BUFSIZE));
		unsigned long cmp_len = compressBound((long)(BUFSIZE*1.1));
		compress2(comp, &cmp_len, (const unsigned char *)unComp, (mz_ulong)BUFSIZE, 9);
		ASSERT_GT((float)cmp_len/BUFSIZE, 0.9 );

}

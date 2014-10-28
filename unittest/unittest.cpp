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
		const unsigned long BUFSIZE = nb_floats*sizeof(long);
		unsigned char *unComp = (unsigned char *)malloc(BUFSIZE);
		//unsigned char unComp[BUFSIZE];
		long result;
		for(long i = 0; i<nb_floats; ++i)
			{
				result = murmurHash2(i);
				memcpy(unComp + i*sizeof(long), &result, sizeof(long));
			}
		unsigned char* comp = (unsigned char *)malloc((long)(1.1*BUFSIZE));
		int cmp_status = 1;
		unsigned long cmp_len = compressBound((long)(BUFSIZE*1.1));
		cmp_status = compress2(comp, &cmp_len, (const unsigned char *)unComp, BUFSIZE, 9);
		ASSERT_GT((float)cmp_len/BUFSIZE, 0.9 );

}

#ifndef INCLUDE_PERLIN
#define INCLUDE_PERLIN

#include <random>

#define BASE_WAVE_LENGTH 128
#define BASE_AMPLITUDE 256
#define PERSISTENCE 0.5 //coeff par lequel on multiplie l'amplitude à chaque octave (à chaque fois que la frequence double)
#define MAX_SEED 1317624576693539401 // This is long_max / 7

long murmurHash2(long seed);

class Noise
{
public:
    Noise(long seed);
	double outputValue(long x, long y, int octaves);
    void generateOutputFile(long size, int numberOfOctaves); // ToDo - Remove, debug only

protected:
    long seed;
	//std::mt19937 rng; // Random number generator
    long discreteNoise(long x, long y, int octave);
    double interpolatedNoise(long x, long y, int waveLength = BASE_WAVE_LENGTH);
    double cosineInterpolate(long X1, long X2, double Z1, double Z2, long x);

};

#endif

#ifndef INCLUDE_PERLIN
#define INCLUDE_PERLIN

#include <random>

#define BASE_WAVE_LENGTH 128
#define BASE_AMPLITUDE 256
#define MEAN_NOISE_VALUE 0
#define PERSISTENCE 0.5 //coeff par lequel on multiplie l'amplitude à chaque octave (à chaque fois que la frequence double)

class Noise
{
public:
    Noise(long seed);
	double outputValue(long x, long y, int octaves);
    void generateOutputFile(long size, int numberOfOctaves); // ToDo - Remove, debug only

protected:
    long seed;
	std::mt19937 rng; // Random number generator
    double discreteNoise(long x, long y, long amplitude);
    double interpolatedNoise(long x, long y, long waveLength = BASE_WAVE_LENGTH);
    double cosineInterpolate(long X1, long X2, double Z1, double Z2, long x);
    
};

#endif

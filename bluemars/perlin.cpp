#include <iostream>
#define _USE_MATH_DEFINES // Needed on windows for M_PI
#include <math.h>
#include <stdlib.h>
#include <perlin.h>

long murmurHash2(long seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	long h = seed;
	long k = (unsigned int)(seed && 0xFF);
	k *= m;
	k ^= k >> r;
	k *= m;
	h *= m;
	h ^= k;
	h ^= (unsigned int)((seed >> 24) && 0xFF) << 16;
	h ^= (unsigned int)((seed >> 16) && 0xFF) << 8;
	h ^= (unsigned int)((seed >> 8) && 0xFF);
	h *= m;
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h;
}

Noise::Noise(long s)
{
    seed = s;
}

long Noise::discreteNoise(long x, long y, int octave)
{
    int amplitude = (int) (BASE_AMPLITUDE * pow(PERSISTENCE, octave));
	long temp_seed = (seed * 524288) % MAX_SEED;
	temp_seed = (temp_seed + (15859 * x)) % MAX_SEED;
	temp_seed = (temp_seed + (11117 * y)) % MAX_SEED;
	temp_seed = (temp_seed + (28433 * octave)) % MAX_SEED;
	long random_value = murmurHash2(temp_seed);
	long ret = random_value % amplitude;
	if (ret < 0) // Because % is not modulo !
	{
		ret += amplitude;
	}
	return ret;
}

double Noise::cosineInterpolate(long X1, long X2, double Z1, double Z2, long x) //renvoie la hauteur z du point d'abscisse x interpollé entre les points (X1,Z1) et (X2,Z2)
{
    if (X1==X2)
    {
        return Z1;
    }
    else
    {
		double normerX = (double)((((double)-x) / (X2 - X1)) + ((double)X1 / (double)(X2 - X1)));//norme x entre 0 et 1 (renvoie 0 si x vaut z1, 1 si x vaut z2)
		double f = 0.5*(1 - cos(normerX*M_PI));
        return (Z1*(1-f) + Z2*f);
    }
}

double Noise::interpolatedNoise(long x, long y, int octave)
{
	long waveLength = static_cast<long>(round(BASE_WAVE_LENGTH / pow(2, octave)));
	if (waveLength <= 0) { waveLength = 1; } // We can't have a waveLenght lower than 0

    //on détermine les quatre points les plus proches de (x,y) et entre lesquels il faut faire l'interpolation :(X1,Y1), (X1,Y2), (X2,Y1), (X2,Y2)
	long X1 = static_cast<long>(floor(((double)x) / waveLength) * (double)waveLength);
    long X2 = X1 + waveLength;
	long Y1 = static_cast<long>(floor(((double)y) / waveLength) * (double)waveLength);
    long Y2 = Y1 + waveLength;

    //on calcule la valeur du bruit aux quatre points

    long discreteNoiseX1_Y1 = discreteNoise(X1, Y1, octave);
    long discreteNoiseX1_Y2 = discreteNoise(X1, Y2, octave);
    long discreteNoiseX2_Y1 = discreteNoise(X2, Y1, octave);
    long discreteNoiseX2_Y2 = discreteNoise(X2, Y2, octave);

    //on interpole entre (X1,Y1) et (X2,Y1) puis entre (X1,Y2) et (X2,Y2)
    double Z1 = cosineInterpolate(X1, X2, (double)discreteNoiseX1_Y1, (double)discreteNoiseX2_Y1, x);
    double Z2 = cosineInterpolate(X1, X2, (double)discreteNoiseX1_Y2, (double)discreteNoiseX2_Y2, x);

    //on interpole entre (x,Y1) et (x,Y2) de hauteur respectives Z1 et Z2
    return (cosineInterpolate(Y1,Y2,Z1,Z2,y));
}

double Noise::outputValue(long x, long y, int numberOfOctaves = 8) //l'octave 0 correspondant à BASE_AMPLITUDE et BASE_WAVE_LENGTH compte comme une octave
{
    double result = 0;
	for (int octave = 0; octave < numberOfOctaves; octave++)
    {
        result += interpolatedNoise(x, y, octave);
    }
	result /= (2 * BASE_AMPLITUDE); // So we are sure we have values between 0 and 1
    return result;
}


#include <iostream>
#define _USE_MATH_DEFINES // Needed on windows for M_PI
#include <math.h>
#include <stdlib.h>
#include <perlin.h>

Noise::Noise(long s)
{
    seed = s;
}

double Noise::discreteNoise(long x, long y, long octave)
{
    int amplitude = BASE_AMPLITUDE * pow(PERSISTENCE, octave);
	long temp_seed = (seed + 15859 * x + 11117 * y) * 524288 + 28433 * octave;
	rng.seed(temp_seed);
	long random_value = rng();
	double ret = ((random_value % amplitude) + MEAN_NOISE_VALUE - amplitude / 2);//retourne un nombre aléatoire entre MEAN_NOISE_VALUE + amplitude/2 et MEAN_NOISE_VALUE - amplitude/2
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
		double normerX = ((double)-x) / (X2 - X1) + ((double)X1) / (X2 - X1);//norme x entre 0 et 1 (renvoie 0 si x vaut z1, 1 si x vaut z2)
		double f = 0.5*(1 - cos(normerX*M_PI));
        return (Z1*(1-f) + Z2*f);
    }
}

double Noise::interpolatedNoise(long x, long y, long octave)
{
    double waveLength = 1.0f * BASE_WAVE_LENGTH / pow(2,octave);
    //on détermine les quatre points les plus proches de (x,y) et entre lesquels il faut faire l'interpolation :(X1,Y1), (X1,Y2), (X2,Y1), (X2,Y2)
    int X1 = floor(x / waveLength) * waveLength;
    int X2 = X1 + waveLength;
    int Y1 = floor(y / waveLength) * waveLength;
    int Y2 = Y1 + waveLength;
    
    //on calcule la valeur du bruit aux quatre points
    double discreteNoiseX1_Y1 = discreteNoise(X1, Y1, octave);
    double discreteNoiseX1_Y2 = discreteNoise(X1, Y2, octave);
    double discreteNoiseX2_Y1 = discreteNoise(X2, Y1, octave);
    double discreteNoiseX2_Y2 = discreteNoise(X2, Y2, octave);
    
    //on interpole entre (X1,Y1) et (X2,Y1) puis entre (X1,Y2) et (X2,Y2)
    double Z1 = cosineInterpolate(X1, X2, discreteNoiseX1_Y1, discreteNoiseX2_Y1, x);
    double Z2 = cosineInterpolate(X1, X2, discreteNoiseX1_Y2, discreteNoiseX2_Y2, x);

    //on interpole entre (x,Y1) et (x,Y2) de hauteur respectives Z1 et Z2
    return (cosineInterpolate(Y1,Y2,Z1,Z2,y));
}

double Noise::outputValue(long x, long y, int numberOfOctaves = 8) //l'octave 0 correspondant à BASE_AMPLITUDE et BASE_WAVE_LENGTH compte comme une octave
{
    double outputValue = 0;
	for (int octave = 0; octave < numberOfOctaves; octave++)
    {
        outputValue += interpolatedNoise(x, y, octave);
    }
    return outputValue;
}

void Noise::generateOutputFile(long size, int numberOfOctaves)
{
    
    FILE* perlinOutputFile = NULL;
    perlinOutputFile = fopen("perlinOutputFile.txt", "w+");
    if (perlinOutputFile != NULL)
    {
        for (double x=0; x<=size; x++)
        {
            for (double y=0; y<=size; y++)
            {
                double z = outputValue(x,y,numberOfOctaves);
                fprintf(perlinOutputFile, "%f;%f;%f\n", x,y,z);
            }
        }
        
        fclose(perlinOutputFile);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier");
    }
}
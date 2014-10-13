#define CHUNK_SIZE 1024
#define BASE_WAVE_LENGTH 128
#define BASE_AMPLITUDE 256
#define MEAN_NOISE_VALUE 0
#define PERSISTENCE 0.5 //coeff par lequel on multiplie l'amplitude à chaque octave (à chaque fois que la frequence double)

class Noise
{
public:
    Noise(double seed);
    void generateOutputFile(double size, int numberOfOctaves);
    double seed;
    double discreteNoise(double x, double y, double amplitude);

    
    double interpolatedNoise(double x, double y, double waveLength);
    double cosineInterpolate(double X1, double X2, double Z1, double Z2, double x);
    double outputValue(double x, double y, int octaves);
};
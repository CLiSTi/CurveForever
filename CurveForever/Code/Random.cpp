#include "Random.h"
#include "time.h"

bool reset = false;

void RandomReset()
{
	reset = true;
	srand(time(NULL));
}
int RandomInt(int low, int high)
{
	if (!reset) RandomReset();
	if (high - low == 0) return high;
	return rand() % (high - low) + low;
}

double RandomDouble(double low, double high)
{
	if (!reset) RandomReset();
	double f = (double)rand() / RAND_MAX;
	return low + f * (high - low);
}
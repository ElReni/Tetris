#include "TRandomGenerator.h"
#include <stdlib.h>
#include <time.h>

//Randomizer creating
TRandomGenerator::TRandomGenerator()
{
	srand(time(NULL));
}

//Get a number in a given range
int TRandomGenerator::GetRandom(int limit_1, int limit_2)
{
	return (rand() % (limit_2 - limit_1 + 1) + limit_1);
}
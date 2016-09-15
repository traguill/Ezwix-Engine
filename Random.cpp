#include "Random.h"

float Random::RandomFloat()
{
	static unsigned seed = 2463534242U; //xorshift method

	seed ^= (seed << 5);
	seed ^= (seed >> 13);
	seed ^= (seed << 6);

	return seed * (1.0f / 4294967295.0f);
}

int Random::RandomInt(int min, int max)
{
	static unsigned seed = 2463534242U; //xorshift method

	seed ^= (seed << 5);
	seed ^= (seed >> 13);
	seed ^= (seed << 6);
	float rnd =  seed * (1.0f / 4294967295.0f);

	return rnd * (float)(max - min) + min;
}
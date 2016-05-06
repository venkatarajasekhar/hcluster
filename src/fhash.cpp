
#include <math.h> 
#include "fhash.h"



//Compute the similarity score between a and b
//template<int N>
float jaccard_index(bitset<FEATURE_SIZE> &a, bitset<FEATURE_SIZE> &b)
{
	float num = (a & b).count();
	float den = (a | b).count();
	if (fabs(den) < 1e-10) return 0.0;
	return num/den;
}

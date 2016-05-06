#include <bitset>
#include <vector>
#include <string>
#include <set>

#include "fhash.h"
using namespace std;

template<int N>
double dot_distance(bitset<N> & v1, bitset<N> & v2)
{
	double d_distance = 0;
	for(unsigned int index = 0; index < v1.size(); ++index)
	{
		d_distance+= v1[index]*v2[index];
	}
	return d_distance;
}


// dot distance computing
double dot_distance(vector<int> &v1, vector<int> &v2)
{
	double d_distance = 0;
	for(unsigned int index = 0; index < v1.size(); ++index)
	{
		d_distance+= v1[index]*v2[index];
	}
	return d_distance;
}

double dot_distance(set<string> &v1, set<string> &v2)
{
	double d_distance = 0;
	set<string>::iterator v1_it=v1.begin();
	while(v1_it !=v1.end())
	{
		string str_cmp = v1_it->data();
		if(v2.find(str_cmp)!=v2.end())
		{
			d_distance+= 1;
		}
		++v1_it;
	}
	return d_distance;
}

template<int N>
double jaccard_distance(bitset<N> a, bitset<N> b)
{
	float sim = jaccard_index(a, b);
	double d_distance = 1.0 - sim;
	return d_distance;
}



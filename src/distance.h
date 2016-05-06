#ifndef _DISTANCE_SONGLH
#define _DISTANCE_SONGLH

#include <bitset>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

#include "tools.h"

using namespace std;

double dot_distance(bitset<FEATURE_SIZE> & v1, bitset<FEATURE_SIZE> & v2);

double dot_distance(vector<int> &v1, vector<int> &v2);

double dot_distance(set<string> &v1, set<string> &v2);

double jaccard_distance(bitset<FEATURE_SIZE> & a, bitset<FEATURE_SIZE> & b);

int all_pair_distance(vector< bitset<FEATURE_SIZE> > & features, int distanceType, vector<unordered_map<int, float> > & Dist);

int distancefromfile(const char * file_name, float threshold, vector<unordered_map<int, float> > & dist);

int read_sampledir(char *cur_dir, sample_t **sample_list, int *nsamples);

int distancefromdir(const char *db_path, float threshold, vector<unordered_map<int,float> > & dist);

#endif
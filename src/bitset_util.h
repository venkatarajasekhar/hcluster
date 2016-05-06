#ifndef _BITSET_UTIL_SONGLH
#define _BITSET_UTIL_SONGLH

#include <bitset>
#include <vector>

#include "tools.h"

using namespace std;

int bitsetfromfile(const char *file_name, vector<bitset<FEATURE_SIZE> > &bss);
int bitsetfromCCS(const char *prefix, vector<bitset<FEATURE_SIZE> > &bss);


#endif
#ifndef _CLASSIFIER_SONGLH
#define _CLASSIFIER_SONGLH

#include <bitset>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <unordered_map>

#include "tools.h"

using namespace std;

class classifier
{
private:

	struct Distance
	{
		float dist;
		int index;
	};


	struct Cmp
	{
		bool operator()(const Distance d1, const Distance d2) const
		{
			if(d1.dist == d2.dist)
			{
				return d1.index < d2.index;
			}

			return d1.dist < d2.dist;
		}
	};


public:

	// multiset for storing sorted distances: insert/delete operation - O(log N)
	vector<multiset<Distance, Cmp> > P;
	// 2d vector for storing distances matrix
	vector<unordered_map<int, Distance> > C;
	// vector for storing flags for marking currently active clusters
	vector<int> II;
	// 2d vector for storing lists of titles in clusters
	vector<vector<int> > A;

	void init();
	void populate_distance();

	// The way of separating clusters, either by
	// specifying the number of clusters K, or by
	// specifying a threshold T
	int cutoffType;

	// number of classes to cluster
	int numCluster;

	// seperating threshold
	double threshold;

	// distance function
	double distanceType;

	// number of rows of features
	int N;

	// bitset representation of features
	vector<bitset<FEATURE_SIZE> > features;

	// all-pair distance of features, either compute on-the-fly,
	// or read from file
	//vector<vector<float> > Dist;
	vector<unordered_map<int, float> > Dist;

	// constructors with feature and distance data
	classifier(vector<bitset<FEATURE_SIZE> > &bfs, vector<unordered_map<int, float> > &dist);
	// constructors with distance data only
	classifier(vector<unordered_map<int, float> > &dist);

	// destructor
	~classifier(void);

	void setThreshold(double t);
	void setCluster(int k);

	// total clustering (using all linkage criteries and distances)
	//bool run_clustering(void);
	// clustering with provided linkage_criteria
	void clustering(int linkage_criteria);
	// printing clusters with size >= num to file
	void print_clusters(std::string file, unsigned int size_threshold);
	// Populate the appCluster using clusters with size >= size_threshold
	//void clusterStat(int size_threshold, vector<appCluster> &clusters);

};

#endif
#include <algorithm>
#include "classifier.h"

using namespace std;

classifier::classifier(vector<bitset<FEATURE_SIZE> > &bfs, vector<unordered_map<int, float> > &dist)
{
	features = bfs;

	Dist = dist;
	N = Dist.size();
}

// constructor
classifier::classifier(vector<unordered_map<int, float> > &dist)
{
	Dist = dist;
	N = Dist.size();
}

classifier::~classifier(void)
{
	A.clear();
	C.clear();
	II.clear();
	P.clear();

	Dist.clear();
	features.clear();
}


void classifier::setCluster(int k)
{
	cutoffType = BY_NUMBER;
	numCluster = k;

	// Give threshold a big value, so threshold would not take effect,
	// and numCluster will control the result.
	threshold = 10*MIN_DIST;
}

void classifier::setThreshold(double t)
{
	// get the threshold for separating clusters
	cutoffType = BY_THRESHOLD;
	threshold = t;

	// Set numCluster = 1, so numCluster would not take effect,
	// and T will control the result.
	numCluster = 1;

}

void classifier::init()
{
	A.clear();
	C.clear();
	II.clear();
	P.clear();

	C.resize(N);
	P.resize(N);
	II.resize(N);
	A.resize(N);

	// Populating the all-pair distances
	// complexity is: O(N*N*log(N))
	populate_distance();
}

void classifier::populate_distance()
{
	int i, j;
	int added = 0;
	for(i=0; i<N; ++i)
	{
		//std::vector<Distance> V_temp;
		unordered_map<int, float>    E_temp;
		unordered_map<int, Distance> V_temp;
 		multiset<Distance, Cmp> Q_temp;
		Distance D_temp;

        E_temp = Dist[i];
		Q_temp.clear();
		V_temp.clear();
		

		for (unordered_map<int, float>::iterator it = E_temp.begin(); it != E_temp.end(); ++it)
		{
			j = it->first;
			D_temp.dist = it->second;
			D_temp.index = j;

			V_temp[j]=D_temp;

			if(j!=i)
			{
				Q_temp.insert(D_temp);
			}
			added++;
		}

		C[i]=V_temp;
		II[i]=1;
		P[i]=Q_temp;

		vector<int> A_i;
		A_i.push_back(i);
		A[i]=A_i;
	}
}


void classifier::clustering(const int linkage_criteria)
{
	double min_dist, dist;
	int n;

	// fill distances matrix C, then I and P
	printf("    Initialization ...\n");
	init();

	// next: build clusters until numCluster clusters left
	// complexity is: O(N*N*log(N))
	printf("    Merging clusters ...\n");
	for(n=0; n<N-numCluster; ++n)
	{
		min_dist = MIN_DIST;
		int min_index = 0;
		for(int k=0; k<N-1; ++k)
		{
			if(II[k]==1 && P[k].size() > 0)
			{
				dist = P[k].begin()->dist;
				if(dist < min_dist)
				{
					min_dist = dist;
					min_index = P[k].begin()->index;
				}
			}
		}

		if (min_dist > threshold)
			break;

		// we have minimum distance
		// k1, k2 - indexes of most nearest clusters
		int k1 = min_index;
		int k2 = P[k1].begin()->index;

		int N_k1 = A[k1].size();
		int N_k2 = A[k2].size();

		P[k1].clear();
		// add cluster k2 to A[k1]
		for(unsigned int l=0;l<A[k2].size();++l)
		{
			A[k1].push_back(A[k2][l]);
		}


		// clear the second cluster
		II[k2] = 0;
		// O(N*log(N))
		for(int m=0; m<N; ++m)
		{
			// O(log(N)): insert, erase operations
			if (C[m].size() > 0)
			{
				if((II[m] != 0) && (m != k1) && (C[m].find(k1) != C[m].end()) && (C[m].find(k2) != C[m].end()) )
				{
					P[m].erase(C[m][k1]);
 					P[m].erase(C[m][k2]);

					if(linkage_criteria == SINGLE_LINKAGE)
					{
						C[m][k1].dist = C[m][k1].dist<C[m][k2].dist ? C[m][k1].dist : C[m][k2].dist;
						C[k1][m].dist = C[m][k1].dist;
					}

					if(linkage_criteria == COMPLETE_LINKAGE)
					{
						C[m][k1].dist = C[m][k1].dist>C[m][k2].dist ? C[m][k1].dist : C[m][k2].dist;
 						C[k1][m].dist = C[m][k1].dist;
					}

					if(linkage_criteria == AVG_LINKAGE)
					{
                        // Not sure whehter this is 100% correct: further investigation is required.
                        //int N_m = A[m].size();
                        //C[m][k1].dist = (N_k1*C[m][k1].dist + N_k2*C[m][k2].dist)/N_m;
						C[m][k1].dist = (N_k1*C[m][k1].dist + N_k2*C[m][k2].dist)/(N_k1 + N_k2);
						C[k1][m].dist = C[m][k1].dist;
					}

					P[m].insert(C[m][k1]);
					P[k1].insert(C[k1][m]);

				}
			}
		}
	}

	cout << "  The min distance betwee clusters less than " << min_dist << "\n";
	return;
}

// printing clusters with size >= num to file
void classifier::print_clusters(std::string file, unsigned int size_threshold)
{
	ofstream outfile(file.c_str(), ios_base::out);
	outfile<< "Output Clusters with size at least "<< size_threshold << " :" << endl;
	cout   << "Output Clusters to " << file << " with size at least "<< size_threshold << " :" << endl;

	int class_num = 0;
	for(int i=0; i<N; ++i)
	{
		if(II[i]==1)
		{
			if (A[i].size() >= size_threshold)
			{
				++class_num;
				outfile<<std::endl<<"Cluster "<<class_num<<" has "<<A[i].size()<<" apps: ";
				cout   <<std::endl<<"Cluster "<<class_num<<" has "<<A[i].size()<<" apps: ";
				sort(A[i].begin(), A[i].end());
				vector<int>::iterator it_a(A[i].begin());

				while(it_a!=A[i].end())
				{
					int num_of_title = (*it_a);
					outfile << num_of_title << " ";
					cout    << num_of_title << " ";
					++it_a;
				}

				outfile << endl;
				cout    << endl;
			}
		}
	}
	
	outfile.close();
}

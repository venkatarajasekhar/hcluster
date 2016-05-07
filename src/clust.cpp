#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unordered_map>

#include "tools.h"
#include "bitset_util.h"
#include "distance.h"
#include "classifier.h"

using namespace std;

int main(int argc, char * argv[])
{
	commandLineArgument myCLA;
	setCommandLine(myCLA); // set default command line arguments
	getCommandLine(argc, argv, myCLA); // get command line arguments

	if (!myCLA.havingArgument)
	{
		printHelp();
		exit(EXIT_SUCCESS);
	}
	else if (myCLA.numInvalidCLA != 0)
	{
		cout << "There are invalid CLAs!" << endl;
		exit(EXIT_FAILURE);
    }
    
    printCommandInfo(myCLA);

    //Input feature files
	int f_ok = 0;
	vector<bitset<FEATURE_SIZE> > bbs;
	if (myCLA.inputMatrixType == DENSE_MATRIX)
		f_ok = bitsetfromfile(myCLA.inputFilename, bbs);
	else if (myCLA.inputMatrixType == SPARSE_MATRIX)
		f_ok = bitsetfromCCS(myCLA.inputFilename, bbs);
	else if (myCLA.inputMatrixType == EMPTY_MATRIX)
		f_ok = 1;
	if (!f_ok)
	{
		cout << "Cannot load feature data!\n";
		exit(EXIT_FAILURE);
	}

	int d_ok = 0;
	vector<unordered_map<int, float> > Dist;
	if (myCLA.inputMatrixType != EMPTY_MATRIX && (myCLA.distanceType == JACCARD_DISTANCE || myCLA.distanceType ==  EUCLID_DISTANCE ))
		d_ok = all_pair_distance(bbs, JACCARD, Dist);
	else if (myCLA.distanceType ==  MATRIX_DISTANCE)
		d_ok = distancefromfile(myCLA.distanceFilename, 0.5, Dist);
	else
		d_ok = distancefromdir(myCLA.distanceFilename, 0, Dist);

	if (!d_ok)
	{
		cout << "Cannot load distance data!\n";
		exit(EXIT_FAILURE);
	}

	classifier classf(bbs, Dist);

	if (myCLA.cutoffType == BY_NUMBER)
		classf.setCluster(myCLA.numCluster);
	else
		classf.setThreshold(myCLA.threshold);

	cout << endl << "  Do clustering ..." << endl;
	classf.clustering(myCLA.algorithmType);
	cout<<"  Clustering completed, output results ..." << endl;

	
	unsigned int size_threshold = 1;
	string s = string(myCLA.clusterFilename);
	classf.print_clusters(s, size_threshold);

	return 0;

}
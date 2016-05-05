#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

using namespace std;

void printUsage()
{
	cout << endl;
	cout << "USAGE" << endl;
	cout << "\t" << EXEC_PROGRAM << "[SWITCHES]" << endl;
	cout << "\t Using either -K or -T to determine number of clusters." << endl;
}

void printClusterNum()
{
	cout << "\t-K  ClusterNum" << endl;
	cout << "\t    specifies number of clusters (DEFAULT = " << DEFAULT_numCluster << ")" << endl; 
}

void printThreshold()
{
	cout << "\t-T  Threshold" << endl;
	cout << "\t    specifies a threshold for cutting off clusters (DEFAULT = " << DEFAULT_threshold << ")" << endl;
}

void printInputMatrixType()
{
	cout << "\t-I  inputMatrixType  inputFile (name or prefix)  (REQUIRED)" << endl;
	cout << "\t    specifies details of input matrix file" << endl;
	cout << "\t    inputMatrixType can be one of" << endl;
	cout << "\t      d -- dense matrix (file name)" << endl;
	cout << "\t      s -- sparse matrix in CCS (file prefix)" << endl;
}

void printDistanceType()
{
	cout << "\t-D  distanceType  [distanceFilename]" << endl;
	cout << "\t    specifies distance type for computing similarity" << endl;
	cout << "\t    distanceType can be one of" << endl;
	cout << "\t      j -- Jaccard distance" << endl;
	cout << "\t      e -- euclidean distance" << endl;
	cout << "\t      m inputFilename -- a distance matrix from a file" << endl;
	cout << "\t      d inputFilename -- a distance matrix from a dir of files" << endl;
}

void printOutputLabelType()
{
	cout << "\t-O  outputFilename" << endl;
}

void printAlgorithmType()
{
	cout << "\t-L  Linkage Type " << endl;
	cout << "\t    specifies type of linkage for merging clusters" << endl;
	cout << "\t      s -- single linkage (DEFAULT)" << endl;
	cout << "\t      c -- complete linkage " << endl;
	cout << "\t      a -- average linkage" << endl;
}

// show command-line parameters...
void printHelp()
{
	printUsage();
	printClusterNum();
	printThreshold();
	printInputMatrixType();
	printDistanceType();
    //printDescription();
    //printAuthor();

	printAlgorithmType();
	printOutputLabelType();
	cout << endl;
}


// set default parameters...
void setCommandLine(commandLineArgument &myCLA)
{
	// default parameters
	myCLA.numInvalidCLA = 0;

	myCLA.cutoffType = DEFAULT_cutoffType;
	myCLA.algorithmType = DEFAULT_algorithmType;
	myCLA.numCluster = DEFAULT_numCluster;
	myCLA.threshold = DEFAULT_threshold;

	myCLA.inputMatrixType = DEFAULT_inputMatrixType;
	myCLA.distanceType = DEFAULT_distanceType;

	// input and output files
	strncpy(myCLA.distanceFilename, EMPTY_STRING, FILENAME_LENGTH );
	strncpy(myCLA.inputFilename,    EMPTY_STRING, FILENAME_LENGTH);
	strncpy(myCLA.clusterFilename,  EMPTY_STRING, FILENAME_LENGTH);
}


// get type of co-clustering algorithm from command-line...
char **getAlgorithmType(int argc, char **argv, commandLineArgument &myCLA)
{
	bool validCLA = true;
	switch (toupper((*(++argv))[0]))
	{
		case SINGLE_LINKAGE_ALGORITHM:
			myCLA.algorithmType = SINGLE_LINKAGE;
			break;
		case COMPLETE_LINKAGE_ALGORITHM:
			myCLA.algorithmType = COMPLETE_LINKAGE;
			break;
		case AVG_LINKAGE_ALGORITHM:
			myCLA.algorithmType = AVG_LINKAGE;
			break;
		default:
			cout << "  !!! Invalid linkage type: " << *argv << " !!!" << endl << endl;
			validCLA = false;
			break;
	}

	if (!validCLA)
	{
		printAlgorithmType();
		myCLA.numInvalidCLA++;
	}

	return argv;
}


// get # of column clusters from command-line...
char **getClusterNum(int argc, char **argv, commandLineArgument &myCLA)
{
	bool validCLA = true;

	if (*(argv+1) == NULL)
	{
		cout << "  !!! Invalid # of column cluster(s): (# of column cluster(s) should be specified.) !!!" << endl << endl;
		validCLA = false;
	}

	if (validCLA && !isdigit((*(argv+1))[0]))
	{
		cout << "  !!! Invalid # of column cluster(s): " << *(argv+1) << " !!!" << endl << endl;
		validCLA = false;
	}

	if (validCLA)
	{
		int tempNum = atoi(*(argv+1));

		if (tempNum <= 0)
		{
			cout << "  !!! Invalid # of column cluster(s): (# of column cluster(s) should be a positive integer.): " << tempNum << " !!!" << endl;
			validCLA = false;
		}
	}
  
	if (validCLA)
	{
		myCLA.numCluster = atoi(*(++argv));
		myCLA.threshold = MIN_DIST;
		myCLA.cutoffType = BY_NUMBER;
	}

	if (!validCLA)
	{
		printClusterNum();
		myCLA.numInvalidCLA++;
	}

	return argv;
}


// get # of column clusters from command-line...
char **getThreshold(int argc, char **argv, commandLineArgument &myCLA)
{
	bool validCLA = true;

	if (*(argv+1) == NULL)
	{
		cout << "  !!! Invalid # threshold: (a threshold value should be specified.) !!!" << endl << endl;
		validCLA = false;
	}

	if (validCLA && !isdigit((*(argv+1))[0]))
	{
		cout << "  !!! Invalid # of column cluster(s): " << *(argv+1) << " !!!" << endl << endl;
		validCLA = false;
	}

	if (validCLA)
	{

		double tempNum = atof(*(argv+1));
		if (tempNum <= 0)
		{
			cout << "  !!! Invalid threshold value: (threshold should be a positive value.): " << tempNum << " !!!" << endl;
			validCLA = false;
		}
	}

	if (validCLA)
	{
		myCLA.numCluster = 1;
		myCLA.threshold = atof(*(++argv));
		myCLA.cutoffType = BY_THRESHOLD;
	}

	if (!validCLA)
	{
		printThreshold();
		myCLA.numInvalidCLA++;
	}

	return argv;
}

void extractFileName(char *path, char *name)
{
	int length = strlen(path);
	for(int i = length-1; i >= 0; i--)
		if ((path[i] == '/') || (path[i] == '\\'))
		{
			i++;
			for (int j = i; j < length; j++)
				name[j-i] = path[j];
			break;
		} 
		else if (i == 0)
		{
			for (int j = i; j < length; j++)
				name[j-i]=path[j];
			break;
		}
}

// get input file information from command-line...
char **getInputFileInformation(int argc, char **argv, commandLineArgument &myCLA)
{
	bool validCLA = true;
	switch (toupper((*(++argv))[0]))
	{
		case DENSE_INPUT:
			myCLA.inputMatrixType = DENSE_MATRIX;
			break;
		case SPARSE_INPUT:
			myCLA.inputMatrixType = SPARSE_MATRIX;
			break;
		default:
			cout << "  !!! Invalid input matrix argument(s): " << *argv << " !!!" << endl << endl;
			validCLA = false;
			break;
    }

	if (validCLA && ((*(++argv))[0] != '-'))
	{
		sprintf(myCLA.inputFilename, "%s",*argv);
		if (strcmp(myCLA.clusterFilename, "") == 0)
			extractFileName(*argv, myCLA.clusterFilename);
		myCLA.havingArgument = true;
	}
	else
	{
		cout << "  !!! Invalid input matrix argument(s): " << *argv << " !!!" << endl << endl;
		validCLA = false;
	}

	if (!validCLA)
	{
		printInputMatrixType();
		myCLA.numInvalidCLA++;
	}

	return argv;
}


// get input file information from command-line...
char **getDistanceFileInformation(int argc, char **argv, commandLineArgument &myCLA)
{
	bool validCLA = true;

	char d_type = toupper((*(++argv))[0]);
	switch (d_type)
	{
		case JACCARD_DISTANCE_TYPE:
			myCLA.distanceType = JACCARD_DISTANCE;
			break;
		case EUCLID_DISTANCE_TYPE:
			myCLA.distanceType = EUCLID_DISTANCE;
			break;
		case MATRIX_DISTANCE_TYPE:
			myCLA.distanceType = MATRIX_DISTANCE;
			break;
		case DIR_DISTANCE_TYPE:
			myCLA.distanceType = DIR_DISTANCE;
			break;
		default:
			cout << "  !!! Invalid distance argument(s): " << *argv << " !!!" << endl << endl;
			validCLA = false;
			break;
    }

	if (validCLA && (myCLA.distanceType == MATRIX_DISTANCE || myCLA.distanceType == DIR_DISTANCE))
    {
		if ((*(++argv))[0] != '-')
		{
			sprintf(myCLA.distanceFilename, "%s",*argv);
			if (strcmp(myCLA.clusterFilename, "") == 0)
				extractFileName(*argv, myCLA.clusterFilename);

			myCLA.havingArgument = true;
		}
		else
        {
			cout << "  !!! Invalid distance argument(s): " << *argv << " !!!" << endl << endl;
			validCLA = false;
		}
	}

	if(!validCLA)
	{
		printDistanceType();
		myCLA.numInvalidCLA++;
	}

	return argv;
}

// get output file information from command-line...
char **getOutputFileInformation(int argc, char **argv, commandLineArgument &myCLA)
{
	bool validCLA = true;

	if (validCLA && (*(argv+1) == NULL))
	{
		cout << "  !!! Invalid output filename: (output filename should be specified.) !!!" << endl << endl;
		validCLA = false;
	}

	if (validCLA && (isdigit((*(argv+1))[0]) || ((*(argv+1))[0] == '-')))
	{
		cout << "  !!! Invalid output filename: " << *(argv+1) << " !!!" << endl << endl;
		validCLA = false;
	}

	if (validCLA)
		strcpy(myCLA.clusterFilename, *(++argv));
	else
	{
		printOutputLabelType();
		myCLA.numInvalidCLA++;
	}

	return argv;
}


// get clustering parameter values from command-line...
void getCommandLine(int argc, char **argv, commandLineArgument &myCLA)
{
	bool validCLA = true;
	for (argv++; *argv != NULL; argv++)
	{
		if ((*argv)[0] == '-')
		{
			switch (toupper((*argv)[1]))
			{
				case MY_ALGORITHM:              // -L  algorithmType
					argv = getAlgorithmType(argc, argv, myCLA);
					break;
				case MY_NUM_CLUSTER:    // -K  numColCluster
					argv = getClusterNum(argc, argv, myCLA);
					break;
				case MY_THRESHOLD:              // -T thresholdType thresholdValue
					argv = getThreshold(argc, argv, myCLA);
					break;
				case MY_HELP:           //-H print help
					printHelp();
					break;
				case MY_INPUT_FILE:             // -I  inputMatrixType  inputFilename
					argv = getInputFileInformation(argc, argv, myCLA);
					break;
				case MY_DISTANCE_TYPE:          // -D  distance [distFilename]
					argv = getDistanceFileInformation(argc, argv, myCLA);
					break;
				case MY_OUTPUT_FILE:            // -O  outputLabelType  outputAccessMode  labelOffsetType  outputFilename
					argv = getOutputFileInformation(argc, argv, myCLA);
 					break;
				default:
				//printHelp();
					cout << "  !!! Invalid argument setting: " << *argv << " !!!" << endl << endl;
					validCLA = false;
				break;
			}
		}
		else
		{
			sprintf(myCLA.inputFilename, "%s",*argv);
			if (strcmp(myCLA.clusterFilename, "") == 0)
				extractFileName(*argv, myCLA.clusterFilename);
			myCLA.havingArgument = true;
		}
	}
  
	if (!validCLA)
	{
		//printHelp();
		myCLA.numInvalidCLA++;
	}
}

void printCommandInfo(commandLineArgument &myCLA)
{
	cout << "The command will be executed with:" << endl;

	if (myCLA.cutoffType == BY_NUMBER)
		cout << "Given cluster number K = " << myCLA.numCluster << endl;
	else
		cout << "Given threshold T =  " << myCLA.threshold << endl;

	cout << "The linkage type is ";
	switch (myCLA.algorithmType)
	{
		case SINGLE_LINKAGE:
			cout << "SINGLE" << endl;
			break;
		case COMPLETE_LINKAGE:
			cout << "COMPLETE" << endl;
			break;
		case AVG_LINKAGE:
			cout << "AVERAGE" << endl;
			break;
		default:
			cout << "  !!! Invalid linkage argument(s): " << myCLA.algorithmType << " !!!" << endl << endl;
    }

	cout << "The input data is ";
	switch (myCLA.inputMatrixType)
	{
		case DENSE_MATRIX:
			cout << "DENSE MATRIX";
			break;
		case SPARSE_MATRIX:
			cout << "SPARSE MATRIX";
			break;
		case EMPTY_MATRIX:
			cout << "EMPTY MATRIX";
			break;
		default:
			cout << "  !!! Invalid input argument(s): " << myCLA.inputMatrixType << " !!!" << endl << endl;
			break;
	}

    cout << " with file name " << myCLA.inputFilename << endl;

	cout << "Distance type is ";
	switch (myCLA.distanceType)
	{
		case JACCARD_DISTANCE:
			cout << "JACCARD DISTANCE" << endl;
			break;
		case EUCLID_DISTANCE:
			cout << "EUCLID DISTANCE" << endl;
			break;
		case MATRIX_DISTANCE:
			cout << "MATRIX DISTANCE with file name ";
			cout << myCLA.distanceFilename << endl;
			break;
		case DIR_DISTANCE:
			cout << "DIR DISTANCE with dir name ";
			cout << myCLA.distanceFilename << endl;
			break;
		default:
			cout << "  !!! Invalid distance argument(s): " << myCLA.distanceType << " !!!" << endl << endl;
			break;

	}

	cout << "Output file name is " << myCLA.clusterFilename << endl;
}


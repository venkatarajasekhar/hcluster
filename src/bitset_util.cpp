#include <iostream>
#include <fstream>
#include <cstdlib>
#include <malloc.h>
#include "bitset_util.h"

using namespace std;


int bitsetfromfile(const char *file_name, vector<bitset<FEATURE_SIZE> > &bss)
{
	ifstream ccsfile;
	int row, col, val;
	int i, j;

	ccsfile.open(file_name);
	if (ccsfile.is_open())
	{
		//fscanf(ccsfile, "%d %d\n", &row, &col);
		ccsfile >> row >> col;
		cout << "Reading feature matrix with " << row << " rows and " << col << " cols ..." << endl;
		// Create row # of bitfileds
		for (i = 0; i < row; i++)
		{
			bitset<FEATURE_SIZE> bs;
			for (j = 0; j < col; j++)
			{
				ccsfile >> val;
				if (val > 0)
					bs[j] = 1;
			}

			bss.push_back(bs);
		}

		ccsfile.close();
		return 1;
	}

	cout << "Cannot open file " << file_name << endl;
	return 0;
}

int bitsetfromCCS(const char *prefix, vector<bitset<FEATURE_SIZE> > &bss)
{
	char filename[1024];
	int numRow, numCol, numVal;
	int *colPtr, *rowIdx;
	int i, j;

	//Files related to CCS
	sprintf(filename, "%s%s", prefix, "_dim");
	std::ifstream dimFile(filename);

	if (dimFile == 0)
		cout << "  !!! File open error: " << filename << " !!!" << endl << endl;
	sprintf(filename, "%s%s", prefix, "_row_ccs");

	std::ifstream rowPtrFile(filename);
	if (rowPtrFile == 0)
		cout << "  !!! File open error: " << filename << " !!!" << endl << endl;
	sprintf(filename, "%s%s", prefix, "_col_ccs");

	std::ifstream colIdxFile(filename);
	if (colIdxFile == 0)
		cout << "  !!! File open error: " << filename << " !!!" << endl << endl;

	if (dimFile == 0 || rowPtrFile == 0 || colIdxFile == 0)
	{
		cout << "  !!! Matrix file " << prefix << "_* has missing file(s) !!!" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "  Reading dimension file ... " << endl;

	// Reading CCS data
	dimFile >> numRow >> numCol >> numVal;
	cout << "numRow " << numRow << "\n";
	cout << "numCol " << numCol << "\n";
	cout << "numVal " << numVal << "\n";

	dimFile.close();
	colPtr = (int *)malloc((numCol+1) * sizeof(int));
	rowIdx = (int *)malloc(numVal * sizeof(int));
    
	cout << "  Reading column pointer file ... " << endl;
	for (i = 0; i < numCol+1; i++)
		colIdxFile >> colPtr[i];
	colIdxFile.close();
	cout << "  Reading row index file ... " << endl;
	for (i = 0; i < numVal; i++)
		rowPtrFile >> rowIdx[i];
	rowPtrFile.close();


	// Initialize the bitfield of features
	bss.clear();
	for (i = 0; i < numRow; i++)
	{
		bitset<FEATURE_SIZE> bs;
		bss.push_back(bs);
	}

    // Set bits in bitfield according to CCS data
	for (i = 0; i < numCol; i++)
	{
		for (j = colPtr[i]; j < colPtr[i+1]; j++)
			bss[rowIdx[j]][i] = 1;
	}

	free(colPtr);
	free(rowIdx);
	return 1;
}

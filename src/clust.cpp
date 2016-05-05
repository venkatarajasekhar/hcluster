#include <iostream>
#include <stdlib.h>
#include <vector>

#include "tools.h"

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

}
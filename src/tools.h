#ifndef _TOOTLS_SONGLH
#define _TOOTLS_SONGLH

#define EXEC_PROGRAM				"hclustering"

#define DENSE_INPUT					'D'
#define SPARSE_INPUT				'S'

#define JACCARD_DISTANCE_TYPE		'J'
#define EUCLID_DISTANCE_TYPE		'E'
#define MATRIX_DISTANCE_TYPE		'M'
#define DIR_DISTANCE_TYPE			'D'

#define SINGLE_LINKAGE_ALGORITHM	'S'
#define COMPLETE_LINKAGE_ALGORITHM	'C'
#define AVG_LINKAGE_ALGORITHM		'A'

#define MY_ALGORITHM				'L'
#define MY_NUM_CLUSTER				'K'
#define MY_THRESHOLD				'T'
#define MY_HELP						'H'
#define MY_INPUT_FILE				'I'
#define MY_DISTANCE_TYPE			'D'
#define MY_OUTPUT_FILE				'O'


#define BY_NUMBER					0
#define BY_THRESHOLD				1


#define SPARSE_MATRIX				0       // myCLA.inputMatrixType
#define DENSE_MATRIX				1       // myCLA.inputMatrixType 
#define EMPTY_MATRIX				2       // myCLA.inputMatrixType (DEFAULT)

#define JACCARD_DISTANCE			0
#define EUCLID_DISTANCE				1
#define MATRIX_DISTANCE				2   // distance provided by a matrix file
#define DIR_DISTANCE				3   // distance provided by a dir of files

#define SINGLE_LINKAGE				0
#define COMPLETE_LINKAGE			1
#define AVG_LINKAGE					2

#define DEFAULT_cutoffType			BY_NUMBER;
#define DEFAULT_algorithmType		SINGLE_LINKAGE;
#define DEFAULT_numCluster			3
#define DEFAULT_threshold			100000.0
#define DEFAULT_distanceType		JACCARD_DISTANCE;
#define DEFAULT_inputMatrixType		EMPTY_MATRIX

#define DEFAULT_STRING_LENGTH		1024
#define FILENAME_LENGTH				DEFAULT_STRING_LENGTH
#define EMPTY_STRING				""

#define FEATURE_SIZE 				240007

const int MIN_DIST=10000;

const int DOT_PRODUCT=0;
const int JACCARD=1;


//const int SINGLE_LINKAGE=0;
//const int COMPLETE_LINKAGE=1;
//const int AVG_LINKAGE=2;

//const int BY_NUMBER = 0;
//const int BY_THRESHOLD = 1;

struct commandLineArgument {
	int numInvalidCLA; // used to keep track of # of invalid command-line-arguments

	int    cutoffType; // either by numCluster or by threshold
	int    numCluster; // -K, numCluster;
	double threshold; // -T, the threshold for cutoff cluster;
	int    algorithmType; // -L, LINKAGE

	// input features
	int  inputMatrixType; // -I
	char inputFilename[FILENAME_LENGTH]; // -I

	// distance measure
	int  distanceType; // -D
	// input distance matrix
	char distanceFilename[FILENAME_LENGTH]; // -D

	char clusterFilename[FILENAME_LENGTH]; // -O

	bool havingArgument; // Is input matrix specified or not?
};

typedef struct sample {
	int sample_id;
	char sample_path[256];
} sample_t;

extern void printHelp();
extern void printCommandInfo(commandLineArgument &myCLA);
extern void setCommandLine(commandLineArgument &myCLA);
extern void getCommandLine(int argc, char **argv, commandLineArgument &myCLA);
extern int read_sampledir(char *cur_dir, sample_t **sample_list, int *nsamples);

#endif
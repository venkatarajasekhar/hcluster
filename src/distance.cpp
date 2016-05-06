#include <bitset>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include "distance.h"
#include "fhash.h"


using namespace std;

double dot_distance(bitset<FEATURE_SIZE> & v1, bitset<FEATURE_SIZE> & v2)
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

double jaccard_distance(bitset<FEATURE_SIZE> & a, bitset<FEATURE_SIZE> & b)
{
	float sim = jaccard_index(a, b);
	double d_distance = 1.0 - sim;
	return d_distance;
}


int all_pair_distance(vector< bitset<FEATURE_SIZE> > & features, int distanceType, vector<unordered_map<int, float> > & Dist)
{
	int i, j, N;

	N = features.size();
	printf("  Computing %d X %d distance matrix ...\n", N, N);

	Dist.clear();
	Dist.resize(N);

	for(i=0; i<N; ++i)
	{
		unordered_map<int, float> d_temp;

		for(j=0; j<N; ++j)
		{
			if(distanceType == JACCARD)
			{
				d_temp[j] = jaccard_distance(features[i], features[j]);
			}
			else
			{
				d_temp[j] = -dot_distance(features[i], features[j]);
			}
		}

		Dist[i] = d_temp;
	}

	return 1;
}


int distancefromfile(const char * file_name, float threshold, vector<unordered_map<int, float> > & dist)
{
	ifstream ccsfile;
	float val;
	int row, col;
	int i, j;

	int added = 0;
	ccsfile.open(file_name);
	if (ccsfile.is_open())
	{
        //fscanf(ccsfile, "%d %d\n", &row, &col);
		ccsfile >> row >> col;
		dist.clear();
		dist.resize(row);
		printf("  Reading distance matrix with %d rows and %d cols ...\n\n", row, col);
		for (i = 0; i < row; i++)
		{
			unordered_map<int, float> d_temp;
			for (j = 0; j < col; j++)
			{
				ccsfile >> val;
				if (val < threshold)
				{
					d_temp[j] = val;
					added++;
				}
			}
			dist[i] = d_temp;
		}

		printf("%d distance are read from %s\n", added, file_name);

		ccsfile.close();
		return 1;
	}

	printf("Cannot open file %s\n", file_name);
	return 0;
}


int read_sampledir(char *cur_dir, sample_t **sample_list, int *nsamples) 
{
	DIR *dirp;
	struct dirent *entry;
	char sub_dir[1024];

	dirp = opendir(cur_dir);
	if (dirp != NULL) 
	{
		while ((entry = readdir(dirp))) 
		{
			if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..") == 0)
				continue;

			if (entry->d_type & DT_DIR) 
			{
				snprintf(sub_dir, 1024, "%s/%s", cur_dir, entry->d_name);
				read_sampledir(sub_dir, sample_list, nsamples);
            }
			else 
			{
				if((*nsamples)%128 == 0) 
				{
					if((*sample_list = (sample_t *)realloc(*sample_list, sizeof(sample_t)*((*nsamples)+128))) == NULL) 
					{
						perror("samples_list realloc()");
						exit(EXIT_FAILURE);
					}
				}
                //(*sample_list)[(*nsamples)].sample_id = *nsamples;
				snprintf((*sample_list)[(*nsamples)].sample_path, 256, "%s/%s", cur_dir, entry->d_name);
				*nsamples += 1;
			}
		}
		closedir(dirp);
		return 0;
	}
    else
		return -1;
}



int distancefromdir(const char *db_path, float threshold, vector<unordered_map<int,float> > & dist)
{
	FILE *fp = NULL;
	FILE *fp_jdb = NULL;
	sample_t *jdb_list = NULL;
	int k, nsamples, njdb = 0;
	unsigned int vid_a, vid_b;
	float jaccard_ab;
	char buf[256];
	char *ptr;

	cout << "Creating distance matrix from dir " << db_path << " of files with similarity values ..." << endl;
	sprintf(buf, "%s/vdb_nsamples", db_path);

	if((fp = fopen(buf, "r")) == NULL) 
	{
		perror("vdb_namples fopen()");
		exit(EXIT_FAILURE);
    }

	if((fgets(buf, 64, fp)) == NULL) 
	{
		perror("reading vdb_nsamples");
		exit(EXIT_FAILURE);
    }

	nsamples = atoi(buf);
	fclose(fp);

	for (k = 0; k < nsamples; k++)
	{
		unordered_map<int,float> d_temp;
		dist.push_back(d_temp);
	}

	sprintf(buf, "%s/jdb", db_path);
	read_sampledir(buf, &jdb_list, &njdb);

	cout << "Reading " << njdb << " distance files for " << nsamples << " samples" << endl;

	for(k=0; k<njdb; k++)
	{
		if((fp_jdb = fopen(jdb_list[k].sample_path, "r")) == NULL)
		{
			perror("jdb fopen()");
			exit(EXIT_FAILURE);
		}

		cout << "Reading from " << k << "-th file " << jdb_list[k].sample_path << endl;
        // format of jaccard db file: "vid_a:vid_b:jaccard_ab:"
		while(fgets(buf, 64, fp_jdb) != NULL)
		{
			ptr = strtok(buf, ":");
			vid_a = atoi(ptr);
			ptr = strtok(NULL, ":");
			vid_b = atoi(ptr);
			ptr = strtok(NULL, ":");
			jaccard_ab = atof(ptr);

			//printf("Reading dist[%d][%d] = %f\n", vid_a, vid_b, jaccard_ab);
			if(jaccard_ab >= threshold)
			{
				dist[vid_a][vid_b] = 1.0 - jaccard_ab;
				dist[vid_b][vid_a] = 1.0 - jaccard_ab;
			}
		}
        //dist[k][k] = 0.0;

		fclose(fp_jdb);
	}

	free(jdb_list);
	return 1;
}

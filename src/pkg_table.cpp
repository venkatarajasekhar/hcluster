#include <fstream>
#include <iostream>

#include "pkg_table.h"

pkg_string_table::pkg_string_table() : counter(1) {
	
}

vector<string> pkg_string_table::get_packages() 
{
	vector<string> ret;
	map<string, int>::iterator it = pkg_to_index.begin();

	for(; it != pkg_to_index.end(); ++it) 
	{
		ret.push_back(it->first.c_str());
	}

	return ret;
}

int pkg_string_table::check_pkg_index(string pkg_name) 
{
	if(pkg_to_index.count(pkg_name) == 0) return -1;
	return pkg_to_index[pkg_name];
}

int pkg_string_table::get_pkg_index(string pkg_name) 
{
	if(pkg_to_index.count(pkg_name) == 0)  
	{
		index_to_pkg.push_back(pkg_name);
		pkg_to_index[pkg_name] = counter;
		counter += 1;
	}

	return pkg_to_index[pkg_name];
}

string pkg_string_table::get_pkg_name(unsigned int index) 
{
	return index_to_pkg[index];
}

void pkg_string_table::reset() 
{
	pkg_to_index.clear();
	index_to_pkg.clear();
	//index_to_pkg.push_back("DUMMY");
	counter = 1;
}

bool write_data(string filename, string data) 
{
	ofstream myfile;
	myfile.open(filename.c_str());
	if(!myfile.fail()) 
	{
		myfile << data;
 		myfile.close();
                //printf("writing to %s\n%s", filename.c_str(), data.c_str());
		return true;
	}

	return false;
}


bool pkg_string_table::save(string filename) 
{
	return write_data(filename, to_string());
}

string pkg_string_table::to_string() 
{
	char buff[1024];
	string buffer = "";
	map<string, int>::iterator i = pkg_to_index.begin();

	for(; i != pkg_to_index.end(); ++i) 
	{
		snprintf(buff, sizeof(buff), "%s %d\n", i->first.c_str(), i->second);
		buffer += buff;
	}

	snprintf(buff, sizeof(buff), "%zu\n", pkg_to_index.size());
	buffer = buff + buffer;
	return buffer;
}


bool pkg_string_table::load(string filename) 
{
	string line;
	unsigned int num_entries = 0 ;
	FILE* pFile = fopen (filename.c_str(), "r");
	if(!pFile)
		return false;

	reset();
	fscanf (pFile, "%u", &num_entries);
 	if(feof(pFile)) return false;

	index_to_pkg.resize(num_entries + 1);

	char pkg[1024];
	unsigned int index;

	while(fscanf(pFile, "%s %u", pkg, &index) != EOF)
	{
		index_to_pkg[index] = pkg;
		pkg_to_index[pkg] = index;
        //      cnt += 1;
		counter += 1;
	}

	fclose(pFile);

	return (unsigned int)(counter - 1) == num_entries;
}

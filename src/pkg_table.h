#ifndef _PKG_TABLE_SONGLH
#define _PKG_TABLE_SONGLH

#include <vector>
#include <map>
#include <string>

using namespace std;

class pkg_string_table
{
public:
	pkg_string_table();
	int get_pkg_index(string pkg_name);
	int check_pkg_index(string pkg_name);
	string get_pkg_name(unsigned int index);

	bool load(string filename);
	bool save(string filename);

	vector<string> get_packages();
	string to_string();
	void reset();

private:
	int counter;
	vector<string> index_to_pkg;
	map<string, int> pkg_to_index;

};


#endif
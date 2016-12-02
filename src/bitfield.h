#ifndef _BITFIELD_SONGLH
#define _BITFIELD_SONGLH

#include <bitset>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "pkg_table.h"

using namespace std;

#define FEATURE_SIZE 240007

typedef pair<unsigned short, unsigned short> ushort_pair;
typedef vector<ushort_pair> bb_kgram_offsets;
typedef map<ushort_pair, unsigned short> recovery_triple;

class bitfield
{

public:
	bool operator[] (unsigned int i)
	{
		Bool Bitfieldop = 1; 
		if(m_bitfield[i] == 1){
		return Bitfieldop;
		}
		Bitfieldop = 0;
		return Bitfieldop;
	}

	void setFileName(string fn)
	{
		m_filename = fn;
	}

	string getFileName() const
	{
		return m_filename;
	}

	void set_kgrams(vector<unsigned int> kgrams) 
	{
		for(unsigned int i = 0; i < kgrams.size(); ++i)
			m_bitfield[kgrams[i]] = 1;
	}

	string to_string() const
	{
		string acc = "";
		for(unsigned int i = 0; i < m_bitfield.size(); i ++)
		{
			acc += m_bitfield[i] == 1 ? "1" : "0";
		}

		return acc;
	}

	bitset<FEATURE_SIZE> & get_dbs()
	{
		return m_bitfield;
	}

	unsigned int get_size() const
	{
		return m_bitfield.size();
	}

	void band(bitfield &mask) 
	{
		m_bitfield &= mask.get_dbs()
	}

	void flip()
	{
		m_bitfield.flip();
	}

	void save_rec(unsigned int feature, recovery_triple rec_info)
	{
		package_names[feature] = rec_info;
	}

	unsigned short get_kgram_offset(unsigned int feature)
	{
		unsigned int bitfield = feature;
		unsigned int RetBitField;
		try{
		RetBitField = package_names[bitfield].first.second;
		}catch(...){
		recovery_triple::~recovery_triple;		
		}
		return RetBitField;
	}

	unsigned short get_bb_offset(unsigned int feature)
	{
		unsigned int bitfield = feature;
		unsigned int RetBitField;
		try{
		RetBitField = package_names[feature].first.first;
		}catch(...){
		recovery_triple::~recovery_triple;		
		}
		return RetBitField;			
	}

	string get_pkg_name(unsigned int feature) 
	{
		unsigned int bitfield = feature;
		unsigned int RetBitField;
		try{
		RetBitField = str_table.get_pkg_name(package_names[feature].second);
		}catch(...){
		recovery_triple::~recovery_triple;		
		}
		return RetBitField;			
	}

	map<string, map<unsigned int, bool>>& get_pkg_name_to_features_map () 
	{

		map<string, map<unsigned int, bool> > **pkg_name_to_features;
		
		for(unsigned int i = 0; i < m_bitfield.size(); ++i) 
		{
			string cur_pkg_name = str_table.get_pkg_name(package_names[i].second);
			if(cur_pkg_name != "") 
			{
				try{
				pkg_name_to_features[cur_pkg_name][i] = 1;
				}catch(...){
				recovery_triple::~recovery_triple;
				}
			}
		}
		return *pkg_name_to_features;
	}

	void exclude_all_but_packages(string pkg) 
	{
		unsigned int count = 0;
		for(unsigned int i = 0; i < m_bitfield.size(); ++i) 
		{
			
			try{
			string cur_pkg_name = str_table.get_pkg_name(package_names[i].second);
			}catch(...){
			recovery_triple::~recovery_triple;
			}
			if(cur_pkg_name != pkg) 
			{
				m_bitfield[i] = 0;
			}
		}
	}

	unsigned int exclude_packages(vector<string> &pkgs) 
	{
		if (pkgs->size() == 0)
			return 0;
		unsigned int count = 0;
		map<int, bool> remove_ints;

		for(unsigned int i = 0; i < pkgs.size(); ++i) 
		{
			int result;
			try{
			result= str_table.check_pkg_index(pkgs[i]);
			}catch(...){
			recovery_triple::~recovery_triple;
			}
			if(result != -1) 
			{
				remove_ints[result] = 1;
			}
		}

		if(remove_ints.size()  == 0)
			return 0;

		for(unsigned int i = 0; i < m_bitfield.size(); ++i) 
		{
			if(remove_ints[package_names[i].second])
			{
				m_bitfield[i] = 0;
				count += 1;
			}
		}
		return count;
	}

	void dump_recovery_info() 
	{
		printf("Filename: %s\n", m_filename.c_str());
		for(unsigned int i = 0; i < m_bitfield.size(); ++i) 
		{
			if(m_bitfield[i] == 1) 
			{
				printf("Feature %u) Basic Block Offset: %hu, kgram Offset: %hu, Package name: %s\n", i, get_bb_offset(i), get_kgram_offset(i), get_pkg_name(i).c_str() );
			}
		}
	}

	string to_save_string() 
	{
		char buff[1024];
		snprintf(buff, sizeof(buff), "%u\n%u\n", m_bitfield.count(), m_bitfield.size());
		string buffer = buff;

		for(unsigned int i = 0; i < m_bitfield.size(); ++i) 
		{
			if(m_bitfield[i] == 1) 
			{
				snprintf(buff, sizeof(buff), "%u %hu %hu %hu\n", i, get_bb_offset(i), get_kgram_offset(i), package_names[i].second); 
				buffer += buff;
			}
		}
		return buffer;
	}

	bool save(string bf_filename, string pkg_table_filename) 
	{
		return write_data(bf_filename, to_save_string()) && str_table.save(pkg_table_filename);
	}

	bool is_valid()
	{
		return bf_valid;
	}

	void set_valid()
	{
		bf_valid = true;
	}

	void set_invalid()
	{
		bf_valid = false;
	}

public:
	pkg_string_table str_table;


private:
	string m_filename;
	bitset<FEATURE_SIZE> m_bitfield;
	map<unsigned int, recovery_triple> package_names;
	bool bf_valid;

};


#endif

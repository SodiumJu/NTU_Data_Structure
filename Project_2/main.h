#include <fstream>      
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;


class Hash{
private:
	int size;
	struct item{
		string password;
		int hash_val;
		int salt;
		int line;
	};
	item* Hash_table;
public:
	Hash();
	void get_size(int size);
	long long string_to_num(string& key);
	int Hash_fun(long long string_num);
	int num_with_salt(string& key,int salt);
	int main_hash(long long key);
	long long assist_Hash(long long& key);
	void insert(int& final_hash,string& password,int& salt,int& line);
	void find(int& final_hash,string& password,int& salt,int& line);
};

#ifndef functions
#define functions

void process_file_in(string& path,Hash& hashtable);
void file_in(Hash& hashtable);
void output_pwdir_file(ofstream& outfile,string& password,int& final_hash,int& salt);
void search_pw(Hash& hashtable);

#endif
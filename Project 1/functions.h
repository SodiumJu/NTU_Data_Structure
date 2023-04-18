#include <fstream>      
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <algorithm> 
#include <iterator> 

using namespace std;

typedef struct page_struct page_struct;
struct page_struct {
    float rank_val;
    int index; 
};

class Hash{
private:
	static const int hash_table_size=500;
	struct item{
		string word;
		int page_num;
		item* next; 
	};
	item* Hash_table[hash_table_size];
public:
	Hash();
	int Hash_fun(string key);
	void add_item(string word, int page_num);
	void search_word(string word,vector<page_struct>& word_vec,vector<float>& page_rank);
	//void reverse_index(string file_name);
	//void out_reverse_index(vector<vector<rev_index> >& reverse_index, string file_name);


};

#ifndef functions
#define functions
   
   void input_variable(string& list_file_name, string& input_dir, float& d, float& DIFF);
   vector<vector<float> > read_pages(string& dir_name, float ratio,Hash& hash_search, vector<string>& reverse_index);
   vector<string> getdir(string dir);
   vector<float> PageRank(vector<vector<float> > matrix,float d,float DIFF);
   
   vector<float> add_vector(vector<float> vec1,vector<float> vec2); 
   vector<float> matrix_multi_vec(vector<vector<float> > matrix,vector<float> k);
   float abs_vector(vector<float> vec1,vector<float> vec2);

   vector<page_struct> sort_rankpage(vector<float> page_rank);
   void write_rank_list(vector<page_struct> rank_id,float d,float DIFF);
   
   void write_reverse_index(Hash hash_search,vector<string> reverse_index,vector<float>& page_rank);

   void search_engine(string list_file_name, Hash& hash_search,float d,float DIFF, vector<float>& page_rank);
   void AND_OR_search_list(ofstream& outfile,vector<vector<page_struct> > words_index_vec);

   //For sorting and uniqing the page struct
   bool compare_string(string A, string B);
   bool compare_page_st(page_struct i1, page_struct i2);
   bool compare_page_st_same(page_struct A, page_struct B);

   //For the testing
   void print_matrix(vector<vector<float> > matrix);
   void print_vec(vector<float> vec);

#endif
#include "functions.h"


int main() 
{ 
	//default values
	float d=0.85;
	float DIFF=0.001;
	string input_dir="web-search-files2";
	string list_file_name="list.txt";	
	Hash hash_search;
	vector<string> reverse_index;

	input_variable(list_file_name,input_dir, d, DIFF);

    vector<vector<float> > matrix_d = read_pages(input_dir,d,hash_search,reverse_index);
    vector<float> page_rank=PageRank(matrix_d,d,DIFF);
    vector<page_struct> rank_list=sort_rankpage(page_rank);
    write_rank_list(rank_list,d,DIFF);
    write_reverse_index(hash_search, reverse_index,page_rank);
    search_engine(list_file_name, hash_search,d, DIFF,page_rank);

	return 0;
}
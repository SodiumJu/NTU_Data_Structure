#include "main.h"

int main() 
{ 
	
	int num_pw=100;
	int salt_range=1000;
	int table_size=2*num_pw*salt_range;

	Hash hash_table;
	hash_table.get_size(table_size);
	file_in(hash_table);
	search_pw(hash_table);

	return 0;
}
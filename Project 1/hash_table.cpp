#include "functions.h"

Hash::Hash()
{
	for(int i=0; i< hash_table_size; i++ ){
		Hash_table[i]= new item;
		Hash_table[i]->word="---";
		Hash_table[i]->page_num=-1;
		Hash_table[i]->next=NULL;
	}
}

void Hash::add_item(string word, int page_num){
	int index = Hash_fun(word);
	if(Hash_table[index]->word=="---"){
		Hash_table[index]->word=word;
		Hash_table[index]->page_num=page_num;
	}else{
		item* ptr = Hash_table[index];
		
		while(ptr->next!=NULL){
			ptr=ptr->next;
		}
		//while the word in the same page haven't been added yet, just 
		//add it
		if(ptr->page_num!=page_num){
			item* n = new item;
			n->word=word;
			n->page_num=page_num;
			n->next=NULL;
			ptr->next=n;
		}		
	}
}

int Hash::Hash_fun(string key){
	int hash = 0;
	int index;

	for(int i=0;i<key.length();i++){
		hash=hash+(int)key[i]*17;
	}
	index=hash%hash_table_size;
	return index;
}

void Hash::search_word(string word,vector<page_struct>& word_vec,vector<float>& page_rank){
	int index=Hash_fun(word);
	bool foundword = false;
	int page_num;
	page_struct initial={.rank_val=0, .index=0};

	item* ptr = Hash_table[index];
	while(ptr!=NULL){
		if(ptr->word==word){
			foundword=true;
			initial.index = ptr->page_num;
			initial.rank_val = page_rank[ptr->page_num];
			word_vec.push_back(initial);
		}
		ptr=ptr->next;
	}
}


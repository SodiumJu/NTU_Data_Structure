#include "main.h"

Hash::Hash()
{	

}

void Hash::get_size(int size){
	this->size=size;
	this->Hash_table = (item*)malloc(size * sizeof(item));
	for(int i=0;i<size;i++){
		Hash_table[i]=*(new item);
		//cout << Hash_table[i] << endl;
		Hash_table[i].password="";
		Hash_table[i].hash_val=-1;
		Hash_table[i].salt=-1;
		Hash_table[i].line=-1;
		//cout << Hash_table[i].password << " " << Hash_table[i].hash_val << endl;
	}
}

long long Hash::string_to_num(string& key){
	long long hash = 0;
	for(int i=0;i<key.length();i++){
		hash=(hash*100)+(int)key[i];
	}
	
	return hash;
}

int Hash::Hash_fun(long long string_num){
	long long right_num=string_num%10000000;
	long long left_num=string_num/10000000;
	int hash=((243*left_num)+right_num)%85767489;
	return hash;
}

int Hash::num_with_salt(string& key,int salt){
	long long hash=string_to_num(key);
	hash=hash+(long long)salt*1000000000000;
	int final_hash=Hash_fun(hash);
	return final_hash;
}

int Hash::main_hash(long long key){
	int hash = (1301*key)%((long long)this->size);
	return hash;
}

long long Hash::assist_Hash(long long& key){
	long long hash2 = 7919-(key%7919);
	return hash2;
}

void Hash::insert(int& final_hash,string& password,int& salt,int& line){
	long long long_final = (long long)final_hash;
	int hash_hash_val=main_hash(long_final);

	if(Hash_table[hash_hash_val].hash_val==-1){
		Hash_table[hash_hash_val].hash_val=final_hash;
		Hash_table[hash_hash_val].password=password;
		Hash_table[hash_hash_val].salt=salt;
		Hash_table[hash_hash_val].line=line;
	}else{
		do{
			hash_hash_val=((long long)hash_hash_val+assist_Hash(long_final))%(this->size);
		}while(Hash_table[hash_hash_val].hash_val!=-1);
			Hash_table[hash_hash_val].hash_val=final_hash;
			Hash_table[hash_hash_val].password=password;
			Hash_table[hash_hash_val].salt=salt;
			Hash_table[hash_hash_val].line=line;
	}

}

void Hash::find(int& final_hash,string& password,int& salt,int& line){
	long long long_final = (long long)final_hash;
	int hash_hash_val=main_hash(long_final);

	if(Hash_table[hash_hash_val].hash_val==final_hash){
		password=Hash_table[hash_hash_val].password;
		salt=Hash_table[hash_hash_val].salt;
		line=Hash_table[hash_hash_val].line;
	}else{
		while(Hash_table[hash_hash_val].hash_val!=final_hash & Hash_table[hash_hash_val].hash_val!=-1){
			hash_hash_val=((long long)hash_hash_val+assist_Hash(long_final))%(this->size);
		}
		if(Hash_table[hash_hash_val].hash_val==-1){			
			password="******";
			salt=-1;
			line=-1;
		}else{
			password=Hash_table[hash_hash_val].password;
			salt=Hash_table[hash_hash_val].salt;
			line=Hash_table[hash_hash_val].line;
		}
		
	}
}

#include "main.h"

void process_file_in(string& path,Hash& hashtable){
	ifstream infile(path);
	string strline;
	ofstream outfile;
	string file_name="Dictionary.txt";
	outfile.open(file_name, ios::out);
	int salt_range=1000;
	int final_hash;
	int line=0;

	while (getline (infile, strline))
    {	
    	if(strline.length()!=1){
    		int str_size=strline.length()-1;
	    	strline.resize(str_size); 
	    	//cout << strline << endl;
	    	for (int salt=0;salt<salt_range;salt++){
	    		line++;
	    		final_hash=hashtable.num_with_salt(strline,salt);
	    		output_pwdir_file(outfile,strline,final_hash,salt);
	    		hashtable.insert(final_hash,strline,salt,line);
    		}
    	}
    	
    }

  	outfile.close();
}

void output_pwdir_file(ofstream& outfile,string& password,int& final_hash,int& salt){
	char ssalt[3];
	sprintf(ssalt, "%03d", salt);
	outfile << password << " " << ssalt << " " << final_hash << endl;
}

void file_in(Hash& hashtable){
	string path;
	cout << "Enter the name of password file: (path/password.txt)" << endl;	
	//path="password.txt";
	cin >> path;
	process_file_in(path,hashtable);
}

void search_pw(Hash& hashtable){

	int input_hash;
	string password;
	int line;
	int salt;
	char ssalt[3];

	while(true){
		cout << "Input hash value:" << endl;
		cin >> input_hash;
		if(input_hash==-1){
			break;
		}
		hashtable.find(input_hash,password,salt,line);
		if(salt==-1){
			cout << input_hash << " " << password << " " << "***" << endl;
		}else{
			sprintf(ssalt, "%03d", salt);
			cout << input_hash << " " << password << " " << ssalt << " " << line << endl;
		}		
	}
	//cout << "Searching end" << endl;
}
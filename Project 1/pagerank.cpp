#include "functions.h"


string strline;
int prefix_len = 4;

vector<vector<float> > read_pages(string& dir_name,float ratio,Hash& hash_search, vector<string>& reverse_index) {
  vector<string> files = getdir(dir_name);
  int dim=files.size()-2+1; //add page 500 (total 501 pages)
  //int dim=files.size()-2;//minus .. and .
  cout << "The number of processing pages: " << dim << endl;

  vector<vector<float> > matrix( dim , vector<float> (dim, 0));
  vector<float> out_count(dim,0);

  for(int i=0; i<files.size(); i++){
        if(files[i][0]=='p'){ //check the file name starting with word p
          bool word_flag = false;
          string file_name = files[i];
          ifstream infile(dir_name+"/"+file_name);
          int page=stoi(file_name.substr(prefix_len));

          while (getline (infile, strline))
          {  
             if(word_flag==false){
                if(strline[0]=='-'){
                  word_flag=true;
                  continue;
                }else{
                  int page_in=stoi(strline.substr(prefix_len));
                  //fill in the matrix and the dividing out-counts 
                  out_count[page]++;
                  matrix[page_in][page]=1;
                }
                
             }else{ //reading words to hashtable
                int j=0,start=0;
                string word;
                while(j<strline.length()){
                  if(strline[j]=='\t' || strline[j]==' '){
                    word=strline.substr(start,j-start);
                    hash_search.add_item(word,page);
                    reverse_index.push_back(word);
                    start=j+1;
                  }
                  j++;
                }
                break;
             }          
          }
        }
    }


  for(int i=0; i<dim; i++){
      for(int j=0; j<dim; j++){
        if(out_count[j]>0)
          matrix[i][j]=(matrix[i][j]*ratio)/out_count[j];
      }
  }

  sort(reverse_index.begin(), reverse_index.end());
  vector<string>::iterator ip; 
  ip=unique(reverse_index.begin(), reverse_index.end(),compare_string);
  reverse_index.erase(ip,reverse_index.end());

  return matrix;   
}

void print_matrix(vector<vector<float> > matrix){
  for(int i=0;i<matrix.size();i++)
    {
        for(int j=0;j<matrix[i].size();j++)
        {
            printf("%.3lf ",matrix[i][j]);
        }
        cout<< endl;
    }
}

void print_vec(vector<float> vec){
  for(int i=0;i<vec.size();i++)
    //printf("%.5lf ",vec[i]);
    cout << vec[i] << " ";
  cout << endl;
}

vector<string> getdir(string dir){
    vector<string> files = vector<string>();
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL){
        cout << "Error(" << errno << ") opening " << dir << endl;
        return files;
    }
    while((dirp = readdir(dp)) != NULL){
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    
    return files;
}

vector<float> matrix_multi_vec(vector<vector<float> > matrix,vector<float> k){
  vector<float> rank_vec(matrix.size(), 0);
  for(int i=0;i<k.size();i++){
    for(int j=0;j<k.size();j++){
      rank_vec[i]=rank_vec[i]+matrix[i][j]*k[j];
    }
  }
  return rank_vec;
}

float abs_vector(vector<float> vec1,vector<float> vec2){
  float diff=0;
  float dis=0;
  for(int i=0;i<vec1.size();i++){
    dis=vec1[i]-vec2[i];
    if(dis>=0){
      diff=diff+dis;
    }else{
      diff=diff-dis;
    }
  }
  return diff;
}
vector<float> add_vector(vector<float> vec1,vector<float> vec2){
  vector<float> add_vec;
  for(int i=0;i<vec1.size();i++){
    add_vec.push_back(vec1[i]+vec2[i]);
  }
  return add_vec;
}


vector<float> PageRank(vector<vector<float> > matrix,float d,float DIFF){
  float diff=DIFF;
  int dim=matrix.size();
  vector<float> new_vec;
  vector<float> rank_vec=vector<float> (dim, (float)1/dim);
  vector<float> re_vec=vector<float> (dim, ((float)1-d)/dim); //(1/N)*(1-d)
  int test_count=0;
  
  while(diff>=DIFF){

    new_vec=add_vector(matrix_multi_vec(matrix,rank_vec),re_vec);
    diff=abs_vector(rank_vec,new_vec);
    rank_vec=new_vec;
    test_count++;
  }
  cout << "Iteration counts: " << test_count << endl;
  cout << "Difference: " << diff << endl;
  return rank_vec;
}

bool compare_page_st(page_struct i1, page_struct i2) 
{ 
    if(i1.rank_val > i2.rank_val){
      return 1;
    }else if(i1.rank_val == i2.rank_val){
      return (i1.index < i2.index);
    }else{
      return 0;
    }
} 

vector<page_struct> sort_rankpage(vector<float> page_rank){
    page_struct initial={.rank_val=0, .index=0};
    vector<page_struct> rank_id(page_rank.size(), initial);;
    for(int i=0; i<page_rank.size();i++){
      rank_id[i].rank_val=page_rank[i];
      rank_id[i].index=i;
    }
    sort(rank_id.begin(), rank_id.end(), compare_page_st);
    return rank_id;
}

void write_rank_list(vector<page_struct> rank_id,float d,float DIFF){
  ofstream outfile;
  string file_name="pr_"+to_string(d).substr(2,2)+"_"+to_string(DIFF).substr(2,3)+".txt";
  outfile.open(file_name, ios::out);
  for(int i=0; i<rank_id.size(); i++){
    outfile << "Page" << rank_id[i].index << "\t" << i+1 << "\t" << rank_id[i].rank_val << endl;
  }
  outfile.close();
}

void write_reverse_index(Hash hash_search,vector<string> reverse_index,vector<float>& page_rank){
  ofstream outfile;
  string file_name="reverseindex.txt";
  outfile.open(file_name, ios::out);
  vector<page_struct> word_vec;

  for(int i=0; i<reverse_index.size(); i++){
    word_vec.clear();
    hash_search.search_word(reverse_index[i], word_vec , page_rank);
    outfile << reverse_index[i];
    for(int j=0; j<word_vec.size(); j++){
      outfile << " " << word_vec[j].index;
    }
    outfile << endl;
  }

  outfile.close();
}

bool compare_string(string A, string B){
  if(A.length()==B.length()){
    for(int i=0; i<A.length(); i++){
      if(A[i]!=B[i])
        return 0;
    }
    return 1;
  }else{
    return 0;
  }
}

bool compare_page_st_same(page_struct A, page_struct B){
  if(A.index==B.index){
      return 1;
    }else{
      return 0;
    }
}

void input_variable(string& list_file_name, string& input_dir, float& d, float& DIFF){
  cout << "Input the file path of search directory (web-search-files2):" << endl;
  cin >> input_dir;
  cout << "Input the arguments of damping factor in PageRank:" << endl;
  cin >> d;
  cout << "Input the arguments of difference:" << endl;
  cin >> DIFF;
  cout << "Input the file path of search words file (list.txt):" << endl;
  cin >> list_file_name;
}

void search_engine(string list_file_name, Hash& hash_search,float d,float DIFF, vector<float>& page_rank){
  ifstream infile(list_file_name);
  string strline, word;
  int j, start;
  bool multi_flag;
  vector<string> words;
  vector<page_struct> index_vec;
  vector<vector<page_struct> > words_index_vec;

  ofstream outfile;
  string file_name="result_"+to_string(d).substr(2,2)+"_"+to_string(DIFF).substr(2,3)+".txt";
  outfile.open(file_name, ios::out);

  while (getline (infile, strline)){
    words_index_vec.clear();
    multi_flag=false;
    j=0;start=0;
    words.clear();
    while(j<strline.length()){
      word.clear();
      if(strline[j]=='\t' || strline[j]==' '){
        multi_flag=true;
        word=strline.substr(start,j-start);
        start=j+1;
        words.push_back(word);
      }else if(j==strline.length()-1){
        word=strline.substr(start,j-start);
        words.push_back(word);
      }     
      j++;
    }
    if(multi_flag){
      for(int i=0; i<words.size(); i++){
        hash_search.search_word(words[i],index_vec,page_rank);
        words_index_vec.push_back(index_vec);     
        index_vec.clear();
      }
      AND_OR_search_list(outfile,words_index_vec);
      
    }else{
      hash_search.search_word(words[0],index_vec,page_rank);
      if(index_vec.size()==0){
        outfile << " none" << endl;
      }else{
        sort(index_vec.begin(), index_vec.end(), compare_page_st);

        for(int i=0; i<index_vec.size(); i++){
          outfile << " " << index_vec[i].index;
        }
        outfile << endl;
        index_vec.clear();
      }     
    }
  }
  outfile.close();
}

void AND_OR_search_list(ofstream& outfile,vector<vector<page_struct> > words_index_vec){

  vector<page_struct> tmp_vec;
  vector<page_struct>::iterator ip;
  int num_words =  words_index_vec.size();

  for(int i=0; i<num_words; i++){
    tmp_vec.insert(tmp_vec.end(), words_index_vec[i].begin(),words_index_vec[i].end());  
  }
  sort(tmp_vec.begin(), tmp_vec.end(), compare_page_st); 

  bool no_AND=true;
  int len=0;
  outfile << "AND";
  int tmp_size=tmp_vec.size()-1;
  for(int k =0; k<tmp_size; k++){
    if(tmp_vec[k].index==tmp_vec[k+1].index){
      len++;
      if(len==num_words-1){
        no_AND=false;
        outfile << " " << tmp_vec[k].index;
      } 
    }else{
      len=0;
    }    
  }
  if(no_AND)
    outfile << " none";

  outfile << endl;
  outfile << "OR";    
  ip=unique(tmp_vec.begin(), tmp_vec.end(),compare_page_st_same);
  tmp_vec.erase(ip,tmp_vec.end());
  for(int k=0; k<tmp_vec.size(); k++)
    outfile << " " << tmp_vec[k].index;
  outfile << endl;
}



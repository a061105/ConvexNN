#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const int LINE_LEN = 10000000;
typedef vector<pair<int,double> > SparseVec;
typedef vector<SparseVec> SparseMat;

void print(ostream& out, SparseVec& x){

				for(SparseVec::iterator it=x.begin();it!=x.end();it++){
								out << it->first << ":" << it->second << " ";
				}	
}

void print(ostream& out, SparseMat& X){

				for(int i=0;i<X.size();i++){
								print(out, X[i]);
								out << endl;
				}
}

double inner_prod(double* w, SparseVec& xi){

				double sum = 0.0;
				for(SparseVec::iterator it=xi.begin(); it!=xi.end(); it++){
								sum += w[it->first]*it->second;
				}
				return sum;
}

void subXTv(vector<int>& sub_ind, SparseMat& X, int N, int D, double* v, double* out){
				
				for(int j=0;j<D;j++)
								out[j] = 0.0;
				for(vector<int>::iterator it=sub_ind.begin(); it!=sub_ind.end(); it++){
						int i = *it;
						double vi = v[i];
						SparseVec& xi = X[i];
						for(SparseVec::iterator it=xi.begin(); it!=xi.end(); it++)
								out[it->first] += vi*it->second;
				}
}

void subXv(vector<int>& sub_ind, SparseMat& X, int N, int D, double* v,  double* out){
				
				for(vector<int>::iterator it=sub_ind.begin(); it!=sub_ind.end(); it++){
						int i = *it;
						out[i] = inner_prod(v, X[i]);
				}
}


vector<string> split(string str, string pattern){

	vector<string> str_split;
	size_t i=0;
	size_t index=0;
	while( index != string::npos ){

		index = str.find(pattern,i);
		str_split.push_back(str.substr(i,index-i));

		i = index+1;
	}
	
	if( str_split.back()=="" )
		str_split.pop_back();

	return str_split;
}

#endif

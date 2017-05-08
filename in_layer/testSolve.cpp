#include "util.h"
#include "solver.h"
#include <stdlib.h>

void readData(char* fname, SparseMat& X, vector<double>& z, int& D)
{
	
	ifstream fin(fname);
	char* line = new char[LINE_LEN];
	int d = -1;
	while( !fin.eof() ){
		
		fin.getline(line, LINE_LEN);
		string line_str(line);
		
		if( line_str.length() < 2 && fin.eof() )
			break;
		size_t found = line_str.find("  ");
		while (found != string::npos){
			line_str = line_str.replace(found, 2, " ");
			found = line_str.find("  ");
		}
		found = line_str.find(", ");
		while (found != string::npos){
			line_str = line_str.replace(found, 2, ",");
			found = line_str.find(", ");
		}
		vector<string> tokens = split(line_str, " ");
		//get label index
		int st = 0;
		while (st < tokens.size() && tokens[st].find(":") == string::npos){
			// truncate , out
			if (tokens[st].size() == 0){
				st++;
				continue;
			}
			vector<string> subtokens = split(tokens[st], ",");
			for (vector<string>::iterator it_str = subtokens.begin(); it_str != subtokens.end(); it_str++){
				string str = *it_str;
				if (str == "" || str == " ")
					continue;
				z.push_back( atoi(str.c_str()) );
			}
			st++;
		}
		
		X.push_back(SparseVec());
		SparseVec& x = X.back();
		for(int i=st;i<tokens.size();i++){
			vector<string> kv = split(tokens[i],":");
			int ind = atoi(kv[0].c_str());
			double val = atof(kv[1].c_str());
			x.push_back(make_pair(ind,val));
			if( ind > d )
				d = ind;
		}
	}
	fin.close();
	D = d + 1; //bias
	
	delete[] line;
}

int main(int argc, char** argv){

		if( argc < 1+1 ){
				cerr << "usage: testSolve [train_data]" << endl;
				exit(0);
		}
		
		char* train_fpath = argv[1];
		
		SparseMat X;
		vector<double> z;
		int N, D;
		readData(train_fpath, X, z, D);
		N = X.size();

		cerr << "N=" << N << ", D=" << D << endl;

		double* w = new double[D];
		double* a = new double[N];
		tron_solve(X,N,D,z.data(), a, w);
		
		double acc = 0.0;
		for(int i=0;i<N;i++){
				double r = inner_prod(w, X[i]);
				if( r*z[i] > 0.0 ){
						acc += 1.0;
				}
		}
		acc /= N;

		cerr << "Acc=" << acc << endl;
}

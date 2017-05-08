#include "mex.h"
#include "util.h"
#include "solver.h"

void usage(string err_msg="")
{
		string str = string("Usage: [a,w] = InputLayerSolve(X,z)\n")+string(err_msg);
		mexErrMsgTxt(str.c_str());
}

void mexFunction(int nlhr, mxArray *plhs[], int nrhs, const mxArray* prhs[]){
	
	if( nrhs != 2 )
					usage();
	
	//1st input
	int D = mxGetM(prhs[0]);
	int N = mxGetN(prhs[0]);
	double* X_val = mxGetPr(prhs[0]);
	size_t* X_ir = mxGetIr(prhs[0]);
	size_t* X_jc = mxGetJc(prhs[0]);
	if( ! mxIsSparse(prhs[0]) ){
			usage("1st input X must be a sparse matrix.");
	}

	SparseMat X; //N by D (transposed of matlab's X)
	X.resize(N);
	for(int j=0;j<N;j++){
			size_t start = X_jc[j];
			size_t end = X_jc[j+1];
			for(int r=start;r<end;r++){
					size_t i = X_ir[r];
					double v = X_val[r];
					X[j].push_back(make_pair(i,v));
			}
	}
	
	//2nd input
	int N2 = mxGetM(prhs[1]);
	if( mxGetN(prhs[1]) != 1 || N != N2 ){
			usage("dimension must match. (X:D*N sparse, z:N*1)");
	}
	double* z = mxGetPr(prhs[1]);
	
  plhs[0]	= mxCreateDoubleMatrix(N,1,mxREAL);
	plhs[1] = mxCreateDoubleMatrix(D,1,mxREAL);
	double* a = mxGetPr(plhs[0]);
	double* w = mxGetPr(plhs[1]);
	
	tron_solve(X,N,D,z, a, w);
}

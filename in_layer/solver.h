#include "tron.h"
#include "util.h"
#include "loss_function.h"

void tron_solve(SparseMat& X, int N, int D, double* z, double* a, double* w){
		
		double rho = 1.0;
		double gamma = 10.0;
		double tol = 1e-4;
		
		function* sign_loss_func = new sign_loss_function(rho, gamma, X, N, D, z);
		TRON tron_solver( sign_loss_func, tol );
		
		tron_solver.tron(w);
		
		delete sign_loss_func;
}

#include "tron.h"
#include "util.h"

class sign_loss_function:public function{

		public:
		sign_loss_function(double _rho, double _gamma, SparseMat& _X, int _N, int _D, double* _z){
			
				rho = _rho;
				gamma = _gamma;
				X = _X;
				z = _z;
				N = _N;
				D = _D;
				
				resi = new double[N];
		}

		~sign_loss_function(){
				delete[] resi;
		}
		
		int get_nr_variable(){
				return D;
		}
		
		double fun(double* w){
				
				double obj = 0.0;
				//loss part
				nz_ind.clear();
				for(int i=0;i<N;i++){
						double ri = inner_prod(w,X[i]);
						double zi = z[i];
						if( ri >= zi+rho ){
								obj += 1.0 + ri - zi - zi*ri - rho/2.0;
								
								if( zi == -1.0 ){//compute residual for gradient
										nz_ind.push_back(i);
										resi[i] = 1.0-zi;
								}
						}else if( ri <= zi-rho ){
								obj += 1.0 - ri + zi - zi*ri - rho/2.0;
								
								if( zi == 1.0 ){
										nz_ind.push_back(i);
										resi[i] = -1.0-zi;
								}
						}else{
								obj += 1.0 + (ri-zi)*(ri-zi)/2.0/rho - zi*ri;

								nz_ind.push_back(i);
								resi[i] = (ri-zi)/rho - zi;
						}
				}
				obj *= gamma;

				//regularization part
				double reg = 0.0;
				for(int j=0;j<D;j++)
						reg += w[j]*w[j];
				reg /= 2.0;
				obj+= reg;
				
				return obj;
		}
		
		/* Assume residual has been computed in fun(..)
		 */
		void grad(double* w, double* g){
				
				//loss part
				subXTv(nz_ind, X, N, D, resi, g);
				
				//reg part
				for(int j=0;j<D;j++){
						g[j] = w[j] + gamma*g[j];
				}
		}
		
		/* Assume residual has been computed in fun(...)
		 */
		void Hv(double* s, double* Hs){
				
				double* tmp = new double[N];
				//loss part
				subXv( nz_ind, X, N, D, s, tmp );
				for(vector<int>::iterator it=nz_ind.begin(); it!=nz_ind.end(); it++)
								tmp[*it] /= rho;

				subXTv( nz_ind, X, N, D, tmp, Hs);

				//reg part
				for(int j=0;j<D;j++)
						Hs[j] = s[j] + gamma*Hs[j];
		}
		
		private:
		/** The following are read-only within the class
		 */
		int N;
		int D;
		SparseMat X; //feature matrix
		double* z;  //label {-1,+1}
		double rho;  //smoothing parameter (larger smoother)
		double gamma; //weight of loss compared to regularizer.
		////////////

		/**The following are manipulated within the class
		 */
		double* resi;   //values of non-zero residuals
		vector<int> nz_ind; //indexes of non-zero residuals (out of N)
};

#ifndef METAHEURISTICA_BUSCALOCAL_H
#define METAHEURISTICA_BUSCALOCAL_H

#include "CaixeiroViajanteMetaHeuristica.h"

class BuscaLocal {
	
	private:		
		bool bestImprovementSwap( double** matrizAdj, int dim, Solucao* s );
		bool bestImprovement2Opt( double** matrizAdj, int dim, Solucao* s );
		bool bestImprovementOrOpt( double** matrizAdj, int dim, Solucao* s, int k );
		
		double calculaSwapCusto( double** matrizAdj, int dim, int i, int j );
		double calcula2OptCusto( double** matrizAdj, int dim, int i, int j );
		double calculaOrOptCusto( double** matrizAdj, int dim, int i, int j, int k );
		
		void exec2Opt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j );
		void execOrOpt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j, int k );
			
	public:		
		void buscaLocal( double** matrizAdj, int dim, Solucao* solucao );
	
};

#endif
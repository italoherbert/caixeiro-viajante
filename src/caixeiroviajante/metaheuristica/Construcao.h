#ifndef METAHEURISTICA_CONSTRUCAO_H
#define METAHEURISTICA_CONSTRUCAO_H

#include "CaixeiroViajanteMetaHeuristica.h"

class Construcao {
	
	private:		
		vector<InsersaoInfo> calculaInsercaoInfos( double** matrizAdj, int dim, vector<int>& sequencia, int k );
		void ordenaPorCusto( vector<InsercaoInfo>& insersaoInfos );
			
	public:		
		Solucao construcao( double** matrizAdj, int dim );
	
};

#endif
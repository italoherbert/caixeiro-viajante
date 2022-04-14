#ifndef METAHEURISTICA_CONSTRUCAO_H
#define METAHEURISTICA_CONSTRUCAO_H

#include "CaixeiroViajanteMetaHeuristica.h"

typedef struct InsercaoInfo {
	int noI;
	double custo;
} InsersaoInfo;

class Construcao {
	
	private:		
		vector<InsersaoInfo> calculaInsercaoInfos( double** matrizAdj, int dim, vector<int>& sequencia, int k );
			
	public:		
		Solucao construcao( double** matrizAdj, int dim );
	
};

#endif
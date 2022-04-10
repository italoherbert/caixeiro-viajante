#ifndef CAIXEIRO_VIAJANTE_METAHEURISTICA_H
#define CAIXEIRO_VIAJANTE_METAHEURISTICA_H

#include "../CaixeiroViajante.h"

typedef struct InsercaoInfo {
	int noI;
	double custo;
} InsersaoInfo;

class CaixeiroViajanteMetaHeuristica : public CaixeiroViajante {
	
	private:
		vector<InsersaoInfo> calculaInsercaoInfos( vector<int>& sequencia, int k );
		void ordenaPorCusto( vector<InsercaoInfo>& insersaoInfos );
	
	public:
		Solucao construcao();
		Solucao calculaCaminho();
	
};

#endif
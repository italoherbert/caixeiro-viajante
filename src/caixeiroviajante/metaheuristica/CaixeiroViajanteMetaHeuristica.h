#ifndef CAIXEIRO_VIAJANTE_METAHEURISTICA_H
#define CAIXEIRO_VIAJANTE_METAHEURISTICA_H

#include <vector>

#include "../CaixeiroViajante.h"

typedef struct InsercaoInfo {
	int noI;
	double custo;
} InsersaoInfo;

class CaixeiroViajanteMetaHeuristica : public CaixeiroViajante {
		
	private:		
		void perturbacao( vector<int>& sequencia );	
		
	public:	
		CaixeiroViajanteMetaHeuristica();
		Solucao calculaCaminho();
	
};

#endif
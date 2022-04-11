#ifndef CAIXEIRO_VIAJANTE_METAHEURISTICA_H
#define CAIXEIRO_VIAJANTE_METAHEURISTICA_H

#include "../CaixeiroViajante.h"

typedef struct InsercaoInfo {
	int noI;
	double custo;
} InsersaoInfo;

class CaixeiroViajanteMetaHeuristica : public CaixeiroViajante {
		
	public:						
		Solucao calculaCaminho();
	
};

#endif
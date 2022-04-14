#ifndef CAIXEIRO_VIAJANTE_METAHEURISTICA_H
#define CAIXEIRO_VIAJANTE_METAHEURISTICA_H

#include <vector>

#include "../CaixeiroViajante.h"

class CaixeiroViajanteMetaHeuristica : public CaixeiroViajante {
						
	public:	
		Solucao calculaCaminho();
	
};

#endif
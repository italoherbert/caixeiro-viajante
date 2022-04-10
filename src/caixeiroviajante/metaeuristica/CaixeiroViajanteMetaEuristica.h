#ifndef CAIXEIRO_VIAJANTE_METAEURISTICA_H
#define CAIXEIRO_VIAJANTE_METAEURISTICA_H

#include "../CaixeiroViajante.h"

class CaixeiroViajanteMetaEuristica : public CaixeiroViajante {
	
	public:
		Solucao construcao();
		Solucao calculaCaminho();
	
};

#endif
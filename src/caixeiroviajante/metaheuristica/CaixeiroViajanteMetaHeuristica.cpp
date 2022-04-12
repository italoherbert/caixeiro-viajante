#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

#include "CaixeiroViajanteMetaHeuristica.h"
#include "Construcao.h"
#include "BuscaLocal.h"

Solucao CaixeiroViajanteMetaHeuristica::calculaCaminho() {	
	Construcao c;	
	BuscaLocal bl;
	
	Solucao solucao = c.construcao( matrizAdj, dim );
	//bl.buscaLocal( matrizAdj, dim, &solucao );		
	
	cout << "Custo total1: " << this->calculaCusto( solucao.sequencia ) << endl;
	cout << "Custo total11: " << solucao.valorObj << endl;
	
	int custo = bl.calculaOrOptCusto( matrizAdj, dim, solucao.sequencia, 5, 9, 2 );	
	bl.execOrOpt( matrizAdj, dim, solucao.sequencia, 5, 9, 2 );
	
	cout << "Custo: " << custo << endl;
	
	cout << "Custo total2: " << this->calculaCusto( solucao.sequencia ) << endl;
	cout << "Custo total22: " << solucao.valorObj + custo << endl;

	return solucao;
}


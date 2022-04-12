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
	bl.buscaLocal( matrizAdj, dim, &solucao );		
		
	this->perturbacao( solucao.sequencia );

	bl.buscaLocal( matrizAdj, dim, &solucao );		

	solucao.valorObj = 0;
	for( int i = 0; i < solucao.sequencia.size()-1; i++ ) {
		int j = ( i < solucao.sequencia.size() - 1 ? i+1 : 0 );
		solucao.valorObj += matrizAdj[ solucao.sequencia[ i ] ][ solucao.sequencia[ j ] ];
	}

	return solucao;
}

void CaixeiroViajanteMetaHeuristica::perturbacao( vector<int>& sequencia ) {
	int pinter1 = 2;
	int pinter2 = max( (int)(sequencia.size() / 10 ), 4 );
	
	int r = rand() % ( sequencia.size() - pinter1 );
	int r2 = rand() % ( sequencia.size() - pinter2 );
	while( ( r2 >= r && r2 <= r+pinter1 ) || ( r2+pinter2 >= r && r2+pinter2 <= r+pinter1 ) )
		r2 = rand() % ( sequencia.size() - pinter2 );
		
	vector<int> v1;
	for( int i = 0; i < pinter1; i++ )
		v1.push_back( sequencia[ r+i ] );
		
	vector<int> v2;
	for( int i = 0; i < pinter2; i++ )
		v2.push_back( sequencia[ r2+i ] );
		
	sequencia.erase( sequencia.begin() + r, sequencia.begin() + r+pinter1 );
	sequencia.erase( sequencia.begin() + r2-pinter1, sequencia.begin() + r2-pinter1+pinter2 ); 

	if ( r < r2 ) {		
		sequencia.insert( sequencia.begin() + r, v2.begin(), v2.end() );
		sequencia.insert( sequencia.begin() + r2, v1.begin(), v1.end() );
	} else {
		sequencia.insert( sequencia.begin() + r2, v1.begin(), v1.end() );
		sequencia.insert( sequencia.begin() + r, v2.begin(), v2.end() );
	}				
}
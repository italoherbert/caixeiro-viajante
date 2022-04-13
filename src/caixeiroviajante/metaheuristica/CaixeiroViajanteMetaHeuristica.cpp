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

CaixeiroViajanteMetaHeuristica::CaixeiroViajanteMetaHeuristica() {
	
}

Solucao CaixeiroViajanteMetaHeuristica::calculaCaminho() {	
	Construcao c;	
	BuscaLocal bl;
	
	vector<int> globalMelhorSequencia;
	vector<int> melhorSequencia;
	
	double globalMelhorCusto = DBL_MAX;
	double melhorCusto = DBL_MAX;				
	
	for( int i = 0; i < 5; i++ ) {
		Solucao s = c.construcao( matrizAdj, dim );
		
		melhorSequencia.clear();
		copy( s.sequencia.begin(), s.sequencia.end(), back_inserter( melhorSequencia ) );
		melhorCusto = s.custo;
				
		int j = 0;
		while( j < 10 ) {			
			bl.buscaLocal( matrizAdj, dim, &s );			
			
		    if ( s.custo < melhorCusto ) {
				melhorSequencia.clear();
				copy( s.sequencia.begin(), s.sequencia.end(), back_inserter( melhorSequencia ) );
				melhorCusto = s.custo;
				j = 0;
			}			
						
			s.sequencia.clear();
			copy( melhorSequencia.begin(), melhorSequencia.end(), back_inserter( s.sequencia ) );
			s.custo = melhorCusto;			
								
			this->perturbacao( s.sequencia );
			s.custo = 0;
			for( int k = 0; k < s.sequencia.size(); k++ ) {
				int k2 = ( k < s.sequencia.size()-1 ? k+1 : 0 );
				s.custo += matrizAdj[ s.sequencia[ k ] ][ s.sequencia[ k2 ] ];	
			}
			j++;
		}				
				
		if ( melhorCusto < globalMelhorCusto ) {		
			globalMelhorSequencia.clear();
			copy( melhorSequencia.begin(), melhorSequencia.end(), back_inserter( globalMelhorSequencia ) );
			globalMelhorCusto = melhorCusto;
		}
	}			
	
	Solucao solucao = { globalMelhorSequencia, globalMelhorCusto };	
	return solucao;
}

void CaixeiroViajanteMetaHeuristica::perturbacao( vector<int>& sequencia ) {
	srand( time( NULL ) );
	
	int pinter1 = 2;
	int pinter2 = max( (int)(sequencia.size() / 10 ), 4 );
	
	int r = rand() % ( sequencia.size() - pinter1 - 2 ) + 1;
	int r2 = rand() % ( sequencia.size() - pinter2 - 2 ) + 1;
	while( ( r2 <= r && r2+pinter2 >= r ) || ( r2 <= r+pinter1 ))
		r2 = rand() % ( sequencia.size() - pinter2 - 2 ) + 1;
		
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
		sequencia.insert( sequencia.begin() + r2+pinter2-pinter1, v1.begin(), v1.end() );
	} else {
		sequencia.insert( sequencia.begin() + r2, v1.begin(), v1.end() );
		sequencia.insert( sequencia.begin() + r+pinter1-pinter2, v2.begin(), v2.end() );
	}					
}
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
#include "Perturbacao.h"

Solucao CaixeiroViajanteMetaHeuristica::calculaCaminho() {	
	Construcao c;	
	BuscaLocal bl;
	Perturbacao per;
		
	Solucao globalMelhor;
	globalMelhor.custo = DBL_MAX;	
	
	int	maxIter = 10;
	int maxIterILS = dim;
	
	if ( dim >= 100 ) {	
		maxIterILS /= 2;		
		//maxIter *= 2;
	}
		
	int sequencias[ maxIter ][ dim ];
								
	for( int i = 0; i < maxIter; i++ ) {				
		Solucao s = c.construcao( matrizAdj, dim );
		Solucao melhor = s;				
						
		if ( i > 0 ) {							
			double melhorDist = 0;
			for ( int ii = 0; ii < i; ii++ ) 
				for( int jj = 0; jj < dim; jj++ )
					melhorDist += abs( sequencias[ ii ][ jj ] - s.sequencia[ jj ] );				
				
			int k = 2;
			do {
				s = c.construcao( matrizAdj, dim );
								
				double dist = 0;
				for ( int ii = 0; ii < i; ii++ ) 
				for( int jj = 0; jj < dim; jj++ )
					dist += abs( sequencias[ ii ][ jj ] - s.sequencia[ jj ] );
	
				if ( dist > melhorDist ) {				
					melhor = s;
					melhorDist = dist;
				}
			} while( k++ < maxIterK );
		}

		int j = 0;
		while( j < maxIterILS ) {		
			bl.buscaLocal( matrizAdj, dim, &s );
						
		    if ( s.custo < melhor.custo ) {		    	
				melhor = s;
				j = 0;
			}			
			
			s = melhor;
						
			per.perturbacao( s.sequencia );

			s.custo = 0;
			for( int k = 0; k < s.sequencia.size()-1; k++ )
				s.custo += matrizAdj[ s.sequencia[ k ] ][ s.sequencia[ k+1 ] ];				
			j++;			
		}							
					
		if ( melhor.custo < globalMelhor.custo ) {								
			globalMelhor = melhor;				
		}
		
		for( int jj = 0; jj < melhor.sequencia.size(); jj++ )
				sequencias[ i ][ jj ] = melhor.sequencia[ jj ];
													
		cout << i+1 << "  ";
	}			
	cout << endl;
				
	return globalMelhor;
}



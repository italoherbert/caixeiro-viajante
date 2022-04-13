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
	
	vector<int> globalMelhorSequencia;
	vector<int> melhorSequencia;
	
	double globalMelhorCusto = DBL_MAX;
	double melhorCusto = DBL_MAX;	
	
	int	maxIter = 50;
	int maxIterILS = dim;
	
	if ( maxIterILS > 150 )	
		maxIterILS /= 2;
					
	for( int i = 0; i < maxIter; i++ ) {
		cout << endl << endl;
		cout << "Construcao(" << (i+1) << " de " << maxIter << ") -> ";
		Solucao s = c.construcao( matrizAdj, dim );
		
		melhorSequencia.clear();
		copy( s.sequencia.begin(), s.sequencia.end(), back_inserter( melhorSequencia ) );
		melhorCusto = s.custo;
								
		int j = 0;
		while( j < maxIterILS ) {		
			cout << (j+1) << "  ";
			bl.buscaLocal( matrizAdj, dim, &s );
						
		    if ( s.custo < melhorCusto ) {
				melhorSequencia.clear();
				copy( s.sequencia.begin(), s.sequencia.end(), back_inserter( melhorSequencia ) );
				melhorCusto = s.custo;
				j = 0;
			}			
						
			s.sequencia.clear();
			copy( melhorSequencia.begin(), melhorSequencia.end(), back_inserter( s.sequencia ) );
						
			per.perturbacao( s.sequencia );
			s.custo = 0;
			for( int k = 0; k < s.sequencia.size()-1; k++ )
				s.custo += matrizAdj[ s.sequencia[ k ] ][ s.sequencia[ k+1 ] ];				
			j++;
		}				
				
		if ( melhorCusto < globalMelhorCusto ) {		
			globalMelhorSequencia.clear();
			copy( melhorSequencia.begin(), melhorSequencia.end(), back_inserter( globalMelhorSequencia ) );
			globalMelhorCusto = melhorCusto;
		}
	}			
	
	cout << endl << endl;
	
	Solucao solucao = { globalMelhorSequencia, globalMelhorCusto };	
	return solucao;
}

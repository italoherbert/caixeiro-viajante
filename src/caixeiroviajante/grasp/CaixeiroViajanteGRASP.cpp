#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

#include "CaixeiroViajanteGRASP.h"
#include "../logica/Construcao.h"
#include "../logica/BuscaLocal.h"
#include "../logica/Perturbacao.h"

Solucao CaixeiroViajanteGRASP::calculaCaminho() {
	Construcao c;
	BuscaLocal bl;
	Perturbacao per;

	Solucao globalMelhor;
	globalMelhor.custo = DBL_MAX;

	int	maxIter = 30;
	int maxIterILS = dim;

	int maxIterK = 10;
	int maxSeqsPiores = 10;

	if ( dim >= 150 )
		maxIterILS /= 2;

	Solucao piores[ maxIter ];

	for( int i = 0; i < maxIter; i++ ) {
		Solucao s = c.construcao( matrizAdj, dim );
		Solucao melhor = s;

		if ( i > 0 ) {
			double melhorDist = 0;
			for ( int ii = 0; ii < i; ii++ )
				for( int jj = 0; jj < dim; jj++ )
					melhorDist += abs( piores[ ii ].sequencia[ jj ] - s.sequencia[ jj ] );

			for( int k = 1; k < maxIterK; k++ ) {
				s = c.construcao( matrizAdj, dim );

				double dist = 0;
				for ( int ii = 0; ii < i; ii++ )
					for( int jj = 0; jj < dim; jj++ )
						dist += abs( piores[ ii ].sequencia[ jj ] - s.sequencia[ jj ] );

				if ( dist > melhorDist ) {
					melhor = s;
					melhorDist = dist;
				}
			}
		}

		Solucao pior = melhor;

		int j = 0;
		while( j < maxIterILS ) {
			bl.buscaLocal( matrizAdj, dim, &s );

		    if ( s.custo < melhor.custo ) {
				melhor = s;
				j = 0;
			}

			if ( s.custo > pior.custo )
				pior = s;

			s = melhor;

			per.perturbacao( s.sequencia );

			s.custo = 0;
			for( int k = 0; k < s.sequencia.size()-1; k++ )
				s.custo += matrizAdj[ s.sequencia[ k ] ][ s.sequencia[ k+1 ] ];
			j++;
		}

		if ( melhor.custo < globalMelhor.custo )
			globalMelhor = melhor;

		piores[ i ] = pior;

		cout << i+1 << "  ";
	}
	cout << endl;

	return globalMelhor;
}



#include "BuscaLocal.h"

#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

void BuscaLocal::buscaLocal( double** matrizAdj, int dim, Solucao* s ) {
	vector<int> opcoes = { 0, 1 };
		
	while( !opcoes.empty() ) {
		int r = rand() % opcoes.size();
		bool houveAproveitamento = false;
		switch( opcoes[ r ] ) {
			case 0:
				houveAproveitamento = this->bestImprovementSwap( matrizAdj, dim, s );
				break;
			case 1:
				houveAproveitamento = this->bestImprovement2Opt( matrizAdj, dim, s );
				break;			
			case 2:
				houveAproveitamento = this->bestImprovementOrOpt( matrizAdj, dim, s, 1 );
				break;
			case 3:
				houveAproveitamento = this->bestImprovementOrOpt( matrizAdj, dim, s, 2 );
				break;
			case 4: 
				houveAproveitamento = this->bestImprovementOrOpt( matrizAdj, dim, s, 3 );
				break;			
		}
		
		if ( !houveAproveitamento ) {
			opcoes = { 0, 1, 2, 3, 4 };
		} else {
			opcoes.erase( opcoes.begin() + r );
		}
	}
}

bool BuscaLocal::bestImprovementSwap( double** matrizAdj, int dim, Solucao* s ) {
	double melhorDelta = 0;
	double melhorI, melhorJ;
	for( int i = 1; i < s->sequencia.size(); i++ ) {
		for( int j = i+1; j < s->sequencia.size(); j++ ) {
			double delta = this->calculaSwapCusto( matrizAdj, dim, i, j );
			if ( delta < melhorDelta ) {
				melhorI = i;
				melhorJ = j;
				melhorDelta = delta;
			}
		}
	}
	if ( melhorDelta < 0 ) {
		swap( s->sequencia[ melhorI ], s->sequencia[ melhorJ ] );
		s->valorObj += melhorDelta;
		return true;
	}
	return false;
}

bool BuscaLocal::bestImprovement2Opt( double** matrizAdj, int dim, Solucao* s ) {
	double melhorDelta = 0;
	double melhorI, melhorJ;
	for( int i = 1; i < s->sequencia.size(); i++ ) {
		for( int j = i+1; j < s->sequencia.size(); j++ ) {
			double delta = this->calcula2OptCusto( matrizAdj, dim, i, j );
			if ( delta < melhorDelta ) {
				melhorI = i;
				melhorJ = j;
				melhorDelta = delta;
			}
		}
	}
	if ( melhorDelta < 0 ) {
		this->exec2Opt( matrizAdj, dim, s->sequencia, melhorI, melhorJ );
		s->valorObj += melhorDelta;
		return true;
	}
	return false;
}

bool BuscaLocal::bestImprovementOrOpt( double** matrizAdj, int dim, Solucao* s, int k ) {
	double melhorDelta = 0;
	double melhorI, melhorJ;
	for( int i = 1; i < s->sequencia.size(); i++ ) {
		for( int j = i+1; j < s->sequencia.size(); j++ ) {
			double delta = this->calculaOrOptCusto( matrizAdj, dim, i, j, k );
			if ( delta < melhorDelta ) {
				melhorI = i;
				melhorJ = j;
				melhorDelta = delta;
			}
		}
	}
	if ( melhorDelta < 0 ) {
		this->execOrOpt( matrizAdj, dim, s->sequencia, melhorI, melhorJ, k );
		s->valorObj += melhorDelta;
		return true;
	}
	return false;
}

double BuscaLocal::calculaSwapCusto( double** matrizAdj, int dim, int i, int j ) {
	int i0 = i > 0 ? i-1 : dim-1;
	int i2 = i < dim-1 ? i+1 : 0;
	int j0 = j > 0 ? j-1 : dim-1;
	int j2 = j < dim-1 ? j+1 : 0;
	return matrizAdj[ i0 ][ j ] + matrizAdj[ j ][ i2 ] - ( matrizAdj[ j0 ][ i ] + matrizAdj[ i ][ j2 ] );
}

double BuscaLocal::calcula2OptCusto( double** matrizAdj, int dim, int i, int j ) {
	int i2 = i < dim-1 ? i+1 : 0;
	int j2 = j > 0 ? j-1 : dim-1;
	return matrizAdj[ i ][ i2 ] + matrizAdj[ j ][ j2 ] - ( matrizAdj[ i ][ j2 ] + matrizAdj[ j ][ i2 ] );
}

double BuscaLocal::calculaOrOptCusto( double** matrizAdj, int dim, int i, int j, int k ) {
	double dk = 0;
	
	int i1 = i;
	int i2;
	for( int c = 0; c < k-1; c++ ) {
		i2 = i1 < dim-1 ? i1+1 : 0;
		
		dk += matrizAdj[ i1 ][ i2 ];
		
		i1 = i2;
	}
	i1 = i;

	int i0 = i < dim-1 ? i+1 : 0;	
	int i3 = i2 > 0 ? i2-1 : dim-1;	
	
	int j1 = j;
	int j2 = j < dim-1 ? j+1 : 0;
			
	double subD = dk + matrizAdj[ i0 ][ i1 ] + matrizAdj[ i2 ][ i3 ] + matrizAdj[ j1 ][ j2 ];
	double somaD = dk + matrizAdj[ j1 ][ i1 ] + matrizAdj[ j2 ][ i2 ];
	return somaD - subD;
}

void BuscaLocal::exec2Opt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j ) {
	int i2 = i > 0 ? i-1 : dim-1;
	int j2 = j < dim-1 ? j+1 : 0;
	for( int c = 0; c <= abs(i2-j2)/2; c++ )
		swap( sequencia[ i2+c ], sequencia[ j2-c ] );			
}

void BuscaLocal::execOrOpt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j, int k ) {
	int i2 = i;
	for( int c = 0; c < k-1; c++ )
		i2 = i2 < dim-1 ? i2+1 : 0;					

	sequencia.erase( sequencia.begin() + i, sequencia.begin() + i2 );
	
	for( int c = 0; c < k-1; c++ )
		sequencia.insert( sequencia.begin() + j+c, sequencia[ i+c ] );	
}

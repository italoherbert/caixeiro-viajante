#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "BuscaLocal.h"

void BuscaLocal::buscaLocal( double** matrizAdj, int dim, Solucao* s ) {
	vector<int> opcoes = { 0, 1, 2, 3, 4 };

	bool houveAproveitamento = false;

	while( !opcoes.empty() ) {
		int r = rand() % opcoes.size();
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

		if ( houveAproveitamento ) {
			opcoes = { 0, 1, 2, 3, 4 };
		} else {
			opcoes.erase( opcoes.begin() + r );
		}
	}
}

bool BuscaLocal::bestImprovementSwap( double** matrizAdj, int dim, Solucao* s ) {
	double melhorDelta = 0;
	double melhorI, melhorJ;
	for( int i = 1; i < s->sequencia.size()-1; i++ ) {
		for( int j = i+1; j < s->sequencia.size()-1; j++ ) {
			double delta = this->calculaSwapCusto( matrizAdj, dim, s->sequencia, i, j );
			if ( delta < melhorDelta ) {
				melhorI = i;
				melhorJ = j;
				melhorDelta = delta;
			}
		}
	}
	if ( melhorDelta < 0 ) {
		swap( s->sequencia[ melhorI ], s->sequencia[ melhorJ ] );
		s->custo += melhorDelta;
		return true;
	}
	return false;
}

bool BuscaLocal::bestImprovement2Opt( double** matrizAdj, int dim, Solucao* s ) {
	double melhorDelta = 0;
	double melhorI, melhorJ;
	for( int i = 1; i < s->sequencia.size()-1; i++ ) {
		for( int j = i+1; j < s->sequencia.size()-3; j++ ) {
			int jj = (j+2) % s->sequencia.size();
			double delta = this->calcula2OptCusto( matrizAdj, dim, s->sequencia, i, jj );
			if ( delta < melhorDelta ) {
				melhorI = i;
				melhorJ = jj;
				melhorDelta = delta;
			}
		}
	}
	if ( melhorDelta < 0 ) {
		this->exec2Opt( matrizAdj, dim, s->sequencia, melhorI, melhorJ );
		s->custo += melhorDelta;
		return true;
	}
	return false;
}

bool BuscaLocal::bestImprovementOrOpt( double** matrizAdj, int dim, Solucao* s, int k ) {
	double melhorDelta = 0;
	double melhorI, melhorJ;
	for( int i = 1; i < s->sequencia.size()-1; i++ ) {
		for( int j = i+1; j < s->sequencia.size()-k-2; j++ ) {
		 	int jj = (j+k+1) % s->sequencia.size();
			double delta = this->calculaOrOptCusto( matrizAdj, dim, s->sequencia, i, jj, k );
			if ( delta < melhorDelta ) {
				melhorI = i;
				melhorJ = jj;
				melhorDelta = delta;
			}
		}
	}
	if ( melhorDelta < 0 ) {
		this->execOrOpt( matrizAdj, dim, s->sequencia, melhorI, melhorJ, k );
		s->custo += melhorDelta;
		return true;
	}
	return false;
}

double BuscaLocal::calculaSwapCusto( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j ) {
	if ( i+1 == j ) {
		int elI0 = sequencia[ i > 0 ? i-1 : dim-1];
		int elI = sequencia[ i ];
		int elJ = sequencia[ j ];
		int elJ2 = sequencia[ j < dim-1 ? j+1 : 0 ];

		double somaD = matrizAdj[ elI0 ][ elJ ] + matrizAdj[ elI ][ elJ2 ];
		double subD = matrizAdj[ elI0 ][ elI ] + matrizAdj[ elJ ][ elJ2 ];
		return somaD - subD;
	} else {
		int elI0 = sequencia[ i > 0 ? i-1 : dim-1 ];
		int elI = sequencia[ i ];
		int elI2 = sequencia[ i < dim-1 ? i+1 : 0 ];
		int elJ0 = sequencia[ j > 0 ? j-1 : dim-1 ];
		int elJ = sequencia[ j ];
		int elJ2 = sequencia[ j < dim-1 ? j+1 : 0 ];

		double somaD = matrizAdj[ elI0 ][ elJ ] + matrizAdj[ elJ ][ elI2 ] + matrizAdj[ elJ0 ][ elI ] + matrizAdj[ elI ][ elJ2 ];
		double subD = matrizAdj[ elI0 ][ elI ] + matrizAdj[ elI ][ elI2 ] + matrizAdj[ elJ0 ][ elJ ] + matrizAdj[ elJ ][ elJ2 ];
		return somaD - subD;
	}
}

double BuscaLocal::calcula2OptCusto( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j ) {
	int elI = sequencia[ i ];
	int elJ = sequencia[ j ];
	int elI2 = sequencia[ i < dim-1 ? i+1 : 0 ];
	int elJ2 = sequencia[ j < dim-1 ? j+1 : 0 ];
	return matrizAdj[ elI ][ elJ ] + matrizAdj[ elI2 ][ elJ2 ] - ( matrizAdj[ elI ][ elI2 ] + matrizAdj[ elJ ][ elJ2 ] );
}

double BuscaLocal::calculaOrOptCusto( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j, int k ) {
	int ik = i;
	for( int c = 0; c < k-1; c++ )
		ik = ik < dim-1 ? ik+1 : 0;

	int i0 = i > 0 ? i-1 : dim-1;
	int i3 = ik < dim-1 ? ik+1 : 0;

	int j2 = j < dim-1 ? j+1 : 0;

	int elI0 = sequencia[ i0 ];
	int elI = sequencia[ i ];
	int elIK = sequencia[ ik ];
	int elI3 = sequencia[ i3 ];

	int elJ = sequencia[ j ];
	int elJ2 = sequencia[ j2 ];

	double somaD = matrizAdj[ elI ][ elJ2 ] + matrizAdj[ elIK ][ elJ ] + matrizAdj[ elI0 ][ elI3 ];
	double subD = matrizAdj[ elI0 ][ elI ] + matrizAdj[ elIK ][ elI3 ] + matrizAdj[ elJ ][ elJ2 ];
	return somaD - subD;
}

void BuscaLocal::exec2Opt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j ) {
	int i2 = i < dim-1 ? i+1 : 0;
	for( int c = 0; c <= abs(i2-j)/2; c++ )
		swap( sequencia[ i2+c ], sequencia[ j-c ] );
}

void BuscaLocal::execOrOpt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j, int k ) {
	vector<int> vet;

	for( int c = k-1; c >=0; c-- )
		vet.push_back( sequencia[ i+c ] );

	for( int c = 0; c < k; c++ )
		sequencia.erase( sequencia.begin() + i );

	sequencia.insert( sequencia.begin() + j - k + 1, vet.begin(), vet.end() );
}

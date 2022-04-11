#include "BuscaLocal.h"

#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

void BuscaLocal::buscaLocal( double** matrizAdj, int dim, Solucao* s ) {
	
}

bool BuscaLocal::bestImprovementSwap( double** matrizAdj, int dim, Solucao* s ) {
	return true;
}

bool BuscaLocal::bestImprovement2Opt( double** matrizAdj, int dim, Solucao* s ) {
	return true;
}

bool BuscaLocal::bestImprovementOrOpt( double** matrizAdj, int dim, Solucao* s, int k ) {
	return true;
}

double BuscaLocal::calculaSwapCusto( double** matrizAdj, int dim, int i, int j ) {
	int i1 = i > 0 ? i-1 : dim-1;
	int i2 = i < dim-1 ? i+1 : 0;
	int j1 = j > 0 ? j-1 : dim-1;
	int j2 = j < dim-1 ? j+1 : 0;
	return matrizAdj[ i ][ i-1 ] + matrizAdj[ i ][ i+1 ] - ( matrizAdj[ j ][ j-1 ] + matrizAdj[ j ][ j+1 ] );
}

double BuscaLocal::calcula2OptCusto( double** matrizAdj, int dim, int i, int j ) {
	int i2 = i < dim-1 ? i+1 : 0;
	int j2 = j > 0 ? j-1 : dim-1;
	return matrizAdj[ i ][ i2 ] + matrizAdj[ j ][ j2 ] - ( matrizAdj[ i ][ j2 ] + matrizAdj[ j ][ i2 ] );
}

double BuscaLocal::calculaOrOptCusto( double** matrizAdj, int dim, int i, int j, int k ) {
	return 0;
}

void BuscaLocal::exec2Opt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j ) {
	
}

void BuscaLocal::execOrOpt( double** matrizAdj, int dim, vector<int>& sequencia, int i, int j, int k ) {
	
}
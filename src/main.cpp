
#include <iostream>
#include <cstdio>
#include <vector>

#include "caixeiroviajante/minhasolucao/CaixeiroViajanteMinhaSolucao.h"
#include "caixeiroviajante/metaheuristica/CaixeiroViajanteMetaHeuristica.h"
#include "caixeiroviajante/metaheuristica/BuscaLocal.h"

using namespace std;

int main(int argc, char** argv) {
	CaixeiroViajanteMetaHeuristica cv;
	
	if ( argc < 2 ) {		
		char* inst = (char*) malloc( 100 * sizeof( char ) );
		strcpy( inst, "instances/burma14.tsp" );
		
		argv = (char**) malloc( 2 * sizeof( char* ) );
		argv[1] = inst;
		
		argc = 2;
	}
			
    cv.readTSPData( argc, argv );
    		
	cout << "Buscando solucao..." << endl;
		
	cv.imprimeMatrizAdj();	
		
	Solucao s = cv.calculaCaminho();
	cv.imprimeSolucao( s );
    
    return 0;
}

int main2() {
	CaixeiroViajanteMinhaSolucao cv;	
		
	string arquivo = "madj2.txt";
	bool carregou = cv.carregaMatrizAdj( arquivo );
	if ( carregou ) {	
		cv.imprimeMatrizAdj();
		
		Solucao s = cv.calculaCaminho();
		cv.imprimeSolucao( s );
	} else {
		cout << "Arquivo nao encontrado." << endl;
	}
	cout << "Finalizando..." << endl;
	return 0;		
}

int main6(int argc, char** argv) {
	BuscaLocal cv;
	
	double mat[7][7] = {
		{ 1, 4, 3, 1, 2, 5, 6 },
		{ 4, 7, 8, 4, 1, 1, 9 },
		{ 3, 8, 6, 5, 9, 9, 7 },
		{ 1, 4, 5, 2, 1, 8, 4 },
		{ 2, 1, 9, 1, 6, 8, 5 },
		{ 5, 1, 9, 8, 8, 1, 2 },		
		{ 6, 9, 7, 4, 5, 2, 3 }
	};
	int dim = 7;
	
	double** matrizAdj = (double**)malloc( dim * sizeof( double*) );

	cout << "Matriz de adjacencias" << endl;	
	cout << endl;
	for( int i = 0; i < dim; i++ ) {	
		matrizAdj[i] = (double*)malloc( dim * sizeof( double ) );
		for( int j = 0; j < dim; j++ ) {		
			matrizAdj[i][j] = mat[i][j];
			printf( "%7.2f", matrizAdj[i][j] );
		}
		cout << endl;
	}
		
	vector<int> sequencia = { 0, 1, 2, 3, 4, 5, 6 };
	
	cout << endl;
	cout << "Sequencia= ";
	for( int c = 0; c < sequencia.size(); c++ )
		cout << sequencia[ c ] << "  ";
	cout << endl;
	
	double dist = 0;
	for( int c = 0; c < sequencia.size()-1; c++ )
		dist += matrizAdj[ sequencia[ c ]][ sequencia[ c+1 ] ];
	cout << "Dist: " << dist << endl;
		
	double custo = cv.calculaOrOptCusto( matrizAdj, dim, sequencia, 1, 4, 2 );
	cv.execOrOpt( matrizAdj, dim, sequencia, 1, 4, 2 );	
	
	cout << endl;
	cout << "Custo=" << custo << endl;
	
	cout << "Sequencia= ";
	for( int c = 0; c < sequencia.size(); c++ )
		cout << sequencia[ c ] << "  ";
	cout << endl;		
	
	dist = 0;
	for( int c = 0; c < sequencia.size()-1; c++ )
		dist += matrizAdj[ sequencia[ c ]][ sequencia[ c+1 ] ];
	cout << "Dist: " << dist + custo << endl;
    
    return 0;
}
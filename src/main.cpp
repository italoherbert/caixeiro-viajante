
#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <cfloat>
#include <string>

#include <sys/types.h>
#include <dirent.h>

#include "caixeiroviajante/minhasolucao/CaixeiroViajanteMinhaSolucao.h"
#include "caixeiroviajante/metaheuristica/CaixeiroViajanteMetaHeuristica.h"
#include "caixeiroviajante/metaheuristica/Construcao.h"
#include "caixeiroviajante/metaheuristica/BuscaLocal.h"
#include "caixeiroviajante/metaheuristica/Perturbacao.h"

#include "util/util.h"

using namespace std;

int main( int argc, char** argv ) {
	CaixeiroViajanteMetaHeuristica cv;
	
	DIR *dir = opendir( "instances/" );
	struct dirent* in;
	int cc = 0;
	while ( ( in = readdir( dir ) ) != NULL ) {		
		char* inst = (char*) malloc( 100 * sizeof( char ) );
		strcpy( inst, "instances/" );
		strcat( inst, in->d_name );

		if ( cc++ < 2 )
			continue;		
			
		argv = (char**) malloc( 2 * sizeof( char* ) );
		argv[1] = inst;
		
		argc = 2;
	
					
	    cv.readTSPData( argc, argv );
	    		
		cout << "Buscando solucao..." << endl;
		
		int n = 10;
				
		double tempoMedio = 0;	
		double custoMedio = 0;		
		
		Solucao melhor;
		melhor.custo = DBL_MAX;
		
		for( int i = 0; i < n; i++ ) {		
			long long ms1 = get_ms();	
			Solucao s = cv.calculaCaminho();		
			long long ms2 = get_ms();
			
			double tempo =  ( ms2-ms1 ) / 1000.0;
			//cout << "Tempo: " << tempo << " seg   Custo: " << s.custo << endl;
			
			tempoMedio += tempo;
			custoMedio += s.custo;
			
			if ( s.custo < melhor.custo )
				melhor = s;
		}
		
		tempoMedio /= n;
		custoMedio /= n;
			
	    cout << argv[1] << " : "<< "T(md)= " << tempoMedio << ";  C(md)= " << custoMedio << endl;
	    cout << endl;
	    cout << "T(md)=Tempo médio; C(md)=Custo médio" << endl;
	}
	
	closedir( dir );
	
	return 0;
}

int main22222() {
	DIR *dir = opendir( "instances/" );
	struct dirent* in;
	while ( ( in = readdir( dir ) ) != NULL )
		cout << in->d_name << endl;
		
	return 0;
}

int main231(int argc, char** argv) {
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

int main1(int argc, char** argv) {
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

int main2342( int argc, char** argv ) {
	CaixeiroViajanteMetaHeuristica cv;
	Perturbacao p;
	
	vector<int> sequencia = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 1 };

	//cv.imprimeSequencia( sequencia );
	
	for( int i = 0; i < 100; i++ ) {	
		p.perturbacao( sequencia );				
		cv.imprimeSequencia( sequencia );
	}
	return 0;
}
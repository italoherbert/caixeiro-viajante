#include "caixeiroviajante/minhasolucao/CaixeiroViajanteMinhaSolucao.h"
#include "caixeiroviajante/metaheuristica/CaixeiroViajanteMetaHeuristica.h"

#include <iostream>

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


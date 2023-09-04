
#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <cfloat>
#include <string>

#include <sys/types.h>
#include <dirent.h>

#include "caixeiroviajante/logica/Construcao.h"
#include "caixeiroviajante/logica/BuscaLocal.h"
#include "caixeiroviajante/logica/Perturbacao.h"

#include "caixeiroviajante/minhasolucao/CaixeiroViajanteMinhaSolucao.h"
#include "caixeiroviajante/grasp/CaixeiroViajanteGRASP.h"
#include "caixeiroviajante/acs/CaixeiroViajanteACS.h"

#include "util/util.h"

using namespace std;

int main( int argc, char** argv ) {
	CaixeiroViajanteACS cv;

	if ( argc < 2 ) {
		char* inst = (char*) malloc( 100 * sizeof( char ) );
		strcpy( inst, "instances/rd100.tsp" );

		argv = (char**) malloc( 2 * sizeof( char* ) );
		argv[1] = inst;

		argc = 2;
	}

    cv.readTSPData( argc, argv );

	cout << "Buscando solucao..." << endl;

	long long ms1 = get_ms();
	Solucao s = cv.calculaCaminho();
	long long ms2 = get_ms();

	double tempo =  ( ms2-ms1 ) / 1000.0;
	cout << "Tempo: " << tempo << " seg   Custo: " << s.custo << endl;

	cv.imprimeSolucao( s );

	return 0;
}

int mainOpenDir( int argc, char** argv ) {
	CaixeiroViajanteGRASP cv;

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

int main2091(int argc, char** argv ) {
	CaixeiroViajanteGRASP cv;

	if ( argc < 2 ) {
		char* inst = (char*) malloc( 100 * sizeof( char ) );
		strcpy( inst, "instances/berlin52.tsp" );

		argv = (char**) malloc( 2 * sizeof( char* ) );
		argv[1] = inst;

		argc = 2;
	}

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
		cout << "Tempo: " << tempo << " seg   Custo: " << s.custo << endl;

		tempoMedio += tempo;
		custoMedio += s.custo;

		if ( s.custo < melhor.custo )
			melhor = s;
	}

	tempoMedio /= n;
	custoMedio /= n;

    cout << argv[1] << " : "<< "T(md)= " << tempoMedio << ";  C(md)= " << custoMedio << endl;
    cout << endl;

	return 0;
}

int main231(int argc, char** argv) {
	CaixeiroViajanteGRASP cv;

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

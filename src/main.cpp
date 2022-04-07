#include "readData.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include <sstream>
#include <cfloat>
#include <algorithm>

using namespace std;

typedef struct Solucao {
	vector<int> sequencia;
	double valorObj;
} Solucao;

bool carregaMatrizAdj( double*** matrizAdj, int* dim, string arquivo );
void imprimeMatrizAdj( double** matrizAdj, int dim );

Solucao calculaCaminho( double** matrizAdj, int dim );
void imprimeSolucao( Solucao solucao );

void readTSPData( int argc, char** argv, double*** matrizAdj, int* dim );

void printData( double** matrizAdj, int dimension );

int main2() {
	double** matrizAdj;
	int dimensao;
	
	string arquivo = "madj2.txt";
	bool carregou = carregaMatrizAdj( &matrizAdj, &dimensao, arquivo );
	if ( carregou ) {	
		imprimeMatrizAdj( matrizAdj, dimensao );
		
		Solucao s = calculaCaminho( matrizAdj, dimensao );
		imprimeSolucao( s );
	} else {
		cout << "Arquivo nao encontrado." << endl;
	}
	cout << "Finalizando..." << endl;
	return 0;		
}

int main(int argc, char** argv) {
	double** matrizAdj;
	int dimension;

	if ( argc < 2 ) {		
		char* inst = (char*) malloc( 100 * sizeof( char ) );
		strcpy( inst, "instances/burma14.tsp" );
		
		argv = (char**) malloc( 2 * sizeof( char* ) );
		argv[1] = inst;
		
		argc = 2;
	}
			
    readTSPData(argc, argv, &matrizAdj, &dimension);
    
	//imprimeMatrizAdj( matrizAdj, dimension );
		
	cout << "Buscando solucao..." << endl;
		
	Solucao s = calculaCaminho( matrizAdj, dimension );
	imprimeSolucao( s );
    
    return 0;
}

bool carregaMatrizAdj( double*** matrizAdj, int* dim, string arquivo ) {
	fstream stream;
	stream.open( arquivo, ios::in);
	if ( stream.is_open() ) {
		string linha;
		getline( stream, linha );
		
		*dim = stoi( linha );		

		*matrizAdj = new double *[ *dim ];		
				
		for( int i = 0; i < *dim; i++ ) {
			(*matrizAdj)[ i ] = new double[ *dim ];
			
			getline( stream, linha );
					
			stringstream strstream( linha );
			for( int j = 0; j < *dim; j++) {
				string svalor;
				getline( strstream, svalor, ' ' );
				
				(*matrizAdj)[ i ][ j ] = stod( svalor );
			}
		}
		
		return true;
	}
	return false;
}

void imprimeSolucao( Solucao solucao ) {
	cout << endl;
	printf( "Menor distancia: %.2f\n", solucao.valorObj );
	cout << "Ordem de vertices: ";
	for( int i = 0; i < solucao.sequencia.size(); i++ ) 
		cout << (solucao.sequencia[ i ]+1) << "  ";
	cout << endl;
}

Solucao calculaCaminho( double** matrizAdj, int dim ) {	
	Solucao s;
	s.sequencia = {};
	s.valorObj = DBL_MAX;
	
	vector<int> lista;	
	vector<int> profundidades;	
		
	double distancias[ dim ][ dim ];
	int origens[ dim ][ dim ];
	
	for( int i = 0; i < dim; i++ ) {
		for( int j = 0; j < dim; j++ ) {		
			distancias[ i ][ j ] = -1;
			origens[ i ][ j ] = -1;
		}
	}
			
	lista.push_back( 0 );
	profundidades.push_back( 0 );
									
	while( lista.size() > 0 ) {
		int k = lista[ lista.size() - 1 ];		
		int profundidade = profundidades[ profundidades.size() -1 ];				

		lista.pop_back();
		profundidades.pop_back();
												
		if ( profundidade == dim && k == 0 ) {			
			double d = distancias[ profundidade-1 ][ 0 ];									
			if ( d < s.valorObj ) {			
				s.valorObj = d;	
				int orig = 0;			
				
				s.sequencia.clear();
				s.sequencia.push_back( orig );					
				for( int p = dim-1; p >= 0; p-- ) {						
					orig = origens[ p ][ orig ];
					s.sequencia.push_back( orig );					
				}	
				reverse( s.sequencia.begin(), s.sequencia.end() );				
								
				if ( profundidades.size() > 0 ) {
					int prof = profundidades[ profundidades.size() - 1 ];
					for( int p = dim-1; p >= prof; p-- ) {
						for( int j = 0; j < dim; j++ ) {			    	
							distancias[ p ][ j ] = -1;
							origens[ p ][ j ] = -1;				
						}
					}
				}
			}			
		}
		
		bool parar = false;
		for( int i = 0; !parar && profundidade < dim && i < dim; i++ )	{									
			if ( i != k && matrizAdj[ k ][ i ] != -1 ) {
				bool processar = true;
				for( int p = 1; processar && p < profundidade; p++ )
					if ( origens[ p ][ k ] == i )
						processar = false;								
						
				if ( i == 0 && profundidade < dim-1 )
					processar = false;														
				
				if ( processar ) { 
					double d = ( profundidade > 0 ? distancias[ profundidade-1 ][ k ] : 0 ) + matrizAdj[ k ][ i ];					
					
					if ( d >= s.valorObj ) {
						parar = true;
					} else {								
						distancias[ profundidade ][ i ] = d;	                     
						origens[ profundidade ][ i ] = k;																		
						
						lista.push_back( i );
						profundidades.push_back( profundidade+1 );
					}
				}												
			}
		}	
	}	
			
	return s;
}

void imprimeMatrizAdj( double** matrizAdj, int dim ) {
	cout << endl;
	cout << "Matriz de adjacencia: " << endl;
	cout << endl; 
				
	printf( "%8s", "" );
	for( int i = 0; i < dim; i++ ) {	
		printf( "%8d", i+1 );
	}
	
	cout << endl;
	
	for( int i = 0; i < dim; i++ ) {			
		printf( "%8d", i+1 );
		for( int j = 0; j < dim; j++ )
			printf( "%8.2f", matrizAdj[ i ][ j ] ); 		
		cout << endl;
	}
}


void readTSPData( int argc, char** argv, double*** matrizAdj, int* dim ) {
	double** tspMatrizAdj;
	int tspDim;
	
	readData( argc, argv, &tspDim, &tspMatrizAdj );
	
	*dim = tspDim;
	*matrizAdj = new double* [ *dim ];
	
	for( size_t i = 1; i <= tspDim; i++ ) {
		(*matrizAdj)[ i-1 ] = new double[ *dim ];
		for( size_t j = 1; j <= tspDim; j++ )	
			(*matrizAdj)[i-1][j-1] = tspMatrizAdj[i][j];						
	}
}

void printData( double** matrizAdj, int dimension ) {
  cout << "dimension: " << dimension << endl;
  for (size_t i = 1; i <= dimension; i++) {
    for (size_t j = 1; j <= dimension; j++) {
      cout << matrizAdj[i][j] << " ";
    }
    cout << endl;
  }
}

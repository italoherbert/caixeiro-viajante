#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <cfloat>

#include "CaixeiroViajante.h"
#include "readData.h"

bool CaixeiroViajante::carregaMatrizAdj( string arquivo ) {
	fstream stream;
	stream.open( arquivo, ios::in);
	if ( stream.is_open() ) {
		string linha;
		getline( stream, linha );
		
		dim = stoi( linha );		

		matrizAdj = new double *[ dim ];		
				
		for( int i = 0; i < dim; i++ ) {
			matrizAdj[ i ] = new double[ dim ];
			
			getline( stream, linha );
					
			stringstream strstream( linha );
			for( int j = 0; j < dim; j++) {
				string svalor;
				getline( strstream, svalor, ' ' );
				
				matrizAdj[ i ][ j ] = stod( svalor );
			}
		}
		
		return true;
	}
	return false;
}

double CaixeiroViajante::calculaCusto( vector<int> nos ) {
	double d = 0;
	for( int i = 0; i < nos.size()-1; i++ ) {
		int j = ( i < nos.size()-1 ? i+1 : 0 );
		d += matrizAdj[ nos[ i ] ][ nos[ j ] ];
	}
	return d;
}

void CaixeiroViajante::imprimeSolucao( Solucao solucao ) {
	cout << endl;
	printf( "Menor distancia: %.2f\n", solucao.custo );
	cout << "Ordem de vertices: ";
	for( int i = 0; i < solucao.sequencia.size(); i++ ) 
		cout << (solucao.sequencia[ i ]+1) << "  ";
	cout << endl;
}

void CaixeiroViajante::imprimeSequencia( vector<int> sequencia ) {
	cout << "Sequencia: ";
	for( int i = 0; i < sequencia.size(); i++ ) 
		cout << sequencia[ i ] << "  ";
	cout << endl;
}

void CaixeiroViajante::imprimeMatrizAdj() {
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


void CaixeiroViajante::readTSPData( int argc, char** argv ) {
	double** tspMatrizAdj;
	int tspDim;
	
	readData( argc, argv, &tspDim, &tspMatrizAdj );
	
	dim = tspDim;
	matrizAdj = new double* [ dim ];
	
	for( size_t i = 1; i <= tspDim; i++ ) {
		matrizAdj[ i-1 ] = new double[ dim ];
		for( size_t j = 1; j <= tspDim; j++ )	
			matrizAdj[i-1][j-1] = tspMatrizAdj[i][j];						
	}
}

void CaixeiroViajante::printData() {
  cout << "dimension: " << dim << endl;
  for (size_t i = 1; i <= dim; i++) {
    for (size_t j = 1; j <= dim; j++) {
      cout << matrizAdj[i][j] << " ";
    }
    cout << endl;
  }
}

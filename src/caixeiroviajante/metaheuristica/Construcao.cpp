#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

#include "Construcao.h"

Solucao Construcao::construcao( double** matrizAdj, int dim ) {
	srand( time( NULL ) );				
	
	vector<int> nos;
	for( int i = 1; i < dim; i++ )
		nos.push_back( i );
				
	vector<int> sequencia;
	sequencia.push_back( 0 );		
	
	for( int i = 0; i < 3; i++ ) {
		int r = rand() % nos.size();
		sequencia.push_back( nos[ r ] );
		nos.erase( nos.begin() + r );
	}			
			
	while( !nos.empty() ) {
		int r = rand() % nos.size();
		int k = nos[ r ];
	
		vector<InsersaoInfo> insercaoInfos = this->calculaInsercaoInfos( matrizAdj, dim, sequencia, k );		
		this->ordenaPorCusto( insercaoInfos );
				
		double alpha = (double)rand() / RAND_MAX;

		int r2 = rand() % ( (int)ceil( alpha * insercaoInfos.size() ) );		
		int i = insercaoInfos[ r2 ].noI;
		
		nos.erase( nos.begin() + r );
		sequencia.insert( sequencia.begin() + i, k );
	}
	
	sequencia.push_back( 0 );
	
	double custo = 0;
	for( int i = 0; i < sequencia.size()-1; i++ ) {
		int j = ( i < sequencia.size() - 1 ? i+1 : 0 );
		custo += matrizAdj[ sequencia[ i ] ][ sequencia[ j ] ];
	}	
					
	Solucao s = { sequencia, custo };			
	return s;
}

vector<InsersaoInfo> Construcao::calculaInsercaoInfos( double** matrizAdj, int dim, vector<int>& sequencia, int k ) {
	vector<InsercaoInfo> infos;
		
    for( int i = 0; i < sequencia.size()-1; i++ ) {
    	int j = i+1;

    	InsersaoInfo info;
		info.noI = i+1;
    	info.custo = matrizAdj[i][k] + matrizAdj[j][k] - matrizAdj[i][j];
    	
    	infos.push_back( info );
	}
				
	return infos;
}

void Construcao::ordenaPorCusto( vector<InsercaoInfo>& insercaoInfos ) {
	for( int i = 0; i < insercaoInfos.size()-1; i++ ) {
		for( int j = i+1; j < insercaoInfos.size(); j++ ) {
			if ( insercaoInfos[ i ].custo > insercaoInfos[ j ].custo ) {
				InsercaoInfo aux = insercaoInfos[ i ];
				insercaoInfos[ i ] = insercaoInfos[ j ];
				insercaoInfos[ j ] = aux;
			}
		}	
	}	
}
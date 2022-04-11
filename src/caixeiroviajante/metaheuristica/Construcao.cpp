#include "Construcao.h"

#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

Solucao Construcao::construcao( double** matrizAdj, int dim ) {
	vector<int> nos;
	for( int i = 0; i < dim; i++ )
		nos.push_back( i );
		
	vector<int> sequencia;	
	
	for( int i = 0; i < 3; i++ ) {
		int r = rand() % nos.size();
		sequencia.push_back( nos[ r ] );
		nos.erase( nos.begin() + r );
	}	
	
	srand( time( NULL ) );
		
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
		
	double valorObj = 0;
	for( int i = 0; i < sequencia.size(); i++ ) {
		int j = ( sequencia.size() - 1 ? i+1 : 0 );
		valorObj += matrizAdj[ sequencia[ i ] ][ sequencia[ j ] ];
	}	
				
	Solucao s = { sequencia, valorObj };			
	return s;
}

vector<InsersaoInfo> Construcao::calculaInsercaoInfos( double** matrizAdj, int dim, vector<int>& sequencia, int k ) {
	vector<InsercaoInfo> infos;
		
    for( int i = 0; i < sequencia.size(); i++ ) {
    	int j = ( i < sequencia.size()-1 ? i+1 : 0 );

    	InsersaoInfo info;
		info.noI = i;
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
#include "CaixeiroViajanteMetaHeuristica.h"

#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>

Solucao CaixeiroViajanteMetaHeuristica::calculaCaminho() {		
	return this->construcao();
}

Solucao CaixeiroViajanteMetaHeuristica::construcao() {
	vector<int> nos;
	for( int i = 0; i < dim; i++ )
		nos.push_back( i );
		
	Solucao s;
	
	for( int i = 0; i < 3; i++ ) {
		int r = rand() % nos.size();
		s.sequencia.push_back( nos[ r ] );
		nos.erase( nos.begin() + r );
	}	
	
	srand( time( NULL ) );
		
	while( !nos.empty() ) {
		int r = rand() % nos.size();
		int k = nos[ r ];
	
		vector<InsersaoInfo> insercaoInfos = this->calculaInsercaoInfos( s.sequencia, k );		
		this->ordenaPorCusto( insercaoInfos );
				
		double alpha = (double)rand() / RAND_MAX;

		int r2 = rand() % ( (int)ceil( alpha * insercaoInfos.size() ) );		
		int i = insercaoInfos[ r2 ].noI;
		
		nos.erase( nos.begin() + r );
		s.sequencia.insert( s.sequencia.begin() + i, k );
	}
		
	s.valorObj = 0;
	for( int i = 0; i < s.sequencia.size(); i++ ) {
		int j = ( s.sequencia.size() - 1 ? i+1 : 0 );
		s.valorObj += matrizAdj[ s.sequencia[ i ] ][ s.sequencia[ j ] ];
	}	
			
	return s;
}

vector<InsersaoInfo> CaixeiroViajanteMetaHeuristica::calculaInsercaoInfos( vector<int>& sequencia, int k ) {
	vector<InsercaoInfo> infos;
		
    for( int i = 0; i < sequencia.size(); i++ ) {
    	int j = ( i < sequencia.size()-1 ? i+1 : 0 );

    	InsersaoInfo info;
		info.noI = i;
    	info.custo = matrizAdj[i][k] + matrizAdj[ j ][ k ] - matrizAdj[i][j];
    	
    	infos.push_back( info );
	}
				
	return infos;
}

void CaixeiroViajanteMetaHeuristica::ordenaPorCusto( vector<InsercaoInfo>& insercaoInfos ) {
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

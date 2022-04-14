#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

#include "Construcao.h"

#include "../../util/util.h"

Solucao Construcao::construcao( double** matrizAdj, int dim ) {
	srand( get_ms() );				
	
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
	
		InsercaoInfo melhor;
		melhor.custo = DBL_MAX;
		
		vector<InsersaoInfo> insercaoInfos = this->calculaInsercaoInfos( matrizAdj, dim, sequencia, k );				
		
		sort(insercaoInfos.begin(), insercaoInfos.end(), [](InsercaoInfo a, InsercaoInfo b){
			return a.custo > b.custo;	
		});		
						
		double alpha = ((double)rand()+1) / (RAND_MAX+1);
		
		int r2 = rand() % ( (int)ceil( alpha * insercaoInfos.size() ) );		
		int i = insercaoInfos[ r2 ].noI;
		
		nos.erase( nos.begin() + r );
		sequencia.insert( sequencia.begin() + i, k );
	}
	
	sequencia.push_back( 0 );		
	
	double custo = 0;
	for( int i = 0; i < sequencia.size()-1; i++ )
		custo += matrizAdj[ sequencia[ i ] ][ sequencia[ i+1 ] ];	
					
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

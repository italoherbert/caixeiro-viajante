#include "CaixeiroViajanteMinhaSolucao.h"

#include <vector>
#include <cfloat>
#include <algorithm>

Solucao CaixeiroViajanteMinhaSolucao::calculaCaminho() {	
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

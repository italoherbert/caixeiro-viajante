
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>

#include "Perturbacao.h"

void Perturbacao::perturbacao( vector<int>& sequencia ) {
	srand( time( NULL ) );
	
	int pinter1 = 2;
	int pinter2 = max( (int)(sequencia.size() / 10 ), 4 );
	
	int r1,r2;

	int rr = rand() % 2;
	if ( rr == 0 ) {
		r1 = ( rand() % ( sequencia.size() - pinter2 - 2 ) ) + 1;	
		if ( sequencia.size() - r1 - pinter1 - pinter2 == 0 ) {		
			r2 = r1 + pinter1;
		} else {
			r2 = ( rand() % ( sequencia.size() - r1 - pinter1 - pinter2 ) ) + r1 + pinter1;
		}
	} else {
		r1 = ( rand() % ( sequencia.size() - pinter1 - 2 ) ) + pinter2 + 1 ;
		if ( r1 - pinter2 == 0 ) {		
			r2 = 1;
		} else {			
			r2 = ( rand() % ( r1 - pinter2 ) ) + 1;
		}
	}
				
	vector<int> v1;
	for( int i = 0; i < pinter1; i++ )
		v1.push_back( sequencia[ r1+i ] );
		
	vector<int> v2;
	for( int i = 0; i < pinter2; i++ )
		v2.push_back( sequencia[ r2+i ] );
		
	sequencia.erase( sequencia.begin() + r1, sequencia.begin() + r1+pinter1 );
	sequencia.erase( sequencia.begin() + r2-pinter1, sequencia.begin() + r2-pinter1+pinter2 ); 

	if ( r1 < r2 ) {		
		sequencia.insert( sequencia.begin() + r1, v2.begin(), v2.end() );
		sequencia.insert( sequencia.begin() + r2+pinter1, v1.begin(), v1.end() );
	} else {
		sequencia.insert( sequencia.begin() + r2, v1.begin(), v1.end() );
		sequencia.insert( sequencia.begin() + r1+pinter2, v2.begin(), v2.end() );
	}		
}
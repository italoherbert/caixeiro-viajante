
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>

#include "Perturbacao.h"

void Perturbacao::perturbacao( vector<int>& sequencia ) {
	srand( time( NULL ) );
	
	int v10 = (int)(sequencia.size() / 10 ) - 2;
	
	int inter1 = 2 + ( v10 > 0 ? rand() % v10 : 0 );
	int inter2 = 2 + ( v10 > 0 ? rand() % v10 : 0 );
	
	int rr = rand() % 2;
	if ( rr == 0 ) {
		int aux = inter1;
		inter1 = inter2;
		inter2 = aux;
	}
	
	int r1 = 1;
	if ( ( sequencia.size() - 2 - ( inter1 + inter2 ) ) > 0 )
		r1 = ( rand() % ( sequencia.size() - 2 - ( inter1 + inter2 ) ) ) + 1 ;
		
	int r2 = r1 + inter1 + 1;
	if ( sequencia.size() - 2 - ( r1 + inter1 ) - inter2 > 0 )
		r2 = r1 + inter1 + ( rand() % ( sequencia.size() - 2 - ( r1 + inter1 ) - inter2 ) ) + 1;		
		
	vector<int> v1;
	for( int i = 0; i < inter1; i++ )
		v1.push_back( sequencia[ r1+i ] );
		
	vector<int> v2;
	for( int i = 0; i < inter2; i++ )
		v2.push_back( sequencia[ r2+i ] );
		
	sequencia.erase( sequencia.begin() + r1, sequencia.begin() + r1+inter1 );
	sequencia.erase( sequencia.begin() + r2-inter1, sequencia.begin() + r2-inter1+inter2 ); 
                		
	sequencia.insert( sequencia.begin() + r1, v2.begin(), v2.end() );
	sequencia.insert( sequencia.begin() + r2-inter1+inter2, v1.begin(), v1.end() );		
}
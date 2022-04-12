#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include <string>

using namespace std;

typedef struct Solucao {	
	vector<int> sequencia;
	double valorObj;
} Solucao;

class CaixeiroViajante {
	
	protected:
		double** matrizAdj;
		int dim;
			
	public:
		bool carregaMatrizAdj( string arquivo );
		void imprimeMatrizAdj();
				
		double calculaCusto( vector<int> nos );		
				
		void readTSPData( int argc, char** argv );
		void printData();
		
		void imprimeSolucao( Solucao solucao );
		
		virtual Solucao calculaCaminho() = 0;
	
};

#endif
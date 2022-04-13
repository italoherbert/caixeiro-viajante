#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include <string>

using namespace std;

typedef struct Solucao {	
	vector<int> sequencia;
	double custo;
} Solucao;

class CaixeiroViajante {
				
	public:
		double** matrizAdj;
		int dim;
		
		bool carregaMatrizAdj( string arquivo );
		void imprimeMatrizAdj();
				
		double calculaCusto( vector<int> nos );		
				
		void readTSPData( int argc, char** argv );
		void printData();
		
		void imprimeSolucao( Solucao solucao );

		void imprimeSequencia( vector<int> sequencia );
		
		virtual Solucao calculaCaminho() = 0;
	
};

#endif
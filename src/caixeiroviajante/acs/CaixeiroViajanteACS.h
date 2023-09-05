#ifndef CAIXEIRO_VIAJANTE_ACS_H
#define CAIXEIRO_VIAJANTE_ACS_H

#include "../logica/CaixeiroViajante.h"

typedef struct TFormiga {
    double** feromonios;
    Solucao solucao;
} Formiga;

class CaixeiroViajanteACS : public CaixeiroViajante {

    private:
        double** feromonios;

        double* probsAux;
        int* probsJsAux;
        int probsAuxLen;

        double feromonioLocalFator = 0.1;
        double feromonioGlobalFator = 0.1;
        double feromonioIni = 0.0001;
        double alfa = 0.4;
        double beta = 2;
        double q0 = 0.9;

        int nIteracoes = 100;
        int formigasLen = 20;

        Formiga* formigas;

        void inicializa();
        void reiniciaFormigas();
        void atualizaFeromoniosGlobal( Formiga formiga );
        void atualizaFeromonioLocal( double** feromoniosLocal, int i, int melhorJ );

        bool construcao( int i, vector<int>& S, vector<int>& Q, double* custo );

        int selecionaMelhorVizinho( int i, vector<int>& Q );

        int probabilidadeArgmaxVizinho( int i, vector<int>& Q );
        void probabilidades( int i, vector<int>& Q, double* probs, int* probsJs, int* probsLen );

        void atualizaFeromonioLocalAposBuscaLocal( double** feromoniosLocal, vector<int>& sequencia );
    public:
		Solucao calculaCaminho();

};

#endif

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
        double* probs;
        int* probsJs;
        int probsLen;

        double feromonioAtualizacaoLocalFator = 0.1;
        double feromonioAtualizacaoGlobalFator = 0.1;
        double feromonioIni = 0.05;
        double alfa = 1;
        double beta = 5;

        int formigasLen = 10;

        Formiga* formigas;

        void inicializa();
        void reiniciaFormigas();
        void atualizaFeromoniosGlobal( Formiga formiga );

        void construcao( double** feromoniosLocal, int i, vector<int>& S, vector<int>& Q, double* custo );

        void probabilidades( int i, vector<int>& Q );
        int melhor( int i, vector<int>& Q );

    public:
		Solucao calculaCaminho();

};

#endif

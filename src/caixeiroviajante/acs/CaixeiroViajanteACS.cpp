#include "CaixeiroViajanteACS.h"

#include <vector>
#include <cfloat>
#include <algorithm>
#include <cmath>
#include <time.h>

#include "../logica/BuscaLocal.h"

Solucao CaixeiroViajanteACS::calculaCaminho() {
    BuscaLocal bl;

    inicializa();

    Solucao melhorSolucao;
    melhorSolucao.sequencia = {};
    melhorSolucao.custo = INFINITY;

    srand( time( NULL ) );

    for( int iter = 0; iter < nIteracoes; iter++ ) {
        reiniciaFormigas();

        Solucao melhorS;
        melhorS.sequencia = {};
        melhorS.custo = INFINITY;

        Formiga melhorFormiga;
        melhorFormiga.solucao = melhorS;

        for( int k = 0; k < formigasLen; k++ ) {
            int i = rand() % dim;
            if ( i == dim )
                i--;

            Formiga f = formigas[ k ];
            vector<int> Q = {};

            construcao( f.feromonios, i, f.solucao.sequencia, Q, &f.solucao.custo );
            bl.buscaLocal( matrizAdj, dim, &f.solucao );

            if ( f.solucao.custo < melhorFormiga.solucao.custo )
                melhorFormiga = f;
        }

        atualizaFeromoniosGlobal( melhorFormiga );

        if ( melhorFormiga.solucao.custo < melhorSolucao.custo )
            melhorSolucao = melhorFormiga.solucao;
    }

    return melhorSolucao;
}

void CaixeiroViajanteACS::inicializa() {
    feromonios = new double *[dim];
    for( int i = 0; i < dim; i++ ) {
        feromonios[ i ] = new double[ dim ];
        for( int j = 0; j < dim; j++ )
            feromonios[ i ][ j ] = feromonioIni;
    }

    formigas = new Formiga[ formigasLen ];
    for( int i = 0; i < formigasLen; i++ ) {
        formigas[ i ].feromonios = new double*[ dim ];
        for( int fi = 0; fi < dim; fi++ ) {
            formigas[ i ].feromonios[ fi ] = new double[ dim ];
            for( int fj = 0; fj < dim; fj++ )
                formigas[ i ].feromonios[ fi ][ fj ] = feromonioIni;
        }

        Solucao s;
        s.sequencia = {};
        s.custo = 0;

        formigas[ i ].solucao = s;
    }

    probs = new double[ dim ];
    probsJs = new int[ dim ];
    probsLen = 0;
}

void CaixeiroViajanteACS::reiniciaFormigas() {
    for( int k = 0; k < formigasLen; k++ ) {
        for( int i = 0; i < dim; i++ )
            for( int j = 0; j < dim; j++ )
                formigas[ k ].feromonios[ i ][ j ] = feromonioIni;
        Solucao s;
        s.sequencia = {};
        s.custo = 0;

        formigas[ k ].solucao = s;
    }
}

void CaixeiroViajanteACS::atualizaFeromoniosGlobal( Formiga formiga ) {
    for( int k = 0; k < dim-1; k++ ) {
        int i = formiga.solucao.sequencia[ k ];
        int j = formiga.solucao.sequencia[ k+1 ];
        double formigaFeromonio = formiga.feromonios[ i ][ j ];
        feromonios[ i ][ j ] = ( 1.0 - feromonioAtualizacaoGlobalFator ) * feromonios[ i ][ j ] + feromonioAtualizacaoGlobalFator * formigaFeromonio;
    }
}

void CaixeiroViajanteACS::construcao( double** feromoniosLocal, int i, vector<int>& S, vector<int>& Q, double* custo ) {
    if ( S.size() == 0 ) {
        S.push_back( i );
        for( int j = 0; j < dim; j++ )
            if ( j != i )
                Q.push_back( j );
        construcao( feromoniosLocal, i, S, Q, custo );
    } else if ( Q.size() > 0 ) {
        probabilidades( i, Q );

        int melhorJ = melhor( i, Q );

        int k = -1;
        for( int j = 0; k == -1 && j != Q.size(); j++ )
            if ( Q[j] == melhorJ )
                k = j;

        Q.erase( Q.begin() + k );
        S.push_back( melhorJ );

        (*custo) += matrizAdj[ i ][ melhorJ ];

        double fatorP = 1.0 / dim;
        feromoniosLocal[ i ][ melhorJ ] = ( 1.0 - feromonioAtualizacaoLocalFator ) * feromoniosLocal[ i ][ melhorJ ] + feromonioAtualizacaoLocalFator * fatorP;

        if ( S[ S.size()-1 ] == S[ 0 ] ) {
            if ( !Q.empty() ) {
                int j = S[ S.size() - 1 ];
                S.erase( S.begin() + S.size() - 1 );
                Q.push_back( j );
                (*custo) -= matrizAdj[ i ][ j ];

                construcao( feromoniosLocal, S[ S.size() - 1 ], S, Q, custo );
            }
        } else {
            construcao( feromoniosLocal, melhorJ, S, Q, custo );
        }
    } else {
        Q.push_back( S[ 0 ] );
        construcao( feromoniosLocal, i, S, Q, custo );
    }
}



void CaixeiroViajanteACS::probabilidades( int i, vector<int>& Q ) {
    double somaVizinhosP = 0;
    for( auto it = Q.begin(); it != Q.end(); it++ ) {
        int j = *it;

        if ( matrizAdj[ i ][ j ] > 0 ) {
            double f = feromonios[ i ][ j ];
            double invDist = 1.0 / matrizAdj[ i ][ j ];

            somaVizinhosP += pow( f, alfa ) * pow( invDist, beta );
        }
    }

    probsLen = 0;

    for( auto it = Q.begin(); it != Q.end(); it++ ) {
        int j = *it;

        if ( matrizAdj[ i ][ j ] > 0 ) {
            double f = feromonios[ i ][ j ];
            double invDist = 1.0 / matrizAdj[ i ][ j ];

            double p = 0;
            if ( somaVizinhosP == 0 ) {
                p = 0;
            } else {
                p = ( pow( f, alfa ) * pow( invDist, beta ) ) / somaVizinhosP;
            }

            probs[ probsLen ] = p;
            probsJs[ probsLen ] = j;
            probsLen++;
        }
    }
}

int CaixeiroViajanteACS::melhor( int i, vector<int>& Q ) {
    int melhorJ = -1;

    srand( time( NULL ) );
    double r = (double)(rand() % RAND_MAX) / (double)RAND_MAX;

    double soma = 0;
    for( int j = 0; melhorJ == -1 && j < probsLen; j++ ) {
        soma += probs[ j ];
        if ( r <= soma )
            melhorJ = probsJs[ j ];
    }

    return melhorJ;
}

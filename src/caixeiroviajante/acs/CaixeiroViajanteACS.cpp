#include "CaixeiroViajanteACS.h"

#include <vector>
#include <cfloat>
#include <algorithm>
#include <cmath>
#include <time.h>

#include "../logica/BuscaLocal.h"
#include "../logica/Perturbacao.h"

Solucao CaixeiroViajanteACS::calculaCaminho() {
    BuscaLocal bl;
    Perturbacao pert;

    inicializa();

    Solucao melhorSolucao;
    melhorSolucao.sequencia = {};
    melhorSolucao.custo = INFINITY;

    double melhorCusto = dim * 1000;

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

            construcao( f.feromonios, i, f.solucao.sequencia, Q, &f.solucao.custo, melhorCusto );

            f.solucaoConstruida = f.solucao;

            bl.buscaLocal( matrizAdj, dim, &f.solucao );

            if ( f.solucao.custo < melhorFormiga.solucao.custo )
                melhorFormiga = f;
        }

        atualizaFeromoniosGlobal( melhorFormiga );

        if ( melhorFormiga.solucao.custo < melhorSolucao.custo ) {
            melhorSolucao = melhorFormiga.solucao;
            melhorCusto = melhorFormiga.solucao.custo;
        }

        printf( "%d ", iter );
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
                formigas[ k ].feromonios[ i ][ j ] = 0;
        Solucao s;
        s.sequencia = {};
        s.custo = 0;

        formigas[ k ].solucao = s;
    }
}

void CaixeiroViajanteACS::atualizaFeromoniosGlobal( Formiga formiga ) {
    for( int k = 0; k < dim-1; k++ ) {
        int i = formiga.solucaoConstruida.sequencia[ k ];
        int j = formiga.solucaoConstruida.sequencia[ k+1 ];
        double formigaFeromonio = formiga.feromonios[ i ][ j ];
        feromonios[ i ][ j ] = ( 1.0 - feromonioAtualizacaoGlobalFator ) * feromonios[ i ][ j ] + feromonioAtualizacaoGlobalFator * formigaFeromonio;
    }
}

void CaixeiroViajanteACS::atualizaFeromonioLocal( double** feromoniosLocal, int i, int melhorJ, double melhorP, double melhorCusto ) {
    double p = dim / ( melhorP * melhorCusto );
    feromoniosLocal[ i ][ melhorJ ] = ( 1.0 - feromonioAtualizacaoLocalFator ) * feromoniosLocal[ i ][ melhorJ ] + feromonioAtualizacaoLocalFator * p;
}

void CaixeiroViajanteACS::construcao( double** feromoniosLocal, int i, vector<int>& S, vector<int>& Q, double* custo, double melhorCusto ) {
    if ( S.size() == 0 ) {
        S.push_back( i );
        for( int j = 0; j < dim; j++ )
            if ( j != i )
                Q.push_back( j );
        construcao( feromoniosLocal, i, S, Q, custo, melhorCusto );
    } else if ( Q.size() > 0 ) {
        probabilidades( i, Q );

        int melhorJ;
        double melhorP;
        selecionaMelhor( i, Q, &melhorJ, &melhorP );

        atualizaFeromonioLocal( feromoniosLocal, i, melhorJ, melhorP, melhorCusto );

        int k = -1;
        for( int j = 0; k == -1 && j != Q.size(); j++ )
            if ( Q[j] == melhorJ )
                k = j;

        Q.erase( Q.begin() + k );
        S.push_back( melhorJ );

        (*custo) += matrizAdj[ i ][ melhorJ ];

        if ( S[ S.size()-1 ] == S[ 0 ] ) {
            if ( !Q.empty() ) {
                int j = S[ S.size() - 1 ];
                S.erase( S.begin() + S.size() - 1 );
                Q.push_back( j );
                (*custo) -= matrizAdj[ i ][ j ];

                construcao( feromoniosLocal, S[ S.size() - 1 ], S, Q, custo, melhorCusto );
            }
        } else {
            construcao( feromoniosLocal, melhorJ, S, Q, custo, melhorCusto );
        }
    } else {
        Q.push_back( S[ 0 ] );
        construcao( feromoniosLocal, i, S, Q, custo, melhorCusto );
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

void CaixeiroViajanteACS::selecionaMelhor( int i, vector<int>& Q, int* melhorJ, double* melhorP ) {
    *melhorJ = -1;
    *melhorP = 0;

    srand( time( NULL ) );
    double r = (double)(rand() % RAND_MAX) / (double)RAND_MAX;

    double soma = 0;
    for( int j = 0; *melhorJ == -1 && j < probsLen; j++ ) {
        soma += probs[ j ];
        if ( r <= soma ) {
            *melhorJ = probsJs[ j ];
            *melhorP = probs[ j ];
        }
    }
}

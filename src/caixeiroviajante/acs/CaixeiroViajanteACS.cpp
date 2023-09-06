#include "CaixeiroViajanteACS.h"

#include <vector>
#include <cfloat>
#include <algorithm>
#include <cmath>
#include <time.h>

#include "../logica/BuscaLocal.h"

#include "../../util/util.h"

Solucao CaixeiroViajanteACS::calculaCaminho() {
    BuscaLocal bl;

    srand( time( 0 ) );

    inicializa();

    Solucao melhorSolucao;
    melhorSolucao.sequencia = {};
    melhorSolucao.custo = INFINITY;

    vector<int> Q = {};

    for( int iter = 0; iter < nIteracoes; iter++ ) {
        reiniciaFormigas();

        Solucao melhorS;
        melhorS.sequencia = {};
        melhorS.custo = INFINITY;

        Formiga melhorFormiga;
        melhorFormiga.solucao = melhorS;

        for( int k = 0; k < formigasLen; k++ ) {
            int i = rand() % dim;

            Formiga f = formigas[ k ];

            Q.clear();

            bool construiu = construcao( i, f.solucao.sequencia, Q, &f.solucao.custo );
            if ( !construiu )
                continue;

            bl.buscaLocal( matrizAdj, dim, &f.solucao );

            atualizaFeromoniosLocal( f );

            if ( f.solucao.custo < melhorFormiga.solucao.custo )
                melhorFormiga = f;
        }

        if ( melhorFormiga.solucao.custo < melhorSolucao.custo )
            melhorSolucao = melhorFormiga.solucao;

        atualizaFeromoniosGlobal( melhorFormiga );

        printf( "%d ", iter+1 );
    }

    printf( "\n\n" );

    return melhorSolucao;
}

void CaixeiroViajanteACS::inicializa() {
    feromonioIni = 1.0 / pow( dim, 2 );

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
                formigas[ i ].feromonios[ fi ][ fj ] = 0;
        }

        Solucao s;
        s.sequencia = {};
        s.custo = 0;

        formigas[ i ].solucao = s;
    }

    probsAux = new double[ dim ];
    probsJsAux = new int[ dim ];
    probsAuxLen = 0;
}

void CaixeiroViajanteACS::reiniciaFormigas() {
    for( int k = 0; k < formigasLen; k++ ) {
        for( int i = 0; i < dim; i++ )
            for( int j = 0; j < dim; j++ )
                formigas[ k ].feromonios[ i ][ j ] = 0;

        formigas[ k ].solucao.sequencia = {};
        formigas[ k ].solucao.custo = 0;
    }
}

void CaixeiroViajanteACS::atualizaFeromoniosGlobal( Formiga formiga ) {
    for( int i = 0; i < dim; i++ ) {
        for( int j = 0; j < dim; j++ ) {
            double formigaFeromonio = formiga.feromonios[ i ][ j ];
            feromonios[ i ][ j ] = ( 1.0 - feromonioGlobalFator ) * feromonios[ i ][ j ] + ( feromonioGlobalFator * formigaFeromonio );
        }
    }
}

void CaixeiroViajanteACS::atualizaFeromoniosLocal( Formiga formiga ) {
    double** feromoniosLocal = formiga.feromonios;
    vector<int> sequencia = formiga.solucao.sequencia;

    for( int i = 0; i < dim; i++ )
        for( int j = 0; j < dim; j++ )
            feromoniosLocal[ i ][ j ] = 0;

    for( int k = 0; k < sequencia.size()-1; k++ ) {
        int i = sequencia[ k ];
        int j = sequencia[ k+1 ];

        feromoniosLocal[ i ][ j ] = ( 1.0 - feromonioLocalFator ) * feromoniosLocal[ i ][ j ] + ( feromonioLocalFator * feromonioIni );
    }
}

bool CaixeiroViajanteACS::construcao( int i, vector<int>& S, vector<int>& Q, double* custo ) {
    if ( S.size() == 0 ) {
        S.push_back( i );
        for( int j = 0; j < dim; j++ )
            if ( i != j )
                Q.push_back( j );
        return construcao( i, S, Q, custo );
    } else if ( Q.size() > 0 ) {
        int melhorJ = selecionaMelhorVizinho( i, Q );

        if ( melhorJ != -1 ) {
            int k = -1;
            for( int j = 0; k == -1 && j < Q.size(); j++ )
                if ( Q[j] == melhorJ )
                    k = j;

            Q.erase( Q.begin() + k );
            S.push_back( melhorJ );

            (*custo) += matrizAdj[ i ][ melhorJ ];

            return construcao( melhorJ, S, Q, custo );
        } else {
            return false;
        }
    } else {
        if ( S[ 0 ] != S[ S.size() -1 ] ) {
            Q.push_back( S[ 0 ] );
            return construcao( i, S, Q, custo );
        } else {
            return true;
        }
    }
}

int CaixeiroViajanteACS::selecionaMelhorVizinho( int i, vector<int>& Q ) {
    int melhorJ = -1;

    double q = (double)rand() / (double)RAND_MAX;

    if ( q <= q0 ) {
        melhorJ = probabilidadeArgmaxVizinho( i, Q );
    } else {
        probabilidades( i, Q, probsAux, probsJsAux, &probsAuxLen );

        double r = (double)rand() / (double)RAND_MAX;

        double soma = 0;
        for( int j = 0; melhorJ == -1 && j < probsAuxLen; j++ ) {
            soma += probsAux[ j ];
            if ( r <= soma )
                melhorJ = probsJsAux[ j ];
        }
    }
    return melhorJ;
}

int CaixeiroViajanteACS::probabilidadeArgmaxVizinho( int i, vector<int>& Q ) {
    double argmaxP = -INFINITY;
    int argmaxJ = -1;
    for( auto it = Q.begin(); it != Q.end(); it++ ) {
        int j = *it;

        if ( matrizAdj[ i ][ j ] > 0 ) {
            double f = feromonios[ i ][ j ];
            double invDist = 1.0 / matrizAdj[ i ][ j ];

            double p = f * pow( invDist, beta );
            if ( p > argmaxP ) {
                argmaxP = p;
                argmaxJ = j;
            }
        }
    }

    return argmaxJ;
}

void CaixeiroViajanteACS::probabilidades( int i, vector<int>& Q, double* probs, int* probsJs, int* probsLen ) {
    double somaVizinhosP = 0;
    for( auto it = Q.begin(); it != Q.end(); it++ ) {
        int j = *it;

        if ( matrizAdj[ i ][ j ] > 0 ) {
            double f = feromonios[ i ][ j ];
            double invDist = 1.0 / matrizAdj[ i ][ j ];

            somaVizinhosP += pow( f, alfa ) * pow( invDist, beta );
        }
    }

    *probsLen = 0;

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

            probs[ *probsLen ] = p;
            probsJs[ *probsLen ] = j;
            (*probsLen)++;
        }
    }
}

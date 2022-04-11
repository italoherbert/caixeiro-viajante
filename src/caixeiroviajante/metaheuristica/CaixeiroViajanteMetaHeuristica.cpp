#include "CaixeiroViajanteMetaHeuristica.h"
#include "Construcao.h"

#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>

Solucao CaixeiroViajanteMetaHeuristica::calculaCaminho() {	
	Construcao c;	
	return c.construcao( matrizAdj, dim );
}


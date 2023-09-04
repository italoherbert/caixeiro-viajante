#ifndef CAIXEIRO_VIAJANTE_HEURISTICA_LISTENER_H
#define CAIXEIRO_VIAJANTE_HEURISTICA_LISTENER_H

class CaixeiroViajanteMetaHeuristicaListener {
	
	public:
		virtual void executouIter( int i, int maxI ) = 0;
		virtual void executouIterILS( int i, int maxI ) = 0;
	
};

#endif
#ifndef MATRIZ_PERCORR_H
#define MATRIZ_PERCORR_H

#include "arv.h"

#include <algorithm>

namespace matriz
{
	////////////////////////////////////////
	/*
	 * Encontra o ancestral comum mais próximo "subindo" a árvore.
	 */
	No ancestralSimples(No a, No b)
	{
		while (a->nivel > b->nivel)
			a = a->pai;
		while (b->nivel > a->nivel)
			b = b->pai;
		while (a != b)
		{
			a = a->pai;
			b = b->pai;
		}
		return a;
	}

	////////////////////////////////////////
	No ancestral(Arvore* arv, No a, No b)
	{
		return arv->ancestral[std::max(a->id, b->id)][std::min(a->id, b->id)];
	}
}

#endif /* MATRIZ_PERCORR_H */

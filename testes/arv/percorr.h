#ifndef PERCORR_H
#define PERCORR_H

#include "arv.h"

////////////////////////////////////////
/*
 * Encontra o ancestral comum mais próximo "subindo" a árvore.
 */
No* ancestralSimples(No* a, No* b)
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
No* ancestral(Arvore* arv, No* a, No* b)
{
	/* Opções:
	 * 	mascaras[bsf(dif)] // bit scan forward (__builtin_ctz(x))
	 * 	(signed) ((dif-1) ^ dif) >> 1
	 * 	((dif-1) | dif) ^ dif
	 */
	unsigned long dif, bit, masc, id;
	// Bits que diferem
	dif = a->id ^ b->id;
	// Primeiro diferente (usar para encontrar o array do nível?)
	bit = dif & (-dif);
	masc = bit - 1; // Todos bits antes do primeiro diferente
	//masc = (((dif-1) ^ dif) >> 1);
	// Id do ancestral comum mais próximo
	id = a->id & masc; // a->id ou b->id, tanto faz
	
	// TODO Encontrar nó
	
}

#endif /* PERCORR_H */

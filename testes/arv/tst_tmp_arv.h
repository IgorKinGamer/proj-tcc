/*
 * Classes para testes de tempo de ancestral comum usando as estruturas em arv.h
 */

#ifndef TST_TMP_ARV_H
#define TST_TMP_ARV_H

#include "arv.h"
#include "constr.h"
#include "percorr.h"
#include "tst_tmp.h"

// Teste usando Arvore e No
template <int... NUM_ITER>
class TesteAncestralComumArvore : public BaseTesteAncestralComum<Arvore*, No, NUM_ITER...>
{
	Arvore* criarArvore(std::vector<int> graus)
	{
		return new Arvore(construirArvore(graus.size(), graus.data()), graus.size()+1);
	}
	
	std::vector<No> pegarFolhas(Arvore *a)
	{
		int ultNivel = a->numNiveis - 1;
		No *folhas = a->nosNiveis[ultNivel];
		return std::vector<No>(folhas, folhas + a->nosPorNivel[ultNivel]);
	}
	
	void destruirArvore(Arvore *a)
	{
		delete a;
	}
};

// Teste usando ancestral comum simples
template <int... NUM_ITER>
class TesteAncestralComumSimples : public TesteAncestralComumArvore<NUM_ITER...>
{
	No ancestralComum(Arvore *arv, No a, No b)
	{
		return ancestralSimples(a, b);
	}
};

// Teste usando ancestral comum novo (usando as estruturas)
template <int... NUM_ITER>
class TesteAncestralComumNovo : public TesteAncestralComumArvore<NUM_ITER...>
{
	No ancestralComum(Arvore *arv, No a, No b)
	{
		return ancestral(arv, a, b);
	}
};

// Teste que não usa nada: Mede o overhead do loop
template <int... NUM_ITER>
class TesteAncestralComumOverhead : public TesteAncestralComumArvore<NUM_ITER...>
{
	No ancestralComum(Arvore *arv, No a, No b)
	{
		return nullptr;
	}
};

#endif /* TST_TMP_ARV_H */

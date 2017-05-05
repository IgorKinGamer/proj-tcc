/*
 * Classes para testes de tempo de ancestral comum usando as estruturas em arv.h
 */

#ifndef TST_TMP_ARV_H
#define TST_TMP_ARV_H

#include "tst_tmp.h"
#include "../arv/arv.h"
#include "../arv/constr.h"
#include "../arv/percorr.h"

// Teste usando Arvore e No
class TesteAncestralComumArvore : public BaseTesteAncestralComum<Arvore*, No>
{
	public:
	
	TesteAncestralComumArvore(int nI, int nIA) : BaseTesteAncestralComum(nI, nIA) {};
	
	Arvore* criarArvore(std::vector<int> graus)
	{
		return new Arvore(construirArvore(graus), graus.size()+1);
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
class TesteAncestralComumSimples : public TesteAncestralComumArvore
{
	public:
	
	TesteAncestralComumSimples(int nI, int nIA) : TesteAncestralComumArvore(nI, nIA) {};
	
	No ancestralComum(Arvore *arv, No a, No b)
	{
		return ancestralSimples(a, b);
	}
};

// Teste usando ancestral comum novo (usando as estruturas)
class TesteAncestralComumNovo : public TesteAncestralComumArvore
{
	public:
	
	TesteAncestralComumNovo(int nI, int nIA) : TesteAncestralComumArvore(nI, nIA) {};
	
	No ancestralComum(Arvore *arv, No a, No b)
	{
		return ancestral(arv, a, b);
	}
};

// Teste que não usa nada: Mede o overhead do loop
class TesteAncestralComumOverhead : public TesteAncestralComumArvore
{
	public:
	
	TesteAncestralComumOverhead(int nI, int nIA) : TesteAncestralComumArvore(nI, nIA) {};
	
	No ancestralComum(Arvore *arv, No a, No b)
	{
		return nullptr;
	}
};

#endif /* TST_TMP_ARV_H */

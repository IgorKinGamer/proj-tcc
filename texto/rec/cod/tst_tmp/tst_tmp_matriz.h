/*
 * Classes para testes de tempo de ancestral comum usando as estruturas em arv.h
 */

#ifndef TST_TMP_MATRIZ_H
#define TST_TMP_MATRIZ_H

#include "tst_tmp.h"
#include "../matriz/arv.h"
#include "../matriz/constr.h"
#include "../matriz/percorr.h"

namespace matriz
{
	// Teste usando matriz Arvore e No
	class TesteAncestralComumMatriz : public BaseTesteAncestralComum<Arvore*, No>
	{
		public:
		
		TesteAncestralComumMatriz(int nI, int nIA) : BaseTesteAncestralComum(nI, nIA) {};
		
		Arvore* criarArvore(std::vector<int> graus)
		{
			return new Arvore(construirArvore(graus));
		}
		
		std::vector<No> pegarFolhas(Arvore *a)
		{
			std::vector<No> vetorFolhas;
			pegarFolhas(a->raiz, vetorFolhas);
			No *folhas = vetorFolhas.data();
			return std::vector<No>(folhas, folhas + vetorFolhas.size());
		}
		
		void pegarFolhas(No no, std::vector<No> &nos)
		{
			if (no->numFilhos == 0)
				nos.push_back(no);
			else
				for (No filho : std::vector<No>(no->filhos, no->filhos+no->numFilhos))
					pegarFolhas(filho, nos);
		}
		
		No ancestralComum(Arvore *arv, No a, No b)
		{
			return ancestral(arv, a, b);
		}
		
		int acessar(No n)
		{
			return int(n->id);
		}
		
		void destruirArvore(Arvore *a)
		{
			delete a;
		}
	};
}

#endif /* TST_TMP_MATRIZ_H */

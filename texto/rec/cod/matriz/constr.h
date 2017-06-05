#ifndef MATRIZ_CONSTR_H
#define MATRIZ_CONSTR_H

#include <vector>

namespace matriz
{
	////////////////////////////////////////
	// Constrói árvore simétrica com niveis níveis abaixo da raiz
	// e graus[i] filhos para cada nó no nível i (nível 0 = raiz).
	// <raiz> deve ser um nó (até então folha) válido (pai, id e nivel válidos)
	// numFilhos e filhos de raiz são atribuídos.
	static void construirSubArvore(No raiz, int niveis, int *graus, unsigned &proxID)
	{
		if (niveis == 0) // Nó folha
		{
			raiz->numFilhos = 0;
			raiz->filhos = nullptr;
		}
		else
		{
			int grau = *graus, nivelF = raiz->nivel + 1;
			++graus;
			--niveis;
			// Cria filhos
			No *filhos = new No[grau];
			for (int i = 0; i < grau; i++)
			{
				No f = new estr_No();
				f->id = proxID++;
				f->pai = raiz;
				f->nivel = nivelF;
				filhos[i] = f;
				construirSubArvore(f, niveis, graus, proxID);
			}
			raiz->numFilhos = grau;
			raiz->filhos = filhos;
		}
	}

	////////////////////////////////////////
	// Constrói árvore simétrica
	// niveis: Níveis *abaixo* da raiz
	No construirArvore(int niveis, int *graus)
	{
		unsigned proxID = 0;
		
		No raiz = new estr_No();
		raiz->id = proxID++;
		raiz->pai = NULL;
		raiz->nivel = 0;
		construirSubArvore(raiz, niveis, graus, proxID);
		return raiz;
	}

	No construirArvore(std::vector<int> graus)
	{
		return construirArvore(graus.size(), graus.data());
	}
}

#endif /* MATRIZ_CONSTR_H */

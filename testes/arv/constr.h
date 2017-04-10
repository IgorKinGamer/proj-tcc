#ifndef CONSTR_H
#define CONSTR_H

////////////////////////////////////////
// Constrói árvore simétrica com niveis níveis abaixo da raiz
// e graus[i] filhos para cada nó no nível i (nível 0 = raiz).
// <raiz> deve ser um nó (até então folha) válido (pai, id e nivel válidos)
// numFilhos e filhos de raiz são atribuídos.
static void construirSubArvore(No raiz, int niveis, int *graus, int soma)
{
	if (niveis == 0) // Nó folha
	{
		raiz->numFilhos = 0;
		raiz->filhos = NULL;
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
			f->id = raiz->id | (1 << (soma+i));
			f->pai = raiz;
			f->nivel = nivelF;
			filhos[i] = f;
			construirSubArvore(f, niveis, graus, soma+grau);
		}
		raiz->numFilhos = grau;
		raiz->filhos = filhos;
	}
}

////////////////////////////////////////
// Constrói árvore simétrica
No construirArvore(int niveis, int *graus)
{
	No raiz = new estr_No();
	raiz->id = 0;
	raiz->pai = NULL;
	raiz->nivel = 0;
	construirSubArvore(raiz, niveis, graus, 0);
	return raiz;
}

#endif /* CONSTR_H */

#ifndef CONSTR_H
#define CONSTR_H

////////////////////////////////////////
// Constrói árvore simétrica com niveis níveis abaixo da raiz
// e graus[i] filhos para cada nó no nível i (nível 0 = raiz).
// <raiz> deve ser um nó (até então folha) válido (pai, id e nivel válidos)
// numFilhos e filhos (se necessário) de raiz são atribuídos.
static void construirSubArvore(No *raiz, unsigned niveis, unsigned *graus, unsigned soma)
{
	if (niveis == 0) // Nó folha
		raiz->numFilhos = 0;
	else
	{
		unsigned grau = *graus, nivelF = raiz->nivel + 1;
		++graus;
		--niveis;
		// Cria filhos
		No **filhos = new No*[grau];
		for (unsigned i = 0; i < grau; i++)
		{
			No *f = new No();
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
No* construirArvore(unsigned niveis, unsigned *graus)
{
	No *raiz = new No();
	raiz->id = 0;
	raiz->pai = NULL;
	raiz->nivel = 0;
	construirSubArvore(raiz, niveis, graus, 0);
	return raiz;
}

#endif /* CONSTR_H */

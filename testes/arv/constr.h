#ifndef CONSTR_H
#define CONSTR_H

////////////////////////////////////////
// Constr�i �rvore sim�trica com niveis n�veis abaixo da raiz
// e graus[i] filhos para cada n� no n�vel i (n�vel 0 = raiz).
// <raiz> deve ser um n� (at� ent�o folha) v�lido (pai, id e nivel v�lidos)
// numFilhos e filhos de raiz s�o atribu�dos.
static void construirSubArvore(No raiz, int niveis, int *graus, int soma)
{
	if (niveis == 0) // N� folha
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
// Constr�i �rvore sim�trica
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

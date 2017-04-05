#ifndef ARV_H
#define ARV_H

#include <iostream>
#include <bitset>
#include <climits>

using namespace std;

struct No
{
	// Id usado para encontrar ancestral mais próximo
	unsigned long id;
	
	No *pai;
	unsigned nfilhos;
	No **filhos;
	
	unsigned nivel;
	
	void imprimir() { imprimir(0); }
	
	friend inline ostream& operator<<(ostream &o, const No* n) { o << n->id; return o; } // !!!
	
	void imprimir(int ind)
	{
		for (int i = 0; i < ind; i++)
			cout << '|'; // Indentação
		cout << "No " << bitset<sizeof(id)*CHAR_BIT>(id) << " (" << nivel << ' ' << nfilhos << ')' << endl;
		++ind;
		for (int f = 0; f < nfilhos; f++)
			filhos[f]->imprimir(ind);
	}
};

class Arvore
{
	public:
	No *raiz;
	
	// Módulo usado para cada nível da árvore
	// Raiz e folhas não precisam
	unsigned long *mods;
	
	
};


// Constrói árvore simétrica com niveis níveis abaixo da raiz
// e graus[i] filhos para cada nó no nível i (nível 0 = raiz).
// <raiz> deve ser um nó (até então folha) válido (pai, id e nivel válidos)
// nfilhos e filhos (se necessário) de raiz são atribuídos.
static void construirSubArvore(No *raiz, int niveis, int *graus, int soma)
{
	if (niveis == 0) // Nó folha
		raiz->nfilhos = 0;
	else
	{
		int grau = *graus, nivelF = raiz->nivel + 1;
		++graus;
		--niveis;
		// Cria filhos
		No **filhos = new No*[grau];
		for (int i = 0; i < grau; i++)
		{
			No *f = new No();
			f->id = raiz->id | (1 << (soma+i));
			f->pai = raiz;
			f->nivel = nivelF;
			filhos[i] = f;
			construirSubArvore(f, niveis, graus, soma+grau);
		}
		raiz->nfilhos = grau;
		raiz->filhos = filhos;
	}
}

////////////////////////////////////////
No* construirArvore(int niveis, int *graus)
{
	No *raiz = new No();
	raiz->id = 0;
	raiz->pai = NULL;
	raiz->nivel = 0;
	construirSubArvore(raiz, niveis, graus, 0);
	return raiz;
}

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

#endif /* ARV_H */

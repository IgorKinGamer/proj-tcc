#ifndef ARV_H
#define ARV_H

#include <iostream>
#include <bitset>
#include <climits>

using namespace std;

struct No
{
	// Id usado para encontrar ancestral mais pr�ximo
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
			cout << '|'; // Indenta��o
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
	
	// M�dulo usado para cada n�vel da �rvore
	// Raiz e folhas n�o precisam
	unsigned long *mods;
	
	
};


// Constr�i �rvore sim�trica com niveis n�veis abaixo da raiz
// e graus[i] filhos para cada n� no n�vel i (n�vel 0 = raiz).
// <raiz> deve ser um n� (at� ent�o folha) v�lido (pai, id e nivel v�lidos)
// nfilhos e filhos (se necess�rio) de raiz s�o atribu�dos.
static void construirSubArvore(No *raiz, int niveis, int *graus, int soma)
{
	if (niveis == 0) // N� folha
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
 * Encontra o ancestral comum mais pr�ximo "subindo" a �rvore.
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
	/* Op��es:
	 * 	mascaras[bsf(dif)] // bit scan forward (__builtin_ctz(x))
	 * 	(signed) ((dif-1) ^ dif) >> 1
	 * 	((dif-1) | dif) ^ dif
	 */
	unsigned long dif, bit, masc, id;
	// Bits que diferem
	dif = a->id ^ b->id;
	// Primeiro diferente (usar para encontrar o array do n�vel?)
	bit = dif & (-dif);
	masc = bit - 1; // Todos bits antes do primeiro diferente
	//masc = (((dif-1) ^ dif) >> 1);
	// Id do ancestral comum mais pr�ximo
	id = a->id & masc; // a->id ou b->id, tanto faz
	
	// TODO Encontrar n�
	
}

#endif /* ARV_H */

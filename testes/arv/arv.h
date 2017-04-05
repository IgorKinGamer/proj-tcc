#ifndef ARV_H
#define ARV_H

#include <iostream>
#include <bitset>
#include <climits>

using namespace std;

typedef unsigned long ID;

struct No
{
	// Id usado para encontrar ancestral mais pr�ximo
	ID id;
	
	No *pai;
	unsigned numFilhos;
	No **filhos;
	
	// N�vel do n�: 0 -> raiz
	unsigned nivel;
	
	void imprimir() { imprimir(0); }
	
	friend inline ostream& operator<<(ostream &o, const No* n) { o << n->id; return o; } // !!!
	
	void imprimir(int ind)
	{
		for (int i = 0; i < ind; i++)
			cout << '|'; // Indenta��o
		cout << "No " << bitset<sizeof(id)*CHAR_BIT>(id) << " (" << nivel << ' ' << numFilhos << ')' << endl;
		++ind;
		for (int f = 0; f < numFilhos; f++)
			filhos[f]->imprimir(ind);
	}
};

class Arvore
{
	public:
	No *raiz;
	
	// Quantidade de n�veis
	unsigned numNiveis;
	// Arranjos por n�vel
	unsigned long **niveis;
	
	// M�dulo usado para cada n�vel da �rvore
	// Raiz e folhas n�o precisam
	unsigned long *mods;
	
	Arvore(No* r, unsigned int nNiveis, unsigned int)
	{
		raiz = r;
		numNiveis = nNiveis;
	}
	
	~Arvore()
	{
		delete raiz;
	}
	
	// Descobrir quantos n�s por n�vel
	// Descobrir quantos n�veis s�o realmente necess�rios
	// Pegar todos os n�s de um n�vel em um array e descobrir o m
	
	void montarEstruturas()
	{
		// Descobre quantos n�s h� em cada nivel
		unsigned nosPorNivel[numNiveis]; // Inicia com 0
		for (int i = 0; i < numNiveis; i++)
			cout << nosPorNivel[i] << '\n'; 
		preencherNosPorNivel(nosPorNivel, raiz);
		for (int i = 0; i < numNiveis; i++)
			cout << nosPorNivel[i] << '\n'; 
		
		// Cria um arranjo por n�vel com os ids do nivel para descobrir os m�dulos
		ID* idsNiveis[numNiveis];
		for (unsigned nivel = 0; nivel < numNiveis; nivel++)
			idsNiveis[nivel] = new ID[nosPorNivel[nivel]];
		// Pega os ids
		//preencherIdsNiveis(idsNiveis, raiz);
		// Libera
		for (unsigned nivel = 0; nivel < numNiveis; nivel++)
			delete idsNiveis[nivel];
		
		// Descobrir o m�dulo para cada n�vel
		
	}
	
	void preencherNosPorNivel(unsigned *nosPorNivel, No *no)
	{
		++nosPorNivel[no->nivel];
		for (int i = 0; i < no->numFilhos; i++)
			preencherNosPorNivel(nosPorNivel, no->filhos[i]);
	}
	
	//void preencherIdsNiveis
};

#endif /* ARV_H */

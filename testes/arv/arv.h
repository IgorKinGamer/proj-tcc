#ifndef ARV_H
#define ARV_H

#include <iostream>
#include <bitset>
#include <climits>

#include "defs.h"
#include "arv_mod.h"

using namespace std;

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
	
	// TODO Destrutor
};

class Arvore
{
	public:
	No *raiz;
	
	// Quantidade de n�veis
	unsigned numNiveis;
	// Arranjos por n�vel
	No **hashNiveis;
	
	// M�dulo usado para cada n�vel da �rvore
	// Raiz e folhas n�o precisam
	DadosFuncao *dadosFuncao;
	
	Arvore(No* r, unsigned nNiveis)
	{
		raiz = r;
		numNiveis = nNiveis;
		dadosFuncao = new DadosFuncao[numNiveis];
	}
	
	~Arvore()
	{
		for (unsigned i = 0; i < numNiveis; i++)
			delete hashNiveis[i];
		delete hashNiveis;
		delete raiz;
		delete dadosFuncao;
	}
	
	// Descobrir quantos n�s por n�vel
	// Descobrir quantos n�veis s�o realmente necess�rios
	// Pegar todos os n�s de um n�vel em um array e descobrir o m
	
	void montarEstruturas()
	{
		// Descobre quantos n�s h� em cada nivel
		unsigned* nosPorNivel = new unsigned[numNiveis](); // Inicia com 0
		preencherNosPorNivel(nosPorNivel, raiz);
		
		// Cria um arranjo por n�vel com os ids do nivel para descobrir os m�dulos
		ID* idsNiveis[numNiveis];
		for (unsigned nivel = 0; nivel < numNiveis; nivel++)
			idsNiveis[nivel] = new ID[nosPorNivel[nivel]];
		// Pr�xima posi��o a preencher de cada n�vel
		unsigned *proxPos = new unsigned[numNiveis]();
		// Pega os ids
		preencherIdsNiveis(idsNiveis, proxPos, raiz);
		// !!! Mostra ids
		/*for (int i = 0; i < numNiveis; i++)
		{
			for (int j = 0; j < nosPorNivel[i]; j++)
				cout << bitset<sizeof(ID)*CHAR_BIT>(idsNiveis[i][j]) << '\n';
			cout << '\n';
		}*/
		
		// Descobrir o m�dulo para cada n�vel (menos raiz e �ltimo n�vel)
		for (unsigned nivel = 1; nivel < numNiveis-1; nivel++)
		{
			unsigned numNos = nosPorNivel[nivel];
			ID *ids = idsNiveis[nivel];
			// Sa�das (img = ((<id> & mascE ^ mascXor | mascOu) + ad) % m)
			buscarFuncaoIdeal(numNos, ids, &dadosFuncao[nivel]);
		}
		
		// Libera
		for (unsigned nivel = 0; nivel < numNiveis; nivel++)
			delete idsNiveis[nivel];
		delete nosPorNivel;
		delete proxPos;
	}
	
	void preencherNosPorNivel(unsigned *nosPorNivel, No *no)
	{
		++nosPorNivel[no->nivel];
		for (int i = 0; i < no->numFilhos; i++)
			preencherNosPorNivel(nosPorNivel, no->filhos[i]);
	}
	
	void preencherIdsNiveis(ID **idsNiveis, unsigned *proxPos, No *no)
	{
		// Coloca id na pr�xima posi��o da linha do n�vel do n�
		idsNiveis[no->nivel][proxPos[no->nivel]++] = no->id;
		for (unsigned i = 0; i < no->numFilhos; i++)
			preencherIdsNiveis(idsNiveis, proxPos, no->filhos[i]);
	}
};

#endif /* ARV_H */

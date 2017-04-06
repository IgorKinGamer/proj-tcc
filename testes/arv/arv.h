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
	// Id usado para encontrar ancestral mais próximo
	ID id;
	
	No *pai;
	unsigned numFilhos;
	No **filhos;
	
	// Nível do nó: 0 -> raiz
	unsigned nivel;
	
	void imprimir() { imprimir(0); }
	
	friend inline ostream& operator<<(ostream &o, const No* n) { o << n->id; return o; } // !!!
	
	void imprimir(int ind)
	{
		for (int i = 0; i < ind; i++)
			cout << '|'; // Indentação
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
	
	// Quantidade de níveis
	unsigned numNiveis;
	// Arranjos por nível
	No **hashNiveis;
	
	// Módulo usado para cada nível da árvore
	// Raiz e folhas não precisam
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
	
	// Descobrir quantos nós por nível
	// Descobrir quantos níveis são realmente necessários
	// Pegar todos os nós de um nível em um array e descobrir o m
	
	void montarEstruturas()
	{
		// Descobre quantos nós há em cada nivel
		unsigned* nosPorNivel = new unsigned[numNiveis](); // Inicia com 0
		preencherNosPorNivel(nosPorNivel, raiz);
		
		// Cria um arranjo por nível com os ids do nivel para descobrir os módulos
		ID* idsNiveis[numNiveis];
		for (unsigned nivel = 0; nivel < numNiveis; nivel++)
			idsNiveis[nivel] = new ID[nosPorNivel[nivel]];
		// Próxima posição a preencher de cada nível
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
		
		// Descobrir o módulo para cada nível (menos raiz e último nível)
		for (unsigned nivel = 1; nivel < numNiveis-1; nivel++)
		{
			unsigned numNos = nosPorNivel[nivel];
			ID *ids = idsNiveis[nivel];
			// Saídas (img = ((<id> & mascE ^ mascXor | mascOu) + ad) % m)
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
		// Coloca id na próxima posição da linha do nível do nó
		idsNiveis[no->nivel][proxPos[no->nivel]++] = no->id;
		for (unsigned i = 0; i < no->numFilhos; i++)
			preencherIdsNiveis(idsNiveis, proxPos, no->filhos[i]);
	}
};

#endif /* ARV_H */

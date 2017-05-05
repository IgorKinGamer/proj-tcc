#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
#include <bitset>
#include <climits>
#include <string>
#include <vector>

#include <algorithm>

#include "config_dbg.h"

namespace matriz
{
	struct estr_No;
	typedef estr_No* No;

	using ID = unsigned;

	struct estr_No
	{
		// "Preenchimento" para testes
		char preenchimento[ConfigDebug::No::PREENCHIMENTO];
		
		// Id usado para encontrar ancestral mais próximo
		ID id;
		
		No pai;
		int numFilhos;
		No *filhos;
		
		// Nível do nó: 0 -> raiz
		int nivel;
		
		void imprimir() { imprimir(0); }
		
		friend inline std::ostream& operator<<(std::ostream &o, const No n) { o << n->id; return o; } // !!!
		
		void imprimir(int ind)
		{
			for (int i = 0; i < ind; i++)
				std::cout << '|'; // Indentação
			std::cout << "No " << id
				/*<< " (" << nivel << ' ' << numFilhos << ')'*/ << '\n';
			++ind;
			for (int f = 0; f < numFilhos; f++)
				filhos[f]->imprimir(ind);
		}
		
		// TODO Destrutor
	};

	No ancestralSimples(No a, No b);

	class Arvore
	{
		static const bool DEBUG = ConfigDebug::Arvore::DEBUG;
		static const int DEBUG_MAX_LINHAS_IDS = ConfigDebug::Arvore::DEBUG_MAX_LINHAS_IDS;
		
		public:
		No raiz;
		
		// Matriz de ancestral comum
		// ancestral[i][j], i > j
		No **ancestral;
		
		// Quantidade de nós da árvore inteira
		int numNos;
		
		Arvore(No r)
		{
			raiz = r;
			montarEstruturas();
		}
		
		~Arvore()
		{
			for (int i = 1; i < numNos; i++)
				delete[] ancestral[i];
			delete[] ancestral;
		}
		
		void montarEstruturas()
		{
			if (DEBUG)
				std::cout << "<Arvore::montarEstruturas>\n";
			
			// Lista nós
			std::vector<No> nos;
			pegarNos(raiz, nos);
			numNos = nos.size();
			
			if (DEBUG)
				std::cout << "Número de nós: " << numNos << '\n';
			
			// Cria a (meia) matriz
			ancestral = new No*[numNos] ();
			for (int i = 1; i < numNos; i++)
				ancestral[i] = new No[i];
			
			// Preenche a matriz
			for (int i = 0; i < numNos; i++)
			{
				for (int j = i+1; j < numNos; j++)
				{
					No a, b;
					a = nos[i];
					b = nos[j];
					ancestral[std::max(a->id, b->id)][std::min(a->id, b->id)] = ancestralSimples(a, b);
				}
			}
		}
		
		void pegarNos(No no, std::vector<No> &nos)
		{
			nos.push_back(no);
			for (No filho : std::vector<No>(no->filhos, no->filhos+no->numFilhos))
				pegarNos(filho, nos);
		}
	};
}

#endif /* MATRIZ_H */

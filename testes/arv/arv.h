#ifndef ARV_H
#define ARV_H

#include <iostream>
#include <bitset>
#include <climits>
#include <string>
#include <vector>

#include <algorithm>

#include "id.h"
#include "arv_mod.h"

struct estr_No;
typedef estr_No* No;

struct estr_No
{
	// Id usado para encontrar ancestral mais próximo
	ID id;
	
	No pai;
	int numFilhos;
	No *filhos;
	
	// Nível do nó: 0 -> raiz
	int nivel;
	
	void imprimir(int nBits = sizeof(id)*CHAR_BIT) { imprimir(0, nBits); }
	
	friend inline std::ostream& operator<<(std::ostream &o, const No n) { o << n->id; return o; } // !!!
	
	void imprimir(int ind, int nBits)
	{
		for (int i = 0; i < ind; i++)
			std::cout << '|'; // Indentação
		std::cout << "No " << idBin(nBits)
			/*<< " (" << nivel << ' ' << numFilhos << ')'*/ << '\n';
		++ind;
		for (int f = 0; f < numFilhos; f++)
			filhos[f]->imprimir(ind, nBits);
	}
	
	std::string idBin(int bits)
	{
		return ::idBin(id, bits);
	}
	
	// TODO Destrutor
};

class Arvore
{
	const bool DEBUG = true;
	
	public:
	No raiz;
	
	// Quantidade de níveis
	int numNiveis;
	
	// Quantidade de nós e grau de cada nível
	int *nosPorNivel, *grauPorNivel;
	
	// Nós de cada nível
	No **nosNiveis;
	
	// Arranjos por nível com nós na posição resultante
	No **imagemNiveis;
	// NOTA: Níveis cujos nós têm um só filho poderiam ser totalmente
	// omitidos na busca do ancestral mais próximo, desde o bit ocupado
	// desnecessariamente no id até as estruturas (pois não são ancestral
	// mais próximo de nenhum par de nós distintos), mas isso exigiria um
	// tratamento todo especial...
	// A não ser que os nós tenham um No *filhosEspecial.
	
	// nivelDosBits[i]: Nível do ancestral mais próximo de a e b quando
	// o primeiro bit que difere entre a->id e b->id é i
	int *nivelDosBits;
	
	// Função usada para cada nível da árvore
	// Folhas não precisam
	DadosFuncao *dadosFuncao;
	
	Arvore(No r, int nNiveis)
	{
		raiz = r;
		numNiveis = nNiveis;
		dadosFuncao = new DadosFuncao[numNiveis];
		
		montarEstruturas();
	}
	
	~Arvore()
	{
		delete raiz;
		
		delete[] nosPorNivel;
		delete[] grauPorNivel;
		for (int i = 0; i < numNiveis; i++)
			delete[] nosNiveis[i];
		delete[] nosNiveis;
		
		for (int i = 0; i < numNiveis-1; i++)
			delete[] imagemNiveis[i];
		delete[] imagemNiveis;
		delete[] nivelDosBits;
		delete[] dadosFuncao;
	}
	
	// Descobrir quantos nós por nível
	// Descobrir quantos níveis são realmente necessários
	// Pegar todos os nós de um nível em um array e descobrir o m
	
	int bitParaNivel(ID bit)
	{
		int pos = __builtin_ctz(bit); // Primeiro bit 1
		return nivelDosBits[pos];
	}
	
	void montarEstruturas()
	{
		if (DEBUG)
			std::cout << "<Arvore::montarEstruturas>\n";
		
		// Descobre quantos nós há em cada nivel e o grau máximo
		nosPorNivel = new int[numNiveis] (); // Zerado
		grauPorNivel = new int[numNiveis] (); // Zerado
		preencherNosGrauPorNivel(nosPorNivel, grauPorNivel, raiz);
		
		// Preenche mapeamento (primeiro bit diferente -> nivel do ancestral)
		// Se os graus (da raiz, filhos e netos) são [2, 3, 3]: [0, 0, 1, 1, 1, 2, 2, 2]
		int somaGraus = std::accumulate(grauPorNivel, grauPorNivel + numNiveis-1, 0);
		if (DEBUG)
			std::cout << "Somatório dos graus: " << somaGraus << '\n';
		nivelDosBits = new int[somaGraus];
		int *bit = nivelDosBits, nivelAnc = 0;
		while (grauPorNivel[nivelAnc] != 0)
		{
			for (int i = 0; i < grauPorNivel[nivelAnc]; i++)
				*(bit++) = nivelAnc;
			nivelAnc++;
		}
		if (DEBUG)
		{
			std::cout << "Nível dos bits:";
			for (int nivel : std::vector<int>(nivelDosBits, nivelDosBits+somaGraus))
				std::cout << ' ' << nivel;
			std::cout << '\n';
		}
		
		// Se os nós ainda não têm ids, atribuir aqui... (agora têm-se os graus)
		// (Árvores geradas com construirArvore() já têm os ids)
		// TODO
		
		// Cria um arranjo por nível com os ids e outro com os nós
		// do nivel para descobrir os módulos e distribuir os nós
		nosNiveis = new No*[numNiveis];
		ID* idsNiveis[numNiveis];
		for (int nivel = 0; nivel < numNiveis; nivel++)
		{
			nosNiveis[nivel] = new No[nosPorNivel[nivel]];
			idsNiveis[nivel] = new ID[nosPorNivel[nivel]];
		}
		// Próxima posição a preencher de cada nível
		int proxPos[numNiveis];
		std::fill(proxPos, proxPos+numNiveis, 0);
		// Pega os ids
		preencherNosIdsNiveis(nosNiveis, idsNiveis, proxPos, raiz);
		// Mostra ids
		if (DEBUG)
		{
			std::cout << "####### IDS DOS NÓS #######\n";
			for (int i = 0; i < numNiveis; i++)
			{
				std::cout << "Nível " << i << '\n';
				for (int j = 0; j < nosPorNivel[i]; j++)
					std::cout << idBin(idsNiveis[i][j], somaGraus) << '\n';
			}
			std::cout << '\n';
		}
		
		// Descobrir o módulo para cada nível (menos último nível)
		imagemNiveis = new No*[numNiveis-1];
		for (int nivel = 0; nivel < numNiveis-1; nivel++)
		{
			int numNos = nosPorNivel[nivel];
			No *nos = nosNiveis[nivel];
			ID *ids = idsNiveis[nivel];
			DadosFuncao *dadosNivel = &dadosFuncao[nivel];
			// Encontra valores para a função que distribui os nós
			buscarFuncaoIdeal(numNos, ids, dadosNivel);
			// Coloca os nós nas posições resultantes da função
			No *imagemNivel = new No[dadosNivel->tam] (); // Zerado
			for (int no = 0; no < numNos; no++)
				imagemNivel[dadosNivel->aplicar(ids[no])] = nos[no];
			imagemNiveis[nivel] = imagemNivel;
		}
		// Mostra dados da função para cada nível (que tem)
		if (DEBUG)
		{
			std::cout << "####### DADOS DAS FUNÇÕES #######\n";
			for (int i = 0; i < numNiveis-1; i++)
				std::cout << "Nível " << i << '\n' << dadosFuncao[i] << '\n';
		}
		
		// Libera
		for (int nivel = 0; nivel < numNiveis; nivel++)
			delete[] idsNiveis[nivel];
		
		if (DEBUG)
			std::cout << "<fim Arvore::montarEstruturas>\n";
	}
	
	void preencherNosGrauPorNivel(int *nosPorNivel, int *grauPorNivel, No no)
	{
		++nosPorNivel[no->nivel];
		grauPorNivel[no->nivel] = std::max(grauPorNivel[no->nivel], no->numFilhos);
		for (int i = 0; i < no->numFilhos; i++)
			preencherNosGrauPorNivel(nosPorNivel, grauPorNivel, no->filhos[i]);
	}
	
	void preencherNosIdsNiveis(No **nosNiveis, ID **idsNiveis, int *proxPos, No no)
	{
		// Coloca nó e id na próxima posição da linha do nível do nó
		nosNiveis[no->nivel][proxPos[no->nivel]  ] = no;
		idsNiveis[no->nivel][proxPos[no->nivel]++] = no->id;
		for (int i = 0; i < no->numFilhos; i++)
			preencherNosIdsNiveis(nosNiveis, idsNiveis, proxPos, no->filhos[i]);
	}
};

#endif /* ARV_H */

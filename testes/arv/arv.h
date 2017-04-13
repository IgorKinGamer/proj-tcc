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
	// Id usado para encontrar ancestral mais pr�ximo
	ID id;
	
	No pai;
	int numFilhos;
	No *filhos;
	
	// N�vel do n�: 0 -> raiz
	int nivel;
	
	void imprimir(int nBits = sizeof(id)*CHAR_BIT) { imprimir(0, nBits); }
	
	friend inline std::ostream& operator<<(std::ostream &o, const No n) { o << n->id; return o; } // !!!
	
	void imprimir(int ind, int nBits)
	{
		for (int i = 0; i < ind; i++)
			std::cout << '|'; // Indenta��o
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
	
	// Quantidade de n�veis
	int numNiveis;
	
	// Quantidade de n�s e grau de cada n�vel
	int *nosPorNivel, *grauPorNivel;
	
	// N�s de cada n�vel
	No **nosNiveis;
	
	// Arranjos por n�vel com n�s na posi��o resultante
	No **imagemNiveis;
	// NOTA: N�veis cujos n�s t�m um s� filho poderiam ser totalmente
	// omitidos na busca do ancestral mais pr�ximo, desde o bit ocupado
	// desnecessariamente no id at� as estruturas (pois n�o s�o ancestral
	// mais pr�ximo de nenhum par de n�s distintos), mas isso exigiria um
	// tratamento todo especial...
	// A n�o ser que os n�s tenham um No *filhosEspecial.
	
	// nivelDosBits[i]: N�vel do ancestral mais pr�ximo de a e b quando
	// o primeiro bit que difere entre a->id e b->id � i
	int *nivelDosBits;
	
	// Fun��o usada para cada n�vel da �rvore
	// Folhas n�o precisam
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
	
	// Descobrir quantos n�s por n�vel
	// Descobrir quantos n�veis s�o realmente necess�rios
	// Pegar todos os n�s de um n�vel em um array e descobrir o m
	
	int bitParaNivel(ID bit)
	{
		int pos = __builtin_ctz(bit); // Primeiro bit 1
		return nivelDosBits[pos];
	}
	
	void montarEstruturas()
	{
		if (DEBUG)
			std::cout << "<Arvore::montarEstruturas>\n";
		
		// Descobre quantos n�s h� em cada nivel e o grau m�ximo
		nosPorNivel = new int[numNiveis] (); // Zerado
		grauPorNivel = new int[numNiveis] (); // Zerado
		preencherNosGrauPorNivel(nosPorNivel, grauPorNivel, raiz);
		
		// Preenche mapeamento (primeiro bit diferente -> nivel do ancestral)
		// Se os graus (da raiz, filhos e netos) s�o [2, 3, 3]: [0, 0, 1, 1, 1, 2, 2, 2]
		int somaGraus = std::accumulate(grauPorNivel, grauPorNivel + numNiveis-1, 0);
		if (DEBUG)
			std::cout << "Somat�rio dos graus: " << somaGraus << '\n';
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
			std::cout << "N�vel dos bits:";
			for (int nivel : std::vector<int>(nivelDosBits, nivelDosBits+somaGraus))
				std::cout << ' ' << nivel;
			std::cout << '\n';
		}
		
		// Se os n�s ainda n�o t�m ids, atribuir aqui... (agora t�m-se os graus)
		// (�rvores geradas com construirArvore() j� t�m os ids)
		// TODO
		
		// Cria um arranjo por n�vel com os ids e outro com os n�s
		// do nivel para descobrir os m�dulos e distribuir os n�s
		nosNiveis = new No*[numNiveis];
		ID* idsNiveis[numNiveis];
		for (int nivel = 0; nivel < numNiveis; nivel++)
		{
			nosNiveis[nivel] = new No[nosPorNivel[nivel]];
			idsNiveis[nivel] = new ID[nosPorNivel[nivel]];
		}
		// Pr�xima posi��o a preencher de cada n�vel
		int proxPos[numNiveis];
		std::fill(proxPos, proxPos+numNiveis, 0);
		// Pega os ids
		preencherNosIdsNiveis(nosNiveis, idsNiveis, proxPos, raiz);
		// Mostra ids
		if (DEBUG)
		{
			std::cout << "####### IDS DOS N�S #######\n";
			for (int i = 0; i < numNiveis; i++)
			{
				std::cout << "N�vel " << i << '\n';
				for (int j = 0; j < nosPorNivel[i]; j++)
					std::cout << idBin(idsNiveis[i][j], somaGraus) << '\n';
			}
			std::cout << '\n';
		}
		
		// Descobrir o m�dulo para cada n�vel (menos �ltimo n�vel)
		imagemNiveis = new No*[numNiveis-1];
		for (int nivel = 0; nivel < numNiveis-1; nivel++)
		{
			int numNos = nosPorNivel[nivel];
			No *nos = nosNiveis[nivel];
			ID *ids = idsNiveis[nivel];
			DadosFuncao *dadosNivel = &dadosFuncao[nivel];
			// Encontra valores para a fun��o que distribui os n�s
			buscarFuncaoIdeal(numNos, ids, dadosNivel);
			// Coloca os n�s nas posi��es resultantes da fun��o
			No *imagemNivel = new No[dadosNivel->tam] (); // Zerado
			for (int no = 0; no < numNos; no++)
				imagemNivel[dadosNivel->aplicar(ids[no])] = nos[no];
			imagemNiveis[nivel] = imagemNivel;
		}
		// Mostra dados da fun��o para cada n�vel (que tem)
		if (DEBUG)
		{
			std::cout << "####### DADOS DAS FUN��ES #######\n";
			for (int i = 0; i < numNiveis-1; i++)
				std::cout << "N�vel " << i << '\n' << dadosFuncao[i] << '\n';
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
		// Coloca n� e id na pr�xima posi��o da linha do n�vel do n�
		nosNiveis[no->nivel][proxPos[no->nivel]  ] = no;
		idsNiveis[no->nivel][proxPos[no->nivel]++] = no->id;
		for (int i = 0; i < no->numFilhos; i++)
			preencherNosIdsNiveis(nosNiveis, idsNiveis, proxPos, no->filhos[i]);
	}
};

#endif /* ARV_H */

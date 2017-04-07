#ifndef ARV_H
#define ARV_H

#include <iostream>
#include <bitset>
#include <climits>

#include <algorithm>

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
	// Arranjos por n�vel com n�s na posi��o resultante
	No ***imagemNiveis;
	// NOTA: N�veis cujos n�s t�m um s� filho poderiam ser totalmente
	// omitidos na busca do ancestral mais pr�ximo, desde o bit ocupado
	// desnecessariamente no id at� as estruturas (pois n�o s�o ancestral
	// mais pr�ximo de nenhum par de n�s distintos), mas isso exigiria um
	// tratamento todo especial...
	// A n�o ser que os n�s tenham um No **filhosEspecial.
	
	// nivelDosBits[i]: N�vel do ancestral mais pr�ximo de a e b quando
	// o primeiro bit que difere entre a->id e b->id � i
	unsigned *nivelDosBits;
	
	// M�dulo usado para cada n�vel da �rvore
	// Folhas n�o precisam
	DadosFuncao *dadosFuncao;
	
	Arvore(No* r, unsigned nNiveis)
	{
		raiz = r;
		numNiveis = nNiveis;
		dadosFuncao = new DadosFuncao[numNiveis];
	}
	
	~Arvore()
	{
		for (unsigned i = 0; i < numNiveis-1; i++)
			delete imagemNiveis[i];
		delete imagemNiveis;
		delete raiz;
		delete dadosFuncao;
	}
	
	// Descobrir quantos n�s por n�vel
	// Descobrir quantos n�veis s�o realmente necess�rios
	// Pegar todos os n�s de um n�vel em um array e descobrir o m
	
	unsigned bitParaNivel(ID bit)
	{
		unsigned pos = __builtin_ctz(bit); // Primeiro bit 1
		return nivelDosBits[pos];
	}
	
	void montarEstruturas()
	{
		// Descobre quantos n�s h� em cada nivel e o grau m�ximo
		unsigned *nosPorNivel = new unsigned[numNiveis] (); // Inicia com 0
		unsigned *grauPorNivel = new unsigned[numNiveis] (); // Inicia com 0
		preencherNosGrauPorNivel(nosPorNivel, grauPorNivel, raiz);
		
		// Preenche mapeamento (primeiro bit diferente -> nivel do ancestral)
		// Se os graus (da raiz, filhos e netos) s�o [2, 3, 3]: [0, 0, 1, 1, 1, 2, 2, 2]
		unsigned *grau = grauPorNivel, somaGraus = 0;
		while (*grau != 0)
			somaGraus += *(grau++);
		cout << "Somat�rio dos graus: " << somaGraus << '\n';
		nivelDosBits = new unsigned[somaGraus];
		unsigned *bit = nivelDosBits, nivelAnc = 0;
		while (grau[nivelAnc] != 0)
		{
			for (unsigned i = 0; i < grau[nivelAnc]; i++)
				*(bit++) = nivelAnc;
			nivelAnc++;
		}
		
		// Se os n�s ainda n�o t�m ids, atribuir aqui... (agora t�m-se os graus)
		// (�rvores geradas com construirArvore() j� t�m os ids)
		// TODO
		
		// Cria um arranjo por n�vel com os ids e outro com os n�s
		// do nivel para descobrir os m�dulos e distribuir os n�s
		No** nosNiveis[numNiveis];
		ID* idsNiveis[numNiveis];
		for (unsigned nivel = 0; nivel < numNiveis; nivel++)
		{
			nosNiveis[nivel] = new No*[nosPorNivel[nivel]];
			idsNiveis[nivel] = new ID[nosPorNivel[nivel]];
		}
		// Pr�xima posi��o a preencher de cada n�vel
		unsigned *proxPos = new unsigned[numNiveis] (); // Zerado
		// Pega os ids
		preencherNosIdsNiveis(nosNiveis, idsNiveis, proxPos, raiz);
		// !!! Mostra ids
		/*for (int i = 0; i < numNiveis; i++)
		{
			for (int j = 0; j < nosPorNivel[i]; j++)
				cout << bitset<sizeof(ID)*CHAR_BIT>(idsNiveis[i][j]) << '\n';
			cout << '\n';
		}*/
		
		// Descobrir o m�dulo para cada n�vel (menos �ltimo n�vel)
		imagemNiveis = new No**[numNiveis-1];
		imagemNiveis[0] = 0;
		for (unsigned nivel = 0; nivel < numNiveis-1; nivel++)
		{
			unsigned numNos = nosPorNivel[nivel];
			No **nos = nosNiveis[nivel];
			ID *ids = idsNiveis[nivel];
			DadosFuncao *dadosNivel = &dadosFuncao[nivel];
			// Encontra valores para a fun��o que distribui os n�s
			buscarFuncaoIdeal(numNos, ids, dadosNivel);
			// Coloca os n�s nas posi��es resultantes da fun��o
			No **imagemNivel = new No*[dadosNivel->tam] (); // Zerado
			for (unsigned no = 0; no < numNos; no++)
				imagemNivel[dadosNivel->aplicar(ids[no])] = nos[no];
			imagemNiveis[nivel] = imagemNivel;
		}
		
		// Libera
		delete nosPorNivel;
		for (unsigned nivel = 0; nivel < numNiveis; nivel++)
		{
			delete nosNiveis[nivel];
			delete idsNiveis[nivel];
		}
		delete proxPos;
	}
	
	void preencherNosGrauPorNivel(unsigned *nosPorNivel, unsigned *grauPorNivel, No *no)
	{
		++nosPorNivel[no->nivel];
		grauPorNivel[no->nivel] = std::max(grauPorNivel[no->nivel], no->numFilhos);
		for (int i = 0; i < no->numFilhos; i++)
			preencherNosGrauPorNivel(nosPorNivel, grauPorNivel, no->filhos[i]);
	}
	
	void preencherNosIdsNiveis(No ***nosNiveis, ID **idsNiveis, unsigned *proxPos, No *no)
	{
		// Coloca n� e id na pr�xima posi��o da linha do n�vel do n�
		nosNiveis[no->nivel][proxPos[no->nivel]  ] = no;
		idsNiveis[no->nivel][proxPos[no->nivel]++] = no->id;
		for (unsigned i = 0; i < no->numFilhos; i++)
			preencherNosIdsNiveis(nosNiveis, idsNiveis, proxPos, no->filhos[i]);
	}
};

#endif /* ARV_H */

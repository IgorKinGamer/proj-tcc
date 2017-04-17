/*
 * Fun��es para descobrir os valores de m�dulo ideais para uma �rvore.
 */

#ifndef ARV_MOD_H
#define ARV_MOD_H

#include <algorithm>

#include "id.h"

/*
 *
 */
struct DadosFuncao
{
	/*
	 * Possivelmente, as opera��es OuEx e Ou em conjunto minimizam o m.
	 * Pode-se provar que E e Ou t�m o mesmo efeito sob m�dulo m, seja qual
	 * for o m.
	 * ( Aplicar "& 0" ap�s "| 1" � o mesmo que apenas "& 0".
	 *   Fazendo isso, subrai-se 2^bit de todo mundo, apenas "rodando" todos
	 *   os valores m�dulo m.
	 *   Portanto, "& 0" ap�s "| 1" � o mesmo (m�dulo m) que apenas "| 1".
	 *   Logo, "& 0" tem o mesmo efeito que "| 1". )
	 * A ordem n�o importa: um bit pode estar ativo (1) em apenas uma das
	 * duas m�scaras, pois n�o h� sentido estar ativo nas duas.
	 *     (x | 1) ^ 1 == x & 0,  (x ^ 1) | 1 == x | 1
	 * Outras quest�es devem ser observadas com respeito ao Ou,
	 * pois a maioria das combina��es de bits *garantem* colis�es (devido
	 * a como os ids s�o formados).
	 * Pouco foi testado com respeito a isso ainda.
	 */
	ID mascOuEx, mascOu, ad, m;
	// Tamanho do arranjo necess�rio (pode ser < m)
	int tam;
	
	ID aplicar(ID id) const
	{
		return ((id ^ mascOuEx | mascOu) + ad) % m;
	}
	
	friend inline std::ostream& operator<<(std::ostream &o, const DadosFuncao d)
	{
		o << "OuEx: " << idBin(d.mascOuEx) << ", ";
		o << "Ou: "   << idBin(d.mascOu)   << ", ";
		o << "ad: "   << d.ad              << ", ";
		o << "m: "    << d.m               << ", ";
		o << "tam: "  << d.tam;
		
		return o;
	}
};

/*
 * Verifica se os valores no array m�dulo m resultam todos em valores
 * diferentes.
 */
bool funcaoInjetora(int tam, ID valores[], const DadosFuncao &f)
{
	// Valores j� atingidos
	bool imagem[f.m];
	std::fill(imagem, imagem+f.m, false);
	
	// Aplica a fun��o a cada valor
	for (ID *valor = valores; valor < valores+tam; valor++)
	{
		ID res = f.aplicar(*valor);
		// Valor j� foi atingido
		if (imagem[res])
			return false;
		imagem[res] = true;
	}
	
	// N�o houve colis�es
	return true;
}

// Buscar os valores ideais da fun��o
void buscarFuncaoIdeal(int numNos, ID *ids, DadosFuncao *sai_dadosFuncao)
{
	// Busca fun��o
	DadosFuncao dados;
	dados.mascOu = dados.mascOuEx = dados.ad = 0;
	dados.m = numNos;
	
	// Para fazer buscas mais elaboradas (verificar v�rios dados)
	bool continuar = true;
	while (continuar)
	{
		// A fun��o n�o satisfaz
		while (!funcaoInjetora(numNos, ids, dados))
			dados.m++;
		
		// Imagem da fun��o
		ID imagem[numNos];
		for (int i = 0; i < numNos; i++)
			imagem[i] = dados.aplicar(ids[i]);
		// Analisa a imagem para descobrir o melhor ad (que resulta em menor tam)
		// Ideia: encontrar o maior "buraco" na imagem ordenada e
		// "empurr�-lo" para cima, de modo que o maior valor da imagem
		// seja o menor poss�vel
		std::sort(imagem, imagem+numNos);
		ID difMax = (dados.m+imagem[0]) - (imagem[numNos-1]);
		dados.ad = dados.m-imagem[0];
		dados.tam = imagem[numNos-1] - imagem[0] + 1;
		for (int i = 1; i < numNos; i++)
		{
			ID dif = imagem[i] - imagem[i-1];
			if (dif > difMax) // Maior resulta em tam menor
			{
				dados.ad = dados.m - imagem[i];
				dados.tam = imagem[i-1] + dados.ad + 1;
				difMax = dif;
			}
		}
		
		// TODO Por enquanto, pega o primeiro e pronto...
		continuar = false;
	}
	
	*sai_dadosFuncao = dados;
}

#endif /* ARV_MOD_H */

/*
 * Funções para descobrir os valores de módulo ideais para uma árvore.
 */

#ifndef ARV_MOD_H
#define ARV_MOD_H

#include <algorithm>

#include "defs.h"

/*
 *
 */
struct DadosFuncao
{
	/*
	 * Possivelmente, as operações E, OuEx e Ou em conjunto minimizam o m.
	 * A ordem não importa: um bit pode estar ativo (ie, fazer alteração:
	 * 0 na mascE e 1 na mascOu/OuEx) em apenas uma das três máscaras,
	 * pois não há sentido estar ativo em duas.
	 * Por exemplo, (x & 0) ^ 1 == x | 1, e semelhantemente com as demais
	 * combinações.
	 * Outras questões devem ser observadas com respeito ao E e ao Ou,
	 * pois a maioria das combinações de bits *garantem* colisões (devido
	 * a como os ids são formados).
	 * Nada foi testado com respeito a isso ainda.
	 */
	ID mascE, mascOuEx, mascOu, ad, m;
	// Tamanho do arranjo necessário (pode ser < m)
	unsigned tam;
	
	ID aplicar(ID id)
	{
		return ((id & mascE ^ mascOuEx | mascOu) + ad) % m;
	}
};

/*
 * Verifica se os valores no array módulo m resultam todos em valores
 * diferentes.
 */
bool moduloInjetor(int tam, ID valores[], ID m)
{
	// Valores já atingidos
	bool* imagem = new bool[m] ();
	
	// Aplica módulo a cada valor
	for (ID *valor = valores; valor < valores+tam; valor++)
	{
		unsigned long res = *valor % m;
		// Valor já foi atingido
		if (imagem[res])
			return false;
		imagem[res] = true;
	}
	
	delete imagem;
	
	// Não houve colisões
	return true;
}

// Buscar os valores ideais da função
void buscarFuncaoIdeal(unsigned numNos, ID *ids, DadosFuncao *sai_dadosFuncao)
{
	// Busca m
	ID m = numNos;
	// Para fazer buscas mais elaboradas (verificar vários ms)
	bool continuar = true;
	while (continuar)
	{
		// O m não satisfaz
		while (!moduloInjetor(numNos, ids, m))
			m++;
		
		/*// Imagem
		ID imagem[numNos];
		for (unsigned i = 0; i < numNos; i++)
			imagem[i] = ids[i] % m;
		// TODO Analisa imagem para descobrir os melhores valores para ad e tam
		std::sort(imagem, imagem+numNos);
		//ID difMax;*/
		
		// TODO Por enquanto, pega o primeiro e pronto...
		continuar = false;
	}
	
	sai_dadosFuncao->m = m;
	sai_dadosFuncao->ad = 0; // TODO Melhorar
	sai_dadosFuncao->tam = m; // TODO Melhorar
	
	// Não usados por enquanto
	sai_dadosFuncao->mascE = ~((ID) 0);
	sai_dadosFuncao->mascOuEx = 0;
	sai_dadosFuncao->mascOu = 0;
}

#endif /* ARV_MOD_H */

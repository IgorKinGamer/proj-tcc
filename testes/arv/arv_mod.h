/*
 * Funções para descobrir os valores de módulo ideais para uma árvore.
 */

#ifndef ARV_MOD_H
#define ARV_MOD_H

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
	
	ID aplicar(ID id)
	{
		return ((id & mascE ^ mascOuEx | mascOu) + ad) % m;
	}
}

/*
 * Verifica se os valores no array módulo m resultam todos em valores
 * diferentes.
 */
bool moduloInjetor(int tam, unsigned long valores[], unsigned long m)
{
	// Valores já atingidos
	bool* imagem = new bool[m]();
	
	// Aplica módulo a cada valor
	for (unsigned long* valor = valores; valor < valores+tam; valor++)
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

// Buscar o menor m
void buscarFuncaoIdeal(unsigned numNos, ID *ids,
	ID *sai_mascE, ID *sai_mascXor, ID *sai_mascOu, ID *sai_ad, ID *sai_m)
{
	
}

#endif /* ARV_MOD_H */

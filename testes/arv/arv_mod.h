/*
 * Fun��es para descobrir os valores de m�dulo ideais para uma �rvore.
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
	 * Possivelmente, as opera��es E, OuEx e Ou em conjunto minimizam o m.
	 * A ordem n�o importa: um bit pode estar ativo (ie, fazer altera��o:
	 * 0 na mascE e 1 na mascOu/OuEx) em apenas uma das tr�s m�scaras,
	 * pois n�o h� sentido estar ativo em duas.
	 * Por exemplo, (x & 0) ^ 1 == x | 1, e semelhantemente com as demais
	 * combina��es.
	 * Outras quest�es devem ser observadas com respeito ao E e ao Ou,
	 * pois a maioria das combina��es de bits *garantem* colis�es (devido
	 * a como os ids s�o formados).
	 * Nada foi testado com respeito a isso ainda.
	 */
	ID mascE, mascOuEx, mascOu, ad, m;
	
	ID aplicar(ID id)
	{
		return ((id & mascE ^ mascOuEx | mascOu) + ad) % m;
	}
}

/*
 * Verifica se os valores no array m�dulo m resultam todos em valores
 * diferentes.
 */
bool moduloInjetor(int tam, unsigned long valores[], unsigned long m)
{
	// Valores j� atingidos
	bool* imagem = new bool[m]();
	
	// Aplica m�dulo a cada valor
	for (unsigned long* valor = valores; valor < valores+tam; valor++)
	{
		unsigned long res = *valor % m;
		// Valor j� foi atingido
		if (imagem[res])
			return false;
		imagem[res] = true;
	}
	
	delete imagem;
	
	// N�o houve colis�es
	return true;
}

// Buscar o menor m
void buscarFuncaoIdeal(unsigned numNos, ID *ids,
	ID *sai_mascE, ID *sai_mascXor, ID *sai_mascOu, ID *sai_ad, ID *sai_m)
{
	
}

#endif /* ARV_MOD_H */

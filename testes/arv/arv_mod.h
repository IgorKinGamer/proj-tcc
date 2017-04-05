/*
 * Funções para descobrir os valores de módulo ideais para uma árvore.
 */

#ifndef ARV_MOD_H
#define ARV_MOD_H

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

#endif /* ARV_MOD_H */

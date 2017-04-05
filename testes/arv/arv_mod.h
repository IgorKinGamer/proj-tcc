/*
 * Fun��es para descobrir os valores de m�dulo ideais para uma �rvore.
 */

#ifndef ARV_MOD_H
#define ARV_MOD_H

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

#endif /* ARV_MOD_H */

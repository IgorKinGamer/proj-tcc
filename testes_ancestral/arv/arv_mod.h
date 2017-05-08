/*
 * Funções para descobrir os valores de módulo ideais para uma árvore.
 */

#ifndef ARV_MOD_H
#define ARV_MOD_H

#include <algorithm>

#include "id.h"

/*
 * Guarda dados da função para um nível da árvore e aplica a função.
 */
struct DadosFuncao
{
	/*
	 * Possivelmente, as operações OuEx e Ou em conjunto minimizam o m.
	 * Pode-se provar que E e Ou têm o mesmo efeito sob módulo m, seja qual
	 * for o m.
	 * ( Aplicar "& 0" após "| 1" é o mesmo que apenas "& 0".
	 *   Fazendo isso, subrai-se 2^bit de todo mundo, apenas "rodando" todos
	 *   os valores módulo m.
	 *   Portanto, "& 0" após "| 1" é o mesmo (módulo m) que apenas "| 1".
	 *   Logo, "& 0" tem o mesmo efeito que "| 1". )
	 * A ordem não importa: um bit pode estar ativo (1) em apenas uma das
	 * duas máscaras, pois não há sentido estar ativo nas duas.
	 *     (x | 1) ^ 1 == x & 0,  (x ^ 1) | 1 == x | 1
	 * Outras questões devem ser observadas com respeito ao Ou,
	 * pois a maioria das combinações de bits *garantem* colisões (devido
	 * a como os ids são formados).
	 * Pouco foi testado com respeito a isso ainda.
	 */
	ID mascOuEx, mascOu, ad, m;
	// Tamanho do arranjo necessário (pode ser < m)
	unsigned tam;
	// Dados para aplicar o módulo
	// Outro método pode ser necessário caso a multiplicação
	// seja muito cara.
	// Usar __int128_t para multiplicações maiores?
	ID mult, desl;
	
	ID aplicar(ID id) const
	{
		ID v = ((id ^ mascOuEx | mascOu) + ad);
		return v - (v * mult >> desl)*m; // v % m
	}
	
	ID testar(ID id) const
	{
		return (id ^ mascOuEx | mascOu) % m;
	}
	
	friend inline std::ostream& operator<<(std::ostream &o, const DadosFuncao d)
	{
		o << "OuEx: " << idBin(d.mascOuEx) << "\n";
		o << "Ou:   " << idBin(d.mascOu)   << "\n";
		o << "ad: "   << d.ad              << ", ";
		o << "m: "    << d.m               << ", ";
		o << "tam: "  << d.tam             << "\n";
		o << "mult: " << d.mult            << ", ";
		o << "desl: " << d.desl;
		
		return o;
	}
};

/*
 * Verifica se os valores no array módulo m resultam todos em valores
 * diferentes.
 */
bool funcaoInjetora(int tam, ID valores[], const DadosFuncao &f)
{
	// Valores já atingidos
	bool imagem[f.m];
	std::fill(imagem, imagem+f.m, false);
	
	// Aplica a função a cada valor
	for (ID *valor = valores; valor < valores+tam; valor++)
	{
		ID res = f.testar(*valor);
		// Valor já foi atingido
		if (imagem[res])
			return false;
		imagem[res] = true;
	}
	
	// Não houve colisões
	return true;
}

void dadosModulo(int numNos, ID *ids, DadosFuncao *dadosFuncao);

// Buscar os valores ideais da função
void buscarFuncaoIdeal(int numNos, ID *ids, DadosFuncao *sai_dadosFuncao)
{
	// Busca função
	DadosFuncao dados;
	dados.mascOu = dados.mascOuEx = dados.ad = 0;
	dados.m = numNos-1;
	
	// Para fazer buscas mais elaboradas (verificar vários dados)
	bool continuar = true;
	sai_dadosFuncao->tam = ~0u; // Maior possível
	while (continuar)
	{
		// Próxima tentativa
		dados.m++;
		
		// A função não satisfaz
		while (!funcaoInjetora(numNos, ids, dados))
			dados.m++;
		
		// Imagem do módulo
		ID imagem[numNos];
		for (int i = 0; i < numNos; i++)
			imagem[i] = dados.testar(ids[i]);
		// Analisa a imagem para descobrir o melhor ad (que resulta em menor tam)
		// Ideia: encontrar o maior "buraco" na imagem do módulo ordenada e
		// "empurrá-lo" para cima, de modo que o maior valor da imagem
		// seja o menor possível
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
		
		// Conseguiu tamanho melhor
		if (dados.tam < sai_dadosFuncao->tam)
			*sai_dadosFuncao = dados;
			
		
		// TODO Encontrar condição melhor
		continuar = dados.m < numNos*numNos;
	}
	
	dadosModulo(numNos, ids, sai_dadosFuncao);
}

/* Retorna dados para calculcar o módulo sem usar divisão */
void dadosModulo(int numNos, ID *ids, DadosFuncao *dadosFuncao)
{
	// Conta bits
	ID id = *std::max_element(ids, ids+numNos);
	int bits = 0;
	while (id)
	{
		id >>= 1;
		bits++;
	}
	
	// Expansão fracionária
	// Mais precisão pode ser necessária?
	ID m = dadosFuncao->m;
	double r = 1.d/m;
	// Zeros na frente
	int z = 0;
	while ((int) (2*r) == 0) // Próximo bit será 0
	{
		r *= 2;
		z++;
	}
	// Pega (bits+1) bits significativos
	ID mult = 0;
	for (int i = 0; i < bits+1; i++)
	{
		r *= 2;
		int bit = (int) r;
		mult = (mult << 1) + bit;
		r -= bit;
	}
	
	// Arredondamento
	mult++;
	// Deslocamento
	int desl = z + bits + 1;
	
	// Atribui valores
	dadosFuncao->mult = mult;
	dadosFuncao->desl = desl;
	
	// Verifica se deu certo
	for (auto i = 0; i < numNos; i++)
	{
		if (mult*ids[i] >> desl != ids[i]/m)
		{
			dadosFuncao->mult = dadosFuncao->desl = 0;
			break;
		}
	}
}

#endif /* ARV_MOD_H */

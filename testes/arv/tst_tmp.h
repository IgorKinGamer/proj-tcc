#ifndef TST_TMP_H
#define TST_TMP_H

#include <chrono>
#include <random>
#include <vector>

template <class Arv, class No>
class BaseTesteAncestralComum
{
	using Par = std::pair<No, No>;
	
	int numIter, numIterAquec;
	
	public:
	
	BaseTesteAncestralComum(int nI, int nIA) : numIter(nI), numIterAquec(nIA) {}
	
	virtual Arv criarArvore(std::vector<int> graus);
	virtual std::vector<No> pegarFolhas(Arv a);
	virtual No ancestralComum(Arv arv, No a, No b);
	virtual void destruirArvore(Arv a);
	
	// Tempo em milissegundos
	double executar(std::vector<int> graus)
	{
		Arv a = criarArvore(graus);
		
		std::vector<No> fs = pegarFolhas(a);
		std::vector<Par> pares = gerarPares(fs);
		embaralhar(pares);
		
		// Evita otimizações?
		volatile No n;
		
		// Aquecer?
		for (int i = 0; i < numIterAquec; i++)
			for (Par par : pares)
				n = ancestralComum(a, par.first, par.second);
		
		// Mede o tempo
		using namespace std::chrono;
		using relogio = high_resolution_clock;
		using milisDouble = duration<double, std::milli>;
		relogio::time_point t0 = relogio::now();
		
		// Roda
		for (int i = 0; i < numIter; i++)
			for (Par par : pares)
				n = ancestralComum(a, par.first, par.second);
		
		// Mede o tempo
		relogio::time_point t1 = relogio::now();
		
		destruirArvore(a);
		
		double tempo = duration_cast<milisDouble>(t1 - t0).count();
		return tempo;
	}
	
	private:
	
	std::vector<Par> gerarPares(std::vector<No> nos)
	{
		int tam = nos.size();
		std::vector<Par> pares;
		for (int i = 0; i < tam-1; i++)
			for (int j = i+1; j < tam; j++)
				pares.push_back({nos[i], nos[j]});
		return pares;
	}
	
	void embaralhar(std::vector<Par> pares)
	{
		std::shuffle(pares.begin(), pares.end(), std::default_random_engine());
	}
};

#endif /* TST_TMP_H */

/*

Criar uma árvore qualquer
Listar as folhas
Listar cada par de folhas
Embaralhar a lista de pares
Aquecer?
Repetir muitas vezes, medindo o tempo da execução toda:
- ancestral comum para cada par na lista (em ordem)

*/
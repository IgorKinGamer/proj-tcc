/*
 * Testes de desempenho temporal das funções de ancestral comum
 * NUM_ITER = [<iterações> [, <iterações de aquecimento>]]
 */

#include <iostream>
#include <string>
#include <vector>

#include "arv.h"
#include "tst_tmp.h"
#include "tst_tmp_arv.h"
#include "tst_tmp_hwloc.h"

using namespace std;

template <int... NUM_ITER>
void executarTestes(const vector<int> &graus, int vezesFora = 1, int vezesDentro = 1);

template <class A, class N, int... NUM_ITER>
void testar(BaseTesteAncestralComum<A, N, NUM_ITER...> &tst, const vector<int> &graus, int vezes = 1);

int main(int argc, char *argv[])
{
	// Graus de https://www.open-mpi.org/projects/hwloc/lstopo/images/16XeonX7400.v1.11.png
	vector<int> graus({4, 4, 1, 3, 2, 1, 1, 1});
	
	int vezesFora = 2, vezesDentro = 3;
	if (argc > 1)
		vezesFora = stoi(argv[1]);
	if (argc > 2)
		vezesDentro = stoi(argv[2]);
	if (argc > 3)
	{
		graus = vector<int>();
		for (auto i = 3; i < argc; i++)
			graus.push_back(stoi(argv[i]));
	}
	
	executarTestes<10000, 1000>(graus, vezesFora, vezesDentro);
}

template <class A, class N, int... NUM_ITER>
void testar(BaseTesteAncestralComum<A, N, NUM_ITER...> &tst, const vector<int> &graus, int vezes)
{
	vector<double> resultados;
	for (auto i = 0; i < vezes; i++)
		resultados.push_back(tst.executar(graus));
	
	for (auto t : resultados)
		cout << " (" << t << " ms)";
	cout << '\n';
}

template <int... NUM_ITER>
void executarTestes(const vector<int> &graus, int vezesFora, int vezesDentro)
{
	TesteAncestralComumSimples<NUM_ITER...> simples;
	TesteAncestralComumNovo<NUM_ITER...> novo;
	TesteAncestralComumHwloc<NUM_ITER...> hwloc;
	TesteAncestralComumOverhead<NUM_ITER...> overhead;
	for (auto i = 0; i < vezesFora; i++)
	{
		cout << "Simples: ";
		testar(simples, graus, vezesDentro);
		cout << "Novo:    ";
		testar(novo, graus, vezesDentro);
		cout << "Hwloc:   ";
		testar(hwloc, graus, vezesDentro);
		cout << "Overhead:";
		testar(overhead, graus, vezesDentro);
		cout << '\n';
	}
}

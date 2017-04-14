/*
 * Testes de desempenho temporal das funções de ancestral comum
 */

#include <iostream>
#include <string>
#include <vector>

#include "arv.h"
#include "tst_tmp.h"
#include "tst_tmp_arv.h"

using namespace std;

template <int... NUM_ITER>
void executarTestes(const vector<int> &graus, int vezes = 1);

template <class A, class N, int... NUM_ITER>
void testar(BaseTesteAncestralComum<A, N, NUM_ITER...> &tst, const vector<int> &graus, int vezes = 1);

int main(int argc, char *argv[])
{
	// Graus de https://www.open-mpi.org/projects/hwloc/lstopo/images/16XeonX7400.v1.11.png
	vector<int> graus({4, 4, 1, 3, 2, 1, 1, 1});
	
	int vezes = 2;
	/*if (argc > 1)
		vezes = stoi(argv[1]);*/
	
	executarTestes<10000, 1000>(graus, vezes);
}

template <class A, class N, int... NUM_ITER>
void testar(BaseTesteAncestralComum<A, N, NUM_ITER...> &tst, const vector<int> &graus, int vezes)
{
	vector<double> resultados;
	for (auto i = 0; i < vezes; i++)
		resultados.push_back(tst.executar(graus));
	
	cout << "Tempos:";
	for (auto t : resultados)
		cout << " (" << t << " ms)";
	cout << '\n';
}

template <int... NUM_ITER>
void executarTestes(const vector<int> &graus, int vezes)
{
	TesteAncestralComumSimples<NUM_ITER...> simples;
	TesteAncestralComumNovo<NUM_ITER...> novo;
	TesteAncestralComumOverhead<NUM_ITER...> overhead;
	for (auto i = 0; i < vezes; i++)
	{
		cout << "Simples:\n";
		testar(simples, graus/*, vezes*/);
		cout << "Novo:\n";
		testar(novo, graus/*, vezes*/);
		cout << "Overhead:\n";
		testar(overhead, graus/*, vezes*/);
	}
}

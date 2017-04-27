/*
 * Testes de desempenho temporal das funções de ancestral comum
 * NUM_ITER = [<iterações> [, <iterações de aquecimento>]]
 */

#include <iostream>
#include <string>
#include <vector>

#include "arv/arv.h"
#include "tst_tmp.h"
#include "tst_tmp_arv.h"
#include "tst_tmp_hwloc.h"

using namespace std;

void executarTestes(
	const vector<int> &graus,
	int iter, int iterAquec,
	int vezesFora = 1, int vezesDentro = 1);

template <class A, class N>
void testar(BaseTesteAncestralComum<A, N> &tst, const vector<int> &graus, int vezes = 1);

int main(int argc, char *argv[])
{
	// Graus de https://www.open-mpi.org/projects/hwloc/lstopo/images/16XeonX7400.v1.11.png
	vector<int> graus({4, 4, 1, 3, 2, 1, 1, 1});
	int vezesFora = 2, vezesDentro = 3, iteracoes = 10000, aquecimento = 1000;
	
	int i = 1;
	string arg;
	while (i < argc)
	{
		arg = argv[i++];
		string resto = arg.substr(1);
		switch (arg.front())
		{
			case 'i': // Iterações
				iteracoes = stoi(resto);
				break;
			case 'a': // Aquecimento
				aquecimento = stoi(resto);
				break;
			case 'r': // Repetições de cada algoritmo
				vezesDentro = stoi(resto);
				break;
			case 'R': // Repetições de tudo
				vezesFora = stoi(resto);
				break;
			case '-': // Graus
				graus = vector<int>();
				while (i < argc)
					graus.push_back(stoi(argv[i++]));
 				break;
			default:
				cout << "Argumento não reconhecido: " + arg << '\n';
		}
	}
	
	executarTestes(graus, iteracoes, aquecimento, vezesFora, vezesDentro);
}

template <class A, class N>
void testar(BaseTesteAncestralComum<A, N> &tst, const vector<int> &graus, int vezes)
{
	vector<double> resultados;
	for (auto i = 0; i < vezes; i++)
		resultados.push_back(tst.executar(graus));
	
	for (auto t : resultados)
		cout << " (" << t << " ms)";
	cout << '\n';
}

void executarTestes(
	const vector<int> &graus,
	int iter, int iterAquec,
	int vezesFora, int vezesDentro)
{
	TesteAncestralComumSimples  simples (iter, iterAquec);
	TesteAncestralComumNovo     novo    (iter, iterAquec);
	TesteAncestralComumHwloc    hwloc   (iter, iterAquec);
	TesteAncestralComumOverhead overhead(iter, iterAquec);
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

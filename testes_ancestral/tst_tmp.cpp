/*
 * Testes de desempenho temporal das funções de ancestral comum
 */

#include <iostream>
#include <string>
#include <vector>

#include "tmp/tst_tmp.h"
#include "tmp/tst_tmp_arv.h"
#include "tmp/tst_tmp_hwloc.h"
#include "tmp/tst_tmp_matriz.h"

using namespace std;

using matriz::TesteAncestralComumMatriz;

void executarTestes(
	const vector<int> &graus,
	int iter, int iterAquec,
	int vezesFora = 1, int vezesDentro = 1);

template <class A, class N>
void testar(BaseTesteAncestralComum<A, N> &tst, const vector<int> &graus, int vezes = 1);

const int ALG_SIMPLES  = 0,
          ALG_NOVO     = 1,
          ALG_HWLOC    = 2,
          ALG_MATRIZ   = 3,
          ALG_OVERHEAD = 4,
          NUM_ALGS     = 5;
bool algoritmo[5] = {};

int main(int argc, char *argv[])
{
	// Graus de https://www.open-mpi.org/projects/hwloc/lstopo/images/16XeonX7400.v1.11.png
	vector<int> graus({4, 4, 1, 3, 2, 1, 1, 1});
	int vezesFora = 2, vezesDentro = 3, iteracoes = 10000, aquecimento = 1000;
	algoritmo[ALG_SIMPLES] = true;
	algoritmo[ALG_NOVO   ] = true;
	algoritmo[ALG_HWLOC  ] = true;
	
	int i = 1;
	string arg;
	while (i < argc)
	{
		arg = argv[i++];
		string resto = arg.substr(1);
		switch (arg.front())
		{
			case 'i': iteracoes   = stoi(resto); break; // Iterações
			case 'a': aquecimento = stoi(resto); break; // Aquecimento
			case 'r': vezesDentro = stoi(resto); break; // Repetições de cada algoritmo
			case 'R': vezesFora   = stoi(resto); break; // Repetições de tudo
			case 'A': // Algoritmos
				for (bool &b : algoritmo) b = false;
				for (char &alg : resto)
					switch (alg)
					{
						case 's': algoritmo[ALG_SIMPLES ] = true; break; // Simples
						case 'n': algoritmo[ALG_NOVO    ] = true; break; // Novo
						case 'h': algoritmo[ALG_HWLOC   ] = true; break; // Hwloc
						case 'm': algoritmo[ALG_MATRIZ  ] = true; break; // Matriz
						case 'o': algoritmo[ALG_OVERHEAD] = true; break; // Overhead
						case '*': for (bool &b : algoritmo) b = true; break; // Todos
						default: cout << "Algoritmo não reconhecido: " << alg << '\n';
					}
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
	TesteAncestralComumMatriz   matriz  (iter, iterAquec);
	TesteAncestralComumOverhead overhead(iter, iterAquec);
	for (auto i = 0; i < vezesFora; i++)
	{
		if (algoritmo[ALG_SIMPLES])
		{
			cout << "Simples: ";
			testar(simples, graus, vezesDentro);
		}
		if (algoritmo[ALG_NOVO])
		{
			cout << "Novo:    ";
			testar(novo, graus, vezesDentro);
		}
		if (algoritmo[ALG_HWLOC])
		{
			cout << "Hwloc:   ";
			testar(hwloc, graus, vezesDentro);
		}
		if (algoritmo[ALG_MATRIZ])
		{
			cout << "Matriz:  ";
			testar(matriz, graus, vezesDentro);
		}
		if (algoritmo[ALG_OVERHEAD])
		{
			cout << "Overhead:";
			testar(overhead, graus, vezesDentro);
		}
		cout << '\n';
	}
}

/*
 * Testes de desempenho temporal das funções de ancestral comum
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tst_tmp.h"
#include "tst_tmp_arv.h"
#include "tst_tmp_hwloc.h"
#include "tst_tmp_matriz.h"

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
bool alg_ativo[NUM_ALGS] = {};
vector<int> algs_ativos;
string nomes[NUM_ALGS] =
{
	"Simples",
	"Novo",
	"Hwloc",
	"Matriz",
	"Overhead"
};
int tamMaxNome = max_element(nomes, nomes+NUM_ALGS,
	[](string &a, string &b) { return a.size() < b.size(); })->size();
vector<double> tempos[NUM_ALGS];

int main(int argc, char *argv[])
{
	// Graus de https://www.open-mpi.org/projects/hwloc/lstopo/images/16XeonX7400.v1.11.png
	vector<int> graus({4, 4, 1, 3, 2, 1, 1, 1});
	int vezesFora = 2, vezesDentro = 3, iteracoes = 10000, aquecimento = 1000;
	alg_ativo[ALG_SIMPLES] = true;
	alg_ativo[ALG_NOVO   ] = true;
	alg_ativo[ALG_HWLOC  ] = true;
	
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
				for (bool &b : alg_ativo) b = false;
				for (char &alg : resto)
					switch (alg)
					{
						case 's': alg_ativo[ALG_SIMPLES ] = true; break; // Simples
						case 'n': alg_ativo[ALG_NOVO    ] = true; break; // Novo
						case 'h': alg_ativo[ALG_HWLOC   ] = true; break; // Hwloc
						case 'm': alg_ativo[ALG_MATRIZ  ] = true; break; // Matriz
						case 'o': alg_ativo[ALG_OVERHEAD] = true; break; // Overhead
						case '*': for (bool &b : alg_ativo) b = true; break; // Todos
						default: cerr << "Algoritmo não reconhecido: " << alg << '\n';
					}
				break;
			case '-': // Graus
				graus = vector<int>();
				while (i < argc)
					graus.push_back(stoi(argv[i++]));
 				break;
			default:
				cerr << "Argumento não reconhecido: " + arg << '\n';
		}
	}
	for (int alg = 0; alg < NUM_ALGS; alg++)
		if (alg_ativo[alg])
			algs_ativos.push_back(alg);
	if (algs_ativos.empty())
	{
		cerr << "Nenhum algoritmo especificado\n";
		return 0;
	}
	
	executarTestes(graus, iteracoes, aquecimento, vezesFora, vezesDentro);
	
	// Escreve em um arquivo em uma pasta definida pela configuração
	string pasta, iniNome, iniCom;
	// Define nome da pasta
	pasta += 'i' + to_string(iteracoes);
	pasta += 'a' + to_string(aquecimento);
	pasta += '-';
	for (const int &g : graus)
		pasta += to_string(g) + '.';
	pasta.pop_back();
	// Cria a pasta
	#ifdef _WIN32
		iniCom = "mkdir res\\";
	#else
		iniCom = "mkdir -p res/";
	#endif
	system((iniCom + pasta).data());
	// Descobre nome não usado
	iniNome = "res/" + pasta + "/resultados";
	int n = -1;
	while (ifstream(iniNome + to_string(++n) + ".csv")); // Existe
	// Cria o arquivo
	ofstream saida(iniNome + to_string(n) + ".csv");
	if (!saida)
	{
		cerr << "Impossível criar o arquivo\n";
		return 0;
	}
	
	// Escreve no arquivo
	int ult_alg = algs_ativos.back();
	algs_ativos.pop_back();
	// Cabeçalho
	for (const int &alg : algs_ativos)
		saida << nomes[alg] << ',';
	saida << nomes[ult_alg] << '\n';
	// Amostras
	int numAmostras = vezesFora*vezesDentro;
	for (int i = 0; i < numAmostras; i++)
	{
		for (const int &alg : algs_ativos)
			saida << tempos[alg][i] << ',';
		saida << tempos[ult_alg][i] << '\n';
	}
	
	cout << "Arquivo \"" << iniNome + to_string(n) + ".csv" << "\" criado\n";
}

template <class A, class N>
void testar(BaseTesteAncestralComum<A, N> &tst, int alg, const vector<int> &graus, int vezes)
{
	if (!alg_ativo[alg])
		return;
	
	vector<double> resultados;
	for (auto i = 0; i < vezes; i++)
	{
		double t = tst.executar(graus);
		resultados.push_back(t);
		tempos[alg].push_back(t);
	}
	
	string &nome = nomes[alg];
	cout << nome << ": ";
	for (int i = 0; i < tamMaxNome-nome.size(); i++)
		cout << ' ';
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
		testar(simples , ALG_SIMPLES , graus, vezesDentro);
		testar(novo    , ALG_NOVO    , graus, vezesDentro);
		testar(hwloc   , ALG_HWLOC   , graus, vezesDentro);
		testar(matriz  , ALG_MATRIZ  , graus, vezesDentro);
		testar(overhead, ALG_OVERHEAD, graus, vezesDentro);
		cout << '\n';
	}
}

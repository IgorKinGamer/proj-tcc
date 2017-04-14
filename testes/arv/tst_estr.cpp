/*
 * Testes com as estruturas para encontrar o ancestral comum.
 *
 * TODO: Testar as estruturas em si, construídas em Arvore::montarEstruturas()
 * (apenas os resultados finais e a distribuição dos nós estão sendo testados)
 */

#include <algorithm>
#include <vector>

#include "arv.h"
#include "constr.h"
#include "percorr.h"

using namespace std;

void testarDistribuicaoNos(Arvore*);
void testarAncestralComum(Arvore*, vector<int>, vector<int>, vector<int>);
No descendente(No, vector<int>);

void erro(string);
template <class T> void testarIgual(T, T, string);

void testes();
int main()
{
	try { testes(); }
	catch (string &erro) { cout << erro; }
	
	return 0;
}

void testes()
{
	cout << "Oi!" << endl;

	vector<int> graus = {3, 2, 2, 5};
	int niveis, nBits;
	niveis = graus.size() + 1;
	nBits = std::accumulate(graus.begin(), graus.end(), 0);
	
	No raiz = construirArvore(graus);
	//raiz->imprimir(nBits);
	Arvore arv(raiz, niveis);
	
	testarDistribuicaoNos(&arv);
	try // Exceções...?
	{
		try
		{
			Arvore arvErro(construirArvore(graus), niveis);
			arvErro.dadosFuncao[3].ad--; // Deve dar erro
			testarDistribuicaoNos(&arvErro);
		}
		catch (std::string str) { throw 0; } // Deu erro: ok
		erro("Mutação nas estruturas deveria causar erro"); // Não deu erro
	}
	catch (int i) { cout << "Erro (mutação nas estruturas) corretamente detectado\n"; }
	
	// descendente() funcionando
	cout << "descendente() funcionando: ";
	cout << (raiz == descendente(raiz, {})) << ' ';
	No a = raiz->filhos[0]->filhos[1]->filhos[0];
	cout << (a == descendente(raiz, {1, 2, 1})) << ' ';
	No b = raiz->filhos[0]->filhos[0]->filhos[0]->filhos[3];
	cout << (b == descendente(raiz, {1, 1, 1, 4})) << '\n';
	
	cout << "a e b: " << a->idBin(nBits) << ' ' << b->idBin(nBits) << '\n';
	cout << ancestralSimples(a, b)->idBin(nBits) << '\n';
	cout << ancestral(&arv, a, b)->idBin(nBits) << '\n';
	
	testarAncestralComum(&arv, {1}, {2, 1, 4}, {1, 1, 1});
	testarAncestralComum(&arv, {3}, {2, 1, 4}, {1});
	testarAncestralComum(&arv, {3, 2, 1}, {4}, {3});
	testarAncestralComum(&arv, {}, {1}, {});
	testarAncestralComum(&arv, {}, {1}, {2});
	
	//int n;
	//cout << "Ó: " << __builtin_ctz(n) << endl;
}

void testarDistribuicaoNos(Arvore *a, No no)
{
	// Último nível (folhas) não tem
	if (no->numFilhos == 0)
		return;
	
	int nivel = no->nivel;
	No imagem = a->imagemNiveis[nivel][ a->dadosFuncao[nivel].aplicar(no->id) ];
	testarIgual(no, imagem, "testarDistribuicaoNos()");
	
	for (No filho : vector<No>(no->filhos, no->filhos + no->numFilhos))
		testarDistribuicaoNos(a, filho);
}

void testarDistribuicaoNos(Arvore *a)
{
	testarDistribuicaoNos(a, a->raiz);
	cout << "Distribuição dos nós correta\n";
}

/*
 * Verifica se as funções de busca de ancestral retornam o nó correto.
 * camAnc: Caminho até o ancestral
 * cam1, cam2: Caminho do ancestral até os descendentes
 * cam1[0] != cam2[0]
 */
void testarAncestralComum(Arvore *arv, vector<int> camAnc, vector<int> cam1, vector<int> cam2)
{
	if (cam1.size() > 0 && cam2.size() > 0 && cam1[0] == cam2[0])
		erro("testarAncestralComum(): caminhos dos filhos devem começar com valores diferentes");
	
	No anc, desc1, desc2;
	anc = descendente(arv->raiz, camAnc);
	desc1 = descendente(anc, cam1);
	desc2 = descendente(anc, cam2);
	
	No ancSimples, ancNovo;
	ancSimples = ancestralSimples(desc1, desc2);
	ancNovo = ancestral(arv, desc1, desc2);
	
	cout << "-------\n";
	cout << "ancestral simples ok: " << (ancSimples == anc) << '\n';
	cout << "ancestral novo    ok: " << (ancNovo == anc) << '\n';
	cout << "-------\n";
}

/*
 * descendente(raiz, {1, 2, 2}) -> 2º filho do 2º filho do 1º filho de raiz
 * (1, e não 0, indica o primeiro filho).
 */
No descendente(No no, vector<int> caminho)
{
	for (int f : caminho)
		if (f > no->numFilhos)
			erro("descendente(): Caminho inválido");
		else
			no = no->filhos[f-1];
	return no;
}


void erro(string msg)
{
	throw (string("### ERRO ###\n") + msg + string("\n############\n"));
}

template <class T> void testarIgual(T a, T b, string msg)
{
	if (a != b)
		erro("Teste de igualdade falhou: " + msg);
}

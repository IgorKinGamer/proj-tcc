#include <algorithm>
#include <vector>

#include "arv.h"
#include "constr.h"
#include "percorr.h"

void testarAncestralComum(Arvore*, No, vector<int>, vector<int>, vector<int>);
No descendente(No, vector<int>);
string id(int, No);
void erro(const char*);

int main()
{
	cout << "Oi!" << endl;

	int graus[] = {3, 2, 2, 5}, niveis, nBits;
	niveis = sizeof(graus)/sizeof(int) + 1;
	nBits = std::accumulate(graus, graus+niveis-1, 0);
	
	No raiz = construirArvore(niveis-1, graus);
	raiz->imprimir(nBits);
	Arvore arv(raiz, niveis);
	
	// descendente() funcionando
	cout << "descendente() funcionando: ";
	cout << (raiz == descendente(raiz, {})) << ' ';
	No a = raiz->filhos[0]->filhos[1]->filhos[0];
	cout << (a == descendente(raiz, {1, 2, 1})) << ' ';
	No b = raiz->filhos[0]->filhos[0]->filhos[0]->filhos[3];
	cout << (b == descendente(raiz, {1, 1, 1, 4})) << '\n';
	
	cout << "a e b: " << id(nBits, a) << ' ' << id(nBits, b) << '\n';
	cout << id(nBits, ancestralSimples(a, b)) << '\n';
	cout << id(nBits, ancestral(&arv, a, b)) << '\n';
	
	testarAncestralComum(&arv, raiz, {1}, {2, 1, 4}, {1, 1, 1});
	testarAncestralComum(&arv, raiz, {3}, {2, 1, 4}, {1});
	testarAncestralComum(&arv, raiz, {3, 2, 1}, {4}, {3});
	testarAncestralComum(&arv, raiz, {}, {1}, {});
	testarAncestralComum(&arv, raiz, {}, {1}, {2});
	
	//int n;
	//cout << "Ó: " << __builtin_ctz(n) << endl;
	
	return 0;
}

/*
 * Verifica se as funções de busca de ancestral retornam o nó correto.
 * camAnc: Caminho até o ancestral
 * cam1, cam2: Caminho do ancestral até os descendentes
 * cam1[0] != cam2[0]
 */
void testarAncestralComum(Arvore *arv, No raiz, vector<int> camAnc, vector<int> cam1, vector<int> cam2)
{
	if (cam1.size() > 0 && cam2.size() > 0 && cam1[0] == cam2[0])
		erro("testarAncestralComum(): caminhos dos filhos devem começar com valores diferentes");
	
	No anc, desc1, desc2;
	anc = descendente(raiz, camAnc);
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

string id(int tam, No no)
{
	string repr = bitset<64>(no->id).to_string();
	return repr.substr(repr.size() - tam);
}

void erro(const char* msg)
{
	cout << "### ERRO ###\n" << msg << "\n############\n";
	throw 0;
}

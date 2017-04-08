#include "arv.h"

int main()
{
	cout << "Oi!" << endl;

	int graus[] = {3, 2, 1, 5};
	No raiz = construirArvore(4, graus);
	raiz->imprimir();
	
	No a = raiz->filhos[0]->filhos[1]->filhos[0];
	No b = raiz->filhos[0]->filhos[0]->filhos[0]->filhos[3];
	
	cout << a << ' ' << b << endl;
	
	cout << ancestralSimples(a, b) << endl;
	
	//int n;
	//cout << "Ó: " << __builtin_ctz(n) << endl;
	
	return 0;
}

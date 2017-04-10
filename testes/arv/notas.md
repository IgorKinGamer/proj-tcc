Cada nó da árvore recebe um id.
Os ids podem ser tratados como strings binárias, mas são simplesmente números,
então 0s a esquerda não alteram o id, só servem para facilitar a explicação
de como os ids são formados.

Se houver níveis cujos nós têm só 1 filho, tais níveis podem ser "comprimidos"
ou ignorados na hora de atribuir ids.
Ou seja, a árvore é tratada como se cada nó A de um nível assim não existisse
e o filho de A fosse filho do pai de A.
Isto porque, se A tem um só filho, não existem dois nós distintos na árvore
que tenham A como ancestral mais próximo.
(Se A tem um só filho, quaisquer dois nós distintos que têm A como ancestral
comum também têm o filho de A como ancestral comum.)

A raiz tem id 0.

Se ela tem f_r filhos, o id do i-ésimo filho dela é uma string binária
de tamanho f_r com o i-ésimo bit ativo.
(Se a raiz tem 3 filhos, os ids deles são: 001, 010, 100).

A partir daí, se os nós do nível n têm no máximo f filhos, os filhos deles
(ie., os nós do nível n+1) tem id formado assim:
O id do i-ésimo filho de um nó do nível n é
	uma string binária de tamanho f com apenas o i-ésimo bit ativo
	concatenada com o id do pai dele
(Se os nós do nível 1 têm no máximo 4 filhos e o nó 001 tem exatamente 4 filhos,
os filhos dele são 0001 001, 0010 001, 0100 001 e 1000 001 (espaços só para
facilitar a visualização))


Dados dois nós, a sequência de bits antes do primeiro bit que difere é o id
do ancestral comum mais próximo.
(100 010 0010 e 100 100 0010 -> 0010 (zeros a esquerda podem ser ignorados)).

Sabendo-se o nível e o id desse ancestral (com poucas operações simples),
é necessário "transformar" esse par (nível, id) na referência para o objeto
em si.

Uma solução ingênua e impraticável é usar o id como índice em um array
(imagine buscar o nó 10 10 100000 0010 em um array de mais de dez mil posições
com apenas 2*2*6*4 = 96 delas ocupadas...).

Uma possível solução é colocar os nós em um array menor e usar uma função
que traga todos os valores de id possíveis para os limites desse array.
Naturalmente, essa função pode ser módulo m, para algum m.
O problema é que podem ocorrer colisões (valores congruentes mod m).
Então, usando apenas mod m, m tem que ser escolhido de modo que não haja
colisões.
No entanto, é desejável que m seja o menor possível.

A seguir estão alguns dados (descobertos por força bruta) de qual o menor m
que não causa colisões para os nós do i-ésimo nível (abaixo da raiz) de uma
árvore n-ária (o qual tem n^i nós), e a razão m/(n^i) (1 seria o ótimo:
o menor espaço possível alocado e, portanto, dados menos esparsos):

n = 2:
- i = 2: m =  6 (1.50)
- i = 3: m =  9 (1.13)
- i = 4: m = 18 (1.13)
- i = 5: m = 33 (1.03)
(Aparentemente, sempre m = 2^i + (2-(i%2)) -- m/(2^i) tende a 1)

n = 3:
- i = 2: m =    20 ( 2.22)
- i = 3: m =    94 ( 3.48)
- i = 4: m =   606 ( 7.48)
- i = 5: m =  2095 ( 8.62)
- i = 6: m =  8380 (11.50)
- i = 7: m = 49023 (22.42)

n = 4:
- i = 2: m =    40 ( 2.50)
- i = 3: m =   343 ( 5.36)
- i = 4: m =  2744 (10.72)
- i = 5: m = 27240 (26.60)

n = 5:
- i = 2: m =    73 ( 2.92)
- i = 3: m =  1168 ( 9.34)
- i = 4: m = 15531 (24.85)

n = 6
- i = 2: m =  145 ( 4.03)
- i = 3: m = 2910 (13.47)

Nota:
	O array não precisa ser de tamanho m; com os ajustes necessários, pode
	ser de tamanho max(img)-min(img), onde img é o conjunto dos ids mod m.
	É possível que esse valor diminua ao se usar um m que não seja o
	menor possível.

Nota:
	Com esses dados podemos montar os testes, usando o m descoberto para uma
	dada lista de graus.
	A função buscar (arquivo buscar.m) no octave faz isso.
	Dados os ids do último nível de uma árvore com os graus gs = [a b ...],
	ie., a raiz tem a filhos, cada um com b filhos... (o que pode ser obtido
	com ids = arv(gs)),
		> buscar(ids)
	encontra o m.
	Ex.:
		> m = buscar( arv( [2 3 5] ) )  ->  m = 86


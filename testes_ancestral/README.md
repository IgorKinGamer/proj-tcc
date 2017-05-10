# Testes de ancestral comum

Pasta | Conteúdo
----- | --------
arv, matriz | Estruturas novas
tst_tmp | Testes de tempo

## Testes de tempo

### Como executar

- Compilar tst_tmp.cpp:
  - [Linux](comptmp.sh) (se necessário, alterar a variável `PKG_CONFIG_PATH`)
  - [Windows](comptmp.bat) (com a pasta do hwloc compilado na variável `hwloc`)
- Adicionar pasta bin do hwloc à variável `PATH`
- Executar (`tmp` representa o executável gerado)

        tmp [i<iterações>] [a<aquecimento>] [r<m>] [R<n>] [A<algs>] [- graus]

  - `m`: número de execuções seguidas com cada tipo de ancestral comum
  - `n`: número de repetições dos testes por inteiro (para intercalar
    os algoritmos e evitar que condições temporárias afetem um só)
  - `graus`: sequência de inteiros que representam os graus da árvore
  - `iterações` e `aquecimento`: quantidade de vezes que o ancestral comum
    será encontrado (com cada algoritmo) durante a medição e antes da medição,
    respectivamente, para todos os pares de folhas da árvore.
  - `algs`: uma ou várias letras ou `*`, representando os algoritmos que serão
    rodados, entre:
    - `s`: Simples
    - `n`: Novo
    - `h`: Hwloc
    - `m`: Matriz
    - `o`: Overhead
    - `*`: Todos
  - Omitindo argumentos, os padrões são `tmp i10000 a1000 r3 R2 Asnh - 4 4 1 3 2 1 1 1`
  - Script de testes ([Linux](testes_tempo.sh), [Windows](testes_tempo.bat))
    - Mesmos argumentos do programa tmp (exceto graus, que são lidos do arquivo
      graus_testes.txt), por exemplo, `testes_tempo i1000 R1`

### Notas e resultados

- [Notas](Notas.md)
- [Resultados](Resultados.txt)

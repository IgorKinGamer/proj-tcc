# Testes de ancestral comum

- arv/arv.h: Estruturas
- arv/arv_mod.h: Definição da função que distribui os nós de um nível
- arv/constr.h: Construção de árvores sintéticas
- arv/percorr.h: Funções de ancestral comum ("normal" e nova)
- tst_estr.cpp: Testes com as estruturas
- tst_tmp.cpp: Testes de desempenho

## Testes de tempo

### Como executar

- Compilar tst_tmp.cpp:
  - [Linux](comptmp.sh) (se necessário, alterar a variável `PKG_CONFIG_PATH`)
  - [Windows](comptmp.bat) (com a pasta do hwloc compilado na variável `hwloc`)
- Adicionar pasta bin do hwloc à variável `PATH`
- Executar (`tmp` representa o executável gerado)

        tmp [i<iterações>] [a<aquecimento>] [r<m>] [R<n>] [- graus]

onde m é o número de execuções seguidas com cada tipo de ancestral comum,
n é o número de repetições dos testes por inteiro
e graus é uma sequência de inteiros que representam os graus da árvore.
Omitindo argumentos, os padrões são `tmp i10000 a1000 r3 R2 - 4 4 1 3 2 1 1 1`.
  - Script de testes ([Linux](testes_tempo.sh), [Windows](testes_tempo.bat))
    - Mesmos argumentos do programa tmp (exceto graus, que são lidos do arquivo
graus_testes.txt), por exemplo, `testes_tempo i1000 R1`

### Notas e resultados

- [Notas](Notas.md)
- [Resultados](Resultados.txt)

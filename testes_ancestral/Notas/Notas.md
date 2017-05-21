# Resultados dos testes

Notas:
- O hwloc faz uma busca mais complicada que o algoritmo simples, pois ele prevê
a existência de "buracos" na hierarquia, ou seja, hierarquias assimétricas
(por exemplo, o ramo à extrema direita [nesta hierarquia](/artigos/relações.png)).
- O "algoritmo" overhead simplesmente não faz nada,
para verificar se os números têm realmente significado
(ou seja, se o loop não está tomando tempo significativo).

Graus de algumas hierarquias:
- [8 1 1 6 1 1 1](https://www.open-mpi.org/projects/hwloc/lstopo/images/8Opteron8400.v1.11.png)
- [4 4 1 3 2 1 1 1](https://www.open-mpi.org/projects/hwloc/lstopo/images/16XeonX7400.v1.11.png)
- [2 1 1 8 1 1 1 2](https://www.open-mpi.org/projects/hwloc/lstopo/images/2XeonE5+2ocl.v1.11.png)
- [2 2 1 5 1 1 1 1 8](https://www.open-mpi.org/projects/hwloc/lstopo/images/2Power8.v1.11.png)
- [1 4 1 1 9 2 1 1 4](https://www.open-mpi.org/projects/hwloc/lstopo/images/KNL.SNC4.H50.v1.11.png) (ignorando `MCDRAM`s)
- [1 61 (sessenta e um!) 1 1 1 4](https://www.open-mpi.org/projects/hwloc/lstopo/images/KNC.v1.11.png)

A princípio, os testes parecem mostrar que a abordagem simples para encontrar
o ancestral comum tem desempenho melhor que as outras em geral, e a nova
tem desempenho melhor que o hwloc.
No entanto, a abordagem simples não trata corretamente hierarquias assimétricas,
enquanto a nova trata, assim como o hwloc.

Testes com graus de
https://www.open-mpi.org/projects/hwloc/lstopo/images/16XeonX7400.v1.11.png
(4, 4, 1, 3, 2, 1, 1, 1)

#################################################

- Testes normais
```
Simples:  (1331.91 ms) (1341.41 ms) (1341.93 ms)
Novo:     (1747.46 ms) (1759.96 ms) (1756.78 ms)
Hwloc:    (2807.05 ms) (3051.18 ms) (2904.44 ms)
Overhead: (59.2883 ms) (59.1166 ms) (59.4811 ms)

Simples:  (1410.2 ms) (1387.68 ms) (1330.98 ms)
Novo:     (1776.34 ms) (1803.18 ms) (1769.59 ms)
Hwloc:    (2866.03 ms) (3097.61 ms) (2881.66 ms)
Overhead: (59.9031 ms) (58.2273 ms) (58.6618 ms)
```


```
Simples:  (1060.43 ms) (997.978 ms) (1009.38 ms)
Novo:     (1838.54 ms) (1786.18 ms) (1812.77 ms)
Hwloc:    (2362.81 ms) (2733.45 ms) (3001.02 ms)
Overhead: (59.8078 ms) (58.2407 ms) (58.2789 ms)

Simples:  (1001.15 ms) (995.819 ms) (997.859 ms)
Novo:     (1773.23 ms) (1799.39 ms) (1759.74 ms)
Hwloc:    (2811 ms) (2657.16 ms) (2700.06 ms)
Overhead: (65.0784 ms) (58.2702 ms) (58.1051 ms)
```

#################################################

- Nó com "enchimento" para ter mesmo tamanho dos objetos do hwloc
- Loop do ancestral simples alterado para ficar igual ao do hwloc
(que prevê hierarquias assimétricas (ex.: sem cache de um lado))

```
Simples: (2741.92 ms) (2676.33 ms) (3458.53 ms)
Novo: (1955.96 ms) (2095.53 ms) (1991.3 ms)
Hwloc: (2428.84 ms) (2734.23 ms) (3385.36 ms)
Overhead: (59.3171 ms) (58.4887 ms) (60.0502 ms)

Simples: (3104.91 ms) (2809.86 ms) (2719.33 ms)
Novo: (1803.04 ms) (1770.57 ms) (1778.25 ms)
Hwloc: (2903.94 ms) (3032.26 ms) (2854.52 ms)
Overhead: (58.5955 ms) (58.2291 ms) (59.3353 ms)
```


```
Simples: (821.686 ms) (806.038 ms) (814.207 ms)
Novo: (550.445 ms) (544.796 ms) (534.721 ms)
Hwloc: (742.574 ms) (815.785 ms) (940.112 ms)
Overhead: (25.6273 ms) (18.5921 ms) (18.5752 ms)

Simples: (931.704 ms) (912.149 ms) (840.712 ms)
Novo: (595.206 ms) (637.791 ms) (608.292 ms)
Hwloc: (871.417 ms) (908.79 ms) (869.82 ms)
Overhead: (18.1534 ms) (18.0424 ms) (17.5273 ms)
```

- Como era de se esperar, o simples ficou com tempos bem semelhantes aos do
hwloc.
- Na segunda execução, os tempos foram bem menores por algum motivo, mas as
proporções permaneceram semelhantes.

#################################################

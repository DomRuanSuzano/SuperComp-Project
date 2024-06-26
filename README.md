# Vehicle Routing Problem

O Vehicle Routing Problem (Problema de Roteamento de Veículos) é um desafio clássico de otimização combinatória, onde o objetivo é determinar uma rota eficiente para um conjunto de veículos que devem atender a um conjunto de clientes, minimizando o custo total da rota.

## Implementações

Este projeto implementa diferentes soluções para o Vehicle Routing Problem utilizando diversas abordagens:

- **Brute Force**: Avalia todas as possíveis rotas para determinar aquela com o menor custo.
- **Local Search**: Realiza iterações para melhorar gradualmente uma solução inicial.
- **OpenMP**: Utiliza paralelismo de memória compartilhada para acelerar o processamento.
- **MPI + OpenMP**: Combina paralelismo de memória compartilhada (OpenMP) com paralelismo de memória distribuída (MPI) para lidar com grandes conjuntos de dados em clusters de computadores.

## Brute Force

O algoritmo de força bruta testa todas as possíveis rotas para determinar aquela com o menor custo. Embora seja uma solução exata, sua escalabilidade é limitada pelo crescimento exponencial do número de rotas à medida que o número de cidades aumenta.

**Pontos Positivos:**
- Solução exata, garantindo a melhor rota possível.
- Simplicidade conceitual.

**Pontos Negativos:**
- Inviável para problemas com muitas cidades devido ao crescimento exponencial do espaço de busca.

### Como Executar

Para executar o algoritmo de força bruta, utilize o seguinte comando:

```
./brute_force arquivo_de_entrada.txt
```

Substitua `arquivo_de_entrada.txt` pelo nome do arquivo contendo a descrição do problema.

## Busca Local

A busca local é uma heurística de melhoria que busca iterativamente melhorar uma solução inicial realizando pequenas modificações em busca de uma solução ótima local. No contexto do Problema do Roteamento de Veículos (VRP), a busca local pode ser aplicada para encontrar uma rota de custo mínimo para cada veículo, respeitando as restrições de capacidade e visitando todos os clientes.

**Pontos Positivos:**
- Pode encontrar soluções de alta qualidade em um tempo razoável para problemas de tamanho moderado.
- Mais eficiente em termos de tempo de execução do que a busca exaustiva.

**Pontos Negativos:**
- Não garante uma solução ótima global, apenas uma solução localmente ótima.
- A qualidade da solução depende da escolha da solução inicial e do movimento de vizinhança.

### Como Executar

Para executar o algoritmo de busca local, utilize o seguinte comando:

```
./localsearch arquivo_de_entrada.txt
```

Substitua `arquivo_de_entrada.txt` pelo nome do arquivo contendo a descrição do problema.

## OpenMP

O OpenMP é uma API para programação paralela de memória compartilhada, que permite que os programas aproveitem múltiplos núcleos em uma única máquina para acelerar o processamento.

**Pontos Positivos:**
- Aproveita o paralelismo de memória compartilhada para acelerar o processamento em sistemas multicore.
- Fácil de usar e integrar em código C/C++ existente.

**Pontos Negativos:**
- Limitado ao paralelismo em uma única máquina.
- Escalabilidade limitada para problemas de grande porte.

### Como Executar

Para executar a versão OpenMP do algoritmo, utilize o seguinte comando:

```
./openmp arquivo_de_entrada.txt
```

Substitua `arquivo_de_entrada.txt` pelo nome do arquivo contendo a descrição do problema.

## MPI + OpenMP

A combinação de MPI (Message Passing Interface) com OpenMP permite que os programas aproveitem tanto o paralelismo de memória compartilhada quanto o paralelismo de memória distribuída. Isso é útil em cenários onde é necessário distribuir o trabalho entre vários processos MPI em um cluster de computadores, enquanto cada processo utiliza paralelismo de memória compartilhada em sua máquina local.

**Pontos Positivos:**
- Escalabilidade para problemas de grande porte em clusters de computadores.
- Aproveita tanto o paralelismo de memória compartilhada quanto o paralelismo de memória distribuída.
- Potencial para melhorias significativas de desempenho em sistemas distribuídos com múltiplos núcleos em cada nó.

**Pontos Negativos:**
- Complexidade adicional de programação e depuração.
- Overhead de comunicação entre processos MPI.

### Como Executar

Para executar o algoritmo utilizando MPI + OpenMP, utilize o seguinte comando:

```
mpirun -np <num_processos> mpi_openmp arquivo_de_entrada.txt
```

Substitua `<num_processos>` pelo número desejado de processos MPI e `arquivo_de_entrada.txt` pelo nome do arquivo contendo a descrição do problema.

## Conclusão

A análise dos resultados obtidos para o Problema do Roteamento de Veículos (VRP) demonstra diferentes desempenhos entre os algoritmos testados: Brute Force, Local Search, OpenMP e OpenMP + MPI.

![Resultados](output.png)

Ao comparar os tempos de execução dos algoritmos, fica evidente que o Brute Force é o menos eficiente à medida que o número de nós aumenta, devido ao crescimento exponencial do espaço de busca. Por outro lado, o algoritmo Local Search se destaca como o mais rápido, oferecendo tempos de execução significativamente menores em comparação com os outros métodos. No entanto, é importante notar que a solução fornecida pela Busca Local não é necessariamente a solução exata para o problema, mas sim uma solução localmente ótima.

O algoritmo OpenMP mostrou-se uma melhoria considerável em relação ao Brute Force, aproveitando a paralelização para reduzir os tempos de execução. Além disso, a combinação de OpenMP com MPI (OpenMP + MPI) manteve tempos de execução próximos aos do OpenMP puro, mesmo em sistemas distribuídos.

Em suma, enquanto o Brute Force oferece uma solução exata, seu desempenho decai rapidamente com o aumento do tamanho do problema. O algoritmo Local Search oferece tempos de execução mais rápidos, embora não garanta a solução exata. Por outro lado, o OpenMP e o OpenMP + MPI fornecem uma abordagem eficiente para reduzir os tempos de execução, especialmente em problemas de grande escala.


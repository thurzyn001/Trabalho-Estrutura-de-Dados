# TASKS.md — Especificação Detalhada de Tarefas do Projeto

> **Projeto:** Merge Sort Recursivo em C (30 a 300.000 dados)  
> **Organização:** Dividido em 6 Épicos (Fases) sequenciais com critérios claros de aceite.

---

## Épico 1: Estrutura do Projeto e Modelagem do Registro (Camada de Adaptação Excel)

### TASK-1.1: Criar Estrutura de Diretórios e Automação de Build
- **Arquivos:** `include/`, `src/`, `data/`, `results/`, `scripts/`, `Makefile`, `compilar.bat`
- **Descrição:** Configurar as pastas do projeto e criar scripts para compilar todos os arquivos `.c` de `src/` usando `gcc` com flags de boas práticas (`-Wall -Wextra -std=c99 -O2 -Iinclude`).
- **Critério de Aceite:** Executar `compilar.bat` (Windows) ou `make` (Linux/MinGW) compila o projeto sem nenhum *warning* ou erro.

### TASK-1.2: Definir a `struct Registro` e a Interface de Comparação
- **Arquivos:** `include/registro.h`, `src/registro.c`
- **Descrição:**
  1. Definir a `struct Registro` representando uma linha genérica de planilha do Excel (ex: `id`, `nome`, `categoria`, `valor`), claramente marcada com o comentário `ZONA DE ADAPTAÇÃO DO EXCEL`.
  2. Implementar `int comparar_registros(const Registro *a, const Registro *b)` para isolar a chave de ordenação.
  3. Implementar `void imprimir_registro(const Registro *r)` para exibir uma linha formatada no terminal.
- **Critério de Aceite:** É possível alterar a coluna de ordenação (ex: de `id` para `valor` ou `nome`) mudando apenas 1 linha em `src/registro.c`.

---

## Épico 2: Implementação do Merge Sort Recursivo e Validador

### TASK-2.1: Implementar a Função Fachada `merge_sort()` e Alocação na Heap
- **Arquivos:** `include/mergesort.h`, `src/mergesort.c`
- **Descrição:**
  1. Criar a função `int merge_sort(Registro *vetor, int n, Metricas *m)`.
  2. Validar se `vetor == NULL` ou `n <= 1`.
  3. Alocar **uma única vez** o vetor auxiliar `Registro *aux = malloc(n * sizeof(Registro))` na Heap.
  4. Chamar `merge_sort_recursivo(vetor, aux, 0, n - 1, m)`.
  5. Liberar `free(aux)` ao final e retornar status de sucesso (`0`) ou erro de memória (`-1`).
- **Critério de Aceite:** Nenhum vetor auxiliar é alocado na Stack; `malloc` e `free` são executados apenas 1 vez por bateria de ordenação.

### TASK-2.2: Implementar `merge_sort_recursivo()` e `intercalar()`
- **Arquivos:** `src/mergesort.c`
- **Descrição:**
  1. Em `merge_sort_recursivo()`:
     - Incrementar `m->chamadas_recursivas++`.
     - Verificar caso base: `if (inicio >= fim) return;`.
     - Calcular `int meio = inicio + (fim - inicio) / 2;`.
     - Chamar recursivamente para `[inicio .. meio]` e `[meio + 1 .. fim]`.
     - Chamar `intercalar(vetor, aux, inicio, meio, fim, m)`.
  2. Em `intercalar()`:
     - Usar três laços `while` clássicos e didáticos (`i` para metade esquerda, `j` para metade direita, `k` para posição no vetor auxiliar `aux`).
     - Usar `comparar_registros(&vetor[i], &vetor[j]) <= 0` para manter a **estabilidade**.
     - Contabilizar `m->comparacoes++` e `m->movimentacoes++`.
     - Copiar o trecho ordenado de `aux[inicio..fim]` de volta para `vetor[inicio..fim]`.
- **Critério de Aceite:** Código amplamente comentado nos 4 passos didáticos e ordenando corretamente qualquer tamanho de vetor.

### TASK-2.3: Implementar Auditor Automático `verificar_ordenacao()`
- **Arquivos:** `include/mergesort.h`, `src/mergesort.c`
- **Descrição:** Percorrer o vetor de `0` até `n - 2` verificando se `comparar_registros(&vetor[i], &vetor[i + 1]) <= 0`.
- **Critério de Aceite:** Retorna `1` (verdadeiro) se todo o vetor estiver ordenado e `0` (falso) caso exista qualquer inversão, comprovando ao professor que os 300.000 itens foram ordenados sem falhas.

---

## Épico 3: Cronômetro de Alta Precisão e Métricas

### TASK-3.1: Implementar Módulo de Tempo de Alta Resolução e Contadores
- **Arquivos:** `include/metricas.h`, `src/metricas.c`
- **Descrição:**
  1. Criar a `struct Metricas` contendo:
     - `int quantidade_dados;`
     - `double tempo_ms;`
     - `double tempo_s;`
     - `unsigned long long comparacoes;`
     - `unsigned long long movimentacoes;`
     - `unsigned long long chamadas_recursivas;`
     - `int ordenado_com_sucesso;`
  2. Implementar timer multiplataforma usando `<windows.h>` (`QueryPerformanceCounter` / `QueryPerformanceFrequency`) no Windows e `clock_gettime(CLOCK_MONOTONIC)` em sistemas POSIX, com fallback para `clock()`.
- **Critério de Aceite:** Mesmo para $N = 30$ elementos, o tempo exibido possui precisão de frações de milissegundo (ex: `0.0045 ms`), nunca exibindo `0.0000 ms` por falta de resolução.

---

## Épico 4: Leitura de Dados do Excel (CSV) e Gerador de Massa

### TASK-4.1: Implementar o Gerador de Dados Sintéticos (300.000 Linhas)
- **Arquivos:** `include/leitor_csv.h`, `src/leitor_csv.c`
- **Descrição:** Criar a função `gerar_csv_teste(const char *caminho, int quantidade, int modo)` capaz de gerar um arquivo `.csv` com `300.000` linhas simulando uma tabela do Excel (com cabeçalho e dados realistas) em 3 modos:
  - `MODO_ALEATORIO` (Caso médio - padrão)
  - `MODO_CRESCENTE` (Melhor caso / já ordenado)
  - `MODO_DECRESCENTE` (Pior caso / ordem inversa)
- **Critério de Aceite:** Gera `data/dados_excel.csv` com 300.000 linhas em menos de 1 segundo.

### TASK-4.2: Implementar o Leitor Flexível de CSV (`carregar_dados_csv`)
- **Arquivos:** `include/leitor_csv.h`, `src/leitor_csv.c`
- **Descrição:**
  1. Abrir o arquivo `.csv` oriundo do Excel.
  2. Detectar e pular automaticamente a linha de cabeçalho.
  3. Detectar automaticamente se o separador do Excel é `;` (padrão PT-BR) ou `,` (padrão internacional).
  4. Ler exatamente `N` registros (`30`, `300`, `3.000`, `30.000` ou `300.000`) para um vetor alocado dinamicamente na Heap.
- **Critério de Aceite:** Carrega qualquer um dos 5 volumes solicitados sem vazamento de memória e avisa claramente caso o arquivo tenha menos linhas do que o solicitado.

### TASK-4.3: Criar Script Auxiliar de Conversão `.xlsx` -> `.csv`
- **Arquivos:** `scripts/excel_para_csv.py`
- **Descrição:** Criar um script curto e opcional em Python que converte qualquer arquivo `.xlsx` para `data/dados_excel.csv`, além de documentar no `README.md` como salvar direto pelo Excel (`Salvar Como -> CSV UTF-8`).
- **Critério de Aceite:** Estudante tem duas formas fáceis (manual pelo Excel em 10 segundos ou via script) de preparar a planilha oficial.

---

## Épico 5: Orquestrador Principal e Bateria de Benchmarks (`30` a `300.000`)

### TASK-5.1: Implementar o Menu e o Executor das 5 Escalas em `main.c`
- **Arquivos:** `src/main.c`
- **Descrição:**
  1. Definir o vetor de tamanhos oficiais do trabalho:
     ```c
     const int TAMANHOS_TESTE[5] = {30, 300, 3000, 30000, 300000};
     ```
  2. Para cada tamanho $N$:
     - Carregar $N$ registros desordenados do CSV na Heap.
     - Iniciar o cronômetro de alta precisão.
     - Executar `merge_sort(vetor, N, &metricas)`.
     - Parar o cronômetro.
     - Executar `verificar_ordenacao(vetor, N)`.
     - Imprimir a linha correspondente na tabela de resultados.
     - Liberar a memória (`free(vetor)`).
  3. Exportar a tabela consolidada para `results/resultados_benchmark.csv`.
  4. Oferecer opção no menu para inspecionar visualmente os primeiros e últimos registros antes e depois da ordenação (ótimo para demonstrar ao vivo para o professor!).
- **Critério de Aceite:** Todas as 5 cargas (`30`, `300`, `3.000`, `30.000`, `300.000`) executam em sequência em poucos segundos, exibindo quantidade e tempo com status `[OK]`.

---

## Épico 6: Adaptação Final da Planilha Oficial e Apresentação

### TASK-6.1: Adaptar à Tabela Oficial do Excel (Quando Disponível)
- **Arquivos:** `include/registro.h`, `src/registro.c`, `src/leitor_csv.c`
- **Descrição:** Seguir o checklist do `TODO.md` para mapear as colunas reais da tabela do Excel assim que o professor ou o grupo definir o arquivo.
- **Critério de Aceite:** Dados reais do Excel lidos, ordenados e validados nas 5 escalas.

### TASK-6.2: Estudo e Simulação da Apresentação
- **Arquivos:** `GUIA_ESTUDO_APRESENTACAO.md`
- **Descrição:** Ler o guia de estudo, revisar as respostas para as perguntas clássicas de defesa e testar a compilação e execução em tela cheia.
- **Critério de Aceite:** Domínio completo do código e da análise de complexidade $\Theta(n \log_2 n)$.

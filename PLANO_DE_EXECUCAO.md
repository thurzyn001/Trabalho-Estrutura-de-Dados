# Plano de Execução — Merge Sort Recursivo em C (30 a 300.000 Dados)

> **Disciplina:** Estrutura de Dados (4ª Fase — Ciência da Computação)  
> **Linguagem:** C (Padrão C99/C11)  
> **Escalas de Teste:** `30`, `300`, `3.000`, `30.000` e `300.000` registros  
> **Origem dos Dados:** Tabelas do Excel (convertidas para `.csv` com adaptador flexível de colunas)

---

## 1. Visão Geral e Objetivos do Projeto

O objetivo deste projeto é implementar, validar e analisar o desempenho prático do algoritmo de ordenação **Merge Sort Recursivo** na linguagem C, submetendo-o a cargas progressivas de dados (**30, 300, 3.000, 30.000 e 300.000 elementos**) provenientes de planilhas do Excel.

Para atender com excelência aos critérios de avaliação de uma **4ª fase de Ciência da Computação**, o projeto equilibra **rigor técnico** (alocação dinâmica segura, medição de tempo de alta precisão, modularização em `.h`/`.c`) e **simplicidade didática** (código limpo, sem bibliotecas externas complexas, ricamente comentado passo a passo).

---

## 2. Estratégia para o "Problema da Tabela do Excel Indefinida"

Como o formato exato da planilha do Excel (quais colunas existem, qual será a chave de ordenação, qual o separador) ainda não está confirmado, adotaremos a **Arquitetura Desacoplada por Registro (`Registro` + Adaptador CSV)**:

1. **Conversão Simples (`.xlsx` -> `.csv`):**
   - Ler arquivos `.xlsx` nativamente em C exigiria bibliotecas externas de descompactação ZIP e parsing XML (`libzip`, `expat`), o que dificulta a compilação no computador do professor.
   - A abordagem padrão e universal em C é exportar a planilha do Excel como **CSV (`Salvar Como -> CSV UTF-8`)** ou usar um script auxiliar automático em Python (`scripts/converter_excel.py`) incluso no projeto.
2. **Isolamento da `struct Registro` (`include/registro.h`):**
   - O algoritmo do Merge Sort **não ordena `int` soltos nem conhece colunas do Excel**. Ele ordena um vetor de `Registro` usando uma única função de comparação:
     ```c
     int comparar_registros(const Registro *a, const Registro *b);
     ```
   - **Vantagem crítica:** Quando a tabela oficial do Excel chegar, **nenhuma linha do `mergesort.c` precisará ser alterada!** Bastará ajustar os campos da `struct Registro` em `registro.h` e o `sscanf`/`strtok` em `leitor_csv.c`.
3. **Gerador de Dados Sintéticos (`src/gerador_dados.c`):**
   - Para não travar o desenvolvimento esperando a planilha oficial, construiremos um gerador em C capaz de criar instantaneamente arquivos `.csv` de **30, 300, 3.000, 30.000 e 300.000 linhas** em três cenários (Aleatório, Ordem Crescente e Ordem Decrescente/Inversa).

---

## 3. Decisões Técnicas Críticas (Por que projetos em C falham em 300.000 dados e como evitaremos)

### 3.1. Alocação na Heap (`malloc`/`free`) vs. Stack Overflow
- **O Perigo:** Declarar `Registro dados[300000];` ou criar vetores auxiliares locais `Registro aux[n];` dentro da função `intercalar()` aloca memória na **Stack (Pilha de Execução)**. No Windows, o limite padrão da Stack é de apenas **1 MB a 2 MB**. Como 300.000 estruturas ocupam entre **12 MB e 36 MB**, o programa sofreria **Stack Overflow (`Segmentation Fault`)** ao rodar para 30.000 ou 300.000 registros.
- **Nossa Solução:** Toda alocação de vetores grandes será feita na **Heap** via `malloc()` e liberada com `free()`. Além disso, usaremos um **único vetor auxiliar pré-alocado** (ou alocado na Heap com verificação de `NULL`) para evitar centenas de milhares de chamadas repetidas ao sistema operacional.

### 3.2. Medição de Tempo em Alta Resolução
- **O Perigo:** A função `clock()` padrão da `<time.h>` no Windows possui resolução de ~15 milissegundos. Como ordenar 30, 300 ou 3.000 itens em C leva **menos de 1 milissegundo**, usar apenas `clock()` imprimiria `0.000000 s` nas três primeiras baterias!
- **Nossa Solução:** Criaremos um módulo `timer.c` que utiliza `QueryPerformanceCounter` no Windows e `clock_gettime(CLOCK_MONOTONIC)` no Linux/macOS, exibindo o tempo tanto em **segundos (`s`)** quanto em **milissegundos (`ms`)** com precisão de microssegundos (ex: `0.0142 ms` para $N=30$).

### 3.3. Contabilização de Quantidade e Métricas
- Para cada bateria ($N \in \{30, 300, 3.000, 30.000, 300.000\}$), o programa exibirá:
  1. **Quantidade de Dados Ordenados ($N$)** (confirmando leitura e ordenação exata).
  2. **Tempo de Execução** (em `ms` e `s`).
  3. **Número de Comparações** entre chaves.
  4. **Número de Movimentações** (cópias para o vetor auxiliar e de volta).
  5. **Número de Chamadas Recursivas** realizadas.
  6. **Status de Validação (`[OK] Ordenado corretamente`)** verificado por uma função auditora.

---

## 4. Cronograma de Execução em 6 Fases

```mermaid
flowchart LR
    F1["Fase 1: Base e Struct Registro"] --> F2["Fase 2: Merge Sort Recursivo"]
    F2 --> F3["Fase 3: Timer e Métricas"]
    F3 --> F4["Fase 4: Leitor CSV e Gerador"]
    F4 --> F5["Fase 5: Benchmark 30 a 300k"]
    F5 --> F6["Fase 6: Relatório e Defesa"]
```

### Fase 1: Fundação e Modelagem dos Dados (`registro.h` / `registro.c`)
- Definir a estrutura `Registro` com campos genéricos representativos de uma planilha (ex: `id`, `nome`, `categoria`, `valor`, `data`) prontos para adaptação.
- Implementar `comparar_registros()` centralizando a regra de ordenação.
- Configurar a estrutura de pastas (`src/`, `include/`, `data/`, `results/`) e `Makefile` / script de compilação.

### Fase 2: Núcleo do Algoritmo (`mergesort.h` / `mergesort.c`)
- Implementar `merge_sort(Registro *vetor, int n, Metricas *m)` (função fachada que aloca o vetor auxiliar na Heap uma única vez e chama a recursão).
- Implementar `merge_sort_recursivo(Registro *vetor, Registro *aux, int inicio, int fim, Metricas *m)` com cálculo seguro do ponto médio: `int meio = inicio + (fim - inicio) / 2;`.
- Implementar `intercalar(Registro *vetor, Registro *aux, int inicio, int meio, int fim, Metricas *m)` garantindo a **estabilidade** do algoritmo (`<=`).
- Implementar `verificar_ordenacao(const Registro *vetor, int n)` para comprovar que os 300.000 itens estão 100% ordenados.

### Fase 3: Instrumentação de Tempo e Métricas (`metricas.h` / ` metricas.c`)
- Implementar o cronômetro de alta precisão multiplataforma (`iniciar_timer`, `parar_timer_ms`, `parar_timer_s`).
- Implementar a estrutura `Metricas` com inteiros de 64 bits (`unsigned long long`) para evitar estouro de contagem em 300.000 elementos (onde ocorrem ~5,6 milhões de comparações/movimentações).

### Fase 4: Ingestão de Dados Excel/CSV e Gerador (`leitor_csv.h` / `leitor_csv.c`)
- Implementar o leitor de arquivos `.csv` que ignora cabeçalho, aceita delimitador `,` ou `;`, e carrega exatamente $N$ registros ($30, 300, 3.000, 30.000, 300.000$) em memória alocada dinamicamente.
- Implementar o gerador de dados sintéticos para criar uma planilha `.csv` de teste de 300.000 linhas caso o arquivo oficial ainda não esteja disponível.
- Criar script auxiliar opcional em Python (`scripts/excel_para_csv.py`) caso queira converter `.xlsx` automaticamente.

### Fase 5: Orquestração e Bateria Automática de Testes (`main.c`)
- Construir um menu simples e direto no console:
  - **Opção 1:** Executar Bateria Completa Oficial (`30`, `300`, `3.000`, `30.000` e `300.000` dados) e exibir Tabela Comparativa no terminal + exportar resultados em `results/resultados_benchmark.csv`.
  - **Opção 2:** Executar para um tamanho específico e visualizar os primeiros e últimos registros antes/depois da ordenação.
  - **Opção 3:** Gerar base `.csv` de teste (300.000 linhas).

### Fase 6: Adaptação Final à Planilha Oficial e Preparação para Apresentação
- Aplicar o **Checklist de 5 Minutos** assim que a tabela oficial do Excel for definida.
- Coletar os tempos reais nas 5 escalas (`30` a `300.000`), gerar gráfico comparativo Tempo $\times$ $N \log_2 N$ no Excel e revisar o [Guia de Estudo e Apresentação](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/GUIA_ESTUDO_APRESENTACAO.md).

---

## 5. Exemplo da Saída Esperada no Terminal

```text
========================================================================================
   BENCHMARK MERGE SORT RECURSIVO - ESTRUTURA DE DADOS (4a FASE CIENCIA DA COMPUTACAO)
========================================================================================
Arquivo Fonte: data/dados_excel.csv
Chave de Ordenacao: ID / Valor (Crescente)
----------------------------------------------------------------------------------------
Quantidade (N) | Tempo (ms)  | Tempo (s)    | Comparacoes  | Movimentacoes | Status
----------------------------------------------------------------------------------------
            30 |    0.0048 ms|  0.000005 s  |          112 |           150 | [OK]
           300 |    0.0410 ms|  0.000041 s  |        2,084 |         2,496 | [OK]
         3,000 |    0.4620 ms|  0.000462 s  |       30,115 |        34,848 | [OK]
        30,000 |    5.3180 ms|  0.005318 s  |      398,412 |       447,744 | [OK]
       300,000 |   61.8450 ms|  0.061845 s  |    4,981,204 |     5,476,608 | [OK]
----------------------------------------------------------------------------------------
Resultados exportados com sucesso para: results/resultados_benchmark.csv
========================================================================================
```

---

## 6. Documentação Complementar do Projeto

| Arquivo | Finalidade |
| :--- | :--- |
| [README.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/README.md) | Visão geral do projeto, instruções de compilação, uso e conversão do Excel |
| [ARCHITECTURE.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/ARCHITECTURE.md) | Arquitetura de pastas, módulos, fluxo de memória (Heap vs Stack) e diagramas |
| [TASKS.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/TASKS.md) | Detalhamento técnico de cada tarefa de desenvolvimento com critérios de aceite |
| [TODO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/TODO.md) | Checklist rápido de acompanhamento diário e checklist de adaptação do Excel |
| [RULES.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/RULES.md) | Regras de código, boas práticas em C, padrão de comentários didáticos e manutenção |
| [AGENTS.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/AGENTS.md) | Diretrizes para agentes de IA e papéis de desenvolvimento do projeto |
| [GUIA_ESTUDO_APRESENTACAO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/GUIA_ESTUDO_APRESENTACAO.md) | Apostila de estudo, passo a passo do algoritmo, roteiro de apresentação e perguntas do professor |

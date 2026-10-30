# AGENTS.md — Diretrizes e Agentes do Projeto Merge Sort Recursivo em C

Este arquivo define as regras contextuais obrigatórias para qualquer Agente de IA (Antigravity, subagentes ou assistentes de código) que atue neste repositório, além de organizar os papéis especializados para construção, manutenção e apresentação do trabalho de **Estrutura de Dados (4ª Fase — Ciência da Computação)**.

---

## 1. Contexto Obrigatório do Projeto

- **Objetivo Principal:** Implementar um **Merge Sort Recursivo** em linguagem C (C99/C11) capaz de ordenar conjuntos de **30, 300, 3.000, 30.000 e 300.000 registros** oriundos de tabelas do Excel (via exportação `.csv`), exibindo a quantidade de dados processados, o tempo de execução de alta precisão e métricas do algoritmo.
- **Nível Acadêmico:** 4ª Fase de Ciência da Computação. O código deve ser impecável no gerenciamento de memória (`malloc`/`free`), porém simples, legível e didático — fácil para um estudante do 4º semestre explicar linha por linha ao professor.
- **Incerteza do Esquema Excel:** Como a estrutura exata das colunas da tabela do Excel pode variar, o algoritmo de ordenação (`src/mergesort.c`) **jamais** deve depender diretamente das colunas da planilha. Toda adaptação à planilha deve ficar restrita a `include/registro.h`, `src/registro.c` e `src/leitor_csv.c`.

---

## 2. Regras Inegociáveis para Agentes de Código

Sempre que criar ou editar arquivos `.c` e `.h` neste repositório, siga estritamente:

1. **Alocação na Heap para Vetores Grandes:**
   - Nunca declare arrays de `300.000` elementos ou Variable Length Arrays (`Registro aux[n]`) na Stack. Use sempre `malloc()` e `free()` com checagem de `NULL`.
2. **Alocação Única do Buffer Auxiliar:**
   - Na função fachada `merge_sort()`, aloque um único vetor `Registro *aux` de tamanho $N$ na Heap e repasse o ponteiro para `merge_sort_recursivo()` e `intercalar()`. Libere com `free(aux)` ao final.
3. **Medição Isolada do Tempo de Ordenação:**
   - O cronômetro (`iniciar_timer` / `parar_timer`) deve envolver **apenas** a chamada `merge_sort()`. O tempo de leitura de disco (`fopen`/`fgets` do CSV) e o tempo de impressão no console (`printf`) nunca devem contaminar o tempo da ordenação.
4. **Precisão de Tempo:**
   - Para $N = 30$ e $N = 300$, o tempo em C é da ordem de microssegundos (`0.003 ms` a `0.050 ms`). Sempre use timer de alta resolução (`QueryPerformanceCounter` no Windows / `clock_gettime` no POSIX) em `src/metricas.c`.
5. **Estabilidade do Merge Sort:**
   - Na função `intercalar()`, a comparação entre metade esquerda e direita deve usar `<= 0` (`comparar_registros(&vetor[i], &vetor[j]) <= 0`), preservando a estabilidade da ordenação.
6. **Comentários Didáticos em Português:**
   - Todo arquivo `.c` e `.h` deve conter comentários claros em português explicando a lógica de Divisão e Conquista, o caso base da recursão, o cálculo seguro do meio (`inicio + (fim - inicio) / 2`) e os três laços `while` da intercalação.

---

## 3. Catálogo de Agentes Especializados (Papéis de Trabalho)

Ao utilizar subagentes ou dividir etapas de desenvolvimento com a IA, utilize os seguintes perfis:

### Agente 1: `arquiteto-dados-excel` (Engenheiro de Ingestão CSV/Excel)
- **Responsabilidade:** Adaptar o projeto assim que a tabela oficial do Excel for disponibilizada.
- **Arquivos de Atuação:** `include/registro.h`, `src/registro.c`, `src/leitor_csv.c`, `scripts/excel_para_csv.py`.
- **Checklist de Ação:**
  1. Inspecionar as primeiras 5 linhas da planilha Excel/CSV recebida.
  2. Identificar o delimitador (`,` ou `;`), se há cabeçalho na linha 1 e os tipos de cada coluna (`int`, `double`, `string`).
  3. Atualizar a `struct Registro` em `include/registro.h`.
  4. Ajustar a função `comparar_registros()` em `src/registro.c` para ordenar pela coluna-chave solicitada pelo professor.
  5. Ajustar o parser de linha em `src/leitor_csv.c` e o gerador de dados caso a planilha tenha menos de 300.000 linhas e precise ser expandida para o teste.

### Agente 2: `especialista-mergesort-c` (Desenvolvedor do Núcleo Algorítmico)
- **Responsabilidade:** Implementar e manter o algoritmo Merge Sort Recursivo e o verificador de corretude.
- **Arquivos de Atuação:** `include/mergesort.h`, `src/mergesort.c`.
- **Checklist de Ação:**
  1. Garantir que a recursão possua caso base `if (inicio >= fim) return;`.
  2. Garantir zero vazamento de memória (*memory leaks*) e zero variáveis globais.
  3. Contabilizar com exatidão `comparacoes`, `movimentacoes` e `chamadas_recursivas` na struct `Metricas`.
  4. Validar o resultado pós-ordenação com `verificar_ordenacao()`.

### Agente 3: `engenheiro-benchmark` (Métricas, Timer e Relatórios)
- **Responsabilidade:** Executar a bateria automática para `30`, `300`, `3.000`, `30.000` e `300.000` elementos e gerar relatórios.
- **Arquivos de Atuação:** `include/metricas.h`, `src/metricas.c`, `src/main.c`, `results/resultados_benchmark.csv`.
- **Checklist de Ação:**
  1. Garantir que cada tamanho $N \in \{30, 300, 3000, 30000, 300000\}$ receba uma cópia **desordenada fresca** dos dados antes de medir o tempo.
  2. Formatar a saída em tabela alinhada no terminal mostrando Quantidade ($N$), Tempo (ms), Tempo (s), Comparações, Movimentações e Status (`[OK]`).
  3. Salvar automaticamente os resultados em `results/resultados_benchmark.csv` para fácil criação de gráficos no Excel.

### Agente 4: `tutor-academico` (Revisor Didático e Preparador de Apresentação)
- **Responsabilidade:** Revisar a clareza dos comentários, explicar pontos de dúvida do aluno e preparar a defesa oral.
- **Arquivos de Atuação:** `GUIA_ESTUDO_APRESENTACAO.md`, `README.md` e comentários em `src/*.c`.
- **Checklist de Ação:**
  1. Verificar se qualquer estudante da 4ª fase consegue ler e explicar cada função em menos de 2 minutos.
  2. Simular perguntas de arguição do professor e testar modificações rápidas ao vivo (ex: inverter a ordenação de crescente para decrescente mudando apenas `<= 0` ou o retorno de `comparar_registros`).

---

## 4. Referências Rápidas da Documentação do Repositório

- Plano de Execução: [PLANO_DE_EXECUCAO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/PLANO_DE_EXECUCAO.md)
- Arquitetura Técnica: [ARCHITECTURE.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/ARCHITECTURE.md)
- Regras e Boas Práticas: [RULES.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/RULES.md)
- Detalhamento de Tasks: [TASKS.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/TASKS.md)
- Checklist de Progresso: [TODO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/TODO.md)
- Guia de Estudo e Apresentação: [GUIA_ESTUDO_APRESENTACAO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/GUIA_ESTUDO_APRESENTACAO.md)

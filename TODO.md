# TODO.md — Checklist de Acompanhamento do Projeto

> Marque com `[x]` cada etapa concluída durante o desenvolvimento do trabalho de Estrutura de Dados.

---

## Fase 0: Planejamento, Arquitetura e Documentação Base
- [x] Criar Plano de Execução ([PLANO_DE_EXECUCAO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/PLANO_DE_EXECUCAO.md))
- [x] Definir Arquitetura Modular e Fluxo de Memória ([ARCHITECTURE.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/ARCHITECTURE.md))
- [x] Definir Regras de Desenvolvimento, Comentários e Boas Práticas ([RULES.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/RULES.md))
- [x] Configurar Diretrizes de Agentes de IA ([AGENTS.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/AGENTS.md))
- [x] Especificar Tarefas Detalhadas ([TASKS.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/TASKS.md))
- [x] Elaborar Guia de Leitura, Estudo e Apresentação ([GUIA_ESTUDO_APRESENTACAO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/GUIA_ESTUDO_APRESENTACAO.md))
- [x] Criar Documentação Principal do Repositório ([README.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/README.md))

---

## Fase 1: Estrutura de Pastas e Modelagem (`Registro`)
- [x] Criar diretórios `include/`, `src/`, `data/`, `results/` e `scripts/`
- [x] Implementar `include/registro.h` com a `struct Registro` adaptável ao Excel
- [x] Implementar `src/registro.c` com `comparar_registros()` e `imprimir_registro()`
- [x] Criar `Makefile` e `compilar.bat` para compilação rápida com `gcc`

---

## Fase 2: Núcleo do Merge Sort Recursivo
- [x] Implementar `include/mergesort.h`
- [x] Implementar `merge_sort()` (alocação única de `aux` na Heap com `malloc` e `free`)
- [x] Implementar `merge_sort_recursivo()` com comentários didáticos sobre a árvore de recursão
- [x] Implementar `intercalar()` com os 3 laços `while` clássicos e estabilidade (`<= 0`)
- [x] Implementar `verificar_ordenacao()` para auditar se os 300.000 itens ficaram 100% ordenados

---

## Fase 3: Medição de Tempo e Métricas
- [x] Implementar `include/metricas.h` com contadores `unsigned long long`
- [x] Implementar `src/metricas.c` com timer de alta resolução (`QueryPerformanceCounter` / `clock_gettime`)
- [x] Implementar exibição da tabela comparativa no terminal e exportação para `results/resultados_benchmark.csv`

---

## Fase 4: Leitura de CSV/Excel e Gerador de Testes
- [x] Implementar `include/leitor_csv.h` e `src/leitor_csv.c`
- [x] Suportar detecção automática de separador (`;` ou `,`) e cabeçalho do Excel
- [x] Implementar gerador automático de `300.000` registros de teste (Aleatório, Crescente, Decrescente)
- [x] Criar script opcional `scripts/excel_para_csv.py`

---

## Fase 5: Integração e Bateria Oficial (30 a 300.000 Dados)
- [x] Implementar `src/main.c` com menu interativo e execução automática das 5 cargas:
  - [x] Teste com **30** dados (Quantidade + Tempo + Status)
  - [x] Teste com **300** dados (Quantidade + Tempo + Status)
  - [x] Teste com **3.000** dados (Quantidade + Tempo + Status)
  - [x] Teste com **30.000** dados (Quantidade + Tempo + Status)
  - [x] Teste com **300.000** dados (Quantidade + Tempo + Status)
- [x] Validar ausência de *warnings* na compilação (`gcc -Wall -Wextra`)

---

## Checklist Especial: "Quando a Tabela do Excel Chegar" (5 Minutos)
> Assim que você receber a planilha oficial do trabalho, siga apenas estes 5 passos:
- [ ] **Passo 1:** Abrir a planilha no Excel e salvar como `CSV UTF-8 (Delimitado por vírgulas) (*.csv)` em `data/dados_excel.csv` (ou rodar `python scripts/excel_para_csv.py planilha.xlsx`).
- [ ] **Passo 2:** Olhar as colunas da planilha e atualizar os campos da `struct Registro` em `include/registro.h`.
- [ ] **Passo 3:** Definir qual coluna será a chave de ordenação dentro de `comparar_registros()` em `src/registro.c`.
- [ ] **Passo 4:** Ajustar a leitura dos campos da linha em `src/leitor_csv.c`.
- [ ] **Passo 5:** Recompilar (`compilar.bat`) e rodar a Opção 1 do programa para colher os tempos finais!

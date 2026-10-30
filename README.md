# Merge Sort Recursivo em C — Análise de Desempenho (30 a 300.000 Dados)

> **Trabalho Acadêmico — Estrutura de Dados (4ª Fase de Ciência da Computação)**  
> Implementação didática, modular e de alto desempenho do algoritmo **Merge Sort Recursivo** em linguagem C, projetada para ordenar dados provenientes de planilhas do Excel nas escalas de **30**, **300**, **3.000**, **30.000** e **300.000** registros.

---

## Sumário Rápido da Documentação

| Documento | Descrição |
| :--- | :--- |
| **[PLANO_DE_EXECUCAO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/PLANO_DE_EXECUCAO.md)** | Estratégia completa de construção em 6 fases, decisões técnicas e solução para planilhas Excel |
| **[ARCHITECTURE.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/ARCHITECTURE.md)** | Diagramas de componentes, fluxo recursivo, arquitetura de memória (Heap vs Stack) e estrutura de pastas |
| **[TASKS.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/TASKS.md)** | Especificação detalhada de todas as tarefas de engenharia e critérios de aceite |
| **[TODO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/TODO.md)** | Checklist de progresso e **Checklist de 5 Minutos** para quando a planilha oficial do Excel chegar |
| **[RULES.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/RULES.md)** | Regras de desenvolvimento em C, boas práticas de memória, padrão de comentários didáticos e manutenção |
| **[AGENTS.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/AGENTS.md)** | Diretrizes para agentes de IA e divisão de papéis especializados no projeto |
| **[GUIA_ESTUDO_APRESENTACAO.md](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/GUIA_ESTUDO_APRESENTACAO.md)** | Guia passo a passo de leitura do código, revisão teórica $\Theta(n \log_2 n)$, roteiro de apresentação e FAQ de arguição |

---

## Destaques Técnicos do Projeto

- **Escalas Oficiais de Teste:** Execução automatizada para `30`, `300`, `3.000`, `30.000` e `300.000` elementos.
- **Proteção contra Stack Overflow:** Alocação dinâmica na **Heap** (`malloc`/`free`) com buffer auxiliar único, permitindo ordenar 300.000 structs no Windows/Linux sem estourar a pilha de execução.
- **Cronômetro de Alta Precisão:** Medição em microssegundos/milissegundos (`QueryPerformanceCounter` no Windows / `clock_gettime` no Linux) para que mesmo a carga de `30` registros apresente tempo real e preciso.
- **Adaptador Flexível para Excel (`Registro`):** O algoritmo `mergesort.c` é 100% independente das colunas da tabela do Excel. Quando a planilha definitiva for recebida, basta ajustar `include/registro.h` e `src/leitor_csv.c`.
- **Auditoria Automática:** Após cada execução, a função `verificar_ordenacao()` confere se todos os elementos estão em ordem crescente antes de exibir `[OK]`.

---

## Como Preparar os Dados do Excel

Você pode alimentar o programa de três maneiras:

1. **Exportando direto pelo Microsoft Excel (Recomendado — 10 segundos):**
   - Abra sua planilha `.xlsx` no Excel.
   - Clique em **Arquivo $\rightarrow$ Salvar Como**.
   - Escolha o formato **CSV UTF-8 (Delimitado por vírgulas) (`*.csv`)**.
   - Salve com o nome `data/dados_excel.csv`.
2. **Usando o Gerador Interno do Programa (Quando ainda não tiver a planilha):**
   - O próprio programa em C possui uma opção no menu para gerar instantaneamente um arquivo `data/dados_excel.csv` com **300.000 linhas** (Aleatórias, Crescentes ou Decrescentes) para testes imediatos.
3. **Usando o Script Auxiliar em Python (Opcional):**
   ```bash
   python scripts/excel_para_csv.py caminho_da_planilha.xlsx data/dados_excel.csv
   ```

---

## Como Compilar e Executar

### Pré-requisitos
- Compilador **GCC** (MinGW-w64 / MSYS2 no Windows, ou GCC nativo no Linux/macOS).

### No Windows (1 Clique ou Terminal)
```powershell
.\compilar.bat
```
Ou diretamente via `gcc`:
```powershell
gcc -Wall -Wextra -std=c99 -O2 src/*.c -Iinclude -o mergesort.exe
.\mergesort.exe
```

### No Linux / macOS
```bash
make
./mergesort
```

---

## Estrutura de Pastas

```text
Trabalho Estrutura de Dados/
├── include/                      # Cabeçalhos (.h) - Contratos e structs
│   ├── registro.h                # Struct Registro (colunas do Excel) e comparação
│   ├── mergesort.h               # Protótipos do Merge Sort Recursivo e validador
│   ├── metricas.h                # Struct Metricas e Timer de alta precisão
│   └── leitor_csv.h              # Leitor de CSV/Excel e gerador de massa de testes
├── src/                          # Implementações (.c)
│   ├── main.c                    # Menu e bateria automática (30, 300, 3k, 30k, 300k)
│   ├── registro.c                # Comparação de chaves e impressão
│   ├── mergesort.c               # Merge Sort Recursivo comentado passo a passo
│   ├── metricas.c                # Timer de alta precisão e relatórios
│   └── leitor_csv.c              # Parser de CSV (',' ou ';') e gerador de 300k linhas
├── data/                         # Arquivos .csv de entrada
├── results/                      # Tabela de resultados exportada em .csv
└── scripts/                      # Utilitário opcional de conversão .xlsx -> .csv
```

---

## Licença

Distribuído sob a licença MIT. Veja o arquivo [LICENSE](file:///c:/Users/arthu/OneDrive/Desktop/Estudos/Trabalho%20Estrutura%20de%20Dados/LICENSE) para mais detalhes.

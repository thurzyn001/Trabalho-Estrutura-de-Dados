# Regras de Desenvolvimento, Manutenção e Boas Práticas em C

> **Público-Alvo:** Estudantes da 4ª Fase de Ciência da Computação e Agentes de IA que auxiliam na construção e manutenção do projeto.  
> **Filosofia Central:** *"Código profissional na segurança de memória, porém didático e direto como uma aula de Estrutura de Dados."*

---

## 1. Regra de Ouro da Simplicidade Acadêmica (Nível 4ª Fase)

1. **Proibido "Over-Engineering":**
   - **NÃO** utilizar ponteiros para funções genéricas complexas (`void *` estilo `qsort` da stdlib) que escondam a estrutura de dados. Na 4ª fase, o professor quer ver o vetor `Registro *vetor` claramente indexado (`vetor[i]`, `vetor[j]`, `aux[k]`).
   - **NÃO** utilizar bibliotecas externas (`libxlsx`, `glib`, `cmake` complexo). O projeto deve compilar com um único comando `gcc src/*.c -Iinclude -o mergesort`.
   - **NÃO** usar macros obscuras (`#define` com múltiplas linhas de código) ou operadores ternários aninhados. Prefira `if / else` explícito e legível.
2. **Proibido Variáveis Globais:**
   - Nem o vetor de dados, nem os contadores de tempo/comparações devem ser variáveis globais. Tudo deve ser passado via parâmetros (ponteiros `Registro *vetor`, `Metricas *m`), demonstrando domínio de passagem por referência em C.

---

## 2. Padrão de Nomenclatura e Estilo de Código

| Elemento | Padrão | Exemplo Correto | Exemplo Incorreto |
| :--- | :--- | :--- | :--- |
| **Funções e Variáveis** | `snake_case` em português claro | `merge_sort_recursivo()`, `ponto_medio` | `msRec()`, `m1`, `doStuff()` |
| **Structs e Typedefs** | `PascalCase` | `Registro`, `Metricas` | `st_reg`, `dados_t` |
| **Constantes e Macros** | `UPPER_SNAKE_CASE` | `MAX_LINHA_CSV`, `TAMANHO_MAX_NOME` | `maxLinha`, `tam` |
| **Índices do Merge Sort** | Clássicos da literatura (`inicio`, `meio`, `fim`, `i`, `j`, `k`) | `int i = inicio; int j = meio + 1;` | `int x1 = a; int x2 = b;` |

### Organização de Cabeçalhos (`.h`)
- Todo arquivo `.h` dentro de `include/` **deve** possuir *Header Guards* (`#ifndef ... #define ... #endif`) para evitar inclusão duplicada.
- Nenhuma função (exceto `static inline` se estritamente necessário, o que evitaremos) deve ser implementada dentro do `.h`. Cabeçalhos declaram o **O QUÊ**; arquivos `.c` implementam o **COMO**.

---

## 3. Regras de Gerenciamento de Memória (Anti-Crash em 300.000 Dados)

1. **Toda alocação dinâmica deve ser verificada imediatamente:**
   ```c
   Registro *vetor = (Registro *) malloc(quantidade * sizeof(Registro));
   if (vetor == NULL) {
       fprintf(stderr, "[ERRO] Falha ao alocar memoria na Heap para %d registros.\n", quantidade);
       return -1;
   }
   ```
2. **Regra de Paridade `malloc` / `free`:**
   - Para cada `malloc()` no código, deve existir um `free()` correspondente antes do encerramento do escopo ou do programa.
3. **Proibição de VLA (Variable Length Arrays) na Stack:**
   - Nunca escrever `Registro temp[fim - inicio + 1];` dentro da função `intercalar()`. Isso corrompe a pilha no Windows para $N = 300.000$.
4. **Cálculo Seguro do Meio (Prevenção de Overflow):**
   - Sempre calcular o meio como:
     ```c
     int meio = inicio + (fim - inicio) / 2;
     ```
   - E explicar em comentário por que essa fórmula evita estouro de inteiro (`inicio + fim`) em grandes índices.

---

## 4. Padrão de Comentários Didáticos (Como Comentar para Tirar Nota 10)

Em um trabalho universitário, comentários não servem apenas para manutenção, mas também para **mostrar ao professor que você compreende cada passo do algoritmo** e para **guiar você durante a apresentação**.

### 4.1. Cabeçalho de Função (Padrão Acadêmico Limpo)
Toda função deve ter um bloco explicativo curto acima dela:
```c
/**
 * @brief Intercala dois sub-vetores ordenados em um único segmento ordenado.
 *
 * Primeiro sub-vetor: vetor[inicio .. meio]
 * Segundo sub-vetor:  vetor[meio+1 .. fim]
 *
 * Complexidade de Tempo: O(n), onde n = (fim - inicio + 1)
 * Complexidade de Espaço: O(n) utilizando o buffer auxiliar na Heap.
 */
```

### 4.2. Comentários nos "4 Passos Clássicos" do Merge Sort
Dentro de `mergesort.c`, os comentários devem numerar claramente as etapas teóricas ensinadas em sala de aula:
- **Passo 1 (Caso Base da Recursão):** Explicar por que `if (inicio >= fim) return;` encerra a divisão quando o sub-vetor tem apenas 1 elemento (que já está trivialmente ordenado).
- **Passo 2 (Dividir - Divide):** Explicar o cálculo do `meio` e as duas chamadas recursivas (metade esquerda e metade direita).
- **Passo 3 (Conquistar / Intercalar - Combine):** Explicar os três laços `while` da função `intercalar()`:
  - Laço 1: Comparação elemento a elemento entre a metade esquerda (`i`) e a metade direita (`j`).
  - Laço 2: Cópia dos elementos restantes da metade esquerda (caso a direita tenha acabado antes).
  - Laço 3: Cópia dos elementos restantes da metade direita (caso a esquerda tenha acabado antes).
- **Passo 4 (Cópia de Volta):** Transferência do segmento ordenado de `aux[inicio..fim]` de volta para `vetor[inicio..fim]`.

### 4.3. O que NÃO fazer nos comentários
- Evitar comentários óbvios que poluem a leitura, como `i++; // incrementa i em 1`. Comente o **significado no algoritmo**: `i++; // Avança para o próximo elemento da metade esquerda`.

---

## 5. Regras de Manutenção para a Tabela do Excel

1. **Isolamento Total:** A função `intercalar()` nunca deve acessar campos internos da struct (ex: `vetor[i].preco`). Ela **sempre** deve chamar `comparar_registros(&vetor[i], &vetor[j])`.
2. **Tolerância a Falhas no CSV:**
   - O leitor de CSV deve suportar tanto separador vírgula (`,`, padrão EUA) quanto ponto-e-vírgula (`;`, padrão Excel em Português do Brasil).
   - O leitor deve remover quebras de linha `\r\n` (Windows) e `\n` (Linux) com segurança.
   - O leitor deve permitir pular a primeira linha caso seja o cabeçalho da planilha.
3. **Preservação do Estado Original entre Testes:**
   - Ao rodar a bateria de `30`, `300`, `3.000`, `30.000` e `300.000`, nunca reutilizar o mesmo vetor já ordenado na medição seguinte! Cada tamanho $N$ deve ser recarregado do arquivo (ou copiado de um vetor-mestre desordenado) antes de iniciar o cronômetro, garantindo que o tempo medido seja **exclusivamente o tempo de ordenação do Merge Sort** (sem somar o tempo de leitura de disco `fopen`/`fgets`).

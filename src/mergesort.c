/**
 * @file mergesort.c
 * @brief Implementação didática do algoritmo Merge Sort Recursivo em C.
 *
 * ============================================================================
 * RESUMO DIDÁTICO PARA APRESENTAÇÃO (4ª FASE — ESTRUTURA DE DADOS):
 * O Merge Sort baseia-se no paradigma "Dividir e Conquistar":
 *   1. DIVIDIR:    Divide o vetor ao meio recursivamente até chegar em
 *                  sub-vetores de apenas 1 elemento (Caso Base).
 *   2. CONQUISTAR: Cada sub-vetor de 1 elemento já está trivialmente ordenado.
 *   3. COMBINAR:   A função 'intercalar()' funde dois sub-vetores ordenados
 *                  em um único segmento ordenado utilizando o vetor auxiliar.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "mergesort.h"

/* Protótipos internos (funções auxiliares restritas a este módulo) */
static void merge_sort_recursivo(Registro *vetor, Registro *aux, int inicio, int fim, Metricas *m);
static void intercalar(Registro *vetor, Registro *aux, int inicio, int meio, int fim, Metricas *m);

/**
 * @brief Passo 3 e 4: Intercala dois sub-vetores adjacentes já ordenados.
 *
 * Primeiro sub-vetor (Esquerda): vetor[inicio .. meio]
 * Segundo sub-vetor  (Direita):  vetor[meio+1 .. fim]
 */
static void intercalar(Registro *vetor, Registro *aux, int inicio, int meio, int fim, Metricas *m) {
    int i = inicio;     /* Índice atual da metade esquerda (inicio até meio) */
    int j = meio + 1;   /* Índice atual da metade direita  (meio+1 até fim)  */
    int k = inicio;     /* Índice atual de escrita no vetor auxiliar 'aux'   */

    /* ------------------------------------------------------------------------
     * LAÇO 1: Compara elemento a elemento enquanto AMBAS as metades possuem
     * itens disponíveis. O menor (ou igual) vai para o vetor auxiliar 'aux'.
     *
     * NOTA SOBRE ESTABILIDADE:
     * Usamos '<= 0' na chamada de comparar_registros(). Assim, se o elemento
     * da esquerda tiver chave IGUAL ao da direita, o da esquerda entra primeiro,
     * preservando a ordem original da planilha do Excel (algoritmo estável).
     * ------------------------------------------------------------------------ */
    while (i <= meio && j <= fim) {
        m->comparacoes++;

        if (comparar_registros(&vetor[i], &vetor[j]) <= 0) {
            aux[k] = vetor[i];
            i++; /* Avança na metade esquerda */
        } else {
            aux[k] = vetor[j];
            j++; /* Avança na metade direita */
        }
        k++;
        m->movimentacoes++;
    }

    /* ------------------------------------------------------------------------
     * LAÇO 2: Se a metade direita acabou primeiro, copia os elementos que
     * sobraram na metade esquerda (eles já estão ordenados entre si).
     * ------------------------------------------------------------------------ */
    while (i <= meio) {
        aux[k] = vetor[i];
        i++;
        k++;
        m->movimentacoes++;
    }

    /* ------------------------------------------------------------------------
     * LAÇO 3: Se a metade esquerda acabou primeiro, copia os elementos que
     * sobraram na metade direita (eles também já estão ordenados entre si).
     * ------------------------------------------------------------------------ */
    while (j <= fim) {
        aux[k] = vetor[j];
        j++;
        k++;
        m->movimentacoes++;
    }

    /* ------------------------------------------------------------------------
     * PASSO 4 (CÓPIA DE VOLTA): Transfere o intervalo já ordenado do buffer
     * auxiliar 'aux[inicio..fim]' de volta para o vetor original 'vetor'.
     * ------------------------------------------------------------------------ */
    for (k = inicio; k <= fim; k++) {
        vetor[k] = aux[k];
        m->movimentacoes++;
    }
}

/**
 * @brief Passo 1 e 2: Divide recursivamente o vetor até restar 1 elemento
 *        e depois combina os sub-vetores na volta da recursão.
 */
static void merge_sort_recursivo(Registro *vetor, Registro *aux, int inicio, int fim, Metricas *m) {
    /* Contabiliza cada ativação da função na árvore de recursão */
    m->chamadas_recursivas++;

    /* ------------------------------------------------------------------------
     * PASSO 1 (CASO BASE DA RECURSÃO):
     * Se 'inicio >= fim', o sub-vetor tem apenas 1 elemento (ou está vazio).
     * Um conjunto de 1 único elemento já está ordenado por definição!
     * Retornamos imediatamente para encerrar este ramo da recursão.
     * ------------------------------------------------------------------------ */
    if (inicio >= fim) {
        return;
    }

    /* ------------------------------------------------------------------------
     * PASSO 2 (DIVIDIR):
     * Calculamos o índice central do intervalo [inicio .. fim].
     * Usamos 'inicio + (fim - inicio) / 2' em vez de '(inicio + fim) / 2'
     * para evitar estouro de inteiro (Integer Overflow) em vetores grandes.
     * ------------------------------------------------------------------------ */
    int meio = inicio + (fim - inicio) / 2;

    /* Chamada recursiva 1: Ordena a metade esquerda [inicio .. meio] */
    merge_sort_recursivo(vetor, aux, inicio, meio, m);

    /* Chamada recursiva 2: Ordena a metade direita [meio + 1 .. fim] */
    merge_sort_recursivo(vetor, aux, meio + 1, fim, m);

    /* ------------------------------------------------------------------------
     * PASSO 3 (CONQUISTAR / COMBINAR):
     * Agora que as duas metades estão ordenadas, intercalamos ambas.
     * ------------------------------------------------------------------------ */
    intercalar(vetor, aux, inicio, meio, fim, m);
}

/**
 * @brief Função fachada que prepara a memória Heap e inicia o Merge Sort.
 */
int merge_sort(Registro *vetor, int n, Metricas *m) {
    if (vetor == NULL || m == NULL || n <= 0) {
        return -1;
    }

    /* Se houver apenas 1 elemento, não há nada para ordenar */
    if (n == 1) {
        return 0;
    }

    /* ------------------------------------------------------------------------
     * ALOCAÇÃO NA HEAP (ANTI-STACK OVERFLOW):
     * Para 300.000 registros, o vetor auxiliar consome vários Megabytes.
     * Alocamos na Heap com malloc() UMA ÚNICA VEZ antes de iniciar a recursão
     * e reutilizamos este mesmo bloco em todas as chamadas de intercalar().
     * ------------------------------------------------------------------------ */
    Registro *aux = (Registro *)malloc((size_t)n * sizeof(Registro));
    if (aux == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar vetor auxiliar na Heap para %d registros.\n", n);
        return -1;
    }

    /* Dispara a árvore de recursão para o intervalo completo [0 .. n-1] */
    merge_sort_recursivo(vetor, aux, 0, n - 1, m);

    /* Libera a memória auxiliar da Heap (Regra: todo malloc tem seu free) */
    free(aux);

    return 0;
}

/**
 * @brief Audita o vetor após a ordenação para garantir que todos os N elementos
 *        respeitam a ordem vetor[i] <= vetor[i + 1].
 */
int verificar_ordenacao(const Registro *vetor, int n) {
    if (vetor == NULL || n <= 0) {
        return 0;
    }

    for (int i = 0; i < n - 1; i++) {
        if (comparar_registros(&vetor[i], &vetor[i + 1]) > 0) {
            /* Encontrou um elemento fora de ordem! */
            return 0;
        }
    }

    return 1; /* 100% ordenado */
}

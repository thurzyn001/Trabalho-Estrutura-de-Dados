/**
 * @file mergesort.h
 * @brief Interface pública do algoritmo Merge Sort Recursivo e do verificador
 *        de corretude da ordenação.
 *
 * Complexidade de Tempo:
 *   - Melhor Caso: Theta(n log2 n)
 *   - Caso Médio:  Theta(n log2 n)
 *   - Pior Caso:   Theta(n log2 n)
 *
 * Complexidade de Espaço:
 *   - O(n) na memória Heap (vetor auxiliar) + O(log2 n) na pilha de recursão.
 */

#ifndef MERGESORT_H
#define MERGESORT_H

#include "registro.h"
#include "metricas.h"

/**
 * @brief Função principal (fachada) do Merge Sort Recursivo.
 *
 * Aloca um único buffer auxiliar 'aux' de tamanho 'n' na memória Heap,
 * dispara a árvore de chamadas recursivas e libera o buffer ao final.
 *
 * @param vetor Ponteiro para o vetor de registros a ser ordenado.
 * @param n Quantidade de elementos no vetor (ex: 30, 300, 3.000, 30.000, 300.000).
 * @param m Ponteiro para a estrutura onde serão salvas as métricas de execução.
 * @return int Retorna 0 em caso de sucesso ou -1 se faltar memória na Heap.
 */
int merge_sort(Registro *vetor, int n, Metricas *m);

/**
 * @brief Verifica se o vetor de registros está 100% ordenado de forma crescente.
 *
 * Percorre o vetor comparando cada posição 'i' com 'i + 1'. Essencial para
 * comprovar ao professor que todos os 300.000 dados foram ordenados sem erros.
 *
 * @param vetor Ponteiro constante para o vetor já processado.
 * @param n Quantidade de elementos no vetor.
 * @return int Retorna 1 se estiver perfeitamente ordenado, ou 0 se houver falha.
 */
int verificar_ordenacao(const Registro *vetor, int n);

#endif /* MERGESORT_H */

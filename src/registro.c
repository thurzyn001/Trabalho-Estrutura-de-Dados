/**
 * @file registro.c
 * @brief Implementação das funções de comparação e exibição de um Registro.
 */

#include <stdio.h>
#include <string.h>
#include "registro.h"

/**
 * @brief Compara dois registros para definir a ordem no Merge Sort.
 *
 * DICA PARA APRESENTAÇÃO AO PROFESSOR:
 * Se o professor pedir para mudar a coluna de ordenação ou inverter para
 * ordem decrescente ao vivo, basta comentar a OPÇÃO 1 abaixo e descomentar
 * uma das outras opções! O arquivo mergesort.c não precisa ser alterado.
 */
int comparar_registros(const Registro *a, const Registro *b) {
    /* ========================================================================
     * OPÇÃO 1 (ATIVA POR PADRÃO): Ordenar pela coluna 'id' (Inteiro Crescente)
     * Evitamos fazer apenas 'return a->id - b->id' para prevenir overflow
     * caso existam números negativos ou muito grandes.
     * ======================================================================== */
    if (a->id < b->id) {
        return -1;
    }
    if (a->id > b->id) {
        return 1;
    }
    return 0;

    /* ========================================================================
     * OPÇÃO 2 (EXEMPLO): Ordenar pela coluna 'valor' (Double Crescente)
     * Para ativar, comente a OPÇÃO 1 acima e descomente as linhas abaixo:
     * ======================================================================== */
    /*
    if (a->valor < b->valor) return -1;
    if (a->valor > b->valor) return 1;
    return 0;
    */

    /* ========================================================================
     * OPÇÃO 3 (EXEMPLO): Ordenar por 'nome' em Ordem Alfabética (Texto)
     * Para ativar, comente a OPÇÃO 1 acima e descomente a linha abaixo:
     * ======================================================================== */
    /*
    return strcmp(a->nome, b->nome);
    */
}

/**
 * @brief Imprime o cabeçalho da tabela de registros para inspeção visual.
 */
void imprimir_cabecalho_registro(void) {
    printf("+------------+------------------------------+--------------------+--------------+------------+\n");
    printf("| %-10s | %-28s | %-18s | %-12s | %-10s |\n",
           "ID (Chave)", "Nome / Descricao", "Categoria", "Valor", "Data");
    printf("+------------+------------------------------+--------------------+--------------+------------+\n");
}

/**
 * @brief Imprime os campos de um Registro formatados em colunas alinhadas.
 */
void imprimir_registro(const Registro *r) {
    if (r == NULL) {
        return;
    }
    printf("| %10d | %-28.28s | %-18.18s | %12.2f | %-10.10s |\n",
           r->id,
           r->nome,
           r->categoria,
           r->valor,
           r->data);
}

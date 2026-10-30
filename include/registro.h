/**
 * @file registro.h
 * @brief Definição da estrutura de dados (Registro) que representa uma linha
 *        da planilha do Excel e contrato das funções de comparação e exibição.
 *
 * ============================================================================
 * NOTA PARA MANUTENÇÃO (QUANDO A TABELA OFICIAL DO EXCEL CHEGAR):
 * O algoritmo Merge Sort (em src/mergesort.c) NÃO depende das colunas abaixo.
 * Ele apenas move estruturas 'Registro' e chama 'comparar_registros()'.
 * Portanto, quando a planilha definitiva for definida, basta ajustar os campos
 * da struct Registro abaixo e atualizar 'src/registro.c' e 'src/leitor_csv.c'.
 * ============================================================================
 */

#ifndef REGISTRO_H
#define REGISTRO_H

/* Tamanhos máximos para campos de texto vindos das colunas do Excel */
#define TAM_MAX_NOME      64
#define TAM_MAX_CATEGORIA 32
#define TAM_MAX_DATA      16

/**
 * @brief ZONA DE ADAPTAÇÃO DO EXCEL — Estrutura que representa 1 linha da tabela.
 *
 * Atualmente configurada com 5 colunas típicas de planilhas acadêmicas/comerciais:
 *   Coluna 1: id        (Inteiro identificador ou chave numérica)
 *   Coluna 2: nome      (Texto: Nome, Produto, Aluno ou Descrição)
 *   Coluna 3: categoria (Texto: Categoria, Curso, Cidade ou Departamento)
 *   Coluna 4: valor     (Decimal: Preço, Nota, Salário ou Pontuação)
 *   Coluna 5: data      (Texto: Data no formato AAAA-MM-DD ou DD/MM/AAAA)
 */
typedef struct {
    int id;
    char nome[TAM_MAX_NOME];
    char categoria[TAM_MAX_CATEGORIA];
    double valor;
    char data[TAM_MAX_DATA];
} Registro;

/**
 * @brief Compara dois registros pela chave de ordenação escolhida.
 *
 * Esta função isola a regra de negócio da ordenação. O Merge Sort utiliza
 * o retorno desta função para decidir qual elemento vem antes:
 *   - Retorna valor < 0 se o registro 'a' deve vir ANTES do registro 'b'.
 *   - Retorna         0 se 'a' e 'b' possuem chaves IGUAIS.
 *   - Retorna valor > 0 se o registro 'a' deve vir DEPOIS do registro 'b'.
 *
 * @param a Ponteiro constante para o primeiro registro (metade esquerda).
 * @param b Ponteiro constante para o segundo registro (metade direita).
 * @return int Resultado da comparação (< 0, 0 ou > 0).
 */
int comparar_registros(const Registro *a, const Registro *b);

/**
 * @brief Imprime no console o cabeçalho alinhado das colunas do Registro.
 */
void imprimir_cabecalho_registro(void);

/**
 * @brief Imprime no console uma única linha formatada com os dados do Registro.
 *
 * @param r Ponteiro constante para o registro a ser impresso.
 */
void imprimir_registro(const Registro *r);

#endif /* REGISTRO_H */

/**
 * @file leitor_csv.h
 * @brief Leitura de arquivos CSV exportados do Excel e gerador de massa de
 *        dados sintéticos para testes de 30 a 300.000 registros.
 */

#ifndef LEITOR_CSV_H
#define LEITOR_CSV_H

#include "registro.h"

/* Caminho padrão do arquivo CSV exportado do Excel */
#define CAMINHO_CSV_PADRAO "data/dados_excel.csv"

/* Quantidade máxima exigida pelo trabalho acadêmico */
#define TOTAL_MAXIMO_DADOS 300000

/* Modos de geração de dados sintéticos para testes de cenário */
typedef enum {
    MODO_ALEATORIO   = 1, /* Caso Médio: Chaves embaralhadas aleatoriamente */
    MODO_CRESCENTE   = 2, /* Melhor Caso: Dados já ordenados de 1 até N     */
    MODO_DECRESCENTE = 3  /* Pior Caso: Dados em ordem inversa de N até 1   */
} ModoGeracao;

/**
 * @brief Carrega 'quantidade' registros a partir de um arquivo CSV do Excel
 *        para um vetor alocado dinamicamente na memória Heap.
 *
 * Características inteligentes para compatibilidade com o Excel:
 *   - Ignora automaticamente o BOM UTF-8 (\xEF\xBB\xBF) gerado pelo Excel.
 *   - Detecta e pula automaticamente a linha de cabeçalho da planilha.
 *   - Detecta automaticamente o separador ';' (Excel PT-BR) ou ',' (Excel EN).
 *   - Caso a planilha tenha menos linhas que 'quantidade', replica os registros
 *     lidos de forma controlada para permitir o benchmark até 300.000 itens.
 *
 * @param caminho_arquivo Caminho para o arquivo .csv.
 * @param quantidade Número de elementos a carregar (ex: 30, 300, 3000, 30000, 300000).
 * @param registros_lidos_reais Ponteiro opcional para receber quantas linhas físicas existiam no CSV.
 * @return Registro* Ponteiro para o vetor alocado na Heap (deve ser liberado com free),
 *                   ou NULL em caso de erro.
 */
Registro *carregar_dados_csv(const char *caminho_arquivo, int quantidade, int *registros_lidos_reais);

/**
 * @brief Gera um arquivo CSV simulando uma tabela do Excel com 'quantidade'
 *        linhas (padrão: 300.000 linhas), permitindo testar o projeto antes
 *        mesmo de receber a planilha definitiva.
 *
 * @param caminho_arquivo Caminho onde o CSV será salvo (ex: "data/dados_excel.csv").
 * @param quantidade Total de registros a gerar (ex: 300000).
 * @param modo MODO_ALEATORIO, MODO_CRESCENTE ou MODO_DECRESCENTE.
 * @return int Retorna 0 em caso de sucesso ou -1 em caso de falha.
 */
int gerar_csv_teste(const char *caminho_arquivo, int quantidade, ModoGeracao modo);

/**
 * @brief Verifica se o arquivo CSV existe no disco.
 *
 * @param caminho_arquivo Caminho do arquivo a verificar.
 * @return int Retorna 1 se o arquivo existe, ou 0 caso contrário.
 */
int arquivo_csv_existe(const char *caminho_arquivo);

#endif /* LEITOR_CSV_H */

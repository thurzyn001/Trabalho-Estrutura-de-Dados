/**
 * @file leitor_csv.c
 * @brief Implementação do leitor de CSV (exportado do Excel) e do gerador de
 *        dados de teste para até 300.000 registros.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "leitor_csv.h"

#define TAM_MAX_LINHA_CSV 512

/**
 * @brief Remove quebras de linha (\r e \n) do final de uma string lida por fgets.
 */
static void remover_quebra_linha(char *linha) {
    if (linha == NULL) {
        return;
    }
    size_t len = strlen(linha);
    while (len > 0 && (linha[len - 1] == '\n' || linha[len - 1] == '\r')) {
        linha[len - 1] = '\0';
        len--;
    }
}

/**
 * @brief Detecta qual delimitador o Excel utilizou na linha (';' ou ',').
 *        O Excel em Português (Brasil) usa ';' por padrão; em Inglês usa ','.
 */
static char detectar_delimitador(const char *linha) {
    int cont_ponto_virgula = 0;
    int cont_virgula = 0;

    for (int i = 0; linha[i] != '\0'; i++) {
        if (linha[i] == ';') cont_ponto_virgula++;
        if (linha[i] == ',') cont_virgula++;
    }

    return (cont_ponto_virgula >= cont_virgula && cont_ponto_virgula > 0) ? ';' : ',';
}

/**
 * @brief Verifica se a primeira linha do arquivo CSV é um cabeçalho de texto
 *        (ex: "ID;Nome;Categoria;Valor;Data") ou se já começa com dados.
 */
static int linha_eh_cabecalho(const char *linha) {
    const unsigned char *p = (const unsigned char *)linha;

    /* Pula possível BOM UTF-8 (0xEF, 0xBB, 0xBF) inserido pelo Excel */
    if (p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) {
        p += 3;
    }

    /* Pula espaços iniciais */
    while (*p != '\0' && isspace(*p)) {
        p++;
    }

    /* Se o primeiro caractere útil não for dígito nem sinal de menos, é cabeçalho */
    if (*p != '\0' && !isdigit(*p) && *p != '-') {
        return 1;
    }
    return 0;
}

/**
 * @brief Extrai o próximo campo de uma linha CSV separada por 'delim'.
 *        Mais seguro que strtok() pois aceita colunas vazias consecutivas (;;).
 */
static void extrair_campo_csv(const char **cursor, char delim, char *destino, size_t tam_max) {
    if (cursor == NULL || *cursor == NULL || destino == NULL || tam_max == 0) {
        return;
    }

    const char *inicio = *cursor;
    size_t i = 0;

    /* Se o campo estiver entre aspas no Excel, pula a aspa inicial */
    if (*inicio == '"') {
        inicio++;
        while (*inicio != '\0' && *inicio != '"') {
            if (i < tam_max - 1) {
                destino[i++] = *inicio;
            }
            inicio++;
        }
        if (*inicio == '"') {
            inicio++;
        }
        if (*inicio == delim) {
            inicio++;
        }
    } else {
        while (*inicio != '\0' && *inicio != delim) {
            if (i < tam_max - 1) {
                destino[i++] = *inicio;
            }
            inicio++;
        }
        if (*inicio == delim) {
            inicio++;
        }
    }

    destino[i] = '\0';
    *cursor = inicio;
}

/**
 * @brief Converte uma string numérica para double, aceitando tanto ponto (12.34)
 *        quanto vírgula (12,34 - padrão Excel Brasil).
 */
static double converter_para_double(const char *texto) {
    char buffer[64];
    size_t i = 0;

    for (; texto[i] != '\0' && i < sizeof(buffer) - 1; i++) {
        buffer[i] = (texto[i] == ',') ? '.' : texto[i];
    }
    buffer[i] = '\0';
    return atof(buffer);
}

/**
 * @brief ZONA DE ADAPTAÇÃO DO EXCEL — Converte 1 linha de texto do CSV em uma
 *        struct Registro.
 *
 * Quando a planilha oficial do Excel chegar, basta ajustar a ordem de leitura
 * das colunas abaixo para corresponder às colunas da planilha!
 */
static int interpretar_linha_csv(const char *linha, char delim, Registro *reg) {
    const char *cursor = linha;
    char campo_id[32];
    char campo_valor[32];

    /* Pula BOM UTF-8 caso esteja na primeira linha sem cabeçalho */
    const unsigned char *u = (const unsigned char *)cursor;
    if (u[0] == 0xEF && u[1] == 0xBB && u[2] == 0xBF) {
        cursor += 3;
    }

    /* Ignora linhas em branco */
    if (*cursor == '\0') {
        return 0;
    }

    /* Coluna 1: ID (Inteiro) */
    extrair_campo_csv(&cursor, delim, campo_id, sizeof(campo_id));
    reg->id = atoi(campo_id);

    /* Coluna 2: Nome / Descrição (Texto) */
    extrair_campo_csv(&cursor, delim, reg->nome, sizeof(reg->nome));
    if (reg->nome[0] == '\0') {
        snprintf(reg->nome, sizeof(reg->nome), "Registro_%d", reg->id);
    }

    /* Coluna 3: Categoria (Texto) */
    extrair_campo_csv(&cursor, delim, reg->categoria, sizeof(reg->categoria));
    if (reg->categoria[0] == '\0') {
        snprintf(reg->categoria, sizeof(reg->categoria), "Geral");
    }

    /* Coluna 4: Valor (Decimal / Double) */
    extrair_campo_csv(&cursor, delim, campo_valor, sizeof(campo_valor));
    reg->valor = converter_para_double(campo_valor);

    /* Coluna 5: Data (Texto) */
    extrair_campo_csv(&cursor, delim, reg->data, sizeof(reg->data));
    if (reg->data[0] == '\0') {
        snprintf(reg->data, sizeof(reg->data), "2026-10-23");
    }

    return 1;
}

/**
 * @brief Verifica se o arquivo CSV existe e pode ser lido.
 */
int arquivo_csv_existe(const char *caminho_arquivo) {
    if (caminho_arquivo == NULL) {
        return 0;
    }
    FILE *f = fopen(caminho_arquivo, "r");
    if (f != NULL) {
        fclose(f);
        return 1;
    }
    return 0;
}

/**
 * @brief Carrega 'quantidade' registros do arquivo CSV para um vetor na Heap.
 */
Registro *carregar_dados_csv(const char *caminho_arquivo, int quantidade, int *registros_lidos_reais) {
    if (caminho_arquivo == NULL || quantidade <= 0) {
        return NULL;
    }

    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "[ERRO] Nao foi possivel abrir o arquivo CSV: %s\n", caminho_arquivo);
        return NULL;
    }

    /* Aloca o vetor principal na memória Heap para suportar até 300.000 itens */
    Registro *vetor = (Registro *)malloc((size_t)quantidade * sizeof(Registro));
    if (vetor == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar memoria na Heap para %d registros.\n", quantidade);
        fclose(arquivo);
        return NULL;
    }

    char linha[TAM_MAX_LINHA_CSV];
    char delimitador = ';';
    int indice = 0;
    int primeira_linha = 1;

    while (indice < quantidade && fgets(linha, sizeof(linha), arquivo) != NULL) {
        remover_quebra_linha(linha);
        if (linha[0] == '\0') {
            continue;
        }

        if (primeira_linha) {
            primeira_linha = 0;
            delimitador = detectar_delimitador(linha);

            /* Se a primeira linha for cabeçalho (ex: "ID;Nome;..."), pula! */
            if (linha_eh_cabecalho(linha)) {
                continue;
            }
        }

        if (interpretar_linha_csv(linha, delimitador, &vetor[indice])) {
            indice++;
        }
    }

    fclose(arquivo);

    if (registros_lidos_reais != NULL) {
        *registros_lidos_reais = indice;
    }

    /* Caso o arquivo CSV tenha 0 registros válidos, libera memória e retorna NULL */
    if (indice == 0) {
        free(vetor);
        return NULL;
    }

    /* ------------------------------------------------------------------------
     * EXPANSÃO AUTOMÁTICA DE SEGURANÇA:
     * Se a planilha do Excel fornecida tiver menos linhas que o teste atual
     * (ex: planilha de 5.000 linhas para testar 300.000), replicamos os dados
     * lidos variando o ID de forma determinística para completar os N itens.
     * ------------------------------------------------------------------------ */
    if (indice < quantidade) {
        int base = indice;
        for (int i = base; i < quantidade; i++) {
            vetor[i] = vetor[i % base];
            vetor[i].id = vetor[i % base].id + ((i / base) * 1000000);
        }
    }

    return vetor;
}

/**
 * @brief Gera um arquivo CSV com até 300.000 registros simulando uma planilha
 *        exportada do Excel.
 */
int gerar_csv_teste(const char *caminho_arquivo, int quantidade, ModoGeracao modo) {
    if (caminho_arquivo == NULL || quantidade <= 0) {
        return -1;
    }

    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "[ERRO] Nao foi possivel criar o arquivo: %s\n", caminho_arquivo);
        return -1;
    }

    const char *categorias[] = {
        "Eletronicos", "Informatica", "Livros", "Engenharia",
        "Laboratorio", "Redes", "Hardware", "Software"
    };
    const int total_categorias = 8;

    /* Escreve o cabeçalho padrão de planilha Excel (separado por ';') */
    fprintf(arquivo, "ID;Nome;Categoria;Valor;Data\n");

    /* Semente fixa (42) para que os testes sejam 100% reprodutíveis */
    unsigned int estado_rng = 42u;

    for (int i = 1; i <= quantidade; i++) {
        int id_gerado;

        /* Gerador pseudo-aleatório rápido de 32 bits (Xorshift32)
         * Evita a limitação do rand() do Windows cujo RAND_MAX é apenas 32.767 */
        estado_rng ^= estado_rng << 13;
        estado_rng ^= estado_rng >> 17;
        estado_rng ^= estado_rng << 5;

        if (modo == MODO_CRESCENTE) {
            id_gerado = i;
        } else if (modo == MODO_DECRESCENTE) {
            id_gerado = (quantidade - i) + 1;
        } else {
            /* MODO_ALEATORIO: chaves distribuídas entre 1 e 999.999 */
            id_gerado = (int)(estado_rng % 999999u) + 1;
        }

        const char *cat = categorias[estado_rng % total_categorias];
        double valor = ((double)(estado_rng % 100000u) / 100.0) + 10.0;
        int dia = (int)(estado_rng % 28u) + 1;
        int mes = (int)((estado_rng >> 5) % 12u) + 1;

        fprintf(arquivo, "%d;Item_Planilha_%06d;%s;%.2f;2026-%02d-%02d\n",
                id_gerado, i, cat, valor, mes, dia);
    }

    fclose(arquivo);
    return 0;
}

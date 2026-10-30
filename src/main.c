/**
 * @file main.c
 * @brief Programa principal e orquestrador de benchmarks do Merge Sort Recursivo
 *        para 30, 300, 3.000, 30.000 e 300.000 dados oriundos do Excel (CSV).
 *
 * Disciplina: Estrutura de Dados — 4ª Fase de Ciência da Computação
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"
#include "metricas.h"
#include "mergesort.h"
#include "leitor_csv.h"

/* Quantidades oficiais exigidas na especificação do trabalho */
#define TOTAL_ESCALAS 5
static const int ESCALAS_TESTE[TOTAL_ESCALAS] = {
    30,
    300,
    3000,
    30000,
    300000
};

#define CAMINHO_RESULTADOS_CSV "results/resultados_benchmark.csv"

/**
 * @brief Garante que exista um arquivo CSV válido antes de iniciar os testes.
 *        Caso o aluno ainda não tenha colocado a planilha do Excel na pasta
 *        'data/', gera automaticamente uma base de 300.000 registros.
 */
static int garantir_arquivo_dados(const char *caminho_csv) {
    if (arquivo_csv_existe(caminho_csv)) {
        return 0;
    }

    printf("\n[AVISO] Arquivo '%s' ainda nao encontrado.\n", caminho_csv);
    printf("[INFO]  Gerando base sintetica de %d registros (Modo Aleatorio)...\n", TOTAL_MAXIMO_DADOS);

    if (gerar_csv_teste(caminho_csv, TOTAL_MAXIMO_DADOS, MODO_ALEATORIO) != 0) {
        fprintf(stderr, "[ERRO] Nao foi possivel gerar o arquivo de dados '%s'.\n", caminho_csv);
        return -1;
    }

    printf("[OK]    Arquivo '%s' gerado com sucesso!\n\n", caminho_csv);
    return 0;
}

/**
 * @brief Executa a bateria completa para 30, 300, 3.000, 30.000 e 300.000 dados,
 *        exibindo a quantidade e o tempo de cada execução em tabela formatada.
 */
static void executar_bateria_completa(const char *caminho_csv) {
    if (garantir_arquivo_dados(caminho_csv) != 0) {
        return;
    }

    Metricas resultados[TOTAL_ESCALAS];
    int lidos_reais = 0;

    printf("\n========================================================================================================\n");
    printf("        BENCHMARK OFICIAL: MERGE SORT RECURSIVO EM C (30, 300, 3.000, 30.000 e 300.000 DADOS)\n");
    printf("========================================================================================================\n");
    printf(" Arquivo de Entrada : %s\n", caminho_csv);
    printf(" Metodo             : Merge Sort Recursivo (Divisao e Conquista - Estavel)\n");
    printf(" Complexidade       : Tempo Theta(N log2 N) | Espaco O(N) na Heap\n");
    printf("--------------------------------------------------------------------------------------------------------\n");

    imprimir_cabecalho_tabela_metricas();

    for (int i = 0; i < TOTAL_ESCALAS; i++) {
        int n = ESCALAS_TESTE[i];
        Cronometro cron;

        /* 1. Carrega N registros desordenados na Heap (FORA do cronômetro!) */
        Registro *vetor = carregar_dados_csv(caminho_csv, n, &lidos_reais);
        if (vetor == NULL) {
            fprintf(stderr, "[ERRO] Falha ao carregar %d registros do arquivo CSV.\n", n);
            return;
        }

        inicializar_metricas(&resultados[i], n);

        /* 2. Mede EXCLUSIVAMENTE o tempo do Merge Sort Recursivo */
        iniciar_timer(&cron);
        int status_exec = merge_sort(vetor, n, &resultados[i]);
        parar_timer(&cron, &resultados[i]);

        /* 3. Audita se os N elementos ficaram realmente ordenados */
        if (status_exec == 0) {
            resultados[i].ordenado_com_sucesso = verificar_ordenacao(vetor, n);
        }

        /* 4. Exibe a linha com Quantidade (N), Tempo (ms e s) e Métricas */
        imprimir_linha_metricas(&resultados[i]);

        /* 5. Libera a memória principal desta bateria */
        free(vetor);
    }

    imprimir_rodape_tabela_metricas();

    /* Exibe resumo de escalabilidade e chamadas recursivas */
    printf("\nResumo da Arvore de Recursao e Escalabilidade:\n");
    for (int i = 0; i < TOTAL_ESCALAS; i++) {
        printf("  -> N = %7d dados | Tempo: %9.4f ms (%10.6f s) | Chamadas Recursivas: %7llu\n",
               resultados[i].quantidade_dados,
               resultados[i].tempo_ms,
               resultados[i].tempo_s,
               resultados[i].chamadas_recursivas);
    }

    if (exportar_metricas_csv(CAMINHO_RESULTADOS_CSV, resultados, TOTAL_ESCALAS) == 0) {
        printf("\n[OK] Tabela de resultados exportada para: %s (pronta para abrir no Excel)\n",
               CAMINHO_RESULTADOS_CSV);
    }
    printf("========================================================================================================\n");
}

/**
 * @brief Executa a ordenação para uma quantidade escolhida e imprime os
 *        primeiros e últimos registros ANTES e DEPOIS da ordenação.
 *        Ideal para demonstrar visualmente ao professor que a ordenação funciona.
 */
static void demonstrar_antes_depois(const char *caminho_csv, int n) {
    if (garantir_arquivo_dados(caminho_csv) != 0) {
        return;
    }

    int lidos_reais = 0;
    Registro *vetor = carregar_dados_csv(caminho_csv, n, &lidos_reais);
    if (vetor == NULL) {
        return;
    }

    int amostra = (n < 5) ? n : 5;

    printf("\n-------------------------------------------------------------------------------------------------\n");
    printf(" DEMONSTRACAO VISUAL — N = %d DADOS (Mostrando os %d primeiros ANTES de ordenar)\n", n, amostra);
    printf("-------------------------------------------------------------------------------------------------\n");
    imprimir_cabecalho_registro();
    for (int i = 0; i < amostra; i++) {
        imprimir_registro(&vetor[i]);
    }
    printf("+------------+------------------------------+--------------------+--------------+------------+\n");

    Metricas m;
    Cronometro cron;
    inicializar_metricas(&m, n);

    iniciar_timer(&cron);
    merge_sort(vetor, n, &m);
    parar_timer(&cron, &m);
    m.ordenado_com_sucesso = verificar_ordenacao(vetor, n);

    printf("\n-------------------------------------------------------------------------------------------------\n");
    printf(" DEPOIS DO MERGE SORT RECURSIVO (%d primeiros e %d ultimos de N = %d)\n", amostra, amostra, n);
    printf("-------------------------------------------------------------------------------------------------\n");
    imprimir_cabecalho_registro();
    for (int i = 0; i < amostra; i++) {
        imprimir_registro(&vetor[i]);
    }
    if (n > amostra * 2) {
        printf("|    ...     |             ...              |        ...         |     ...      |    ...     |\n");
        for (int i = n - amostra; i < n; i++) {
            imprimir_registro(&vetor[i]);
        }
    }
    printf("+------------+------------------------------+--------------------+--------------+------------+\n");

    printf("\nResumo da Execucao:\n");
    printf("  - Quantidade Ordenada : %d registros\n", m.quantidade_dados);
    printf("  - Tempo de Execucao   : %.4f ms (%.6f segundos)\n", m.tempo_ms, m.tempo_s);
    printf("  - Comparacoes         : %llu\n", m.comparacoes);
    printf("  - Movimentacoes       : %llu\n", m.movimentacoes);
    printf("  - Chamadas Recursivas : %llu\n", m.chamadas_recursivas);
    printf("  - Status Auditoria    : %s\n", m.ordenado_com_sucesso ? "[OK] 100% Ordenado" : "[ERRO]");

    free(vetor);
}

/**
 * @brief Menu para gerar bases de teste em diferentes cenários (Aleatório,
 *        Crescente/Melhor Caso, Decrescente/Pior Caso).
 */
static void menu_gerar_base_csv(const char *caminho_csv) {
    int opcao_modo = 1;
    printf("\nEscolha o cenario da planilha CSV de %d registros:\n", TOTAL_MAXIMO_DADOS);
    printf("  [1] Aleatorio   (Caso Medio - Padrao)\n");
    printf("  [2] Crescente   (Melhor Caso - Ja Ordenado)\n");
    printf("  [3] Decrescente (Pior Caso - Ordem Inversa)\n");
    printf("Opcao: ");

    if (scanf("%d", &opcao_modo) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        opcao_modo = 1;
    }

    ModoGeracao modo = MODO_ALEATORIO;
    if (opcao_modo == 2) modo = MODO_CRESCENTE;
    if (opcao_modo == 3) modo = MODO_DECRESCENTE;

    printf("[INFO] Gerando '%s' com %d registros...\n", caminho_csv, TOTAL_MAXIMO_DADOS);
    if (gerar_csv_teste(caminho_csv, TOTAL_MAXIMO_DADOS, modo) == 0) {
        printf("[OK] Base gerada com sucesso!\n");
    }
}

/**
 * @brief Ponto de entrada do programa.
 *        Aceita '--auto' na linha de comando para execução direta sem menu,
 *        ou abre o menu interativo para uso em laboratório/apresentação.
 */
int main(int argc, char *argv[]) {
    char caminho_csv[256];
    snprintf(caminho_csv, sizeof(caminho_csv), "%s", CAMINHO_CSV_PADRAO);

    /* Modo não-interativo (--auto): executa a bateria completa e encerra */
    if (argc > 1 && strcmp(argv[1], "--auto") == 0) {
        if (argc > 2) {
            snprintf(caminho_csv, sizeof(caminho_csv), "%s", argv[2]);
        }
        executar_bateria_completa(caminho_csv);
        demonstrar_antes_depois(caminho_csv, 30);
        return 0;
    }

    int opcao = -1;
    while (opcao != 0) {
        printf("\n======================================================================\n");
        printf("   SISTEMA MERGE SORT RECURSIVO — ESTRUTURA DE DADOS (4a FASE CC)\n");
        printf("======================================================================\n");
        printf(" Arquivo CSV atual: %s\n", caminho_csv);
        printf("----------------------------------------------------------------------\n");
        printf(" [1] Executar Bateria Completa (30, 300, 3.000, 30.000 e 300.000)\n");
        printf(" [2] Inspecionar Antes/Depois (Demonstracao Visual para o Professor)\n");
        printf(" [3] Gerar/Recriar Planilha CSV de Teste (300.000 linhas)\n");
        printf(" [4] Informar outro caminho de arquivo CSV do Excel\n");
        printf(" [0] Sair\n");
        printf("----------------------------------------------------------------------\n");
        printf(" Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1:
                executar_bateria_completa(caminho_csv);
                break;

            case 2: {
                int escolha_n = 30;
                printf("\nDigite a quantidade desejada (ex: 30, 300, 3000, 30000, 300000): ");
                if (scanf("%d", &escolha_n) != 1 || escolha_n <= 0 || escolha_n > TOTAL_MAXIMO_DADOS) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    escolha_n = 30;
                }
                demonstrar_antes_depois(caminho_csv, escolha_n);
                break;
            }

            case 3:
                menu_gerar_base_csv(caminho_csv);
                break;

            case 4:
                printf("\nDigite o caminho do arquivo CSV (ex: data/minha_planilha.csv): ");
                if (scanf("%255s", caminho_csv) != 1) {
                    snprintf(caminho_csv, sizeof(caminho_csv), "%s", CAMINHO_CSV_PADRAO);
                }
                break;

            case 0:
                printf("\nEncerrando o programa. Bom trabalho e boa apresentacao!\n");
                break;

            default:
                printf("\n[AVISO] Opcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}

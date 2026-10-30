/**
 * @file metricas.c
 * @brief Implementação do cronômetro de alta precisão e dos relatórios de
 *        quantidade e tempo de execução das baterias do Merge Sort.
 */

#include <stdio.h>
#include <time.h>
#include "metricas.h"

/* Inclusão condicional da API de alta resolução do Windows */
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

/**
 * @brief Inicializa e zera todos os campos da estrutura Metricas.
 */
void inicializar_metricas(Metricas *m, int quantidade) {
    if (m == NULL) {
        return;
    }
    m->quantidade_dados    = quantidade;
    m->tempo_ms            = 0.0;
    m->tempo_s             = 0.0;
    m->comparacoes         = 0ULL;
    m->movimentacoes       = 0ULL;
    m->chamadas_recursivas = 0ULL;
    m->ordenado_com_sucesso = 0;
}

/**
 * @brief Captura o instante inicial com precisão de microssegundos/nanossegundos.
 */
void iniciar_timer(Cronometro *c) {
    if (c == NULL) {
        return;
    }

#ifdef _WIN32
    LARGE_INTEGER freq, ini;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&ini);
    c->frequencia      = (long long)freq.QuadPart;
    c->inicio_ticks    = (long long)ini.QuadPart;
    c->inicio_fallback = 0.0;
#else
    c->frequencia      = 0;
    c->inicio_ticks    = 0;
    c->inicio_fallback = (double)clock() / CLOCKS_PER_SEC;
#endif
}

/**
 * @brief Calcula a diferença entre o instante final e o inicial, convertendo
 *        para milissegundos (ms) e segundos (s).
 */
void parar_timer(Cronometro *c, Metricas *m) {
    if (c == NULL || m == NULL) {
        return;
    }

#ifdef _WIN32
    LARGE_INTEGER fim;
    QueryPerformanceCounter(&fim);
    long long delta_ticks = (long long)fim.QuadPart - c->inicio_ticks;

    if (c->frequencia > 0) {
        m->tempo_s  = (double)delta_ticks / (double)c->frequencia;
        m->tempo_ms = m->tempo_s * 1000.0;
    }
#else
    double fim_s = (double)clock() / CLOCKS_PER_SEC;
    m->tempo_s   = fim_s - c->inicio_fallback;
    m->tempo_ms  = m->tempo_s * 1000.0;
#endif
}

/**
 * @brief Imprime o topo da tabela comparativa das 5 baterias (30 a 300.000).
 */
void imprimir_cabecalho_tabela_metricas(void) {
    printf("+----------------+---------------+---------------+-----------------+-----------------+-------------------+\n");
    printf("| Quantidade (N) |  Tempo (ms)   |   Tempo (s)   |   Comparacoes   |  Movimentacoes  | Status Validacao  |\n");
    printf("+----------------+---------------+---------------+-----------------+-----------------+-------------------+\n");
}

/**
 * @brief Imprime uma linha formatada com a quantidade de dados, tempos e métricas.
 */
void imprimir_linha_metricas(const Metricas *m) {
    if (m == NULL) {
        return;
    }

    const char *status_str = m->ordenado_com_sucesso ? "[OK] Ordenado" : "[ERRO] Falha";

    printf("| %14d | %10.4f ms | %11.6f s | %15llu | %15llu | %-17s |\n",
           m->quantidade_dados,
           m->tempo_ms,
           m->tempo_s,
           m->comparacoes,
           m->movimentacoes,
           status_str);
}

/**
 * @brief Imprime a borda inferior da tabela de métricas.
 */
void imprimir_rodape_tabela_metricas(void) {
    printf("+----------------+---------------+---------------+-----------------+-----------------+-------------------+\n");
}

/**
 * @brief Salva os resultados das execuções em um arquivo .csv pronto para
 *        ser aberto no Excel e gerar gráficos de Tempo x Quantidade.
 */
int exportar_metricas_csv(const char *caminho_arquivo, const Metricas *lista, int total_baterias) {
    if (caminho_arquivo == NULL || lista == NULL || total_baterias <= 0) {
        return -1;
    }

    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (arquivo == NULL) {
        return -1;
    }

    /* Cabeçalho do CSV separado por ponto-e-vírgula (padrão Excel PT-BR) */
    fprintf(arquivo, "Quantidade_N;Tempo_ms;Tempo_s;Comparacoes;Movimentacoes;Chamadas_Recursivas;Status\n");

    for (int i = 0; i < total_baterias; i++) {
        fprintf(arquivo, "%d;%.6f;%.8f;%llu;%llu;%llu;%s\n",
                lista[i].quantidade_dados,
                lista[i].tempo_ms,
                lista[i].tempo_s,
                lista[i].comparacoes,
                lista[i].movimentacoes,
                lista[i].chamadas_recursivas,
                lista[i].ordenado_com_sucesso ? "OK" : "ERRO");
    }

    fclose(arquivo);
    return 0;
}

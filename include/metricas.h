/**
 * @file metricas.h
 * @brief Estruturas e funções para medição de tempo de alta precisão e
 *        contabilização de métricas do algoritmo Merge Sort Recursivo.
 *
 * POR QUE USAR TIMER DE ALTA RESOLUÇÃO?
 * Em C, ordenar 30 ou 300 elementos leva poucos microssegundos (ex: 0.003 ms).
 * A função padrão clock() da <time.h> no Windows possui resolução de ~15 ms,
 * o que exibiria "0.000000 s" para 30, 300 e 3.000 dados. Por isso utilizamos
 * o contador de alta frequência do hardware (QueryPerformanceCounter no Windows
 * e clock_gettime no Linux/macOS).
 */

#ifndef METRICAS_H
#define METRICAS_H

/**
 * @brief Armazena todas as métricas coletadas em uma execução do Merge Sort.
 *
 * Usamos 'unsigned long long' (inteiro de 64 bits) para os contadores porque,
 * para 300.000 registros, o número de comparações e movimentações passa da
 * casa dos milhões.
 */
typedef struct {
    int quantidade_dados;                   /* Quantidade N ordenada (30 a 300.000) */
    double tempo_ms;                        /* Tempo de execução em milissegundos   */
    double tempo_s;                         /* Tempo de execução em segundos        */
    unsigned long long comparacoes;         /* Total de comparações entre chaves    */
    unsigned long long movimentacoes;       /* Total de cópias de registros         */
    unsigned long long chamadas_recursivas; /* Total de chamadas recursivas (2N-1)  */
    int ordenado_com_sucesso;               /* 1 = Validado [OK], 0 = Falha [ERRO]  */
} Metricas;

/**
 * @brief Estrutura opaca para guardar o instante inicial do cronômetro.
 */
typedef struct {
    long long inicio_ticks;
    long long frequencia;
    double inicio_fallback;
} Cronometro;

/**
 * @brief Zera todos os contadores da estrutura Metricas antes de um novo teste.
 *
 * @param m Ponteiro para a estrutura de métricas.
 * @param quantidade Quantidade de elementos (N) que será processada.
 */
void inicializar_metricas(Metricas *m, int quantidade);

/**
 * @brief Dispara o cronômetro de alta precisão.
 *        Deve ser chamado IMEDIATAMENTE ANTES de merge_sort().
 *
 * @param c Ponteiro para o cronômetro.
 */
void iniciar_timer(Cronometro *c);

/**
 * @brief Para o cronômetro e grava o tempo decorrido (ms e s) em Metricas.
 *        Deve ser chamado IMEDIATAMENTE APÓS merge_sort().
 *
 * @param c Ponteiro para o cronômetro iniciado.
 * @param m Ponteiro para a estrutura de métricas onde o tempo será salvo.
 */
void parar_timer(Cronometro *c, Metricas *m);

/**
 * @brief Imprime o cabeçalho da tabela comparativa de tempos no terminal.
 */
void imprimir_cabecalho_tabela_metricas(void);

/**
 * @brief Imprime uma linha de resultado (Quantidade, Tempo, Métricas e Status).
 *
 * @param m Ponteiro constante para as métricas da bateria executada.
 */
void imprimir_linha_metricas(const Metricas *m);

/**
 * @brief Imprime o rodapé da tabela comparativa de tempos no terminal.
 */
void imprimir_rodape_tabela_metricas(void);

/**
 * @brief Exporta o vetor de métricas das 5 baterias (30 a 300.000) para um
 *        arquivo CSV, facilitando a geração de gráficos no Excel.
 *
 * @param caminho_arquivo Caminho do arquivo de saída (ex: "results/resultados_benchmark.csv").
 * @param lista Vetor contendo as métricas coletadas.
 * @param total_baterias Quantidade de baterias no vetor (ex: 5).
 * @return int Retorna 0 em caso de sucesso ou -1 em caso de erro ao abrir arquivo.
 */
int exportar_metricas_csv(const char *caminho_arquivo, const Metricas *lista, int total_baterias);

#endif /* METRICAS_H */

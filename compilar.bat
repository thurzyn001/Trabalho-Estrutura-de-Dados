@echo off
REM ============================================================================
REM Script de Compilacao Rapida no Windows (GCC / MinGW)
REM Trabalho de Estrutura de Dados - 4a Fase Ciencia da Computacao
REM ============================================================================

if not exist "data" mkdir data
if not exist "results" mkdir results

echo [INFO] Compilando projeto Merge Sort Recursivo em C...
gcc -Wall -Wextra -std=c99 -O2 src/main.c src/registro.c src/mergesort.c src/metricas.c src/leitor_csv.c -Iinclude -o mergesort.exe

if %ERRORLEVEL% NEQ 0 (
    echo [ERRO] Falha na compilacao! Verifique se o GCC esta instalado no PATH.
    exit /b 1
)

echo [OK] Compilacao concluida com sucesso: mergesort.exe
echo.

if "%1"=="--auto" (
    .\mergesort.exe --auto
) else (
    echo Para executar o menu interativo, digite: .\mergesort.exe
    echo Para rodar a bateria completa direto, digite: .\mergesort.exe --auto
)

@echo off
REM Script de compilação para o compilador P- (Windows)
REM Uso: build.bat

setlocal enabledelayedexpansion

echo === Compilando Compilador P- ===
echo.

REM Verifica se estamos no diretório correto
if not exist "src\parser.y" (
    echo ERRO: Execute este script do diretorio trabalho-compiladores
    exit /b 1
)

REM Cria diretório build se não existir
if not exist "build" mkdir build
cd build

REM Verifica dependências
where bison >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERRO: bison nao encontrado.
    echo Instale WinFlexBison de: https://github.com/lexxmark/winflexbison
    echo Ou adicione bison ao PATH do sistema
    exit /b 1
)

where flex >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERRO: flex nao encontrado.
    echo Instale WinFlexBison de: https://github.com/lexxmark/winflexbison
    echo Ou adicione flex ao PATH do sistema
    exit /b 1
)

REM Detecta compilador
set COMPILER=gcc
where gcc >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    where clang >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        set COMPILER=clang
    ) else (
        where cl >nul 2>&1
        if %ERRORLEVEL% EQU 0 (
            set COMPILER=cl
            echo AVISO: Usando MSVC. Pode ser necessario ajustar flags de compilacao.
        ) else (
            echo ERRO: Nenhum compilador C encontrado
            echo Instale MinGW-w64, Clang ou MSVC
            exit /b 1
        )
    )
)

echo 1. Gerando parser e scanner...
bison -d -o parser.c ..\src\parser.y
if %ERRORLEVEL% NEQ 0 (
    echo    ERRO: Falha ao gerar parser
    exit /b 1
)

flex -o scanner.c ..\src\scanner.l
if %ERRORLEVEL% NEQ 0 (
    echo    ERRO: Falha ao gerar scanner
    exit /b 1
)
echo    Parser e scanner gerados
echo.

echo 2. Compilando projeto...
echo    Compilador: %COMPILER%
echo.

REM Compilação com GCC/Clang
if "%COMPILER%"=="gcc" (
    gcc -I ..\include -I . -std=c11 ..\src\ast.c ..\src\symbol_table.c ..\src\semantic.c ..\src\utils.c parser.c scanner.c ..\src\main.c -o main.exe -ly
) else if "%COMPILER%"=="clang" (
    clang -I ..\include -I . -std=c11 ..\src\ast.c ..\src\symbol_table.c ..\src\semantic.c ..\src\utils.c parser.c scanner.c ..\src\main.c -o main.exe -ly
) else (
    REM MSVC - pode precisar de ajustes
    cl /I ..\include /I . /TC /std:c11 ..\src\ast.c ..\src\symbol_table.c ..\src\semantic.c ..\src\utils.c parser.c scanner.c ..\src\main.c /Fe:main.exe /link
)

if %ERRORLEVEL% EQU 0 (
    echo    Compilacao concluida com sucesso!
    echo.
    echo 3. Executavel criado: build\main.exe
    echo.
    echo Para executar:
    echo   cd build
    echo   main.exe ..\codigo.txt
    echo.
    echo Ou teste agora:
    echo   main.exe ..\codigo.txt
) else (
    echo    Erro na compilacao
    echo.
    echo DICAS:
    echo - Se usar MinGW, certifique-se de que bison e flex estao no PATH
    echo - No Windows, pode ser necessario usar -lfl ao inves de -ly
    echo - Verifique se todas as dependencias estao instaladas
    exit /b 1
)

endlocal


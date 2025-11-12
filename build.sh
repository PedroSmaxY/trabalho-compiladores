#!/bin/bash
# Script de compilação simples para o compilador P-

set -e

echo "=== Compilando Compilador P- ==="
echo ""

# Verifica se estamos no diretório correto
if [ ! -f "src/parser.y" ]; then
    echo "ERRO: Execute este script do diretório trabalho-compiladores"
    exit 1
fi

# Cria diretório build se não existir
mkdir -p build
cd build

# Verifica dependências
if ! command -v bison &> /dev/null; then
    echo "ERRO: bison não encontrado. Instale com: brew install bison"
    exit 1
fi

if ! command -v flex &> /dev/null; then
    echo "ERRO: flex não encontrado. Instale com: brew install flex"
    exit 1
fi

# Detecta compilador
COMPILER="gcc"
if ! command -v gcc &> /dev/null; then
    if command -v clang &> /dev/null; then
        COMPILER="clang"
    else
        echo "ERRO: Nenhum compilador C encontrado"
        exit 1
    fi
fi

echo "1. Gerando parser e scanner..."
bison -d -o parser.c ../src/parser.y
flex -o scanner.c ../src/scanner.l
echo "   ✓ Parser e scanner gerados"
echo ""

echo "2. Compilando projeto..."
$COMPILER -I ../include -I . -std=c11 \
  ../src/ast.c ../src/symbol_table.c ../src/semantic.c ../src/utils.c \
  parser.c scanner.c ../src/main.c -o main -ly

if [ $? -eq 0 ]; then
    echo "   ✓ Compilação concluída com sucesso!"
    echo ""
    echo "3. Executável criado: build/main"
    echo ""
    echo "Para executar:"
    echo "  cd build"
    echo "  ./main ../codigo.txt"
    echo ""
    echo "Ou teste agora:"
    echo "  ./main ../codigo.txt"
else
    echo "   ✗ Erro na compilação"
    exit 1
fi


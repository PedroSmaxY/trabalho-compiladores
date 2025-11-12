# Compilador da linguagem P\-

Projeto da matéria de Compiladores (6º semestre). Repositório evolutivo: inicia com analisador léxico e seguirá para parser, AST, análise semântica e geração de código.

## Estado Atual
- Lexer em C (`src/lexer.c`, `include/lexer.h`)
- CLI simples (`src/main.c`) imprime tokens de um arquivo
- Testes unitários iniciais em `tests/` (ver seção Ambiente)

## Estrutura
- `src/` código principal
- `include/` headers
- `tests/` testes (somente Linux & MacOS no momento)
- `docs/` especificações
- `codigo.txt` exemplo
- `CMakeLists.txt` build

## Build e Execução

### Opção 1: Script Automático (Mais Fácil - Não Requer CMake)

**Linux/macOS:**
```bash
./build.sh
```

**Windows:**
```cmd
build.bat
```

### Opção 2: Compilação Manual (Não Requer CMake)

```bash
mkdir -p build
cd build
bison -d -o parser.c ../src/parser.y
flex -o scanner.c ../src/scanner.l
gcc -I ../include -I . -std=c11 \
  ../src/ast.c ../src/symbol_table.c ../src/semantic.c ../src/utils.c \
  parser.c scanner.c ../src/main.c -o main -ly
./main ../codigo.txt
```

### Opção 3: CMake (Opcional - Requer CMake instalado)

```bash
mkdir build
cd build
cmake ..          # gera Makefiles
make              # ou: make -j
./main            # executa binário gerado aqui dentro
```

## Testes (apenas Linux & MacOS)
```bash
mkdir build
cd build
cmake ..          # gera Makefiles
make              # ou: make -j
./test_lexer      # executa binário gerado aqui dentro

# ou gcc -Iinclude src/lexer.c tests/test_lexer.c -o tests_lexer
# ./test_lexer
```
Observação: os testes em `tests/` estão configurados/validados apenas em ambiente Linux & MacOS;

## Convenções
- C11
- Erros com linha e coluna
- Código prioriza clareza

## Referências
Arquivos em `docs/`.

## Licença
Ver `LICENSE`.
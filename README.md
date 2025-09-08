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

## Build e Execução (CMake)
```bash
mkdir build
cd build
cmake ..          # gera Makefiles
make              # ou: make -j
./main            # executa binário gerado aqui dentro
```

## Build e Execução (compilação direta)
```bash
gcc -Iinclude src/lexer.c src/main.c -o main
./main            # usa `codigo.txt` na raiz
./main caminho/arquivo.p-   # para outro arquivo
```
(Use `gcc` ou `clang` conforme disponível.)

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
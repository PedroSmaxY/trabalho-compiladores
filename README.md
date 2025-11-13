# Compilador da linguagem P\-

Projeto da matéria de Compiladores (6º semestre). Repositório evolutivo: inicia com analisador léxico e seguirá para parser, AST, análise semântica e geração de código.

## Estado Atual
- Analisador léxico (Flex)
- Analisador sintático (Bison)
- Análise semântica (tabela de símbolos, verificação de tipos)
- Construção de AST

## Estrutura
- `src/` código principal
- `include/` headers
- `tests/` testes (funcionam em Linux, macOS e Windows)
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

## Testes

### Executando Testes Semânticos

Após compilar o projeto (usando qualquer uma das opções acima), você pode testar o compilador com os arquivos de teste:

**Linux/macOS:**
```bash
cd build
./main ../tests/teste_semantico_correto.p-    # Teste sem erros
./main ../tests/teste_semantico_erros.p-     # Teste com múltiplos erros
./main ../codigo.p-                            # Seu próprio arquivo
```

**Windows:**
```cmd
cd build
main.exe ..\tests\teste_semantico_correto.p-    # Teste sem erros
main.exe ..\tests\teste_semantico_erros.p-     # Teste com múltiplos erros
main.exe ..\codigo.p-                           # Seu próprio arquivo
```

**Nota:** 
- No Windows, o executável é `main.exe` e os caminhos usam `\` ao invés de `/`.
- O compilador agora lê arquivos `.p-` por padrão. Os arquivos de teste foram convertidos para a extensão `.p-`.
- Cada arquivo analisado gera automaticamente um arquivo `.semantico` com os resultados da análise.

### Arquivos de Teste Disponíveis

- `teste_semantico_correto.p-` - Código sem erros que testa todas as regras semânticas
- `teste_semantico_erros.p-` - Código com múltiplos erros para testar detecção de erros

**Nota:** Os arquivos de teste usam a extensão `.p-` conforme a linguagem. Cada teste gera automaticamente um arquivo `.semantico` com os resultados.

### O que os Testes Verificam

Os testes semânticos verificam:
- Declaração de variáveis antes do uso
- Detecção de declarações duplicadas
- Verificação de tipos em atribuições
- Verificação de tipos em expressões
- Verificação de condições booleanas em `se`, `enquanto`, `repita`
- Verificação de variáveis em `ler` e `mostrar`
- Promoção de tipos (inteiro → real)
- Compatibilidade de tipos em operações

## Convenções
- C11
- Erros com linha e coluna
- Código prioriza clareza

## Referências
Arquivos em `docs/`.

## Licença
Ver `LICENSE`.
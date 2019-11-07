# INTRO

IA desenvolvida para resolver o jogo snaze.

# Compilando

Nota: é necessario ter instalado o cmake.

Certifique-se de estar dentro do diretorio snaze.

## Compilando o executável

Para compilar o executável siga os passos abaixo:

1. `mkdir build`: cria um diretorio chamado build.
2. `cd build`: entra no diretorio build.
3. `cmake -G "Unix Makefiles" ..`: pede para o cmake criar Makefiles baseado no scrip CMakeLits.txt que esta no diretorio pai do build.
4. `make`: inicia o processo de compilação do executavel.

Pronto o executavel foi criado.

## Rodando o executável

```
$ ./snaze arquivo_com_informações_dos_levels
``` 

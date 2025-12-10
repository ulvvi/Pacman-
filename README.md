PACMAN+

[![Feito com RayLib](https://img.shields.io/badge/Made%20with-RayLib-000000.svg?style=flat&logo=raylib)](https://www.raylib.com/)
[![Linguagem](https://img.shields.io/badge/Language-C-555555.svg?style=flat&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))

## 📌 O Projeto

**PACMAN+** é um Remake do clássico Pac-Man, desenvolvido em **C** utilizando a biblioteca **RayLib**, que busca recriar o espírito do jogo original, mas com adições especiais.

Este projeto foi criado para o **Projeto Final da disciplina de Programação II (2025.2)** da **UFRJ**

### 🕹️ Jogabilidade

Movimente-se usando as **setas do teclado** (↑, ↓, ←, →), e **espaço** para utilizar as frutas coletadas (powerups), o jogo possuí 3 fases que podem ser alteradas por meio de arquivos .txt

---

## 🛠️ Compilação e Execução

### 💻 Windows

#### 1. Instalação de Dependências

1.  **Instale o MSYS2:** Faça o download e instale o MSYS2.
2.  **Abra o MSYS2** e execute os comandos para instalar o compilador (GCC) e a biblioteca RayLib:

    ```bash
    pacman -S mingw-w64-ucrt-x86_64-gcc
    pacman -S mingw-w64-ucrt-x86_64-raylib
    ```

#### 2. Compilar e Rodar

1.  Clone o repositório na pasta `home` do MSYS2.
2.  Navegue até o diretório dos arquivos-fonte:
    ```bash
    cd pacman- 
    ```
3.  Execute o `gcc` e, em seguida, rode o executável:

    ```bash
    # Compila o código C para o executável main.exe
    gcc main.c -o main -lraylib -lwinmm -lgdi32 -lopengl32
    
    # Executa o jogo
    ./main.exe
    ```
    
### 💻 LINUX

#### 1. Instalação de Dependências
Para Ubuntu, Debian e distribuições baseadas em apt:
    ```bash
    sudo apt update
    sudo apt install build-essential
    sudo apt install libraylib-dev
    ```

#### 2. Compilar e Rodar

1.  Clone o repositório
2.  Navegue até o diretório dos arquivos-fonte:
    ```bash
    cd pacman- 
    ```
3.  Execute o `gcc` e, em seguida, rode o executável:

    ```bash
    # Compila o código C para o executável main.exe
    gcc main.c -o main -lraylib -lm -lpthread -ldl -lrt -lX11
    
    # Executa o jogo
    ./main.exe
    ```

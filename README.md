# Trabalho-Gerador-de-Thumbnails
Este trabalho foi realizado na matéria de Algoritmos e Programação II, com o objetivo de realizar um criador de thumbnails, em qualquer tamanho de imagem.
Realizado por: Wesley Luza e Jean Carlos Pegoraro.

## Objetivo:
Criar, em linguagem C, um método que modifica uma imagem, podendo converte-lá em qualquer tamanho, podendo deixar como thumbnail.

## Funcionalidades
✅ Gera thumbnails automaticamente a partir de uma pasta de imagens.  
✅ Suporta formatos `.jpg`, `.jpeg` e `.png`.  
✅ Permite definir tamanho fixo (ex: `128`) ou largura e altura separadas (`50x100`).  
✅ Ignora imagens já geradas (prefixo `thumb_`).  
✅ Utiliza interpolação bilinear para qualidade melhor no redimensionamento.  
✅ Cria as miniaturas na mesma pasta das imagens originais.

## Como Compilar:
Abra um terminal na pasta do projeto e rode:
```
- gcc attGeradorThumbnails.c -o gerador -lm
```
> Os arquivos `stb_image.h` e `stb_image_write.h` (biblioteca *stb*) devem estar na pasta do projeto. Faça o download em: https://github.com/nothings/stb

## Requisitos para Compilação

- GCC (ou outro compilador C compatível com POSIX). No Windows recomendo usar o MinGW ou WSL.
- Arquivos `stb_image.h` e `stb_image_write.h` no mesmo diretório do código.

## Estrutura do projeto
- AttGeradorThumbnails/
- ├── attGeradorThumbnails.c = Programa que gera as thumbnails.
- ├── stb_image.h = Repositório usado para compilar o programa.
- ├── stb_image_write.h = Repositório usado para compilar o programa.
- └── imagens/ = pasta de exemplo com suas imagens
- ├── imagens usadas como exemplo (.jpg)

## Imagens do Programa:
Antes de compilar:
![boathouse-9871057_1280](https://github.com/user-attachments/assets/5eb9f831-d994-452c-a152-b72fa62c824d)

Compilando:
<img width="1115" height="174" alt="image" src="https://github.com/user-attachments/assets/70631daa-9f97-48c0-aa68-47b701592529" />

Depois de Compilar:
<img width="50" height="100" alt="thumb_boathouse-9871057_1280" src="https://github.com/user-attachments/assets/28be876f-813c-4147-8e41-a4638e74d79c" />

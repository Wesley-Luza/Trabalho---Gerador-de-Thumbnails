# Trabalho---Gerador-de-Thumbnails
Este trabalho foi realizado na matéria de Algoritmos e Programação II, com o objetivo de realizar um criador de thumbnails, em qualquer tamanho de imagem.

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

## Estrutura do projeto
AttGeradorThumbnails/
├── attGeradorThumbnails.c = Programa que gera as thumbnails.
├── stb_image.h = Repositório usado para compilar o programa.
├── stb_image_write.h = Repositório usado para compilar o programa.
└── imagens/ = pasta de exemplo com suas imagens
├── imagens usadas como exemplo (.jpg)

#pragma once

/**
 *
 */

#ifndef __ASCII_IMG_h__
#define __ASCII_IMG_h__

/* Inclusões */
#include <stdint.h>
#include <stdbool.h>

/* Constantes */

// Tamanho máximo da URL
#define ASCII_IMG_URL_TAMANHO_MAXIMO      (500)

/* Tipos */
typedef char asciiImgUrl_t[ASCII_IMG_URL_TAMANHO_MAXIMO];

/// Estrutura que representa uma imagem em Ascii
struct asciiImg_s {
  uint8_t* bytes;
  int nBytes;
};

/// Tipo "Imagem ASCII"
typedef struct asciiImg_s asciiImg_t;

/* Funções exportadas */
asciiImg_t* asciiImg_carregaImagem(char url[], bool colorido, int largura);
void asciiImg_imprimeImagem(asciiImg_t* img);
void asciiImg_liberaImagem(asciiImg_t* img);
bool asciiImg_verificaFerramenta(void);

#endif // __ASCII_IMG_h__
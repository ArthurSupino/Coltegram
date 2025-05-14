/**
 *
 */

/* Inclusões */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "ascii_img.h"
#ifdef __unix__ 
#include <unistd.h>
#include <sys/stat.h>
#endif

/* Constantes */

// Tamanho do buffer para cada linha da imagem
#define BUFFER_TAMANHO  50000

// Tamanho máximo da linha de comando
#define LINHA_COMANDO   10000

// Limiar inferior que considera o download com sucesso
#define LIMIAR_INFERIOR_TAMANHO_IMAGEM  500

// Nome do executável da ferramenta de download e conversão da imagem
#ifdef __unix__ 
#define FERRAMENTA_IMAGEM   "./ascii-image-converter.bin"
#else
#define FERRAMENTA_IMAGEM   "ascii-image-converter.exe"
#endif

// Nome do arquivo de saída temporário da imagem
#define ARQUIVO_IMAGEM_TMP  "ascii_art.txt"

/**
 *  \brief Função que carrega uma imagem informada na URL.
 *
 *  \param [in] colorido Define se a imagem será colorida.
 *  \param [in] largura Define a largura da imagem gerada.
 *  \return Endereço da estrutura com a imagem.
 *          Caso a imagem não tenha sido carregada corretamente, a função
 *          retornará NULL.
 */
asciiImg_t* asciiImg_carregaImagem(char url[], bool colorido, int largura) {

  FILE* arquivo;
  char * buffer;
  int nBytes, nBytesTotal = 0;
  char linhaComando[LINHA_COMANDO];
  uint8_t* newBytes;

  asciiImg_t* img;

  // Aloca espaço para o buffer
  buffer = malloc(BUFFER_TAMANHO);
  if (buffer == NULL) return NULL;

  // Aloca espaço para uma imagem
  img = malloc(sizeof(asciiImg_t));
  if (img == NULL) return NULL;

  // Inicializa a estrutura
  img->bytes = NULL;
  img->nBytes = 0;

  // Monta a linha de comando
  (void)sprintf(linhaComando, "%s %s %s -W %d -c > %s", FERRAMENTA_IMAGEM, url, (colorido ? "-C" : ""), largura, ARQUIVO_IMAGEM_TMP);

  // Chama o programa para fazer o download da imagem
  (void)system(linhaComando);

  // Tenta abrir o arquivo recem criado
  arquivo = fopen(ARQUIVO_IMAGEM_TMP, "r");
  if (arquivo != NULL) {

    while (!feof(arquivo)) {

      // Limpa a linha
      (void)memset(buffer, 0, sizeof(buffer));

      // Tenta ler uma linha
      if (fgets(buffer, BUFFER_TAMANHO, arquivo) == NULL) continue;

      // Descobre o número de bytes da linha
      for (nBytes = 0; buffer[nBytes] != 0; nBytes++);

      // Aloca o espaço
      newBytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + nBytes + 1));
      if (newBytes == NULL) {

        // Fecha o arquivo
        fclose(arquivo);

        // Libera a memória alocada previamente
        free(buffer);
        free(img->bytes);
        free(img);

        // Retorna que falhou
        return NULL;
      }

      // Atualiza o vetor
      img->bytes = newBytes;

      // Copia para o espaço alocado
      (void)memcpy(&(img->bytes[nBytesTotal]), buffer, nBytes);
      nBytesTotal += nBytes;
    }

    // Finaliza a imagem colocando o \0 final e o tamanho
    if (img->bytes != NULL) {
      img->bytes[nBytesTotal++] = '\0';
      img->nBytes = nBytesTotal;
    }

    // Fecha o arquivo
    fclose(arquivo);
  }

  // Libera o buffer
  free(buffer);

  // Verifica se a imagem é válida
  if (img->nBytes < LIMIAR_INFERIOR_TAMANHO_IMAGEM) {
    // Libera todo o espaço alocado
    free(img->bytes);
    free(img);

    return NULL;
  }

  // Retorna a imagem carregada
  return img;
}

/**
 *  \brief Função que imprime uma Imagem ASCII.
 *
 *  \param [in] img Endereço da estrutura com os dados da imagem.
 */
void asciiImg_imprimeImagem(asciiImg_t* img) {

  // Verifica se não é uma chamada inválida
  if (img == NULL) return;

  printf("%s", img->bytes);
}


/// <summary>
/// Função que libera a memória alocada por uma imagem.
/// </summary>
/// <param name="img">Endereço da estrutura com os dados da imagem a ser liberada.</param>
///
void asciiImg_liberaImagem(asciiImg_t* img) {
  
  // Verifica se não é uma chamada inválida
  if (img == NULL) return;
  
  // Libera os bytes e a própria estrutura
  free(img->bytes);
  free(img);
}

/// <summary>
/// Verifica se a ferramenta de conversão de imagem está presente e operante.
/// </summary>
/// <returns>Verdadeiro caso a ferramenta estaja disponível, 
/// ou falso caso contrário.</returns>
bool asciiImg_verificaFerramenta(void) {

  FILE* ferramenta;
  ferramenta = fopen(FERRAMENTA_IMAGEM, "r");
  if (ferramenta == NULL) return false;
  fclose(ferramenta);

#ifdef __unix__ 

  // Tenta adicionar permissão de execução à ferramenta
  char linhaComando[LINHA_COMANDO];
  sprintf(linhaComando, "chmod +x %s", FERRAMENTA_IMAGEM);
  (void)system(linhaComando);

  //Verifica se conseguiu
  struct stat stats;
  if (stat(FERRAMENTA_IMAGEM, &stats) == 0) {
    if (stats.st_mode & X_OK) return true;
  } else {
    return false;
  }
#endif

  // Se chegou até aqui é porque está disponível
  return true;
}

/*
.c de funcoes com utilidade geral
*/

#include <stdio.h>
#include <string.h>
#include "utilidade_geral.h"

#define TEXTO_TAMANHO_MAX 50


void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
        dados[tamanho - 1] = '\0';
    }
}

void texto_com_paragrafos(char* texto_real) {
    char texto[TEXTO_TAMANHO_MAX];
    int i = 0, vezes = 0, j = 0;

    int tamanho = 0;


    
    getchar();
    do {
        j = 0;
        fgets(texto, TEXTO_TAMANHO_MAX, stdin);
        if (vezes == 0) {
            strcpy(texto_real, texto);
        }
        else {
            tamanho = strlen(texto_real);

            texto_real[tamanho + strlen(texto)] = '\0';
            texto_real[tamanho] = 'a';

            for (i = tamanho;i < tamanho + strlen(texto);i++) {

                texto_real[i] = texto[j];
                j++;
            }
        }


        vezes++;
    } while (texto[strlen(texto) - 2] != '.');

}

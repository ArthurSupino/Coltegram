
/*
    @file	perfil.h
    @brief	arquivo com funcoes do perfil e todas as interacoes dos menus e com os outros arquivos
    @date
    @authors


*/



#pragma once
#ifndef _0__PERFIL_h_
#define	_0__PERFIL_h_

/* inclusoes */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "utilidade_geral.h"
#include "comentarios_e_curtidas.h"

/*constantes*/

#define SUCESSO 0

//defines do perfil
#define ID_PERFIL_TAMANHO_MAX 50
#define NOME_TAMANHO_MAX 50
#define EMAIL_TAMANHO_MAX 50
#define SENHA_TAMANHO_MAX 50

//defines extras
#define ERROR_MESSAGE_TAMANHO_MAX 50

/*tipos*/

//struct de pointer do perfil e de n_registros

struct perfil_e_registros_s {
    perfil_t* perfil;
    int n_registros;
};


//structs extras

struct error_s {
    int error_message;
    char error_message_string[ERROR_MESSAGE_TAMANHO_MAX];
};

typedef struct error_s error_t;



typedef struct perfil_e_registros_s perfil_e_registros_t;

/*funcoes*/

int procura_conta(char* email, perfil_t* contas, int n_cadastros);
perfil_e_registros_t ler_arquivo_alocando();
bool ler_arquivo_login(perfil_t* perfil_logado);
void colocar_no_arquivo(perfil_t* perfil, int n_registros);
perfil_e_registros_t ler_arquivo_alocando_extra(perfil_t* perfil);
bool corrigir_email(char* email);
void cadastro_perfil();
int particao_email(perfil_t* array, int inicio, int fim);
void quicksort_email(perfil_t* array, int inicio, int fim);
int particao_nome(perfil_t* array, int inicio, int fim);
void quicksort_nome(perfil_t* array, int inicio, int fim);
int particao_ID(perfil_t* array, int inicio, int fim);
void quicksort_ID(perfil_t* array, int inicio, int fim);
int acessar_perfil(perfil_t perfil_acessado);
int visitar_perfil(perfil_t* perfil);
void tabela_pesquisa_perfil(int n_achados, perfil_t* perfis_encontrados);
void pesquisa_perfil(char* pesquisa, char tipo);
void buscar_perfil();
void lista_todos_perfis(perfil_t* perfil, int quantidade);
void listar_como() ;
void deslogar_da_conta();
void menu_do_usuario();
char menu_apos_login();
perfil_t perfil_logado_ler();
void arquivo_perfil_logado(perfil_t perfil_logado);
int login();
char menu_principal();







#endif // !_0__UTILIDADE_GERAL_h_


/*
	@file	posts.h
	@brief	arquivo com funcoes e tipos para posts e comentarios
			no coltegram
	@date
	@authors


*/



#pragma once
#ifndef _0__POSTS_h_
#define	_0__POSTS_h_

/* inclusoes */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "utilidade_geral.h"
#include "ascii_img.h"
/*constantes*/
//defines do post
#define ID_POST_TAMANHO_MAX 50
#define DESCRICAO_TAMANHO_MAX 500
#define TAMANHO_MAXIMO_URL 500
#define URL_QUANTIDADE_MAX 10


//defines do comentario
#define PESSOA_COMENTARIO_TAMANHO_MAX 50
#define ID_COMENTARIO_TAMANHO_MAX 70
#define TEXTO_TAMANHO_MAX_COMENTARIO 500


//defines do perfil

#define ID_PERFIL_TAMANHO_MAX 50
#define NOME_TAMANHO_MAX 50
#define EMAIL_TAMANHO_MAX 50
#define SENHA_TAMANHO_MAX 50

/*tipos*/

//structs do perfil
struct perfil_t {
    char ID[ID_PERFIL_TAMANHO_MAX];
    char Nome[NOME_TAMANHO_MAX];
    char Email[EMAIL_TAMANHO_MAX];
    char Senha[SENHA_TAMANHO_MAX];
    int n_postagens_total;
    int n_postagens_real;
};

typedef struct perfil_t perfil_t;

//defines da curtida
#define PESSOA_CURTIDA_TAMANHO_MAX 50

//defines extras
#define STRING_DE_NUMEROS_TAMANHO_MAXIMO 50



//structs do post
struct post_t {
    char ID_perfil[ID_PERFIL_TAMANHO_MAX];
    char ID[ID_POST_TAMANHO_MAX];
    int n_imagens;
    char descricao[DESCRICAO_TAMANHO_MAX];
    int n_curtidas;
    int n_comentarios_total;
    int n_comentarios_real;
    char url[URL_QUANTIDADE_MAX][TAMANHO_MAXIMO_URL];
};

typedef struct post_t post_t;


struct post_e_n_posts {
    post_t* posts;
    int n_posts;
};
typedef struct post_e_n_posts post_e_n_posts_t;

//structs do comentario
struct comentario_t {
    char ID_post[ID_POST_TAMANHO_MAX];
    char texto[TEXTO_TAMANHO_MAX_COMENTARIO];
    char pessoa[PESSOA_COMENTARIO_TAMANHO_MAX];
    char ID_comentario[ID_COMENTARIO_TAMANHO_MAX];

};

typedef struct comentario_t comentario_t;

struct comentario_e_n_comentario_s {
    comentario_t* comentario;
    int n_comentarios_total;
};

typedef struct comentario_e_n_comentario_s comentario_e_n_comentario_t;

//struct da curtida
struct curtida_t {
    char ID_post[ID_POST_TAMANHO_MAX];
    char pessoa[PESSOA_CURTIDA_TAMANHO_MAX];
};

typedef struct curtida_t curtida_t;

struct curtidas_e_n_curtidas_s {
    curtida_t* curtidas;
    int n_curtidas;
};
typedef struct curtidas_e_n_curtidas_s curtidas_e_n_curtidas_t;

/*funcoes*/
//funcoes de post
post_e_n_posts_t ler_arquivo_post_alocando();

//funcoes de comentario
comentario_e_n_comentario_t ler_arquivo_comentario_alocando_extra();
void concerta_comentario(int n_comentarios, int n_antes, comentario_t* comentario_todos, comentario_t comentario_oficial);
int  atualizar_comentario_no_post(post_t* posts, int n_postagens, char* post_ID);
void adicionar_cometario_no_arquivo(char* ID_postagem, perfil_t perfil_logado);
comentario_t* ler_comentarios_de_um_post_alocando(char* post_ID);
comentario_e_n_comentario_t ler_arquivo_comentario_alocando();
comentario_e_n_comentario_t remove_comentario(int comentario_escolhido, int* posicoes_dos_comentarios, comentario_e_n_comentario_t comentarios_e_quantidade);
void deletar_comentario(perfil_t perfil_logado, char* ID_postagem);
comentario_e_n_comentario_t modifica_comentario(int comentario_escolhido, int* posicoes_dos_comentarios, comentario_e_n_comentario_t comentarios_e_quantidade);
void editar_comentario(perfil_t perfil_logado,char* ID_postagem);

//funcoes de curtida
void concerta_curtida(int n_curtidas, int n_antes, curtida_t* curtidas_todas, curtida_t curtida_oficial);
curtidas_e_n_curtidas_t ler_arquivo_curtidas_alocando_extra();
curtidas_e_n_curtidas_t ler_arquivo_curtidas_alocando();
int atualizar_curtida_no_post(post_t* posts, int n_postagens, char* post_ID);
void curtir(char* ID_postagem, perfil_t perfil_logado);
curtidas_e_n_curtidas_t ler_curtidas_de_um_post_alocando(char* ID_postagem);
bool curtiu(perfil_t perfil_logado, char* ID_postagem);
void descurtir(perfil_t perfil_logado, char* ID_postagem);

#endif // !_0__POSTS_h_


/*
    @file	postagens.h
    @brief	arquivo com funcoes da postagem
    @date
    @authors


*/



#pragma once
#ifndef _0__POSTAGENS_h_
#define	_0__POSTAGENS_h_

/* inclusoes */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "ascii_img.h"
#include "utilidade_geral.h"
#include "comentarios_e_curtidas.h"


/*constantes*/

#define NUMERO_DE_FOTOS_POR_POST 10
#define TAMANHO_MAXIMO_ID 50
#define TAMANHO_DO_POST 120

/*tipos*/





/*funcoes*/
void atualiza_posts_perfil(perfil_t perfil_logado, perfil_t* perfis, int n_perfis);
post_e_n_posts_t ler_arquivo_post_alocando_extra();
void concerta_postagem(int n_postagens, int n_antes, post_t* postagens, post_t postagem_nova);
void salvar_postagem_no_arquivo(post_t postagem_nova);
void postar_imagem(post_t* postagem);
void postar();
post_e_n_posts_t ler_arquivo_posts_de_um_perfil(perfil_t perfil);
void carregar_postagens(perfil_t perfil_visitado, int endereco1);
post_e_n_posts_t remove_postagem(int postagem_escolhido, int* posicoes_das_postagens, post_e_n_posts_t postagens_e_quantidade);
void deletar_postagem(perfil_t perfil_logado);
void detalhar_postagem(perfil_t perfil_logado, bool logado, int endereco_postagem, perfil_t perfil_visitado);
void editar_postagem(perfil_t perfil_logado);
  



#endif // !_0__POSTAGENS_h_

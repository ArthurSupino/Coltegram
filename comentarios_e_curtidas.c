
/*
Esse espaco sera destinado para as postagens e suas funcoes
postagens:
*/

//le arquivo de posts e retorna uma variavel com os posts e o numero de posts

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "utilidade_geral.h"
#include "ascii_img.h"
#include "comentarios_e_curtidas.h"


//defines 
#define ID_POST_TAMANHO_MAX 50
#define DESCRICAO_TAMANHO_MAX 500
#define TAMANHO_MAXIMO_URL 500
#define URL_QUANTIDADE_MAX 10



//defines do comentario
#define PESSOA_COMENTARIO_TAMANHO_MAX 50
#define ID_COMENTARIO_TAMANHO_MAX 70


//defines do perfil

#define ID_PERFIL_TAMANHO_MAX 50
#define NOME_TAMANHO_MAX 50
#define EMAIL_TAMANHO_MAX 50
#define SENHA_TAMANHO_MAX 50

//defines do comentario
#define TEXTO_TAMANHO_MAX_COMENTARIO 500

//defines da curtida
#define PESSOA_CURTIDA_TAMANHO_MAX 50

//defines extras
#define STRING_DE_NUMEROS_TAMANHO_MAXIMO 50




//funcoes do comentario

post_e_n_posts_t ler_arquivo_post_alocando() {
    FILE* arquivo_posts = 0;
    post_t* posts = 0;
    int n_postagens = 0;
    post_e_n_posts_t variavel;

    arquivo_posts = fopen("arquivo_postagens.bin", "a+b");

    if (arquivo_posts == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        fread(&n_postagens, sizeof(int), 1, arquivo_posts);
        posts = (post_t*)malloc(sizeof(post_t) * n_postagens);
        fread(posts, sizeof(post_t), n_postagens, arquivo_posts);
        fclose(arquivo_posts);
        variavel.n_posts = n_postagens;
        variavel.posts = posts;

        return variavel;
    }
}
comentario_e_n_comentario_t ler_arquivo_comentario_alocando_extra() {
    FILE* arquivo_comentario = 0;
    comentario_t* comentario = 0;
    int n_comentarios = 0;
    comentario_e_n_comentario_t variavel;

    arquivo_comentario = fopen("arquivo_comentarios.bin", "a+b");

    if (arquivo_comentario == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        fread(&n_comentarios, sizeof(int), 1, arquivo_comentario);
        n_comentarios++;
        comentario = (comentario_t*)malloc(sizeof(comentario_t) * n_comentarios);
        fread(comentario, sizeof(comentario_t), n_comentarios, arquivo_comentario);
        fclose(arquivo_comentario);
        variavel.n_comentarios_total = n_comentarios;
        variavel.comentario = comentario;

        return variavel;
    }
}
//le arquivo do comentario 
comentario_e_n_comentario_t ler_arquivo_comentario_alocando() {
    FILE* arquivo_comentario = 0;
    comentario_t* comentario = 0;
    int n_comentarios = 0;
    comentario_e_n_comentario_t variavel;

    arquivo_comentario = fopen("arquivo_comentarios.bin", "a+b");

    if (arquivo_comentario == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        fread(&n_comentarios, sizeof(int), 1, arquivo_comentario);
        comentario = (comentario_t*)malloc(sizeof(comentario_t) * n_comentarios);
        fread(comentario, sizeof(comentario_t), n_comentarios, arquivo_comentario);
        fclose(arquivo_comentario);
        variavel.n_comentarios_total = n_comentarios;
        variavel.comentario = comentario;

        return variavel;
    }
}
// funcao que coloca o comentario junto com outros do mesmo post
void concerta_comentario(int n_comentarios, int n_antes, comentario_t* comentario_todos, comentario_t comentario_oficial) {
    int i = 0, pos_array = 0, fim_array = 0;
    pos_array = n_antes;
    fim_array = n_comentarios - 1;


    for (i = fim_array; i > pos_array;i--) {
        comentario_todos[i] = comentario_todos[i-1];
    
    }
    comentario_todos[pos_array] = comentario_oficial;
}
//aumenta em 1 os comentarios do post, retorna o numero de comentarios
int  atualizar_comentario_no_post(post_t* posts, int n_postagens, char* post_ID) {
    FILE* arquivo_postagens;
    int lugar_do_post = 0;


    arquivo_postagens = fopen("arquivo_postagens.bin", "wb");

    if (arquivo_postagens == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        for (lugar_do_post = 0;lugar_do_post < n_postagens - 1;lugar_do_post++) {
            if (strcmp(posts[lugar_do_post].ID, post_ID) == 0) {
                break;
            }
        }
        posts[lugar_do_post].n_comentarios_total++;
        posts[lugar_do_post].n_comentarios_real++;

        fwrite(&n_postagens, sizeof(int), 1, arquivo_postagens);
        fwrite(posts, sizeof(post_t), n_postagens, arquivo_postagens);
        fclose(arquivo_postagens);
        return posts[lugar_do_post].n_comentarios_total;

    }
    
}
//nao parece estar adicionando qnts comentarios tem
void adicionar_cometario_no_arquivo(char* ID_postagem, perfil_t perfil_logado) {
    FILE* arquivo_comentarios = 0;
    bool achou = false;
    comentario_t comentario_aux, comentario_oficial;
    int n_comentarios_antes = 0, n_comentarios_total = 0, n_postagens = 0, n_comentarios_do_post = 0, i = 0;
    post_t* posts;
    post_e_n_posts_t  variavel;
    char n_comentarios_string[STRING_DE_NUMEROS_TAMANHO_MAXIMO];
    comentario_t* comentario_todos = 0;
    comentario_e_n_comentario_t variavel_comentario;


    //garante a criacao de um arquivo se ele nao existir
    arquivo_comentarios = fopen("arquivo_comentarios.bin", "ab");
    if (arquivo_comentarios == NULL) {
        printf("Nao foi possivel abrir o arquivo de comentarios\n");
    }
    else {
        fclose(arquivo_comentarios);

        arquivo_comentarios = fopen("arquivo_comentarios.bin", "rb");

        if (arquivo_comentarios == NULL) {
            printf("Nao foi possivel abrir o arquivo\n");
        }
        else {
            //le a quantidade de comentarios total
            fread(&n_comentarios_total, sizeof(int), 1, arquivo_comentarios);
            //verifica a posicao do primeiro comentario referente a uma postagem, se houver
            
            do {
                fread(&comentario_aux, sizeof(comentario_t), 1, arquivo_comentarios);
                if (feof(arquivo_comentarios) != 0) break;
                if (strcmp(comentario_aux.ID_post, ID_postagem) == 0) {
                    achou = true;
                    break;
                }
                else {
                    n_comentarios_antes++;
                }
            } while (feof(arquivo_comentarios) == 0);
            fclose(arquivo_comentarios);
            n_comentarios_total++;
            //le os arquivos de posts
            variavel = ler_arquivo_post_alocando();
            posts = variavel.posts;
            n_postagens = variavel.n_posts;
            //aumenta em 1 os comentarios do post
            n_comentarios_do_post = atualizar_comentario_no_post(posts, n_postagens, ID_postagem);
            

            //le os comentarios do arquivo
            variavel_comentario = ler_arquivo_comentario_alocando_extra();

            //reabre o arquivo para escrita
            arquivo_comentarios = fopen("arquivo_comentarios.bin", "wb");
            if (arquivo_comentarios == NULL) {
                printf("Nao foi possivel abrir o arquivo de comentarios\n");
            }
            else {

                //verifica se ja havia um comentario nesse post

                if (!achou) {
                    //cria o ID do comentario - formado pelo ID do perfil, ID do post e numero de comentarios do post
                    for (i = 0;i < strlen(perfil_logado.ID);i++) {
                        comentario_oficial.ID_comentario[i] = perfil_logado.ID[i];
                    }
                    for (i = 0;i < strlen(ID_postagem);i++) {
                        comentario_oficial.ID_comentario[i + strlen(perfil_logado.ID)] = ID_postagem[i];
                    }
                    sprintf(n_comentarios_string, "%d", n_comentarios_do_post);
                    for (i = 0;i < strlen(n_comentarios_string);i++) {
                        comentario_oficial.ID_comentario[i + strlen(perfil_logado.ID) + strlen(ID_postagem)] = n_comentarios_string[i];

                    }
                    comentario_oficial.ID_comentario[strlen(perfil_logado.ID) + strlen(ID_postagem) + strlen(n_comentarios_string)] = '\0';
                    //pega as outras informacoes do comentario
                    strcpy(comentario_oficial.ID_post, ID_postagem);
                    strcpy(comentario_oficial.pessoa, perfil_logado.ID);

                    printf("Comentario: \n[termina com um ponto no final da frase]\n");
                    texto_com_paragrafos(comentario_oficial.texto);

                    //escreve o comentario no arquivo
                    fwrite(&n_comentarios_total, sizeof(int), 1, arquivo_comentarios);

                    
                    comentario_todos = variavel_comentario.comentario;
                    comentario_todos[n_comentarios_total - 1] = comentario_oficial;

                    fwrite(comentario_todos, sizeof(comentario_t), n_comentarios_total, arquivo_comentarios);
                    fclose(arquivo_comentarios);
                }
                else {
                    //cria o ID do comentario - formado pelo ID do perfil, ID do post e numero de comentarios do post
                    for (i = 0;i < strlen(perfil_logado.ID);i++) {
                        comentario_oficial.ID_comentario[i] = perfil_logado.ID[i];
                    }
                    for (i = 0;i < strlen(ID_postagem);i++) {
                        comentario_oficial.ID_comentario[i + strlen(perfil_logado.ID)] = ID_postagem[i];
                    }
                    sprintf(n_comentarios_string, "%d", n_comentarios_do_post);
                    for (i = 0;i < strlen(n_comentarios_string);i++) {
                        comentario_oficial.ID_comentario[i + strlen(perfil_logado.ID) + strlen(ID_postagem)] = n_comentarios_string[i];

                    }
                    comentario_oficial.ID_comentario[i + strlen(perfil_logado.ID) + strlen(ID_postagem) + strlen(n_comentarios_string)] = '\0';
                    //pega as outras informacoes do comentario
                    strcpy(comentario_oficial.ID_post, ID_postagem);
                    strcpy(comentario_oficial.pessoa, perfil_logado.ID);

                    printf("Comentario: \n[termina com um ponto no final da frase]\n");
                    texto_com_paragrafos(comentario_oficial.texto);

                    //escreve o comentario no arquivo:
                    // 
                    
                    comentario_todos = variavel_comentario.comentario;

                    //funcao para colocar o novo comentario no lugar certo
                    concerta_comentario(n_comentarios_total, n_comentarios_antes, comentario_todos, comentario_oficial);

                    fwrite(&n_comentarios_total, sizeof(int), 1, arquivo_comentarios);
                    fwrite(comentario_todos, sizeof(comentario_t), n_comentarios_total, arquivo_comentarios);
                    fclose(arquivo_comentarios);
                }
            }


        }

    }
}

//ler comentarios
comentario_t* ler_comentarios_de_um_post_alocando(char* post_ID) {
    FILE* arquivo_comentarios = 0;
    int lugar_do_post = 0, n_postagens = 0, n_comentarios_antes = 0, n_comentarios_total = 0;
    post_e_n_posts_t variavel;
    post_t* posts;
    comentario_t* comentario_post, comentario_aux;
    bool achou = false;
    variavel = ler_arquivo_post_alocando();
    n_postagens = variavel.n_posts;
    posts = variavel.posts;
    

    //acha o post que o usuario esta para pegar o numero de comentarios
    for (lugar_do_post = 0;lugar_do_post < n_postagens - 1;lugar_do_post++) {
        if (strcmp(posts[lugar_do_post].ID, post_ID) == 0) {
            break;
        }
    }

    arquivo_comentarios = fopen("arquivo_comentarios.bin", "rb");
    if (arquivo_comentarios == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
    }
    else {
        if (posts[lugar_do_post].n_comentarios_real > 0) {
            //aloca espaco para o numero de comentarios do post
            comentario_post = (comentario_t*)malloc(sizeof(comentario_t) * posts[lugar_do_post].n_comentarios_real);
            //verifica a posicao do primeiro comentario referente a uma postagem, se houver
            fread(&n_comentarios_total, sizeof(int), 1, arquivo_comentarios);
            do {
                fread(&comentario_aux, sizeof(comentario_t), 1, arquivo_comentarios);
                if (strcmp(comentario_aux.ID_post, post_ID) == 0) {
                    achou = true;
                    break;
                }
                else {
                    n_comentarios_antes++;
                }
            } while (feof(arquivo_comentarios) == 0);
            //colocar o cursor no local dos comentarios do post
            fseek(arquivo_comentarios, n_comentarios_antes * sizeof(comentario_t) + sizeof(int), SEEK_SET);
            //le os comentarios
            fread(comentario_post, sizeof(comentario_t), posts[lugar_do_post].n_comentarios_real, arquivo_comentarios);
            return comentario_post;
        }
        else {
            printf("\tNao ha nenhum comentario\t\n");
        }
    }

}

//funcao para remover comentario unico
comentario_e_n_comentario_t remove_comentario(int comentario_escolhido, int* posicoes_dos_comentarios, comentario_e_n_comentario_t comentarios_e_quantidade) {
    int i = 0;
    //remove esse comentario
    for (i = posicoes_dos_comentarios[comentario_escolhido];i < comentarios_e_quantidade.n_comentarios_total - 1;i++) {
        comentarios_e_quantidade.comentario[i] = comentarios_e_quantidade.comentario[i + 1];
    }
    //diminui a quantidade de comentarios em -1
    comentarios_e_quantidade.n_comentarios_total--;
    return comentarios_e_quantidade;

}

//deletar um comentario feito
void deletar_comentario(perfil_t perfil_logado, char* ID_postagem) {
    FILE* arquivo_comentarios = 0;
    FILE* arquivo_postagens = 0;
    comentario_e_n_comentario_t comentarios_e_quantidade;
    int lugar_no_arquivo = 0, i = 0;
    int achou = 0, comentario_escolhido = 0, deletados = 0;
    int* posicoes_dos_comentarios = 0;
    char opcao = 0;
    post_e_n_posts_t postagens_e_quantidade;

    //aloca todos os comentarios
    comentarios_e_quantidade = ler_arquivo_comentario_alocando();
    //acha onde o perfil comentou no arquivo, mesmo se houver mais de 1 comentario e aloca espaco para essas posicoes
    for (lugar_no_arquivo = 0;lugar_no_arquivo < comentarios_e_quantidade.n_comentarios_total;lugar_no_arquivo++) {
        if (strcmp(comentarios_e_quantidade.comentario[lugar_no_arquivo].ID_post, ID_postagem) == 0 && strcmp(comentarios_e_quantidade.comentario[lugar_no_arquivo].pessoa, perfil_logado.ID) == 0) {
            achou++;
            if (achou == 1) {
                posicoes_dos_comentarios = (int*)malloc(sizeof(int) * achou);
                posicoes_dos_comentarios[achou - 1] = lugar_no_arquivo;
            }
            else {
                posicoes_dos_comentarios = (int*)realloc(posicoes_dos_comentarios, sizeof(int) * achou);
                posicoes_dos_comentarios[achou - 1] = lugar_no_arquivo;
            }
        }
    }
    //envia uma mensagem caso a pessoa nao tenha comentado na postagem
    if (achou == 0) {
        printf("Voce nao comentou nessa publicacao, e impossivel deletar um comentario\n");
    }
    // caso haja somente um comentario, pergunta se o usuario deseja apaga-lo e faz a delecao, se necessario
    else if (achou == 1) {
        do {
            printf("Voce quer deletar esse comentario?\n");
            printf("\t%s\n\n", comentarios_e_quantidade.comentario[posicoes_dos_comentarios[0]].texto);
            printf("[S] - Sim\n[N] - Nao\n");
            scanf(" %c", &opcao);
            switch (opcao) {
            case 'S':
                comentarios_e_quantidade = remove_comentario(achou - 1, posicoes_dos_comentarios, comentarios_e_quantidade);
                deletados++;
                break;
            case 'N':
                break;
            default:
                printf("A opcao escolhida nao existe, tente novamente\n");
            }
            if (opcao != 'N' || opcao != 'S') {
                break;
            }
        } while (opcao != 'N' || opcao != 'S');
    }
    //mostra todos os comentarios do usuario e pergunta quais ele deseja deletar
    else {
        printf("Aqui estao todos os seus comentarios feitos nessa postagem:\n");
        for (i = 0;i < achou;i++) {
            printf("comentario %i ->\t%s\n\n", i, comentarios_e_quantidade.comentario[posicoes_dos_comentarios[i]].texto);
        }
        for (i = 0;i < achou;i++) {
            do {
                printf("Voce deseja apagar o comentario %i?\n", i);
                printf("[S] - Sim\n[N] - Nao\n");
                scanf(" %c", &opcao);
                switch (opcao) {
                case 'S':
                    comentarios_e_quantidade = remove_comentario(i, posicoes_dos_comentarios, comentarios_e_quantidade);
                    deletados++;
                    break;
                case 'N':
                    break;
                default:
                    printf("A opcao escolhida nao existe, tente novamente\n");

                }
                if (opcao != 'N' || opcao != 'S') {
                    break;
                }
            } while (opcao != 'N' || opcao != 'S');
        }
    }


    
    //abre o arquivo para escrever os comentarios novamente
    arquivo_comentarios = fopen("arquivo_comentarios.bin", "wb");
    if (arquivo_comentarios == NULL) {
        printf("Nao foi possivel abrir o arquivo de comentarios\n");
    }
    else {
        fwrite(&comentarios_e_quantidade.n_comentarios_total, sizeof(int), 1, arquivo_comentarios);
        fwrite(comentarios_e_quantidade.comentario, sizeof(comentario_t), comentarios_e_quantidade.n_comentarios_total, arquivo_comentarios);
        fclose(arquivo_comentarios);
    }
    //escreve o novo numero de comentarios no arquivo de postagens
    //le as postagens
    postagens_e_quantidade = ler_arquivo_post_alocando();
    //acha o local da postagem que estamos
    for (i = 0;i < postagens_e_quantidade.n_posts; i++) {
        if (strcmp(postagens_e_quantidade.posts[i].ID, ID_postagem) == 0) {
            
            break;
        }
    }
    postagens_e_quantidade.posts[i].n_comentarios_real = postagens_e_quantidade.posts[i].n_comentarios_real - deletados;
    //abre o arquivo para reescrever o numero de comentarios
    arquivo_postagens = fopen("arquivo_postagens.bin", "wb");
    if (arquivo_postagens == NULL) {
        printf("Nao foi possivel abrir o arquivo de postagens");
    }
    else {
        fwrite(&postagens_e_quantidade.n_posts, sizeof(int), 1, arquivo_postagens);
        fwrite(postagens_e_quantidade.posts, sizeof(post_t), postagens_e_quantidade.n_posts, arquivo_postagens);
        fclose(arquivo_postagens);
    }
    

}
//funcao que edita comentario
comentario_e_n_comentario_t modifica_comentario(int comentario_escolhido, int* posicoes_dos_comentarios, comentario_e_n_comentario_t comentarios_e_quantidade){
    printf("Digite o novo texto do seguinte comentario:\n");
    printf("Comentario:\t%s\n\n", comentarios_e_quantidade.comentario[posicoes_dos_comentarios[comentario_escolhido]].texto);
    printf("Novo texto: \n");
    getchar();
    fgets(comentarios_e_quantidade.comentario[posicoes_dos_comentarios[comentario_escolhido]].texto, TEXTO_TAMANHO_MAX_COMENTARIO, stdin);
    return comentarios_e_quantidade;
}
//editar comentario
void editar_comentario(perfil_t perfil_logado,char* ID_postagem) {
    FILE* arquivo_comentarios = 0;
    comentario_e_n_comentario_t comentarios_e_quantidade;
    int lugar_no_arquivo = 0, i = 0;
    int achou = 0, comentario_escolhido = 0;
    int* posicoes_dos_comentarios = 0;
    char opcao = 0;

    //aloca todos os comentarios
    comentarios_e_quantidade = ler_arquivo_comentario_alocando();
    //acha onde o perfil comentou no arquivo, mesmo se houver mais de 1 comentario e aloca espaco para essas posicoes
    for (lugar_no_arquivo = 0;lugar_no_arquivo < comentarios_e_quantidade.n_comentarios_total;lugar_no_arquivo++) {
        if (strcmp(comentarios_e_quantidade.comentario[lugar_no_arquivo].ID_post, ID_postagem) == 0 && strcmp(comentarios_e_quantidade.comentario[lugar_no_arquivo].pessoa, perfil_logado.ID) == 0) {
            achou++;
            if (achou == 1) {
                posicoes_dos_comentarios = (int*)malloc(sizeof(int) * achou);
                posicoes_dos_comentarios[achou - 1] = lugar_no_arquivo;
            }
            else {
                posicoes_dos_comentarios = (int*)realloc(posicoes_dos_comentarios, sizeof(int) * achou);
                posicoes_dos_comentarios[achou - 1] = lugar_no_arquivo;
            }
        }
    }
    //envia uma mensagem caso a pessoa nao tenha comentado na postagem
    if (achou == 0) {
        printf("Voce nao comentou nessa publicacao, e impossivel editar um comentario\n");
    }
    // caso haja somente um comentario, pergunta se o usuario deseja edita-lo e faz a edicao, se necessario
    else if (achou == 1) {
        do {
            printf("Voce quer editar esse comentario?\n");
            printf("\t%s\n\n", comentarios_e_quantidade.comentario[posicoes_dos_comentarios[0]].texto);
            printf("[S] - Sim\n[N] - Nao\n");
            scanf(" %c", &opcao);
            switch (opcao) {
            case 'S':
                comentarios_e_quantidade = modifica_comentario(achou - 1, posicoes_dos_comentarios, comentarios_e_quantidade);
                break;
            case 'N':
                break;
            default:
                printf("A opcao escolhida nao existe, tente novamente\n");
            }
            if (opcao == 'N' || opcao == 'S') {
                break;
            }
        } while (opcao != 'N');
    }
    //mostra todos os comentarios do usuario e pergunta quais ele deseja editar
    else {
        printf("Aqui estao todos os seus comentarios feitos nessa postagem:\n");
        for (i = 0;i < achou;i++) {
            printf("comentario %i ->\t%s\n\n", i, comentarios_e_quantidade.comentario[posicoes_dos_comentarios[i]].texto);
        }
        for (i = 0;i < achou;i++) {
            do {
                printf("Voce deseja editar o comentario %i?\n", i);
                printf("[S] - Sim\n[N] - Nao\n");
                scanf(" %c", &opcao);
                switch (opcao) {
                case 'S':
                    comentarios_e_quantidade = modifica_comentario(i, posicoes_dos_comentarios, comentarios_e_quantidade);
                    break;
                case 'N':
                    break;
                default:
                    printf("A opcao escolhida nao existe, tente novamente\n");

                }
                if (opcao == 'N' || opcao == 'S') {
                    break;
                }
            } while (opcao != 'N');
        }
    }


    
    //abre o arquivo para escrever os comentarios novamente
    arquivo_comentarios = fopen("arquivo_comentarios.bin", "wb");
    if (arquivo_comentarios == NULL) {
        printf("Nao foi possivel abrir o arquivo de comentarios\n");
    }
    else {
        fwrite(&comentarios_e_quantidade.n_comentarios_total, sizeof(int), 1, arquivo_comentarios);
        fwrite(comentarios_e_quantidade.comentario, sizeof(comentario_t), comentarios_e_quantidade.n_comentarios_total, arquivo_comentarios);
        fclose(arquivo_comentarios);
    }

}






/*
esse espaco sera destinado para as curtidas

*/


//funcao para colocar curtida no lugar
void concerta_curtida(int n_curtidas, int n_antes, curtida_t* curtidas_todas, curtida_t curtida_oficial) {
    int i = 0, pos_array = 0, fim_array = 0;
    pos_array = n_antes;
    fim_array = n_curtidas - 1;


    for (i = fim_array; i > pos_array;i--) {
        curtidas_todas[i] = curtidas_todas[i-1];

    }
    curtidas_todas[pos_array] = curtida_oficial;
}
//funcao para ler arquivo de curtidas e alocar espaco para mais uma
curtidas_e_n_curtidas_t ler_arquivo_curtidas_alocando_extra() {
    FILE* arquivo_curtidas = 0;
    curtida_t* curtidas = 0;
    int n_curtidas = 0;
    curtidas_e_n_curtidas_t variavel;

    arquivo_curtidas = fopen("arquivo_curtidas.bin", "rb");

    if (arquivo_curtidas == NULL) {
        printf("Nao foi possivel abrir o arquivo de curtidas\n");

    }
    else {
        fread(&n_curtidas, sizeof(int), 1, arquivo_curtidas);
        n_curtidas++;
        curtidas = (curtida_t*)malloc(sizeof(curtida_t) * n_curtidas);
        fread(curtidas, sizeof(curtida_t), n_curtidas, arquivo_curtidas);
        fclose(arquivo_curtidas);
        variavel.n_curtidas = n_curtidas;
        variavel.curtidas = curtidas;

        return variavel;
    }
}
//ler curtidas alocando normalmente
curtidas_e_n_curtidas_t ler_arquivo_curtidas_alocando() {
    FILE* arquivo_curtidas = 0;
    curtida_t* curtidas = 0;
    int n_curtidas = 0;
    curtidas_e_n_curtidas_t variavel;

    arquivo_curtidas = fopen("arquivo_curtidas.bin", "rb");

    if (arquivo_curtidas == NULL) {
        printf("Nao foi possivel abrir o arquivo de curtidas\n");

    }
    else {
        fread(&n_curtidas, sizeof(int), 1, arquivo_curtidas);
        curtidas = (curtida_t*)malloc(sizeof(curtida_t) * n_curtidas);
        fread(curtidas, sizeof(curtida_t), n_curtidas, arquivo_curtidas);
        fclose(arquivo_curtidas);
        variavel.n_curtidas = n_curtidas;
        variavel.curtidas = curtidas;

        return variavel;
    }
}
//funcao para atualizar o numero de postagens no post curitdo
int  atualizar_curtida_no_post(post_t* posts, int n_postagens, char* post_ID) {
    FILE* arquivo_postagens;
    int lugar_do_post = 0;


    arquivo_postagens = fopen("arquivo_postagens.bin", "wb");

    if (arquivo_postagens == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        for (lugar_do_post = 0;lugar_do_post < n_postagens - 1;lugar_do_post++) {
            if (strcmp(posts[lugar_do_post].ID, post_ID) == 0) {
                break;
            }
        }
        posts[lugar_do_post].n_curtidas++;

        fwrite(&n_postagens, sizeof(int), 1, arquivo_postagens);
        fwrite(posts, sizeof(post_t), n_postagens, arquivo_postagens);
        fclose(arquivo_postagens);
        return posts[lugar_do_post].n_curtidas;

    }

}
//funcao para curtir postagem
void curtir(char* ID_postagem, perfil_t perfil_logado) {
    FILE* arquivo_curtidas = 0;
    int curtidas_total = 0, curtidas_antes = 0, n_curtidas_post = 0;
    curtida_t curtida_aux, curtida_oficial;
    bool achou = false;
    post_e_n_posts_t variavel;
    curtidas_e_n_curtidas_t curtidas_e_quantidade;

    //garante que havera um arquivo de curtidas caso ele nao exista
    arquivo_curtidas = fopen("arquivo_curtidas.bin", "ab");
    if (arquivo_curtidas == NULL) {
        printf("Nao foi possivel abri o arquivo de curtidas\n");
    }
    else {
        fclose(arquivo_curtidas);
        arquivo_curtidas = fopen("arquivo_curtidas.bin", "rb");
        if (arquivo_curtidas == NULL) {
            printf("Nao foi possivel abrir o arquivo de curitdas\n");
        }
        else {
            //le o numero de curitdas total
            fread(&curtidas_total, sizeof(int), 1, arquivo_curtidas);
            //acha o local da curtida do post, se houver
            do {
                fread(&curtida_aux, sizeof(curtida_t), 1, arquivo_curtidas);
                if (feof(arquivo_curtidas) != 0) break;
                if (strcmp(curtida_aux.ID_post, ID_postagem) == 0) {
                    achou = true;
                    break;
                }
                else {
                    curtidas_antes++;

                }

            } while (feof(arquivo_curtidas) == 0);
            fclose(arquivo_curtidas);
            //atualiza o numero de curtidas total e o numero no post
            curtidas_total++;
            variavel = ler_arquivo_post_alocando();

            atualizar_curtida_no_post(variavel.posts, variavel.n_posts, ID_postagem);



            
            //pega um array com todas as curtidas_t
            curtidas_e_quantidade = ler_arquivo_curtidas_alocando_extra();

            //reabre o arquivo para escrita
            arquivo_curtidas = fopen("arquivo_curtidas.bin", "wb");
            if (arquivo_curtidas == NULL) {
                printf("Nao foi possivel abrir o arquivo de curtidas\n");
            }
            else {



                if (!achou) {
                    
                    //copia os valores da nova curtida para local adequado nesse array
                    strcpy(curtidas_e_quantidade.curtidas[curtidas_total - 1].ID_post, ID_postagem);
                    strcpy(curtidas_e_quantidade.curtidas[curtidas_total - 1].pessoa, perfil_logado.ID);

                    //escreve no arquivo a "nova curtida"
                    fwrite(&curtidas_total, sizeof(int), 1, arquivo_curtidas);
                    fwrite(curtidas_e_quantidade.curtidas, sizeof(curtida_t), curtidas_total, arquivo_curtidas);
                    fclose(arquivo_curtidas);

                }
                else {
                   
                    //coloca a curtida em uma 'curitda_t'
                    strcpy(curtida_oficial.ID_post, ID_postagem);
                    strcpy(curtida_oficial.pessoa, perfil_logado.ID);
                    //coloca a nova curtida no lugar certo no array
                    concerta_curtida(curtidas_total, curtidas_antes, curtidas_e_quantidade.curtidas, curtida_oficial);
                    //escreve a curtida no arquivo
                    fwrite(&curtidas_total, sizeof(int), 1, arquivo_curtidas);
                    fwrite(curtidas_e_quantidade.curtidas, sizeof(curtida_t), curtidas_total, arquivo_curtidas);
                    fclose(arquivo_curtidas);
                }
            }

        }
    }
    


}
//le a curtida de um post e aloca em um curtida_t*
curtidas_e_n_curtidas_t ler_curtidas_de_um_post_alocando(char* ID_postagem) {
    FILE* arquivo_curtidas = 0;
    curtida_t curtida_aux;
    post_e_n_posts_t posts_e_quantidade;
    int lugar_do_post = 0, n_curtidas_do_post = 0, n_curtidas_antes = 0, n_curtidas_total = 0;
    curtida_t* curtidas = 0;
    curtidas_e_n_curtidas_t curtidas_e_quantidade;


    arquivo_curtidas = fopen("arquivo_curtidas.bin", "a+b");
    if (arquivo_curtidas == NULL) {
        printf("Nao foi possivel abrir o arquivo de curtidas\n");
    }
    else {
        //acha o post em que estamos e pega a quantidade de curtidas que ele tem
        posts_e_quantidade = ler_arquivo_post_alocando();
        for (lugar_do_post = 0;lugar_do_post < posts_e_quantidade.n_posts;lugar_do_post++) {
            if (strcmp(posts_e_quantidade.posts[lugar_do_post].ID, ID_postagem) == 0) {
                break;
            }
        }
        n_curtidas_do_post = posts_e_quantidade.posts[lugar_do_post].n_curtidas;
        //entra caso haja curtidas no post
        if (n_curtidas_do_post > 0) {

            //aloca espaco para n curtidas
            curtidas = (curtida_t*)malloc(sizeof(curtida_t) * n_curtidas_do_post);
            //acha onde esta no arquivo
            fread(&n_curtidas_total, sizeof(int), 1, arquivo_curtidas);
            do {
                
                fread(&curtida_aux, sizeof(curtida_t), 1, arquivo_curtidas);
                if (strcmp(curtida_aux.ID_post, ID_postagem) == 0) {
                    break;
                }
                else {
                    n_curtidas_antes++;
                }

            } while (feof(arquivo_curtidas) == 0);
            //coloca o cursor no local certo
            fseek(arquivo_curtidas, sizeof(curtida_t) * n_curtidas_antes + sizeof(int), SEEK_SET);
            //le as curtidas do post
            fread(curtidas, sizeof(comentario_t), n_curtidas_do_post, arquivo_curtidas);
            fclose(arquivo_curtidas);
            curtidas_e_quantidade.curtidas = curtidas;
            curtidas_e_quantidade.n_curtidas = n_curtidas_do_post;
            return curtidas_e_quantidade;
        }
        else {
            //retorna 0 curtidas se nao houver nenhuma
            curtidas_e_quantidade.curtidas = NULL;
            curtidas_e_quantidade.n_curtidas = 0;
            return curtidas_e_quantidade;
        }
        

    }


}

//verifica se o perfil_logado ja curtiu o post
bool curtiu(perfil_t perfil_logado, char* ID_postagem) {
    FILE* arquivo_curtidas = 0;
    int i = 0;
    curtidas_e_n_curtidas_t curtidas_e_quantidade;
    //abre o arquivo para "leitura"
    arquivo_curtidas = fopen("arquivo_curtidas.bin", "a+b");
    if (arquivo_curtidas == NULL) {
        printf("Nao foi possivel abrir o arquivo de curtidas\n");
    }
    else {
        curtidas_e_quantidade = ler_curtidas_de_um_post_alocando(ID_postagem);
        for (i = 0;i < curtidas_e_quantidade.n_curtidas;i++) {
            if (strcmp(curtidas_e_quantidade.curtidas[i].pessoa, perfil_logado.ID) == 0) {
                return true;
            }
        }
        return false;
    }
}
//funcao para "descurtir" uma postagem
void descurtir(perfil_t perfil_logado, char* ID_postagem) {
    FILE* arquivo_curtidas = 0;
    FILE* arquivo_postagens = 0;
    curtidas_e_n_curtidas_t curtidas_e_quantidade;
    int lugar_no_arquivo = 0, i = 0;
    post_e_n_posts_t postagens_e_quantidade;
    
    //aloca todas as curtidas
    curtidas_e_quantidade = ler_arquivo_curtidas_alocando();
    //acha onde o perfil curtiu no arquivo
    for (lugar_no_arquivo = 0;lugar_no_arquivo < curtidas_e_quantidade.n_curtidas;lugar_no_arquivo++) {
        if (strcmp(curtidas_e_quantidade.curtidas[lugar_no_arquivo].ID_post, ID_postagem) == 0 && strcmp(curtidas_e_quantidade.curtidas[lugar_no_arquivo].pessoa, perfil_logado.ID) == 0) {
            break;
        }
    }
    //remove essa curtida_t e coloca as outras no lugar, sobrando um espaco "vazio" no final
    for (i=lugar_no_arquivo;i<curtidas_e_quantidade.n_curtidas-1;i++){
        curtidas_e_quantidade.curtidas[i] = curtidas_e_quantidade.curtidas[i + 1];
    }
    //diminui a quantidade de curtidas em -1
    curtidas_e_quantidade.n_curtidas--;
    //abre o arquivo para escrever as curtidas sem a "retirada"
    arquivo_curtidas = fopen("arquivo_curtidas.bin", "wb");
    if (arquivo_curtidas == NULL) {
        printf("Nao foi possivel abrir o arquivo de curtidas\n");
    }
    else {
        fwrite(&curtidas_e_quantidade.n_curtidas, sizeof(int), 1, arquivo_curtidas);
        fwrite(curtidas_e_quantidade.curtidas, sizeof(curtida_t), curtidas_e_quantidade.n_curtidas, arquivo_curtidas);
        fclose(arquivo_curtidas);
    }

    //escreve o novo numero de curtidas no arquivo de postagens
    //le as postagens
    postagens_e_quantidade = ler_arquivo_post_alocando();
    //acha o local da postagem que estamos
    for (i = 0;i < postagens_e_quantidade.n_posts; i++) {
        if (strcmp(postagens_e_quantidade.posts[i].ID, ID_postagem) == 0) {

            break;
        }
    }
    postagens_e_quantidade.posts[i].n_curtidas--;
    //abre o arquivo para reescrever o numero de curtidas
    arquivo_postagens = fopen("arquivo_postagens.bin", "wb");
    if (arquivo_postagens == NULL) {
        printf("Nao foi possivel abrir o arquivo de postagens");
    }
    else {
        fwrite(&postagens_e_quantidade.n_posts, sizeof(int), 1, arquivo_postagens);
        fwrite(postagens_e_quantidade.posts, sizeof(post_t), postagens_e_quantidade.n_posts, arquivo_postagens);
        fclose(arquivo_postagens);
    }

}


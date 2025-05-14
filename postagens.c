#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "ascii_img.h"
#include "utilidade_geral.h"
#include "postagens.h"
#include "comentarios_e_curtidas.h"
#include "perfil.h"

//definicoes de tamanho//
#define NUMERO_DE_FOTOS_POR_POST 10
#define TAMANHO_MAXIMO_ID 50
#define TAMANHO_DO_POST 120



// estruturas de postagem//

//atualiza o numero de posts no arquivo de perfis e de perfis logados
void atualiza_posts_perfil(perfil_t perfil_logado, perfil_t* perfis, int n_perfis){
    FILE * arquivo_perfil;
    FILE* arquivo_perfil_logado;
    int i =0;
    //aumenta em 1 o numero de postagens no perfil logado
    perfil_logado.n_postagens_total++;
    perfil_logado.n_postagens_real++;

    

    arquivo_perfil = fopen("arquivo_perfil.bin", "wb");
    //abre ambos os arquivos
    if (arquivo_perfil == NULL){
        printf("Nao foi possivel abrir o arquivo de perfil\n");
    }else {
        arquivo_perfil_logado = fopen("arquivo_perfil_logado.bin", "wb");
        if (arquivo_perfil_logado == NULL){
            printf("Nao foi possivel abrir o arquivo de perfil logado\n");
        }else {
            //escre o novo numero de postagens no perifil
            fwrite(&perfil_logado, sizeof(perfil_t),1,arquivo_perfil_logado);
            //acha o perfil em todos os perfis e modifica a quantidade de postagens neles
            for (i=0;i< n_perfis;i++){
                if (strcmp(perfis[i].ID, perfil_logado.ID) == 0){
                    perfis[i].n_postagens_real++;
                    perfis[i].n_postagens_total++;
                    break;
                }
            }
            //escreve o perfil atualizado no arquivo de perfis
            fwrite(&n_perfis, sizeof(int), 1, arquivo_perfil);
            fwrite(perfis, sizeof(perfil_t), n_perfis, arquivo_perfil);
            fclose (arquivo_perfil);
            fclose(arquivo_perfil_logado);

        }
    }
}
//le o arquivo do post e aloca espaco para mais um
post_e_n_posts_t ler_arquivo_post_alocando_extra() {
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
        n_postagens++;
        posts = (post_t*)malloc(sizeof(post_t) * n_postagens);
        fread(posts, sizeof(post_t), n_postagens, arquivo_posts);
        fclose(arquivo_posts);
        variavel.n_posts = n_postagens;
        variavel.posts = posts;

        return variavel;
    }
}
//coloca a postagem no lugar certo do array
void concerta_postagem(int n_postagens, int n_antes, post_t* postagens, post_t postagem_nova) {
    int i = 0, pos_array = 0, fim_array = 0;
    pos_array = n_antes;
    fim_array = n_postagens - 1;


    for (i = fim_array; i > pos_array;i--) {
        postagens[i] = postagens[i-1];
    
    }
    postagens[pos_array] = postagem_nova;
}
//salva a postagem no arquivo
void salvar_postagem_no_arquivo(post_t postagem_nova)
{   
    FILE* arquivo_postagens;
    post_e_n_posts_t postagens_e_quantidade;
    int lugar_postagem = 0;
    bool achou = false;

    //aloca espaco para as postagens ja existentes e uma a mais
    postagens_e_quantidade = ler_arquivo_post_alocando_extra();
    //acha o local das postagens de um perfil
    for (lugar_postagem = 0; lugar_postagem < postagens_e_quantidade.n_posts-1; lugar_postagem++) {
        //compara todos os IDs do perfil das postagens com o ID do perfil da nova postagem, se forem iguais, achou o lugar que a nova postagem deve ser inserida
        if (strcmp(postagens_e_quantidade.posts[lugar_postagem].ID_perfil, postagem_nova.ID_perfil) == 0) {
            achou = true;
            break;
        }
    }

    arquivo_postagens = fopen("arquivo_postagens.bin", "wb");
    if (arquivo_postagens == NULL) {
        printf("Nao foi possivel abrir o arquivo de postagens\n");
    }
    else {
        if (achou) {
            //coloca a nova postagem no lugar correto
            concerta_postagem(postagens_e_quantidade.n_posts, lugar_postagem, postagens_e_quantidade.posts, postagem_nova);
            //escreve a postagem no arquivo
            fwrite(&postagens_e_quantidade.n_posts, sizeof(int), 1, arquivo_postagens);
            fwrite(postagens_e_quantidade.posts, sizeof(post_t), postagens_e_quantidade.n_posts, arquivo_postagens);
            fclose(arquivo_postagens);
            
        }
        else {
            //coloca a nova postagem no lugar
            postagens_e_quantidade.posts[postagens_e_quantidade.n_posts-1] = postagem_nova;
            //escreve a postagem no arquivo
            fwrite(&postagens_e_quantidade.n_posts, sizeof(int), 1, arquivo_postagens);
            fwrite(postagens_e_quantidade.posts, sizeof(post_t), postagens_e_quantidade.n_posts, arquivo_postagens);
            fclose(arquivo_postagens);
        }
    }


    
   




}
//funcao que pede ao usuario para colocar imagens
void postar_imagem(post_t* postagem)
{
    bool colorido = true;
    char url[URL_QUANTIDADE_MAX][TAMANHO_MAXIMO_URL];
    int opcao;
    asciiImg_t ** imagem_enviada = NULL;
    int contador = 0;
    bool ferramenta;
    int i = 0;
   
    do
    {
       

        printf("deseja colocar uma imagem?\t[MAXIMO 10 imagens por postagem]?\n[1] - sim\n[0] - nao:\n");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            getchar();

            contador++;

          

            if(contador == 1)
            {   
                imagem_enviada = (asciiImg_t**)malloc(sizeof(asciiImg_t)*contador);
            }else
            {
                    if(contador <= 10){
                        imagem_enviada = (asciiImg_t**)realloc(imagem_enviada,sizeof(asciiImg_t)*contador);
                    }
                    
                
            }
          

               
               
                if (contador <= 10) {

                
                printf("digite a URL da foto que voce deseja postar:\t");
                fgets(url[contador-1],TAMANHO_MAXIMO_URL,stdin);
                util_removeQuebraLinhaFinal(url[contador-1]);

              
                ferramenta =  asciiImg_verificaFerramenta();
                

                if(ferramenta == false)
                {
                    printf("erro na criacao do arquivo\n");

                }else{
               
               imagem_enviada[contador-1] = asciiImg_carregaImagem(url[contador-1],colorido,TAMANHO_DO_POST);

               if(imagem_enviada[contador-1] != NULL)
               {
                    asciiImg_imprimeImagem(imagem_enviada[contador-1]);
                    asciiImg_liberaImagem(imagem_enviada[contador-1]);                   

               }else
               {
                    printf("algo deu errado , tente novamente\n");
                    
               }     

                }
                }else{
                        printf("voce alcancou o maximo de imagens em uma postagens\n");
                        opcao = 0;
                        break;
                    }
             
        }
        else if (opcao == 0)
        {
            printf("saindo...\n");
        }
        else {
            printf("opcao invalida tente novamente...\n");
        }
      
    } while (opcao != 0);    
    postagem->n_imagens = contador;
    for (i=0;i<contador;i++){
        strcpy(postagem->url[i], url[i]);
    }

}
//funcao para realizar uma postagem
void postar(){
    post_t postagem_nova;
    perfil_e_registros_t perfis_e_quantidade;
    perfil_e_registros_t perfil_e_quantidade;
    FILE* arquivo_perfil;
    int i=0;
    char n_posts_string[STRING_DE_NUMEROS_TAMANHO_MAXIMO];
    perfil_t perfil_logado;
    //le todos os perfis e sua quantidade
    perfis_e_quantidade = ler_arquivo_alocando();
    //pega as informacoes do perfil logado
    ler_arquivo_login(&perfil_logado);

    //pergunta para o usuario e pega a descricao a ser colocada na postagem
    printf("Digite a descricao da postagem a ser feita:[MAX %i]\n", DESCRICAO_TAMANHO_MAX);
    getchar();
    fgets(postagem_nova.descricao, DESCRICAO_TAMANHO_MAX, stdin);
    util_removeQuebraLinhaFinal(postagem_nova.descricao);

    //atualiza a quantidade de postagens nos arquivos
    atualiza_posts_perfil(perfil_logado, perfis_e_quantidade.perfil, perfis_e_quantidade.n_registros);
    //atualiza outros dados da postagem
    strcpy(postagem_nova.ID_perfil, perfil_logado.ID);
    postagem_nova.n_curtidas = 0;
    postagem_nova.n_comentarios_total = 0;
    postagem_nova.n_comentarios_real = 0;

    //atualiza quantidade de postagens na variavel de perfil recebida
    perfil_logado.n_postagens_total++;
    perfil_logado.n_postagens_real++;

    //cria um ID para a postagem baseado na quantidade de postagens de um perfil e no ID do perfil logado
    for (i=0;i<strlen(perfil_logado.ID);i++){
        postagem_nova.ID[i] = perfil_logado.ID[i];
    }

    sprintf(n_posts_string, "%d", perfil_logado.n_postagens_total);
    for (i=0;i<(strlen(n_posts_string));i++){
        postagem_nova.ID[i + strlen(perfil_logado.ID)] = n_posts_string[i];
    }
    postagem_nova.ID[strlen(perfil_logado.ID)+strlen(n_posts_string)] = '\0';
    //funcao para postar as imagens do post
    printf("Colocar imagens:\n\n");
    postar_imagem(&postagem_nova);
    //salva as postagens
    salvar_postagem_no_arquivo(postagem_nova);


    

}

//le e aloca espaco para os posts de um perfil especifico
post_e_n_posts_t ler_arquivo_posts_de_um_perfil(perfil_t perfil) {
    FILE* arquivo_posts = 0;
    int n_postagens_perfil = 0, lugar_postagem = 0, i = 0;
    post_e_n_posts_t posts_total_e_quantidade, posts_perfil_e_quantidade;
    //pega o numero de postagens do perfil
    n_postagens_perfil = perfil.n_postagens_real;
    posts_perfil_e_quantidade.n_posts = n_postagens_perfil;
    //verifica se ele nao postou nada
    if (n_postagens_perfil == 0) {
        return posts_perfil_e_quantidade;
    }
    else {

        //aloca espaco para todas as postagens
        posts_total_e_quantidade = ler_arquivo_post_alocando();
        //verifica onde as postagens do perfil inserido estao
        for (lugar_postagem = 0;lugar_postagem < posts_total_e_quantidade.n_posts;lugar_postagem++) {
            if (strcmp(posts_total_e_quantidade.posts[lugar_postagem].ID_perfil, perfil.ID) == 0) {
                break;
            }
        }
        n_postagens_perfil = perfil.n_postagens_real;
        posts_perfil_e_quantidade.n_posts = n_postagens_perfil;
        //aloca espaco para a quantidade de postagens desse perfil
        posts_perfil_e_quantidade.posts = (post_t*)malloc(sizeof(post_t) * n_postagens_perfil);
        //copia as postagens do perfil para outra variavel
        for (i = 0;i < n_postagens_perfil;i++) {
            posts_perfil_e_quantidade.posts[i] = posts_total_e_quantidade.posts[lugar_postagem + i];
        }
        return posts_perfil_e_quantidade;
    }


    
}
//funcao para carregar imagens iniciais de posts
void carregar_postagens(perfil_t perfil_visitado, int endereco1) {
    post_e_n_posts_t posts_perfil_e_quantidade;
    asciiImg_t** imagem_enviada = NULL;
    bool colorido = true, ferramenta;

    posts_perfil_e_quantidade = ler_arquivo_posts_de_um_perfil(perfil_visitado);
    if (posts_perfil_e_quantidade.n_posts == 0) {
        printf("\t\tO perfil acessado nao possui postagens\t\n");
        
    }
    else {
        imagem_enviada = (asciiImg_t**)malloc(sizeof(asciiImg_t) * 1);

        imagem_enviada[0] = asciiImg_carregaImagem(posts_perfil_e_quantidade.posts[endereco1].url[0], colorido, TAMANHO_DO_POST);

        ferramenta = asciiImg_verificaFerramenta();

        if (ferramenta == false)
        {
            printf("erro na criacao do arquivo de imagem\n");

        }
        else {

            imagem_enviada[0] = asciiImg_carregaImagem(posts_perfil_e_quantidade.posts[endereco1].url[0], colorido, TAMANHO_DO_POST);

            if (imagem_enviada[0] != NULL)
            {
                printf("[%i/%i] ------ POSTAGEM de %s\n\n\n", endereco1 + 1, perfil_visitado.n_postagens_real, perfil_visitado.ID);
                asciiImg_imprimeImagem(imagem_enviada[0]);
                asciiImg_liberaImagem(imagem_enviada[0]);
                printf("\n\n\n");

            }
            else
            {
                printf("Nao foi possivel abrir a imagem do post %i\n", endereco1 + 1);

            }


            free(imagem_enviada);
            free(posts_perfil_e_quantidade.posts);
        }
    }
}
//funcao para carregar as imagens de um perfil detalhado
void carregar_imagens(perfil_t perfil_visitado, int endereco_postagem, int endereco_imagem) {
    post_e_n_posts_t posts_perfil_e_quantidade;
    asciiImg_t** imagem_enviada = NULL;
    bool colorido = true, ferramenta;

    posts_perfil_e_quantidade = ler_arquivo_posts_de_um_perfil(perfil_visitado);
    
    imagem_enviada = (asciiImg_t**)malloc(sizeof(asciiImg_t) * 1);

    imagem_enviada[0] = asciiImg_carregaImagem(posts_perfil_e_quantidade.posts[endereco_postagem].url[endereco_imagem], colorido, TAMANHO_DO_POST);

    ferramenta = asciiImg_verificaFerramenta();

    if (ferramenta == false)
        {
            printf("erro na criacao do arquivo de imagem\n");

        }
    else {

            imagem_enviada[0] = asciiImg_carregaImagem(posts_perfil_e_quantidade.posts[endereco_postagem].url[endereco_imagem], colorido, TAMANHO_DO_POST);

            if (imagem_enviada[0] != NULL)
            {
                printf("[%i/%i] - IMAGEM\n\n", endereco_imagem + 1, posts_perfil_e_quantidade.posts[endereco_postagem].n_imagens);
                asciiImg_imprimeImagem(imagem_enviada[0]);
                asciiImg_liberaImagem(imagem_enviada[0]);
                printf("\n\n\n");

            }
            else
            {
                printf("Nao foi possivel abrir a %i imagem\n", endereco_imagem + 1);

            }


            free(imagem_enviada);
            free(posts_perfil_e_quantidade.posts);
    }
    

   
}
//funcao que tira a postagem do array
post_e_n_posts_t remove_postagem(int postagem_escolhido, int* posicoes_das_postagens, post_e_n_posts_t postagens_e_quantidade) {
    int i;
    for (i = posicoes_das_postagens[postagem_escolhido];i < postagens_e_quantidade.n_posts - 1;i++) {
        postagens_e_quantidade.posts[i] = postagens_e_quantidade.posts[i + 1];
    }
    postagens_e_quantidade.n_posts--;
    return postagens_e_quantidade;
}
//funcao para deletar postagem
void deletar_postagem(perfil_t perfil_logado) {

    FILE* arquivo_postagens = 0;
    FILE* arquivo_perfil = 0;
    FILE* arquivo_perfil_logado = 0;
    post_t postagem_nova;
    int i, lugar_no_arquivo = 0;
    int lugar_postagem = 0;
    int achou = 0, postagem_escolhida = 0, deletadas = 0;
    post_e_n_posts_t postagens_e_quantidade;
    perfil_e_registros_t perfil_e_quantidade;
    int* posicoes_das_postagens = 0;
    char opcao = 0;
    

    postagens_e_quantidade = ler_arquivo_post_alocando();
    for (lugar_no_arquivo = 0;lugar_no_arquivo < postagens_e_quantidade.n_posts;lugar_no_arquivo++) {
        if (strcmp(postagens_e_quantidade.posts[lugar_postagem].ID_perfil, perfil_logado.ID) == 0) {
            achou++;
            if (achou == 1) {
                posicoes_das_postagens = (int*)malloc(sizeof(int) * achou);
                posicoes_das_postagens[achou - 1] = lugar_postagem;

            }
            else {
                posicoes_das_postagens = (int*)realloc(posicoes_das_postagens, sizeof(int) * achou);
                posicoes_das_postagens[achou - 1] = lugar_postagem;
            }
        }
    }
        if (achou == 0) {
            printf("Voce nao postou nada.E impossivel deletar a postagem \n");

        }
        else if (achou == 1) {
            do {
                printf("Voce quer deletar esse post? \n\n");
                carregar_postagens(perfil_logado, 0);
                printf("DESCRICAO:\n%s\n\n", postagens_e_quantidade.posts[posicoes_das_postagens[0]].descricao);
                printf("[S]-Sim\n[N]- Nao\n");
                scanf(" %c", &opcao);
                switch (opcao) {
                case 'S':
                    postagens_e_quantidade = remove_postagem(achou - 1, posicoes_das_postagens, postagens_e_quantidade);
                    deletadas++;
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
        else {
            printf("Aqui estao todas as suas postagens \n");
            for (i = 0;i < achou;i++) {
                carregar_postagens(perfil_logado, i);
                printf("DESCRICAO:\n%s\n\n", postagens_e_quantidade.posts[posicoes_das_postagens[i]].descricao);
            }
            for (i = 0;i < achou;i++) {
                do {
                    printf("Voce deseja apagar a postagem %i\n", i+1);
                    printf("[S] - Sim\n[N]-Nao\n");
                    scanf(" %c", &opcao);
                    switch (opcao) {
                    case'S':
                        postagens_e_quantidade = remove_postagem(i, posicoes_das_postagens, postagens_e_quantidade);
                        deletadas++;
                        break;
                    case 'N':
                        break;
                    default:
                        printf("A opcao escolhida nao existe,tente novamente\n");
                    }
                    if (opcao != 'N' || opcao != 'S') {
                        break;
                    }
                } while (opcao != 'N' || opcao != 'S');
            }
    }

        //abre o arquivo para escrever as postagens novamente
        arquivo_postagens = fopen("arquivo_postagens.bin", "wb");
        if (arquivo_postagens == NULL) {
            printf("Nao foi possivel abrir o arquivo de comentarios\n");
        }
        else {
            fwrite(&postagens_e_quantidade.n_posts, sizeof(int), 1, arquivo_postagens);
            fwrite(postagens_e_quantidade.posts, sizeof(post_t), postagens_e_quantidade.n_posts, arquivo_postagens);
            fclose(arquivo_postagens);
        }
        //escreve o novo numero de postagens no arquivo de perfil e de login
        //le os perfis
        perfil_e_quantidade = ler_arquivo_alocando();
        //acha o local do perfil que estamos
        for (i = 0;i < perfil_e_quantidade.n_registros; i++) {
            if (strcmp(perfil_e_quantidade.perfil[i].ID, perfil_logado.ID) == 0) {

                break;
            }
        }
        perfil_e_quantidade.perfil[i].n_postagens_real = perfil_e_quantidade.perfil[i].n_postagens_real - deletadas;
        //abre o arquivo para reescrever o numero de comentarios
        arquivo_perfil = fopen("arquivo_perfil.bin", "wb");
        if (arquivo_perfil == NULL) {
            printf("Nao foi possivel abrir o arquivo de perfil");
        }
        else {
            fwrite(&perfil_e_quantidade.n_registros, sizeof(int), 1, arquivo_perfil);
            fwrite(perfil_e_quantidade.perfil, sizeof(perfil_t), perfil_e_quantidade.n_registros, arquivo_perfil);
            fclose(arquivo_perfil);
        }
        arquivo_perfil_logado = fopen("arquivo_perfil_logado.bin", "wb");
        //escreve no arquivo do perfil logado
        if (arquivo_perfil_logado == NULL) {
            printf("Nao foi possivel abrir o arquivo de perfil logado");
        }
        else {
            
            fwrite(&perfil_e_quantidade.perfil[i], sizeof(perfil_t), 1, arquivo_perfil_logado);
            fclose(arquivo_perfil_logado);
        }
    
        
}
//funcao que entra em uma postagem especifica de um perfil para mostrar suas imagens, comentar, curtir, etc
void detalhar_postagem(perfil_t perfil_logado, bool logado, int endereco_postagem, perfil_t perfil_visitado) {
    int opcao = 0, i = 0;
    int endereco_imagem = 0;
    post_e_n_posts_t posts_perfil_e_quantidade;
    posts_perfil_e_quantidade = ler_arquivo_posts_de_um_perfil(perfil_visitado);
    comentario_t* comentarios = 0;
    curtidas_e_n_curtidas_t curtidas_e_quantidade;
    perfil_e_registros_t perfil_e_quantidade;
    perfil_e_quantidade.perfil = 0;


    if (perfil_visitado.n_postagens_real == 0) {
        printf("Nao e possivel detalhar uma postagem, pois esse perfil nao tem nenhuma\n");
    }
    else {

        if (logado) {
            do {
                //le o arquivo novamente para "atualizar" as informacoes do perfil visitado
                perfil_e_quantidade = ler_arquivo_alocando();
                for (i = 0;i < perfil_e_quantidade.n_registros; i++) {
                    if (strcmp(perfil_e_quantidade.perfil[i].ID, perfil_visitado.ID) == 0) {
                        perfil_visitado = perfil_e_quantidade.perfil[i];
                        break;
                    }
                }
                if (!curtiu(perfil_logado, posts_perfil_e_quantidade.posts[endereco_postagem].ID)) {

                    posts_perfil_e_quantidade = ler_arquivo_posts_de_um_perfil(perfil_visitado);
                    printf("Postagem %i de %s\n\n", endereco_postagem + 1, perfil_visitado.ID);
                    carregar_imagens(perfil_visitado, endereco_postagem, endereco_imagem);

                    printf("DESCRICAO:\n%s\n", posts_perfil_e_quantidade.posts[endereco_postagem].descricao);
                    printf("CURTIDAS: %i\nCOMENTARIOS: %i\n", posts_perfil_e_quantidade.posts[endereco_postagem].n_curtidas, posts_perfil_e_quantidade.posts[endereco_postagem].n_comentarios_real);

                    /*
                    Espaco das acoes dentro da postagem
                    */

                    printf("Deseja realizar alguma acao?\n");
                    printf("[0] - Sair\n[1] - Passar para a proxima imagem\n[2] - Passar para a imagem anterior\n[3] - curtir\n[4] - Ver quem curtiu\n[5] - Comentar\n[6] - Deletar comentario\n[7] - Editar comentario\n[8] - Ver todos os comentarios\n");
                    scanf(" %i", &opcao);
                    switch (opcao) {
                    case 0:
                        opcao = -1;
                        break;
                    case 1:
                        if (endereco_imagem + 1 >= posts_perfil_e_quantidade.posts[endereco_postagem].n_imagens) {
                            printf("Nao e possivel passar para a proxima imagem, pois esse perfil so postou essas\n");
                        }
                        else {
                            endereco_imagem++;
                        }
                        break;
                    case 2:
                        if (endereco_imagem == 0) {
                            printf("Nao e possivel passar para a imagem anterior, pois essa e a primeira imagem\n");
                        }
                        else {
                            endereco_imagem--;
                        }
                        break;
                    case 3:
                        curtir(posts_perfil_e_quantidade.posts[endereco_postagem].ID, perfil_logado);
                        break;
                    case 4:
                        curtidas_e_quantidade = ler_curtidas_de_um_post_alocando(posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        printf("USUARIOS QUE CURTIRAM: \n");
                        for (i = 0;i < posts_perfil_e_quantidade.posts[endereco_postagem].n_curtidas; i++) {
                            printf("%s\n", curtidas_e_quantidade.curtidas[i].pessoa);
                        }
                        free(curtidas_e_quantidade.curtidas);
                        break;
                    case 5:
                        adicionar_cometario_no_arquivo(posts_perfil_e_quantidade.posts[endereco_postagem].ID, perfil_logado);
                        break;
                    case 6:
                        deletar_comentario(perfil_logado, posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        break;
                    case 7:
                        editar_comentario(perfil_logado, posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        break;
                    case 8:
                        comentarios = ler_comentarios_de_um_post_alocando(posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        printf("COMENTARIOS:\n");
                        for (i = 0;i < posts_perfil_e_quantidade.posts[endereco_postagem].n_comentarios_real;i++) {
                            printf("%s:\n%s\n", comentarios[i].pessoa, comentarios[i].texto);
                        }
                        break;



                    default:
                        printf("Essa nao é uma opcao\n");
                    }

                }
                else {

                    posts_perfil_e_quantidade = ler_arquivo_posts_de_um_perfil(perfil_visitado);
                    printf("Postagem %i de %s\n\n", endereco_postagem + 1, perfil_visitado.ID);
                    carregar_imagens(perfil_visitado, endereco_postagem, endereco_imagem);

                    printf("DESCRICAO:\n%s\n", posts_perfil_e_quantidade.posts[endereco_postagem].descricao);
                    printf("CURTIDAS: %i\nCOMENTARIOS: %i\n", posts_perfil_e_quantidade.posts[endereco_postagem].n_curtidas, posts_perfil_e_quantidade.posts[endereco_postagem].n_comentarios_real);

                    /*
                    Espaco das acoes dentro da postagem
                    */

                    printf("Deseja realizar alguma acao?\n");
                    printf("[0] - Sair\n[1] - Passar para a proxima imagem\n[2] - Passar para a imagem anterior\n[3] - descurtir\n[4] - Ver quem curtiu\n[5] - Comentar\n[6] - Deletar comentario\n[7] - Editar comentario\n[8] - Ver todos os comentarios\n");
                    scanf(" %i", &opcao);
                    switch (opcao) {
                    case 0:
                        opcao = -1;
                        break;
                    case 1:
                        if (endereco_imagem + 1 >= posts_perfil_e_quantidade.posts[endereco_postagem].n_imagens) {
                            printf("Nao e possivel passar para a proxima imagem, pois esse perfil so postou essas\n");
                        }
                        else {
                            endereco_imagem++;
                        }
                        break;
                    case 2:
                        if (endereco_imagem == 0) {
                            printf("Nao e possivel passar para a imagem anterior, pois essa e a primeira imagem\n");
                        }
                        else {
                            endereco_imagem--;
                        }
                        break;
                    case 3:
                        descurtir(perfil_logado, posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        break;
                    case 4:
                        curtidas_e_quantidade = ler_curtidas_de_um_post_alocando(posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        printf("USUARIOS QUE CURTIRAM: \n");
                        for (i = 0;i < posts_perfil_e_quantidade.posts[endereco_postagem].n_curtidas; i++) {
                            printf("%s\n", curtidas_e_quantidade.curtidas[i].pessoa);
                        }
                        free(curtidas_e_quantidade.curtidas);
                        break;
                    case 5:
                        adicionar_cometario_no_arquivo(posts_perfil_e_quantidade.posts[endereco_postagem].ID, perfil_logado);
                        break;
                    case 6:
                        deletar_comentario(perfil_logado, posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        break;
                    case 7:
                        editar_comentario(perfil_logado, posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        break;
                    case 8:
                        comentarios = ler_comentarios_de_um_post_alocando(posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                        printf("COMENTARIOS:\n");
                        for (i = 0;i < posts_perfil_e_quantidade.posts[endereco_postagem].n_comentarios_real;i++) {
                            printf("%s:\n%s\n", comentarios[i].pessoa, comentarios[i].texto);
                        }
                        break;


                    default:
                        printf("Essa nao é uma opcao\n");
                    }


                }
            } while (opcao != -1);
        }
        else {
            do {
                printf("Postagem %i de %s\n\n", endereco_postagem + 1, perfil_visitado.ID);
                carregar_imagens(perfil_visitado, endereco_postagem, endereco_imagem);

                printf("DESCRICAO:\n%s\n", posts_perfil_e_quantidade.posts[endereco_postagem].descricao);
                printf("CURTIDAS: %i\nCOMENTARIOS: %i\n", posts_perfil_e_quantidade.posts[endereco_postagem].n_curtidas, posts_perfil_e_quantidade.posts[endereco_postagem].n_comentarios_real);

                /*
                Espaco das acoes dentro da postagem
                */

                printf("Deseja realizar alguma acao?\n");
                printf("[0] - Sair\n[1] - Passar para a proxima imagem\n[2] - Passar para a imagem anterior\n[3] - Ver todos os comentarios\n[4] - Ver quem curtiu\n");
                scanf(" %i", &opcao);
                switch (opcao) {
                case 0:
                    opcao = -1;
                    break;
                case 1:
                    if (endereco_imagem + 1 >= posts_perfil_e_quantidade.posts[endereco_postagem].n_imagens) {
                        printf("Nao e possivel passar para a proxima imagem, pois esse perfil so postou essas\n");
                    }
                    else {
                        endereco_imagem++;
                    }
                    break;
                case 2:
                    if (endereco_imagem == 0) {
                        printf("Nao e possivel passar para a imagem anterior, pois essa e a primeira imagem\n");
                    }
                    else {
                        endereco_imagem--;
                    }
                    break;
                case 3:
                    comentarios = ler_comentarios_de_um_post_alocando(posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                    printf("COMENTARIOS:\n");
                    for (i = 0;i < posts_perfil_e_quantidade.posts[endereco_postagem].n_comentarios_real;i++) {
                        printf("%s:\n%s\n", comentarios[i].pessoa, comentarios[i].texto);
                    }
                    break;
                case 4:
                    curtidas_e_quantidade = ler_curtidas_de_um_post_alocando(posts_perfil_e_quantidade.posts[endereco_postagem].ID);
                    printf("USUARIOS QUE CURTIRAM: \n");
                    for (i = 0;i < posts_perfil_e_quantidade.posts[endereco_postagem].n_curtidas; i++) {
                        printf("%s\n", curtidas_e_quantidade.curtidas[i].pessoa);
                    }
                    free(curtidas_e_quantidade.curtidas);
                    break;


                default:
                    printf("Essa nao é uma opcao\n");
                }
            } while (opcao != -1);
        }
        free(perfil_e_quantidade.perfil);
        free(comentarios);
        free(posts_perfil_e_quantidade.posts);
    }

}
//funcao que pergunta para um usuario o que deve ser editado na postagem
post_e_n_posts_t muda_postagem(int postagem_escolhida, int* posicoes_das_postagens, post_e_n_posts_t postagens_e_quantidade, perfil_t perfil_logado) {
    int opcao = 0, opcao2 = 0;
    int endereco_imagem = 0;
    do {
        getchar();
        printf("Voce deseja mudar a descricao da postagem?\n[1] - Sim\n[0] - Nao\n");
        scanf(" %i", &opcao);
        switch (opcao) {
        case 0:
            opcao = -1;
            break;
        case 1:
            getchar();
            printf("Digite a nova descriacao:\n");
            fgets(postagens_e_quantidade.posts[posicoes_das_postagens[postagem_escolhida]].descricao, DESCRICAO_TAMANHO_MAX, stdin);
            util_removeQuebraLinhaFinal(postagens_e_quantidade.posts[posicoes_das_postagens[postagem_escolhida]].descricao);
            opcao = -1;
            break;

        }
    } while (opcao != -1);
    do {

        printf("Voce deseja mudar as imagens da postagem?\n[1] - Sim\n[0] - Nao\n");
        scanf(" %i", &opcao);
        switch (opcao) {
        case 0:
            opcao = -1;
            break;
        case 1:
            do {
               

                carregar_imagens(perfil_logado, postagem_escolhida, endereco_imagem);
                printf("Voce deseja alterar essa imagem?\n[0] - Nao\n[1] - Sim\n");
                scanf(" %i", opcao2);
                switch (opcao2) {
                case 0:
                    if (endereco_imagem + 1 >= perfil_logado.n_postagens_real) {
                        opcao2 = -1;
                    }
                    else {


                        endereco_imagem++;
                    }
                    break;
                case 1:
                    printf("Digite o novo url\n");
                    fgets(postagens_e_quantidade.posts[posicoes_das_postagens[postagem_escolhida]].url[endereco_imagem], TAMANHO_MAXIMO_URL, stdin);
                    util_removeQuebraLinhaFinal(postagens_e_quantidade.posts[posicoes_das_postagens[postagem_escolhida]].url[endereco_imagem]);

                    break;
                }
            } while (opcao2 != -1);
            break;


        }
    } while (opcao != -1);
    return postagens_e_quantidade;

}
//funcao que edita uma postagem
void editar_postagem(perfil_t perfil_logado) {
    FILE* arquivo_postagens = 0;
    post_t postagem_nova;
    int i, lugar_no_arquivo = 0;
    int lugar_postagem = 0;
    int achou = 0, postagem_escolhida = 0;
    post_e_n_posts_t postagens_e_quantidade;
    perfil_e_registros_t perfil_e_quantidade;
    int* posicoes_das_postagens = 0;
    char opcao = 0;

    //acha as postagens do arquivo e conta quantas tem
    postagens_e_quantidade = ler_arquivo_post_alocando();
    for (lugar_no_arquivo = 0;lugar_no_arquivo < postagens_e_quantidade.n_posts;lugar_no_arquivo++) {
        if (strcmp(postagens_e_quantidade.posts[lugar_postagem].ID_perfil, perfil_logado.ID) == 0) {
            achou++;
            if (achou == 1) {
                posicoes_das_postagens = (int*)malloc(sizeof(int) * achou);
                posicoes_das_postagens[achou - 1] = lugar_postagem;

            }
            else {
                posicoes_das_postagens = (int*)realloc(posicoes_das_postagens, sizeof(int) * achou);
                posicoes_das_postagens[achou - 1] = lugar_postagem;
            }
        }
    }
    if (achou == 0) {
        printf("Voce nao postou nada.E impossivel editar a postagem \n");

    }
    else if (achou == 1) {
        do {
            printf("Voce quer editar esse post? \n\n");
            carregar_postagens(perfil_logado, 0);
            printf("DESCRICAO:\n%s\n\n", postagens_e_quantidade.posts[posicoes_das_postagens[0]].descricao);
            printf("[S]-Sim\n[N]- Nao\n");
            scanf(" %c", &opcao);
            switch (opcao) {
            case 'S':
                postagens_e_quantidade = muda_postagem(achou - 1, posicoes_das_postagens, postagens_e_quantidade, perfil_logado);
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
    else {
        printf("Aqui estao todas as suas postagens \n");
        for (i = 0;i < achou;i++) {
            carregar_postagens(perfil_logado, i);
            printf("DESCRICAO:\n%s\n\n", postagens_e_quantidade.posts[posicoes_das_postagens[i]].descricao);
        }
        for (i = 0;i < achou;i++) {
            do {
                printf("Voce deseja editar a postagem %i\n", i + 1);
                printf("[S] - Sim\n[N]-Nao\n");
                scanf(" %c", &opcao);
                switch (opcao) {
                case'S':
                    postagens_e_quantidade = muda_postagem(i, posicoes_das_postagens, postagens_e_quantidade,perfil_logado);
                    break;
                case 'N':
                    break;
                default:
                    printf("A opcao escolhida nao existe,tente novamente\n");
                }
                if (opcao != 'N' || opcao != 'S') {
                    break;
                }
            } while (opcao != 'N' || opcao != 'S');
        }
    }

    //abre o arquivo para escrever as postagens novamente
    arquivo_postagens = fopen("arquivo_postagens.bin", "wb");
    if (arquivo_postagens == NULL) {
        printf("Nao foi possivel abrir o arquivo de comentarios\n");
    }
    else {
        fwrite(&postagens_e_quantidade.n_posts, sizeof(int), 1, arquivo_postagens);
        fwrite(postagens_e_quantidade.posts, sizeof(post_t), postagens_e_quantidade.n_posts, arquivo_postagens);
        fclose(arquivo_postagens);
    }
    

}



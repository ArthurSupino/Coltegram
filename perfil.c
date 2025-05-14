
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "utilidade_geral.h"
#include "comentarios_e_curtidas.h"
#include "perfil.h"
#include "postagens.h"



#define SUCESSO 0

//defines do perfil
#define ID_PERFIL_TAMANHO_MAX 50
#define NOME_TAMANHO_MAX 50
#define EMAIL_TAMANHO_MAX 50
#define SENHA_TAMANHO_MAX 50

//defines do post
#define ID_POST_TAMANHO_MAX 50
#define DESCRICAO_TAMANHO_MAX 500




//defines do comentario


//defines extras
#define ERROR_MESSAGE_TAMANHO_MAX 50
#define ERRO_NAO_HA_CONTA -1
#define PERFIL_ACESSADO 0 
#define PERFIL_NAO_ACESSADO 1






//funcoes de utilidade geral




//procura conta e retorna o seu numero no array - sem arquivo

int procura_conta(char* string, perfil_t* contas, int n_cadastros) {

    int i = 0, numero_conta = -1;

    for (i = 0;i < n_cadastros; i++) {
        if (strcmp(string, contas[i].Email) == 0) {
            numero_conta = i;
            break;
        }
        if (strcmp(string, contas[i].ID) == 0) {
            numero_conta = i;
            break;
        }
    }
    if (numero_conta == -1) {
        printf("email nao corresponde a nenhuma conta\n");
        return ERRO_NAO_HA_CONTA;
    }
    else {
        return numero_conta;
    }
}

//funcao para ler o arquivo
perfil_e_registros_t ler_arquivo_alocando() {
    FILE* arquivo_perfil;
    perfil_t* perfil = 0;
    int n_registros = 0;
    perfil_e_registros_t variavel;

    arquivo_perfil = fopen("arquivo_perfil.bin", "a+b");

    if (arquivo_perfil == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        fread(&n_registros, sizeof(int), 1, arquivo_perfil);
        perfil = (perfil_t*)malloc(sizeof(perfil_t) * n_registros);
        if (feof(arquivo_perfil)) {
            variavel.perfil = NULL;
            variavel.n_registros = 0;
            return variavel;
        }else {
        fread(perfil, sizeof(perfil_t), n_registros, arquivo_perfil);
        fclose(arquivo_perfil);
        variavel.n_registros = n_registros;
        variavel.perfil = perfil;

        return variavel;
        }
    }


}
bool ler_arquivo_login(perfil_t* perfil_logado) {
    FILE* arquivo_perfil_logado = 0;
    
    

    arquivo_perfil_logado = fopen("arquivo_perfil_logado.bin", "rb");

    if (arquivo_perfil_logado == NULL) {
        return false;
        
    }
    else {
        fread(perfil_logado, sizeof(perfil_t), 1, arquivo_perfil_logado);
        fclose(arquivo_perfil_logado);
        return true;
    }
}

//funcoes para o cadastro - com arquivo
void colocar_no_arquivo(perfil_t* perfil, int n_registros) {
    FILE* arquivo_perfil;


    arquivo_perfil = fopen("arquivo_perfil.bin", "wb");

    if (arquivo_perfil == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        fwrite(&n_registros, sizeof(int), 1, arquivo_perfil);
        fwrite(perfil, sizeof(perfil_t), n_registros, arquivo_perfil);
        fclose(arquivo_perfil);
    }
    free(perfil);
}
perfil_e_registros_t ler_arquivo_alocando_extra(perfil_t* perfil) {
    FILE* arquivo_perfil;
    int n_registros = 0;
    perfil_e_registros_t variavel;

    arquivo_perfil = fopen("arquivo_perfil.bin", "a+b");

    if (arquivo_perfil == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
        
    }
    else {
        fread(&n_registros, sizeof(int), 1, arquivo_perfil);
        n_registros++;
        perfil = (perfil_t*)malloc(sizeof(perfil_t) * n_registros);
        fread(perfil, sizeof(perfil_t), n_registros, arquivo_perfil);
        fclose(arquivo_perfil);
        variavel.n_registros = n_registros;
        variavel.perfil = perfil;

        return variavel;
    }


}
bool corrigir_email(char* email) {

    uint8_t i, j;
    bool validacao_email;
    uint8_t validacao_ponto = 0;
    uint8_t validacao_arroba = 0;
    uint8_t contador = 0, contador_do_ponto = 0;
    uint8_t validacao_de_maisculos = 0;
    uint8_t tamanho_email = strlen(email);





    validacao_email = false;



    for (i = 0; i < tamanho_email; i++)
    {
        if (email[i] == '@')
        {

            validacao_arroba = 1;
            break;
        }
        contador++;
    }

    for (j = contador; j < tamanho_email; j++)
    {
        if (email[j] == '.')
        {

            validacao_ponto = 1;
            break;

        }
        contador_do_ponto++;
    }
    contador_do_ponto = contador_do_ponto + contador;

    for (i = 0; i < tamanho_email; i++)
    {

        if (isupper(email[i]) == false)
        {
            validacao_de_maisculos = 1;

        }
        else
        {
            validacao_de_maisculos = 0;
            break;
        }
    }

    if (validacao_arroba == 1 && validacao_ponto == 1 && validacao_de_maisculos == 1)
    {
        validacao_email = true;
        return validacao_email;
    }
    else
    {
        printf("email invalido tente novamente : \n\n");
        validacao_email = false;
        return validacao_email;
    }



}
void cadastro_perfil() {

    perfil_t* perfil = 0;
    bool verificar_email = false;
    int n_registros = 0, ultimo_indice = 0;
    perfil_e_registros_t variavel;

    variavel = ler_arquivo_alocando_extra(perfil);
    perfil = variavel.perfil;
    n_registros = variavel.n_registros;


    ultimo_indice = n_registros - 1;

    getchar();
    printf("digite seu nome de usuario:\t");
    fgets(perfil[ultimo_indice].Nome, NOME_TAMANHO_MAX, stdin);
    util_removeQuebraLinhaFinal(perfil[ultimo_indice].Nome);
    printf("digite seu ID(O que ira aparecer para todos):\t");
    fgets(perfil[ultimo_indice].ID, ID_PERFIL_TAMANHO_MAX, stdin);
    util_removeQuebraLinhaFinal(perfil[ultimo_indice].ID);
    do {
        printf("digite seu email:\t");
        fgets(perfil[ultimo_indice].Email, EMAIL_TAMANHO_MAX, stdin);
        util_removeQuebraLinhaFinal(perfil[ultimo_indice].Email);
        verificar_email = corrigir_email(perfil[ultimo_indice].Email);
    } while (verificar_email == false);


    printf("digite sua senha:\t");
    fgets(perfil[ultimo_indice].Senha, SENHA_TAMANHO_MAX, stdin);
    util_removeQuebraLinhaFinal(perfil[ultimo_indice].Senha);
    //garante que o numero de postagens de um perfil comece com 0
    perfil[ultimo_indice].n_postagens_real = 0;
    perfil[ultimo_indice].n_postagens_total = 0;
    colocar_no_arquivo(perfil, n_registros);

}


//quicksort Email
int particao_email(perfil_t* array, int inicio, int fim) {
    int lugar_de_troca = inicio - 1;
    int i = 0;
    perfil_t pivo, string_auxiliar;

    pivo = array[fim];

    for (i = inicio;i <= fim; i++) {
        if (strcmp(pivo.Email, array[i].Email) >= 0) {
            lugar_de_troca++;
            string_auxiliar = array[i];
            array[i] = array[lugar_de_troca];
            array[lugar_de_troca] = string_auxiliar;


        }
    }
    return lugar_de_troca;

}
void quicksort_email(perfil_t* array, int inicio, int fim) {
    int pivo_pos = 0;

    if (inicio == fim) {

    }
    else {
        pivo_pos = particao_email(array, inicio, fim);
        if (pivo_pos != inicio) {
            quicksort_email(array, inicio, pivo_pos - 1);
        }
        if (pivo_pos != fim) {
            quicksort_email(array, pivo_pos + 1, fim);
        }
    }



}
//quicksort nome
int particao_nome(perfil_t* array, int inicio, int fim) {
    int lugar_de_troca = inicio - 1;
    int i = 0;
    perfil_t pivo, string_auxiliar;

    pivo = array[fim];

    for (i = inicio;i <= fim; i++) {
        if (strcmp(pivo.Nome, array[i].Nome) >= 0) {
            lugar_de_troca++;
            string_auxiliar = array[i];
            array[i] = array[lugar_de_troca];
            array[lugar_de_troca] = string_auxiliar;


        }
    }
    return lugar_de_troca;

}
void quicksort_nome(perfil_t* array, int inicio, int fim) {
    int pivo_pos = 1;
    if (inicio == fim) {

    }
    else {
        pivo_pos = particao_nome(array, inicio, fim);
        if (pivo_pos != inicio) {
            quicksort_nome(array, inicio, pivo_pos - 1);
        }
        if (pivo_pos != fim) {
            quicksort_nome(array, pivo_pos + 1, fim);
        }
    }



}
//quicksort ID
int particao_ID(perfil_t* array, int inicio, int fim) {
    int lugar_de_troca = inicio - 1;
    int i = 0;
    perfil_t pivo, string_auxiliar;

    pivo = array[fim];

    for (i = inicio;i <= fim; i++) {
        if (strcmp(pivo.ID, array[i].ID) >= 0) {
            lugar_de_troca++;
            string_auxiliar = array[i];
            array[i] = array[lugar_de_troca];
            array[lugar_de_troca] = string_auxiliar;


        }
    }
    return lugar_de_troca;

}
void quicksort_ID(perfil_t* array, int inicio, int fim) {
    int pivo_pos = 0;

    if (inicio == fim) {

    }
    else {
        pivo_pos = particao_ID(array, inicio, fim);
        if (pivo_pos != inicio) {
            quicksort_ID(array, inicio, pivo_pos - 1);
        }
        if (pivo_pos != fim) {
            quicksort_ID(array, pivo_pos + 1, fim);
        }
    }



}

//funcao de um perfil alheio
int acessar_perfil(perfil_t perfil_acessado){
    int i = 0, endereco = 0;
    int opcao = 0;
    bool logado;
    perfil_t perfil_logado;
    perfil_e_registros_t perfil_e_quantidade;
    //mostra as informacoes do perfil
    printf("\n|Nome do Usuario:\t%-50s|\n", perfil_acessado.Nome);
    printf("|ID do Usuario:\t\t%-50s|\n", perfil_acessado.ID);
    printf("|Email do Usuario:\t%-50s|\n", perfil_acessado.Email);
    printf("|Senha do Usuario:\t%-50s|\n", perfil_acessado.Senha);
    printf("\n");
    for (i=0;i<75;i++){
        printf("-");
    }
    printf("\n");
    /*
    Espaco reservado para colocar as postagens do perfil
    */

    do {
        //le o arquivo novamente para "atualizar" as informacoes do perfil visitado
        perfil_e_quantidade = ler_arquivo_alocando();
        for (i = 0;i < perfil_e_quantidade.n_registros; i++) {
            if (strcmp(perfil_e_quantidade.perfil[i].ID, perfil_acessado.ID) == 0) {
                perfil_acessado = perfil_e_quantidade.perfil[i];
                break;
            }
        }

        logado  = ler_arquivo_login(&perfil_logado);
        carregar_postagens(perfil_acessado, endereco);

        /*
        Espaco das acoes dentro do  perfil
        */

        printf("Deseja realizar alguma acao?\n");
        printf("[0] - Sair\n[1] - Passar para a proxima postagem\n[2] - Passar para a postagem anterior\n[3] - Detalhar a postagem acima\n");
        scanf(" %i", &opcao);
        switch (opcao) {
        case 0:
            opcao = -1;
            break;
        case 1:
            if (endereco + 1 >= perfil_acessado.n_postagens_real) {
                printf("Nao e possivel passar para a proxima postagem, pois esse perfil so postou essas\n");
            }
            else {
                endereco++;
            }
            break;
        case 2:
            if (endereco == 0) {
                printf("Nao e possivel passar para a postagem anterior, pois essa e a primeira postaegm\n");
            }
            else {
                endereco--;
            }
            break;
        case 3:
            detalhar_postagem(perfil_logado, logado, endereco, perfil_acessado);
            break;

        default:
            printf("Essa nao é uma opcao\n");
        }
    } while (opcao != -1);

}
//funcao que pergunta a um usuario se quer entrar em um perfil
int visitar_perfil(perfil_t* perfil){
    int opcao = -1, valor_retornado = -1;
    
    do{
    printf("\nVoce gostaria de visitar algum perfil?\n");
    printf("Digite:\n[0] - Voltar ao menu\nDigite o 'numero' de um perfil para acessa-lo\n\n");
    scanf(" %i", &opcao);
    if(opcao == 0){
        break;
    }else {
        valor_retornado = acessar_perfil(perfil[opcao-1]);
        return 0;
    }
    }while (opcao != -1);
    return 1;
}


//funcao que serve para achar um perfil alheio e mostra-lo
void tabela_pesquisa_perfil(int n_achados, perfil_t* perfis_encontrados) {
    int i = 0;
    do{
    printf("|");
    for (i = 0;i < 214;i++) {
        printf("-");
    }
    printf("|\n");
    printf("|%-10s|%-50s|%-50s|%-50s|%-50s|\n|", "Numeros", "ID", "Nome", "Email", "Senha");

    for (i = 0;i < 214;i++) {
        printf("-");
    }
    printf("|\n");
    for (i = 0;i < n_achados; i++) {
        printf("|%-10i|%-50s|%-50s|%-50s|%-50s|\n", i + 1, perfis_encontrados[i].ID, perfis_encontrados[i].Nome, perfis_encontrados[i].Email, perfis_encontrados[i].Senha);
    }
    printf("|");
    for (i = 0;i < 214;i++) {
        printf("-");
    }
    printf("|\n");
    }while (visitar_perfil(perfis_encontrados) == 0);
    free(perfis_encontrados);
}
void pesquisa_perfil(char* pesquisa, char tipo)
{
    int i = 0, tamanho = 0, n_registros = 0, n_achados = 0;
    perfil_t* perfis_encontrados = 0;
    perfil_t perfil_auxiliar;
    FILE* arquivo_perfil = 0;


    tamanho = strlen(pesquisa);

    arquivo_perfil = fopen("arquivo_perfil.bin", "rb");

    if (arquivo_perfil == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        fread(&n_registros, sizeof(int), 1, arquivo_perfil);
        for (i = 0;i < n_registros; i++) {
            fread(&perfil_auxiliar, sizeof(perfil_t), 1, arquivo_perfil);
            if (strstr(perfil_auxiliar.Email, pesquisa) != NULL || strstr(perfil_auxiliar.Nome, pesquisa) != NULL) {
                n_achados++;
                if (n_achados == 1) {
                    perfis_encontrados = (perfil_t*)malloc(sizeof(perfil_t) * 1);
                }
                else {
                    perfis_encontrados = (perfil_t*)realloc(perfis_encontrados, sizeof(perfil_t) * n_achados);
                }
                perfis_encontrados[n_achados - 1] = perfil_auxiliar;
            }

        }
        fclose(arquivo_perfil);

    }
    if (n_achados > 0) {




        switch (tipo) {
        case 'N':
            quicksort_nome(perfis_encontrados, 0, n_achados - 1);
            break;
        case 'E':
            quicksort_email(perfis_encontrados, 0, n_achados - 1);
            break;
        case 'I':
            quicksort_ID(perfis_encontrados, 0, n_achados - 1);
            break;

        }
        tabela_pesquisa_perfil(n_achados, perfis_encontrados);
    }
    else {
        printf("Nao ha nenhum perfil composto pela chave digitada\n");
    }




}
void buscar_perfil() {
    char chave[NOME_TAMANHO_MAX];
    char tipo = 0;

    printf("Digite a chave de busca para o perfil [pode ser nome ou email]\n");
    printf("chave:  ");
    getchar();
    fgets(chave, NOME_TAMANHO_MAX, stdin);
    util_removeQuebraLinhaFinal(chave);
    printf("Digite a forma de ordenacao desejada\n[N] - por nome\n[E] - por email\n[I] - por ID\n");
    scanf(" %c", &tipo);
    pesquisa_perfil(chave, tipo);
}
//funcao que lista todos os perfis

void lista_todos_perfis(perfil_t* perfil, int quantidade)
{

    int i = 0;

    do {
    printf("\n\t lista de perfis\n|");
    for (i = 0;i < 214; i++) {
        printf("-");
    }
    printf("|\n");
    printf("|%-10s|%-50s|%-50s|%-50s|%-50s|\n|", "Numeros", "Nome", "Id", "Email", "Senha");
    for (i = 0;i < 214; i++) {
        printf("-");
    }
    printf("|\n");
    for (i = 0;i < quantidade;i++) {

        printf("|%-10d|%-50s|%-50s|%-50s|%-50s|\n", i + 1, perfil[i].Nome, perfil[i].ID, perfil[i].Email, perfil[i].Senha); 
    }

    printf("|");
    for (i = 0;i < 214; i++) {
        printf("-");
    }
    printf("|\n\n");
    } while(visitar_perfil(perfil) == 0);
    free(perfil);
}
void listar_como() {
    char opcao = 0;
    perfil_t* perfil = 0;
    int quantidade = 0;
    perfil_e_registros_t variavel;
    variavel = ler_arquivo_alocando();
    if (variavel.n_registros == 0){
        printf("\nNao ha nenhum perfil no coltegram\n");

    }else {

    
    perfil = variavel.perfil;
    quantidade = variavel.n_registros;

    printf("Como deseja fazer a listagem dos perfis?\n");
    printf("[N] - por Nome\n[E] - por Email\n[I] - por ID\n[0] - voltar ao menu\n");
    scanf(" %c", &opcao);
    switch (opcao) {
    case 'N':
        quicksort_nome(perfil, 0, quantidade - 1);
        lista_todos_perfis(perfil, quantidade);
        break;
    case 'E':
        quicksort_email(perfil, 0, quantidade - 1);
        lista_todos_perfis(perfil, quantidade);
        break;
    case 'I':
        quicksort_ID(perfil, 0, quantidade - 1);
        lista_todos_perfis(perfil, quantidade);
        break;
    case '0':
        break;
    default:
        listar_como();
        break;
    }
    }

}

void deslogar_da_conta()
{
    FILE * arquivo_usuario_logado;
    arquivo_usuario_logado = fopen("arquivo_perfil_logado.bin","wb");
    fclose(arquivo_usuario_logado);
    remove("arquivo_perfil_logado.bin");
    

}



// menu de acoes do perfil do usuario logado - mostra a sua conta(no caso o perfil que mostra suas proprias postagens...)
void menu_do_usuario()
{
    perfil_t perfil_logado;
    int opcao = 0, i = 0, endereco = 0;
    ler_arquivo_login(&perfil_logado);
    //mostra as informacoes do perfil
    printf("|Nome do Usuario:\t%-50s|\n", perfil_logado.Nome);
    printf("|ID do Usuario:\t\t%-50s|\n", perfil_logado.ID);
    printf("|Email do Usuario:\t%-50s|\n", perfil_logado.Email);
    printf("|Senha do Usuario:\t%-50s|\n\n", perfil_logado.Senha);
    for (i=0;i<75;i++){
        printf("-");
    }
    printf("\n");
    /*
    Espaco reservado para colocar as postagens do perfil
    */

    do {
        ler_arquivo_login(&perfil_logado);
        carregar_postagens(perfil_logado, endereco);

        /*
        Espaco das acoes dentro do proprio perfil
        */

        printf("Deseja realizar alguma acao?\n");
        printf("[0] - Sair\n[1] - Passar para a proxima postagem\n[2] - Passar para a postagem anterior\n[3] - Adicionar uma postagem\n[4] - Deletar uma postagem\n[5] - Editar uma postagem\n[6] - Detalhar a postagem acima\n");
        scanf(" %i", &opcao);
        switch (opcao) {
        case 0:
            opcao = -1;
            break;
        case 1:
            if (endereco + 1 >= perfil_logado.n_postagens_real) {
                printf("Nao e possivel passar para a proxima postagem, pois esse perfil so postou essas\n");
            }
            else {
                endereco++;
            }
            break;
        case 2:
            if (endereco == 0) {
                printf("Nao e possivel passar para a postagem anterior, pois essa e a primeira postaegm\n");
            }
            else {
                endereco--;
            }
            break;
        case 3:
            postar();
            break;
        case 4:
            deletar_postagem(perfil_logado);


            break;
        case 5:
            editar_postagem(perfil_logado);
            break;
        case 6:
            detalhar_postagem(perfil_logado, true, endereco, perfil_logado);
            break;
        default:
            printf("Essa nao é uma opcao\n");
        }
    } while (opcao != -1 );


}
//menu que da opcoes ao usuario apos o login
char menu_apos_login()
{
    int opcao = 0;
    perfil_t perfil_logado;
    comentario_t* comentarios = 0;
    char id[2] = { '1' };

    do
    {
        printf("\n[1] - Buscar um perfil\n[2] - Listar os perfis\n[3] - Acessar sua conta\n[4] - Deslogar\n[5] - Sair do Coltegram\n");
        scanf(" %d", &opcao);
    
        switch (opcao)
        {
        
        case 4 :
            deslogar_da_conta();
            opcao = 0;
            return 'T';
            break;
        case 1:
            buscar_perfil();
            getchar();
            break;

        case 2:
            listar_como();
            getchar();
            break;

        case 3:
            menu_do_usuario();
            break;
        case 5:
            return 'S';

            break;
        case 6:

            ler_arquivo_login(&perfil_logado);
            adicionar_cometario_no_arquivo(id, perfil_logado);
            break;
        case 7:
            comentarios = ler_comentarios_de_um_post_alocando(id);
            printf("%s", comentarios[0].texto);
            break;
        case 8:
            ler_arquivo_login(&perfil_logado);
            curtir(id, perfil_logado);
            break;
        case 9:

            break;

        default:
            printf("opcao invalido tente novamente\n");
            break;
        }
    } while (opcao != 0);
    return 'F';
}


//funcoes de login
perfil_t perfil_logado_ler() {
    FILE* arquivo_perfil_logado = 0;
    perfil_t perfil_logado;

    arquivo_perfil_logado = fopen("arquivo_perfil_logado.bin", "rb");
    if (arquivo_perfil_logado = NULL) {
        printf("Nao foi possivel abrir o arquivo\n");

    }
    else {
        fread(&perfil_logado, sizeof(perfil_t), 1, arquivo_perfil_logado);
        fclose(arquivo_perfil_logado);
        return perfil_logado;
    }

}
void arquivo_perfil_logado(perfil_t perfil_logado) {
    FILE* arquivo_perfil_logado;

    arquivo_perfil_logado = fopen("arquivo_perfil_logado.bin", "wb");

    if (arquivo_perfil_logado == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
    }
    else {
        fwrite(&perfil_logado, sizeof(perfil_t), 1, arquivo_perfil_logado);
        fclose(arquivo_perfil_logado);
    }
}
int login()
{

    bool email_correto;
    bool senha_correta;
    bool id_correto;
    bool login_bem_sucedido = false;
    char string[EMAIL_TAMANHO_MAX];
    char senha[SENHA_TAMANHO_MAX];
    char ID[ID_PERFIL_TAMANHO_MAX];
    int voltar = 0;

    int local_conta = -2;
    int acessar_perfil = 0;
    perfil_t* informacoes_para_login = 0;
    int n_registros;
    perfil_e_registros_t variavel;

    variavel = ler_arquivo_alocando();
    informacoes_para_login = variavel.perfil;
    n_registros = variavel.n_registros;

    int registros_array = n_registros - 1;



    do {
        do
        {
            getchar();
            printf("digite seu Email ou seu ID\t");
            fgets(string, EMAIL_TAMANHO_MAX, stdin);
            util_removeQuebraLinhaFinal(string);

            local_conta = procura_conta(string, informacoes_para_login, n_registros);


            if (local_conta == ERRO_NAO_HA_CONTA) {
                break;
            }
            else {



                printf("digite sua senha\t");
                fgets(senha, SENHA_TAMANHO_MAX, stdin);
                util_removeQuebraLinhaFinal(senha);

                if (strcmp(senha, informacoes_para_login[local_conta].Senha) == 0)
                {
                    senha_correta = true;
                }
                else
                {
                    senha_correta = false;
                }

                if (senha_correta == true)
                {
                    login_bem_sucedido = true;
                    voltar = 1;
                }

                else
                {
                    login_bem_sucedido = false;
                    printf("seu login esta errado\n");
                    printf("[0] - tentar novamente\n");
                    printf("[1] - voltar para o menu\n\n");
                    scanf(" %d", &voltar);

                }
            }

        } while (voltar == 0);
        if (login_bem_sucedido == true) {
            printf("deseja acessar sua conta?\n[1] - Sim\n[0] - Nao\n");
            scanf("%d", &acessar_perfil);
        }


        if (acessar_perfil == 1)
        {
            arquivo_perfil_logado(informacoes_para_login[local_conta]);
            free(informacoes_para_login);
            return PERFIL_ACESSADO;
        }
        else if (acessar_perfil > 1 || acessar_perfil < 0)
        {
            printf("opcao invalida tente novamente\n");
        }
        else
        {
            free(informacoes_para_login);
            return PERFIL_NAO_ACESSADO;
        }
    } while (acessar_perfil != 1);




}






/*
Esse espaco sera destinado para as postagens e suas funcoes
postagens:
*/




//menu principal
char menu_principal() {
    int sair = 0;
    int numero_registros = 0;
    


    
    printf("Bem vindo ao Instagram!\n");
    do {
        printf("\n\n");
        printf("|Ja tem uma conta? -> faca o login (1)\n");
        printf("|Nao tem ainda? -> cadastre-se (2)\n|\n");
        printf("|Ver os perfis do instagram (3)\n");
        printf("|Buscar por um perfil (4)\n\n\n");
        printf("digite 0 para sair\n\n\n");

        scanf(" %d", &sair);


        switch (sair) {
        case 1:
            sair  = login();

            break;

        case 2:
            cadastro_perfil();

            break;
        case 3:
            listar_como();
            break;
        case 4:
            buscar_perfil();
            break;
        case 0:
            return 'S';
            break;
        default:
            printf("Nao existe essa opcao\n");

        }






    
    }while (sair != 0);
    return 'F';
}




#include "perfil.h"
#include <stdio.h>
#include <stdbool.h>

#define SUCESSO 0


int main(int argc, char** argv) {
    char deslogado = 'F';
    
        do {
        perfil_t perfil_logado;
        bool tem_perfil = false;
        tem_perfil = ler_arquivo_login(&perfil_logado);
        if (tem_perfil) {
            deslogado = menu_apos_login();
            if (deslogado == 'S') {
                break;
            }else if(deslogado == 'T'){
                deslogado = menu_principal();
            }
        }
        else{ 

            deslogado = menu_principal();
            

            }
        }while(deslogado != 'S');
        return SUCESSO;
}
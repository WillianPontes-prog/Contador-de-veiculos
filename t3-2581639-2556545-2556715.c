/**=====================================================================================================================================**/
/*EQUIPE:   Alexandre Aires Amorim          (2581639)*/
/*          John William Souza Harrison     (2556545)*/
/*          Willian Pontes                  (2556715)*/
/**=====================================================================================================================================**/

#include "trabalho3.h"
#include <stdlib.h>
#include <stdio.h>

/**=====================================================================================================================================**/

/**TAMANHOS DOS VE�CULOS**/

#define MOTO_MIN 25
#define MEDIO_MIN 100
#define LONGO_MIN 390
#define MUITO_LONGO_MIN 720

/**=====================================================================================================================================**/

/*Propor��o da redu��o da imagem (tem que ser uma potencia de 2 maior do que 1)*/
#define REDUZ 8

/*Quantidade m�xima esperada de ve�culos*/
#define MAX_VEICULOS 100

/*A nova cor dos carros, ap�s a fun��o contaTamanho*/
#define WILL_COR 150

/*Tanto m�nimo de pixels para considerar na forma reduzida*/
#define PIXEL 5

/*Margem para considerar um pixel diferente o suficiente do fundo*/
#define MARGEM_NOVA 15

/**=====================================================================================================================================**/


/** Zera vetor :).
 *
 * Par�metros: Vetor e seu tamanho.
 *
 * Valor de retorno: Nada. */

void zeraVetor(int* vetor, int tam){
    int i;

    for(i=0; i<tam; i++)
        vetor[i] = 0;
}


/** Cria uma imagem preto e branco do mesmo tamanho que a original e separa o fundo dos ve�culos.
 *
 * Par�metros: Imagem e seu fundo.
 *
 * Valor de retorno: Ponteiro para a imagem criada. */

Imagem1C* separaFundoCriaPb (Imagem3C* img,Imagem3C* bg){
    int i,j,cor,cont;
    Imagem1C* img_bg;

    img_bg = criaImagem1C(img->largura,img->altura);

    for (i=0;i<img->altura;i++){
        for (j=0;j<img->largura;j++){
            cont = 0;
            for (cor=0;cor<3;cor++){
                /*Compara se um pixel � diferente o suficiente do fundo em cada canal de cor*/
                if ((img->dados[cor][i][j]<bg->dados[cor][i][j]-MARGEM_NOVA)||(img->dados[cor][i][j]>bg->dados[cor][i][j]+MARGEM_NOVA))
                    cont++;
            }
            /*Se, pelo menos, dois canais forem diferentes, o pixel recebe branco, se n�o, recebe preto*/
            if (cont >= 2)
                img_bg->dados[i][j] = 255;
            else
                img_bg->dados[i][j] = 0;
        }
    }
    return img_bg;
}


/** Filtra imagem.
 *
 * Par�metros: Imagem a ser filtrada.
 *
 * Valor de retorno: Nada. */

void filtro (Imagem1C*img){
    int i,j;

    /*Se um pixel branco tem um vizinho a direita ou a baixo que seja preto, ele vira preto*/
    for (i=1;i<img->altura-1;i++){
        for (j=1;j<img->largura-1;j++){
            if ((img->dados[i][j])){
                if (img->dados[i+1][j]==0 || img->dados[i][j+1]==0)
                    img->dados[i][j] = 0;
            }
        }
    }

    /*Faz uma segunda passagem e, se um pixel branco tem um vizinho acima e a baixo que seja preto, ele vira preto*/
    for(i=1; i<img->altura-1; i++){
        for(j=1; j<img->largura-1; j++){
            if ((img->dados[i][j])){
                if (img->dados[i+1][j] == 0 && img->dados[i-1][j] == 0){
                    img->dados[i][j] = 0;
                }
            }
        }
    }

    /*Faz uma terceira passagem e, se um pixel preto tem um vizinho a direita e a esquerda que seja branco, ele vira branco*/
    for (i=1;i<img->altura-1;i++){
        for (j=1;j<img->largura-1;j++){
            if (!img->dados[i][j]){
                if (img->dados[i][j+1]&&img->dados[i][j-1]){
                    img->dados[i][j] = 255;
                }
            }
        }
    }
}


/** Reduz a imagem e filtra ela um pouco mais.
 *
 * Par�metros: Imagem a ser reduzida.
 *
 * Valor de retorno: Ponteiro para imagem reduzida. */

Imagem1C* reduzImg (Imagem1C* img){
    int i,j,k,l,cont;

    /*Cria uma imagem nova imagem, em preto e branco, com a largura reduzida pelo par�metro "REDUZ" e faz a mesma coisa na altura, porem a multiplica por 2*/
    /*Fizemos isso na altura para que os carros n�o grudassem uns nos outros*/
    Imagem1C* nova_img;
    nova_img = criaImagem1C(img->largura/REDUZ,img->altura/REDUZ*2);

    /*Percorre a imagem inteira*/
    for (i=0;i<nova_img->altura;i++){
        for (j=0;j<nova_img->largura;j++){
            /*Reinicia a contagem de pixels brancos em cada "�rea" da imagem original*/
            cont = 0;

            /*Percorre cada uma das �reas, contando quantos pixels brancos tem na �rea*/
            for (k=REDUZ*i/2;k<REDUZ*(i+1)/2;k++){
                for (l=REDUZ*j;l<REDUZ*(j+1);l++){
                    if (img->dados[k][l]==255){
                        cont++;
                    }
                }
            }

            /*Se tiver pixels brancos o suficiente, a nova imagem recebe um pixel branco na posi��o correspondente a �rea*/
            if (cont>=PIXEL){
                nova_img->dados[i][j] = 255;
            }
            else{
                nova_img->dados[i][j] = 0;
            }
        }
    }


    /*Tira linhas e colunas de 1 de "grossura", comparando se um pixel branco tem vizinhos pretos na horizontal ou na vertical*/
    for(i=1; i<nova_img->altura-1; i++){
        for(j=1; j<nova_img->largura-1; j++){
            if ((nova_img->dados[i][j])){
                if (nova_img->dados[i+1][j] == 0 && nova_img->dados[i-1][j] == 0){
                    nova_img->dados[i][j] = 0;
                }
                if (!(nova_img->dados[i][j+1])&&!(nova_img->dados[i][j-1])){
                    nova_img->dados[i][j] = 0;
                }
            }
        }
    }

    /*Liga pixels horizontalmente que est�o pr�ximos o suficiente*/
    for (i=1;i<nova_img->altura-1;i++){
        for (j=1;j<nova_img->largura-1;j++){
            if (!nova_img->dados[i][j]){
                if (nova_img->dados[i][j+1]&&nova_img->dados[i][j-1]){
                    nova_img->dados[i][j] = 255;
                }
            }
        }
    }

    /*Destr�i a imagem antiga e retorna o ponteiro para a nova*/
    destroiImagem1C(img);
    return nova_img;
}


/** Completa �reas (ver relat�rio).
 *
 * Par�metros: Imagem e as coordenadas da posi��o inicial.
 *
 * Valor de retorno: Nada. */

void contaTamanho(Imagem1C* img,int posi, int posj){
    int i,j,cor,flag = 1;

    int pos[4];

    /*Os pixels que ser�o usados com par�metro sempre receber�o 2*/
    img->dados[posi][posj] = 2;


    pos[0] = img->altura-1;      //Armazena menor i
    pos[1] = 0;                  //Armazena maior i

    pos[2] = img->largura-1;     //Armazena menor j
    pos[3] = 0;                  //Armazena maior j

    /*Enquanto existir algum 2 na imagem*/
    while (flag){
        flag = 0;
        /*Procura por um 2*/
        for (i=1;i<img->altura-1;i++){
            for (j=1;j<img->largura-1;j++){
               /*Quando acha um 2 ...*/
                if (img->dados[i][j] == 2){

                    /*Se a posi��o em i for menor que a menor posi��o em i armazenada, ele a substitu�*/
                    if(i < pos[0]){
                        pos[0] = i;
                    }

                    /*Se a posi��o em i for maior que a maior posi��o em i armazenada, ele a substitu�*/
                    if(i > pos[1]){
                        pos[1] = i;
                    }

                    /*Se a posi��o em j for menor que a menor posi��o em j armazenada, ele a substitu�*/
                    if(j < pos[2]){
                        pos[2] = j;
                    }

                    /*Se a posi��o em j for maior que a maior posi��o em j armazenada, ele a substitu�*/
                    if(j > pos[3]){
                        pos[3] = j;
                    }


                    /*=======================================================*/
                    /*Quando um pixel ao redor for branco, ele trasforma em 2*/
                    if (img->dados[i+1][j] == 255){
                        img->dados [i+1][j]=2;
                    }

                    if (img->dados[i-1][j] == 255){
                        img->dados [i-1][j]=2;
                    }

                    if (img->dados[i][j+1] == 255){
                        img->dados [i][j+1]=2;
                    }

                    if (img->dados[i][j-1] == 255){
                        img->dados [i][j-1]=2;
                    }
                    /*=======================================================*/

                    /*Transforma a cor do pixel na cor de "WILL_COR"*/
                    img->dados [i][j] = WILL_COR;

                    /*Significa que achor um 2*/
                    flag = 1;

                }
            }
        }
    }

    /*Se a dist�ncia em i for menor igual a 3, a �rea ser� pintada de preto, se n�o, � pintada de "WILL_COR"*/
    cor = 0;
    if((pos[1] - pos[0]) > 3){
        cor = WILL_COR;
    }

    /*Pega as posi��es em i e j e as pinta corretamente (espero), formando um ret�ngulo*/
    for(i = pos[0]; i <= pos[1]; i++){
        for(j = pos[2]; j <= pos[3];  j++){
            img->dados[i][j] = cor;
        }
    }
}


/** Conta o tamanho de cada um dos ve�culos.
 *
 * Par�metros: Imagem (que j� passou por contaTamanho) e o vetor que ser� preenchido com o tamanho de cada um dos ve�culos.
 *
 * Valor de retorno: N�mero de posi��es usadas no vetor dos tamanhos. */

int plagueInc (Imagem1C* img, int tam_veiculos[]){
    int i, j, l, c, n, achou_1;

    /*�ndice do vetor ve�culos come�a em -1, pois vamos checar por ru�do primeiro antes de determinar se vale a pena gastar uma posi��o do vetor para armazenar o tamanho do ve�culo*/
    n = -1;

    /*Percorre a imagem*/
    for(i=0; i<img->altura-1; i++){
        for(j=0; j<img->largura-1; j++){
            /*Quando acha um pixel com cor "WILL_COR" ...*/
            if (img->dados[i][j] == WILL_COR){
                /*Transforma o pixel em preto*/
                img->dados[i][j] = 0;
                achou_1 = 0;

                /*=================================*/
                /*Se o pixel tiver, pelo menos, um vizinho que � WILL_COR, utiliza uma posi��o de tam_veiculos para armazenar o tamanho da �rea, adicionando um ao seu valor, e continua o programa*/
                if(img->dados[i+1][j] == WILL_COR){
                    img->dados[i+1][j] = 1;
                    achou_1 = 1;
                }

                if(img->dados[i][j+1] == WILL_COR){
                    img->dados[i][j+1] = 1;
                    achou_1 = 1;
                }
                if(achou_1){
                    n++;
                    tam_veiculos[n]++;
                }
                /*=================================*/

                /*Continua a contagem, percorrendo a imagem at� n�o achar mais 1*/
                while (achou_1){
                    achou_1 = 0;
                    for(l=1; l<img->altura-1 && !achou_1; l++){
                        for(c=1; c<img->largura-1 && !achou_1; c++){
                            /*Quando achar um 1 ...*/
                            if (img->dados[l][c] == 1){
                                /*Transforma o pixel em preto, adiciona um no valor na posi��o do vetor e "seta" a flag para 1*/
                                img->dados[l][c] = 0;
                                tam_veiculos[n]++;
                                achou_1 = 1;

                                /*=====================================================================================*/
                                /*Os vizinhos que s�o "WILL_COR" do pixel s�o contaminados por 1 (da� o nome da fun��o)*/
                                if (img->dados[l+1][c] == WILL_COR)
                                    img->dados[l+1][c] = 1;

                                if (img->dados[l][c+1] == WILL_COR)
                                    img->dados[l][c+1] = 1;

                                if (img->dados[l-1][c] == WILL_COR)
                                    img->dados[l-1][c] = 1;

                                if (img->dados[l][c-1] == WILL_COR)
                                    img->dados[l][c-1] = 1;
                                /*=====================================================================================*/
                            }
                        }
                    }
                }
            }
        }
    }

    return n;
}


/** Conta ve�culos.
 *
 * Par�metros: Imagem com ve�culos, backgroud e ponteiro para o vetor que armazena cada tipo de ve�culo.
 *
 * Valor de retorno: N�mero total de ve�culos. */

int contaVeiculos (Imagem3C* img, Imagem3C* bg, int contagem [N_TIPOS_DE_VEICULOS]){

    int ult_pos, i, j, total_veiculos = 0,
        tam_veiculos[MAX_VEICULOS] = {0};

    /*Imagem preto e branco criada para as outras fun��es*/
    Imagem1C* img_pb;

    zeraVetor(contagem,N_TIPOS_DE_VEICULOS);

    img_pb = separaFundoCriaPb(img,bg);

    filtro(img_pb);

    img_pb = reduzImg(img_pb);

    /*Percorre a fun��o procurando por um pixel branco*/
    for(i = 0; i < img_pb->altura; i++){
        for(j = 0; j <img_pb->largura; j ++){
            /*Quando acha, chama a contaTamanho*/
            if(img_pb->dados[i][j]==255){
                contaTamanho(img_pb,i,j);
            }
        }
    }

    ult_pos = plagueInc (img_pb, tam_veiculos);

    /*Categoriza os ve�culos e faz a contagem*/
    for(i=0; i<=ult_pos; i++){
        if (tam_veiculos[i]>MUITO_LONGO_MIN){
            total_veiculos++;
            contagem [3]++;
        }
        else if (tam_veiculos[i]>LONGO_MIN){
            total_veiculos++;
            contagem [2]++;
        }
        else if (tam_veiculos[i]>MEDIO_MIN){
            total_veiculos++;
            contagem [1]++;
        }
        else if (tam_veiculos[i]>MOTO_MIN){
            total_veiculos++;
            contagem [0]++;
        }
    }

    /*Destr�i a imagem auxiliar*/
    destroiImagem1C(img_pb);

    return total_veiculos;
}





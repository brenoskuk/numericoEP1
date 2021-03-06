#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <math.h>

#include "operacoes.h"
#include "barras.h"
#include "redes.h"

/**
A partir do struct barra le os dados e cria um vetor de barras
**/
barra** lerDadosBarras(char endereco[100], int* nBarras, int* nPQ, int* nPV, int *posPV, int *posSW){
    barra **b;
    double Vnom, leitura1, leitura2;
    int i, tipo, contQ, contV, linPv, linSW;
    FILE* dadosBarras;
    dadosBarras = fopen(endereco, "r");
    fscanf(dadosBarras, "%d", nBarras);

    //atencao: o malloc recebe o tamanho do struct
    b = malloc(*nBarras * sizeof(barra*));

    contQ = 0;
    contV = 0;
    while(!feof(dadosBarras)){

        fscanf(dadosBarras, "%d", &i);
        fscanf(dadosBarras, "%d", &tipo);
        fscanf(dadosBarras, "%lf", &Vnom);
        fscanf(dadosBarras, "%lf", &leitura1);
        fscanf(dadosBarras, "%lf", &leitura2);
        if (tipo == 0)
        {
            b[i] = newPQ(i,leitura1, leitura2, Vnom);
            contQ++;
        }
        else if (tipo == 1)
        {
            linPv = i;
            b[i] = newPV(i,leitura1, Vnom);
            contV++;
        }
        else if (tipo == 2)
        {
            linSW = i;
            b[i] = newSwing(i,leitura1, leitura2, Vnom);
        }
        else
        {
            printf("Erro na leitura da matriz de Barras");
        }


    }
    fclose(dadosBarras);

    *nPQ = contQ;
    *nPV = contV;
    *posPV = linPv;
    *posSW = linSW;
    return b;
}

/**
Cria nova barra sem SW e com PV na posicao 0
**/
barra** reorganizaBarras(barra **b, int nBarras)
{
    int i, k, temp;
    barra **br;


    //br tem o tamanho de nBarras - 1 pois exclui Swing
    br = malloc((nBarras-1) * sizeof(barra*));
    k=0;
    temp = -1;
    for(i=0; i<nBarras; i++)
    {
        //barra do tipo PQ vai para a posicao k de br
        if (b[i]->tipo == 0)
        {
            br[k] = newPQ(b[i]->indice, b[i]->ativaNom, b[i]->reativaNom, b[i]->vnominal);
            br[k]->ativaCalc = b[i]->ativaCalc;
            br[k]->reativaCalc = b[i]->reativaCalc;
            br[k]->V = b[i]->V;
            br[k]->fase=b[i]->fase;
            k++;
        }
        //guarda a posicao
        if (b[i]->tipo == 1)
        {
            temp = b[i]->indice;
        }
    }
    //guarda PV se existe
    if(temp != -1)
    {
        br[k]=newPV(b[temp]->indice, b[temp]->ativaNom, b[temp]->vnominal);
        br[k]->ativaCalc=b[temp]->ativaCalc;
        br[k]->fase=b[temp]->fase;
        br[k]->V=b[temp]->V;
        br[k]->reativaCalc=b[temp]->reativaCalc;
    }

    return br;
}
/**
Monta as matrizes de condutancias e suseptancias
 OBS: TOMA COMO PARAMETRO O TAMANHO OBTIDO POR getMatrizBarras
**/
void getMatrizAdmitancia(double **G, double **B, int tam, char fileName[100]){

    FILE *input;

    int i, ri, rj;

    int n;

    bool fileFound  = false;

    //printf("Entre com o nome do arquivo (extensao incluida):  ");
    //scanf("%[^\n]%*c", fileName);
    input = fopen(fileName,"r");
    if(input){
        fileFound=true;
    }
    else{
        printf("Erro: Arquivo nao encontrado!\n");
        fileFound=false;
    }

    if(fileFound){
        //le numero de linhas do .txt
        fscanf(input,"%i",&n);

        //aloca as matrizes de acordo com a ordem ja encontrada no .txt das barras

        while(!feof(input)){
            //Le as linhas do arquivo
            for(i=0;i<n;i++){
                fscanf(input, "%i", &ri);
                fscanf(input, "%i", &rj);
                fscanf(input, "%lf", &G[ri][rj]);
                fscanf(input, "%lf", &B[ri][rj]);
            }
        }
    }
}

/**
Preenche o vetor F1 dado um estado de barras da rede
      Fx tem tamanho 2n1 + n2
obs:  Assume que a barra 0 eh tipo Swing
**/
void termoConhecido(barra **b, int n1,int n2, double *Fx){
    int i, j, k, centro, nPV;

    centro = n1 + n2;
    //Calcula a parte de FP de F
    //Note que a barra swing nao contribui com o sistema de equacoes
    for(j=0; j < centro; j++)
    {
        Fx[j] = fp(j,b);
    }
    k=0;
   //Calcula a parte de FQ de F
    for(j = centro; j < centro + n1; j++)
    {
        Fx[j] = fq(k,b);
        k++;
    }

}

/**
Preenche o Jacobiano dado um estado de barras da rede

obs: Assume que a barra 0 eh tipo Swing
**/
void Jacobiana(double **J, int n1, int n2, barra **b, double **G, double **B) {
    int j, k, centro;
    centro = n1 + n2;
    //Calcula o primeiro "quadrante" do Jacobiano


    //Sao as derivadas parciais fpj com relacao a thetak
    for ( j = 0; j < centro; j++)
    {
        for (int k = 0; k < centro ; k++)
        {
            J[j][k] = Dfpj_Dtetak(j, k, b, G, B);
        }
    }
    //Calcula o segundo "quadrante" do Jacobiano

    //Sao as derivadas parciais fpj com relacao a Vk
    for (int j = centro ; j < centro + n1; j++)
    {
        for (int k = 0; k < centro; k++)
        {
            J[j][k] = Dfpj_DVk(j-centro, k, b, G, B);
        }
    }

    //Calcula o terceiro "quadrante" do Jacobiano

    //Sao as derivadas parciais fqj com relacao a Thetak
    for (int j = 0; j < centro; j++)
    {
        for (int k = centro; k < centro + n1; k++)
        {
            J[j][k] = Dfqj_Dthetak(j, k - centro, b, G, B);
        }
    }

    //Calcula o quarto "quadrante" do Jacobiano

    //Sao as derivadas parciais fqj com relacao a Vk
    for (int j = centro; j < centro + n1; j++)
    {
        for (int k = centro; k < centro + n1; k++)
        {
            J[j][k] = Dfqj_DVk(j-centro, k-centro, b, G, B);
        }
    }

}





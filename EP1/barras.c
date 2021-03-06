#include <stdlib.h>
#include "math.h"
#include "barras.h"
/**
A construcao do struct barra determina as condicoes inciais

**/

double tensao(barra* b){
    return b->V;
}

barra* newPQ    (int indice, double ativa,   double reativa,  double vNom){
    barra* p = malloc(sizeof(barra));

    //ativaEsp e reativaEsp sao nulos
    p->ativaEsp     = ativa;
    p->reativaEsp   = reativa;

    p->ativaNom     = ativa;
    p->reativaNom   = reativa;

    p->vnominal     = vNom;

    p->ativaCalc    = 0;
    p->reativaCalc  = 0;

    //Condicoes iniciais
    p->fase         = 0;
    p->V            = vNom;
    p->indice       = indice;
    p->tipo         = 0;

    return p;
}

barra* newPV    (int indice, double ativa, double vNom){
    barra* p = malloc(sizeof(barra));

    p->ativaEsp     = ativa;
    p->V            = vNom;
    p->vnominal     = vNom;


    p->reativaCalc  = 0;
    p->ativaCalc    = 0;

    p->fase         = 0;
    p->tipo         = 1;
    p->indice       = indice;
    return p;
}

barra* newSwing (int indice, double V,  double fase  ,  double vNom){
    barra* p = malloc(sizeof(barra));

    p->V        = V;
    p->fase     = fase;
    p->vnominal = vNom;

    p->tipo     = 2;
    p->indice       = indice;
    return p;
}




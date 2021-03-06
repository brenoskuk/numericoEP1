#include<stdio.h>
//Codigo da decomposicao LU com pivotamento como sugerido
//no enunciado do EP


double modulo(double val) {
  if (val < 0) {
    return -val;
  }
  else {
    return val;
  }
}

void decomposicaoLU(double **A, int *P, int n)
{
    double temp;
    int i,j,k,l;
    for(k=0; k<n; k++)
    {
        for(i=k; i<n; i++)
        {
            double sum = 0;
            for (j=0; j<k; j++)
            {
                sum = sum + A[i][j]*A[j][k];
            }
            A[i][k] = A[i][k] - sum;
        }
        //Pivotacao
        l = k;

        for (i=k; i<n; i++)
        {
            if(modulo(A[l][k]) < modulo(A[i][k]))
            {
                l=i;
            }
        }
        //depois deste for 'l' contem a posicao de maior modulo
        P[k] = l;
        if(P[k] != k)
        {
            //troca de linha de A e B:
             for(j=0; j<n; j++)
             {
                 temp = A[k][j];
                 A[k][j] = A[P[k]][j];
                 A[P[k]][j] = temp;
             }
        }

        //Fim da pivotacao
        for(j=k+1; j<n; j++)
        {
            double sum = 0;
            for (i=0; i<k; i++)
            {
                sum = sum + A[k][i]*A[i][j];
            }
            A[k][j] = A[k][j] - sum;
            //previne valores indefinidos porem nao significa que
            //a resposta esta correta:
            if(A[k][k] == 0)
            {
                printf("\n Aviso: Provavel sistema indeterminado. Entre com valores iniciais distintos.");
                A[k][k] = 0.0000000000001;
            }
            A[j][k] = A[j][k]/A[k][k];
        }
    }

    /** Prints de teste
    printf("[A]: \n");
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
            printf("%9.3f",A[i][j]);
        printf("\n");
    }

    printf("[P]: \n");
    for(i=0; i<n; i++)
    {
        printf("%d ",P[i]);
    }
    printf("\n");
    **/

}

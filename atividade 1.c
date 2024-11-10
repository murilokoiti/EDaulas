#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float cmedia(float *lista ,float n){
    float soma = 0.0;

    for (int i = 0; i < n; i++) {
        soma += lista[i];
    }

    return soma/n;
}

int comparador(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

float cmediana(float *lista, int n) {

    qsort(lista, n, sizeof(float), comparador);

    if (n % 2 == 0) {
        return (lista[n / 2] + lista[(n / 2)-1]) / 2.0;
    } else {
        return lista[n / 2];
    }
}


float cdp(float *lista, int n, float media){
    float somatorio=0;

    for (int i = 0; i < n; i++) {
        somatorio += (lista[i] - media) * (lista[i] - media);
    }
    return sqrt(somatorio / n);
}


int main(){
    int n;
    float max = 0, min = 0, media = 0, mediana = 0, dp = 0;

    printf("digite a quantidade de numeros:");
    scanf("%d",&n);

    float *lista = (float*)malloc(n * sizeof(float));
    if( !lista ) {   
		printf("Erro de alocação de memória\n\n");
		exit(-1);
	}

    printf("digite os %d numeros:\n",n);
    for(int i = 0; i < n; i++){
        scanf("%f", &lista[i]);
    }

    max = lista[0];
    min = lista[0];
    for (int i = 1; i < n; i++) {

        if (lista[i] > max){
            max = lista[i]; 
        } 
        if (lista[i] < min){
            min = lista[i];
        } 
    }


    media = cmedia(lista,n);
    mediana = cmediana(lista,n);
    dp = cdp(lista,n,media);

    printf("os valores de max e min sao respectivamente %.2f e %.2f\n",max,min);
    printf("a media dos numeros eh: %.2f\n", media);
    printf("a mediana dos numeros eh: %.2f\n", mediana);
    printf("o desvio padrao dos numeros eh: %f\n", dp);



    free(lista);
    return 0;
}  
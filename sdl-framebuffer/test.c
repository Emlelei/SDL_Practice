#include <stdlib.h>
#include <stdio.h>



#define A 5
#define B A*sizeof(int)

int main(){
/*
    printf("%d\n", A);
    printf("%d\n", B);



    int* table = (int*) malloc(30*sizeof(int));

    for(int i = 0; i<30; i++){
        table[i] = i;
        printf(" %d ", table[i]);
    }

    free(table);
    table = (int*) malloc(30*sizeof(int));

    printf("\n");
    for(int i = 0; i<30; i++){
       printf(" %d ", table[i]);
       table[i] = 30-i;
    }
    printf("\n");

    for(int i = 0; i<30; i++){
        printf(" %d ", table[i]);
    }
    printf("\n");
    */


    // int frame = 0;
    // for(int i= 0; i<=640; i++){
    //     printf(" %d ", (frame%320)+1);
    //     frame++;
    // }

    int frame = 0;
    for(int i= 0; i<=0xFFFFFF; i+=0x0000FF){
        printf(" %d ", (frame%320)+1);
        printf(" %#08X ", ((0x000000+frame)%0xFFFFFF)+1);
        printf("\n");
        frame++;
    }


}
#include <stdio.h>


int main(){
    int num[] = {1,0,0,1,0,1};
    displaynumber(num,sizeof(num));
    return 0;
}

void displaynumber(int num[],int tam_arr){
    for (int x =0; x< tam_arr/sizeof(int);x++){
        switch(num[x]){
            case 1:
                //digitalWrite(pino,HIGH);
                printf("pressionado");
            break;
            default:
                //digitalWrite(pino,LOW);
            printf("não pressionado");
        }
    }
}
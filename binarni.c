#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]){
        if(argc != 2){
                printf("Greska1\n");
                return -1;
        }
        int broj;
        printf("Unesite broj od 0 do 15:\n");
        scanf("%d", &broj);
        if(broj < 1 || broj > 15){
                printf("Broj van opsega!\n");
                return -1;
        }

        int pauza;
        printf("Unesite pauzu u ms:\n");
        scanf("%d", &pauza);

        char mod;
        printf("Unesite mod (n=NAPRED, z=NAZAD):\n");
        scanf(" %c", &mod);

        FILE *fp;

        if(mod == 'n'){
                for(int i = 1; i <= broj; i++){
                        fp = fopen("/dev/led", "w");
                        if(fp == NULL){
                                printf("Greska2\n");
                                return -1;
                        }

                        fprintf(fp, "0x%X", i);
                        printf("Debug: %d\n", i);
                        usleep(pauza*1000);
                        if(fclose(fp)){
                                printf("Greska3\n");
                                return -1;
                        }

                }
        }else{
	 for(int i = broj; i >= 0; i--){
                        fp = fopen("/dev/led", "w");
                        if(fp == NULL){
                                printf("Greska20\n");
                                return -1;
                        }

                        fprintf(fp, "0x%X", i);
                        printf("Debug: %d\n", i);
                        usleep(pauza*1000);
                        if(fclose(fp)){
                                printf("Greska30\n");
                                return -1;
                        }

                }
        }
        usleep(pauza*1000);

        fp = fopen("/dev/led", "w");
        if(fp == NULL){
                printf("Greska0\n");
                return -1;
        }
        fputs("0x0", fp);
        if(fclose(fp)){
                printf("Greska zatvranja0\n");
                return -1;
        }

        //sada ide upisivanje u fajl
        fp = fopen(argv[1], "w");
        if(fp == NULL){
                printf("Greska4\n");
                return -1;
        }
        fprintf(fp, "Broj: %d\n", broj);
        fprintf(fp, "Pauza: %d ms\n", pauza);

        char *mod_str;

        if(mod == 'n'){
                mod_str = "NAPRED";
        }else{
                mod_str = "NAZAD";
        }
        fprintf(fp, "Mod: %s\n", mod_str);

        if(fclose(fp)){
                printf("Greska5\n");
                return -1;
        }
 printf("\n------sadrzaj-------\n");
        //citanje
        fp = fopen(argv[1], "r");
                if(fp == NULL){
                printf("Greska6\n");
                return -1;
        }
        int c;
        while((c = getc(fp)) != EOF){
                putchar(c);
        }

        if(fclose(fp)){
                printf("Greska7\n");
                return -1;
        }


        return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LED_DEV "/dev/led"   // ISPRAVKA: nedostajala kosa crta na pocetku

int main(int argc, char *argv[]) {

    // provera argumenta
    if (argc != 2) {
        printf("Upotreba: %s <broj_treptaja>\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]);   // pretvara string argument u broj
    if (n <= 0) {
        printf("Broj treptaja mora biti veci od 0.\n");
        return -1;
    }

    // unos jednog karaktera - scanf umesto getchar, \n umesto fflush
    char izbor;
    printf("Unesite rezim (b = brzo, s = sporo):\n");
    scanf("%c", &izbor);

    int pauza_us;
    char *rezim_str;
    if (izbor == 'b') {
        pauza_us  = 200000;
        rezim_str = "brzo";
    } else {
        pauza_us  = 800000;
        rezim_str = "sporo";
    }
    printf("Izabran rezim: %s\n", rezim_str);

    // treptanje LED
    FILE *fp;
    for (int i = 0; i < n; i++) {

        // upali
        fp = fopen(LED_DEV, "w");
        if (fp == NULL) {
            printf("Greska pri otvaranju %s\n", LED_DEV);
            return -1;
        }
        fputs("0xF", fp);
        if (fclose(fp)) {
            printf("Greska pri zatvaranju %s\n", LED_DEV);
            return -1;
        }
        usleep(pauza_us);

        // ugasi
        fp = fopen(LED_DEV, "w");
        if (fp == NULL) {
            printf("Greska pri otvaranju %s\n", LED_DEV);
            return -1;
        }
        fputs("0x0", fp);
        if (fclose(fp)) {
            printf("Greska pri zatvaranju %s\n", LED_DEV);
            return -1;
        }
        usleep(pauza_us);

        printf("Treptaj %d/%d\n", i + 1, n);
    }

    // upis u fajl
    fp = fopen("izvestaj.txt", "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju izvestaj.txt\n");
        return -1;
    }
    fprintf(fp, "Broj treptaja: %d\n", n);
    fprintf(fp, "Rezim: %s\n", rezim_str);
    if (fclose(fp)) {
        printf("Greska pri zatvaranju izvestaj.txt\n");
        return -1;
    }

    // citanje fajla i ispis karakter po karakter
    printf("\n--- izvestaj.txt ---\n");
    fp = fopen("izvestaj.txt", "r");
    if (fp == NULL) {
        printf("Greska pri citanju izvestaj.txt\n");
        return -1;
    }

    int c;   // mora int, ne char - na ARM char je unsigned i ne moze da predstavi EOF(-1)
    while ((c = getc(fp)) != EOF) {
        putchar(c);
    }

    if (fclose(fp)) {
        printf("Greska pri zatvaranju izvestaj.txt\n");
        return -1;
    }

    printf("--- Kraj ---\n");
    return 0;
}

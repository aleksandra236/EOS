#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // sleep, usleep

int main(int argc, char *argv[]) {

    // argc == 2 znaci: ime_programa + ime_fajla, ako nije tacno 2 - greska
    if (argc != 2) {
        printf("Upotreba: %s <ime_fajla>\n", argv[0]);
        return -1;
    }

    // otvara /dev/switch za citanje
    FILE *fp = fopen("/dev/switch", "r");
    if (fp == NULL) {
        printf("Greska pri otvaranju /dev/switch\n");
        return -1;
    }

    char *sw;
    size_t num_of_bytes = 6;               // /dev/switch vraca "0b1010" = tacno 6 karaktera
    sw = (char *)malloc(num_of_bytes + 1); // +1 za terminator stringa '\0'
    getline(&sw, &num_of_bytes, fp);       // cita liniju iz /dev/switch u sw, npr. "0b1010"

    if (fclose(fp)) {
        printf("Greska pri zatvaranju /dev/switch\n");
        free(sw);
        return -1;
    }

    printf("Stanje prekidaca: %s\n", sw);
    sleep(1);   // ceka sekundu da korisnik vidi ispis, kao u switch_test.c sa vezbi

    // otvara /dev/led za upis
    fp = fopen("/dev/led", "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju /dev/led\n");
        free(sw);
        return -1;
    }

    fputs(sw, fp);  // upisuje isti string iz /dev/switch direktno u /dev/led
                    // LED prate stanje prekidaca bit po bit

    if (fclose(fp)) {
        printf("Greska pri zatvaranju /dev/led\n");
        free(sw);
        return -1;
    }

    char komentar[64];
    printf("Unesite komentar (bez razmaka):\n");
    scanf("%s", komentar);

    // otvara izlazni fajl za upis
    fp = fopen(argv[1], "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju %s\n", argv[1]);
        free(sw);
        return -1;
    }

    fprintf(fp, "Prekidaci: %s", sw);         // upisuje stanje prekidaca u fajl
    fprintf(fp, "Komentar: %s\n", komentar);  // upisuje komentar u fajl

    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", argv[1]);
        free(sw);
        return -1;
    }

    free(sw);  // oslobadja memoriju alociranu sa malloc

    printf("\n--- %s ---\n", argv[1]);

    // otvara isti fajl za citanje
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Greska pri citanju %s\n", argv[1]);
        return -1;
    }

    int c;  // mora int, ne char - na ARM char je unsigned i ne moze da predstavi EOF(-1)
    while ((c = getc(fp)) != EOF) {
        putchar(c);
    }

    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", argv[1]);
        return -1;
    }

    printf("--- Kraj ---\n");
    return 0;
}

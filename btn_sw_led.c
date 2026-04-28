#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Upotreba: %s <ime_fajla>\n", argv[0]);
        return -1;
    }

    FILE *fp;

    printf("Postavite prekidace u zeljeno stanje i pritisnite BTN1...\n");

    // cekanje na BTN1
    while (1) {

        fp = fopen("/dev/button", "r");
        if (fp == NULL) {
            printf("Greska pri otvaranju /dev/button\n");
            return -1;
        }
        char *btn_str;
        size_t btn_bytes = 6;
        btn_str = (char *)malloc(btn_bytes + 1);
        getline(&btn_str, &btn_bytes, fp);
        if (fclose(fp)) {
            printf("Greska pri zatvaranju /dev/button\n");
            free(btn_str);
            return -1;
        }

        if (btn_str[4] == '1') {   // BTN1 na indexu [4]
            free(btn_str);
            break;
        }

        free(btn_str);
        usleep(100000);   // 100ms pauza
    }

    printf("BTN1 pritisnut!\n");

    // citanje stanja prekidaca
    fp = fopen("/dev/switch", "r");
    if (fp == NULL) {
        printf("Greska pri otvaranju /dev/switch\n");
        return -1;
    }
    char *sw_str;
    size_t sw_bytes = 6;
    sw_str = (char *)malloc(sw_bytes + 1);
    getline(&sw_str, &sw_bytes, fp);
    if (fclose(fp)) {
        printf("Greska pri zatvaranju /dev/switch\n");
        free(sw_str);
        return -1;
    }
    printf("Stanje prekidaca: %s\n", sw_str);

    // upis u LED
    fp = fopen("/dev/led", "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju /dev/led\n");
        free(sw_str);
        return -1;
    }
    fprintf(fp, "%s", sw_str);
    if (fclose(fp)) {
        printf("Greska pri zatvaranju /dev/led\n");
        free(sw_str);
        return -1;
    }
    printf("LED postavljene prema stanju prekidaca.\n");

    // upis u log fajl
    fp = fopen(argv[1], "a");
    if (fp == NULL) {
        printf("Greska pri otvaranju %s\n", argv[1]);
        free(sw_str);
        return -1;
    }
    fprintf(fp, "Stanje: %s", sw_str);
    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", argv[1]);
        free(sw_str);
        return -1;
    }

    free(sw_str);

    // citanje log fajla i ispis
    printf("\n--- %s ---\n", argv[1]);
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Greska pri citanju %s\n", argv[1]);
        return -1;
    }
    int c;
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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LED_DEV  "/dev/led"
#define SW_DEV   "/dev/switch"
#define BTN_DEV  "/dev/button"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Upotreba: %s <ime_log_fajla>\n", argv[0]);
        return -1;
    }

    const char *log_fajl = argv[1];

    char poruka[64];
    printf("Unesite poruku (bez razmaka):\n");
    scanf("%s", poruka);
    printf("Poruka primljena: %s\n", poruka);

    FILE *fp = fopen(SW_DEV, "r");
    if (fp == NULL) {
        printf("Greska pri otvaranju %s\n", SW_DEV);
        return -1;
    }

    char *sw_str;
    size_t num_of_bytes = 6;
    sw_str = (char *)malloc(num_of_bytes + 1);
    getline(&sw_str, &num_of_bytes, fp);

    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", SW_DEV);
        free(sw_str);
        return -1;
    }
    printf("Stanje prekidaca: %s\n", sw_str);
    sleep(1);

    fp = fopen(LED_DEV, "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju %s\n", LED_DEV);
        free(sw_str);
        return -1;
    }
    fputs(sw_str, fp);
    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", LED_DEV);
        free(sw_str);
        return -1;
    }
    printf("LED postavljene prema stanju prekidaca.\n");

    // cekanje na BTN1
    // format "0bXXXX", indeksi: [2]=BTN3, [3]=BTN2, [4]=BTN1, [5]=BTN0
    // active low - '0' znaci pritisnut
    printf("Pritisnite BTN1 za nastavak...\n");
    while (1) {

        fp = fopen(BTN_DEV, "r");
        if (fp == NULL) {
            printf("Greska pri otvaranju %s\n", BTN_DEV);
            free(sw_str);
            return -1;
        }

        char *btn_str;
        size_t btn_bytes = 6;
        btn_str = (char *)malloc(btn_bytes + 1);
        getline(&btn_str, &btn_bytes, fp);

        if (fclose(fp)) {
            printf("Greska pri zatvaranju %s\n", BTN_DEV);
            free(btn_str);
            free(sw_str);
            return -1;
        }

        printf("Ocitano: %s, BTN1=[%c]\n", btn_str, btn_str[4]);  // debug, obrisati posle

        if (btn_str[4] == '1') {  // BTN1 je na indexu [4]
            free(btn_str);
            break;
        }

        free(btn_str);
        usleep(100000);
    }
    printf("BTN1 pritisnut!\n");

    fp = fopen(log_fajl, "a");
    if (fp == NULL) {
        printf("Greska pri otvaranju %s\n", log_fajl);
        free(sw_str);
        return -1;
    }
    fprintf(fp, "Poruka: %s | Prekidaci: %s", poruka, sw_str);
    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", log_fajl);
        free(sw_str);
        return -1;
    }

    free(sw_str);

    printf("\n--- Sadrzaj log fajla: %s ---\n", log_fajl);
    fp = fopen(log_fajl, "r");
    if (fp == NULL) {
        printf("Greska pri citanju %s\n", log_fajl);
        return -1;
    }

    int c;
    while ((c = getc(fp)) != EOF) {
        putchar(c);
    }

    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", log_fajl);
        return -1;
    }

    printf("--- Kraj ---\n");
    return 0;
}

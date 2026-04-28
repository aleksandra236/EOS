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
    char *sw_str;
    size_t sw_bytes = 6;

    char prethodno[7];
    prethodno[6] = '\0';

    printf("Program pokrenut, menjajte prekidace...\n");

    while (1) {

        // citanje stanja prekidaca
        fp = fopen("/dev/switch", "r");
        if (fp == NULL) {
            printf("Greska pri otvaranju /dev/switch\n");
            return -1;
        }
        sw_str = (char *)malloc(sw_bytes + 1);
        getline(&sw_str, &sw_bytes, fp);
        if (fclose(fp)) {
            printf("Greska pri zatvaranju /dev/switch\n");
            free(sw_str);
            return -1;
        }

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

        // ako se stanje promenilo, upisuje se u log fajl
        if (strcmp(sw_str, prethodno) != 0) {

            printf("Promena: %s\n", sw_str);

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

            strcpy(prethodno, sw_str);
        }

        free(sw_str);
        usleep(200000);
    }

    return 0;
}

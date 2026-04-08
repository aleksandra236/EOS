#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // provera argumenta
    if (argc != 2) {
        printf("Upotreba: %s <ime_fajla>\n", argv[0]);
        return -1;
    }

    // unos broja - scanf direktno, nema fflush jer printf ima \n
    int n;
    printf("Do kog broja da brojim?\n");
    scanf("%d", &n);

    // upis u fajl
    FILE *fp = fopen(argv[1], "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju %s\n", argv[1]);
        return -1;
    }
    for (int i = 1; i <= n; i++) {
        fprintf(fp, "%d\n", i);
    }
    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", argv[1]);
        return -1;
    }

    // citanje i ispis karakter po karakter
    printf("\n--- sadrzaj %s ---\n", argv[1]);
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Greska pri citanju %s\n", argv[1]);
        return -1;
    }
    char c;
    while ((c = getc(fp)) != EOF) {
        putchar(c);
    }
    if (fclose(fp)) {
        printf("Greska pri zatvaranju %s\n", argv[1]);
        return -1;
    }

    printf("--- kraj ---\n");
    return 0;
}

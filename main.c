#include <stdio.h>

int main() {
    FILE *fp;
    fp = fopen("../test.txt", "r"); //On accède au fichier à encoder

    fseek(fp, 0L, SEEK_END); // Recherche de la fin du fichier
    unsigned int size = ftell(fp); // Stockage de la taille
    rewind(fp); // Replacement du pointeur au début du fichier

    char buffer;
    for (int i = 0; i < size; i++) {
        fscanf(fp, "%c", &buffer);
        if (buffer == 10) i++;
        printf("%c", buffer);
    }
    fclose(fp);

    return 0;
}

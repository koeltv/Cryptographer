#include <stdio.h>
#define EncryptionKey "NF05 Rules" //Clé d'encodage, on pourra la demander plus tard

//Fonction permettant de lire un fichier externe en indiquant le lien relatif et qui renvoie la taille du fichier
unsigned int readFile(char file[]){
    FILE *fp;
    char link[] = "../test.txt";
    //printf("Entrer le lien du fichier relatif au programme (ex: \"../test.txt\") : ");
    //scanf("%s", link);
    fp = fopen(link, "rb"); // On accède au fichier à encoder

    fseek(fp, 0L, SEEK_END); // Recherche de la fin du fichier
    unsigned int size = ftell(fp); // Stockage de la taille
    rewind(fp); // Replacement du pointeur au début du fichier

    for (int i = 0; i < size; i++) fscanf_s(fp, "%c", &file[i]);
    fclose(fp);
    return size;
}

//Permutation des caractères (on les décale de la valeur de key puis on applique modulo 255)
char CharPermutation(char car, int key){
    return (char)(((int)car+key)%255);
}
//Fonction inverse de celle ci-dessus
char CharPermutationReverse(char car, int key){
    return (char)((int)car - key%255);
}

int main() {
    int key = 0, i=0;
    //Création d'une valeur utilisée pour la permutation des charactères dépendante d'EncryptionKey
    while (EncryptionKey[i] != '\0'){
        key += EncryptionKey[i];
        i++;
    }
    printf("Key value : %d\n", key);
    char file[1000]; //TODO Mettre en place l'allocation dynamique de mémoire
    unsigned int size = readFile(file);

    //Etape 1
    for (int j = 0; j < size; j++) file[j] = CharPermutation(file[j], key);

    //Affichage du fichier après encodage
    printf("\n======================================\n");
    for (i = 0; i < size; i++) printf("%c", file[i]);
    printf("\n======================================");

    return 0;
}
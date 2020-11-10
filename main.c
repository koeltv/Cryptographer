#include <stdio.h>
#include <stdlib.h>
#define EncryptionKey "NF05 Rules" //TODO Demander la clé de chiffrement

//Fonction permettant de lire un fichier externe en indiquant le lien relatif et qui renvoie la taille du fichier
unsigned char *readFile(int *size){
    FILE *fp;
    char link[] = "../test.txt"; //TODO Demander le lien du fichier à encoder + allocation dynamique
    //printf("Entrer le lien du fichier relatif au programme (ex: \"../test.txt\") : ");
    //scanf("%s", link);
    if ((fp = fopen(link, "rb")) == NULL){//Gestion d'erreurs
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", link);
        return NULL;
    }; // On accède au fichier à encoder

    fseek(fp, 0L, SEEK_END); // Recherche de la fin du fichier
    *size = ftell(fp); // Stockage de la taille
    rewind(fp); // Replacement du pointeur au début du fichier

    unsigned char *file = (unsigned char*)malloc(*size * sizeof(char)); //Création d'un tableau pour contenir les octets du fichier
    for (int i = 0; i < *size; i++) fscanf(fp, "%c", &file[i]);
    fclose(fp);
    return file;
}

//Fonction 1 : Permutation des caractères (on les décale de la valeur de key puis on applique modulo 255)
char CharPermutation(unsigned char car, int key){
    return (char)(((int)car+key)%255);
}
//Fonction inverse de celle ci-dessus
char CharPermutationReverse(unsigned char car, int key){
    return (char)((int)car - key%255);
}

//Utilisé pour la multiplication d'un vecteur et d'une matrice dans la fonction 3
int *multiplyMatrices(const int *v, int H[][8]){
    static int vRes[8];
    for (int rowFinal = 0, temp = 0; rowFinal < 8; rowFinal++) {
        for (int k = 0; k < 8; k++) temp += H[rowFinal][k] * v[k];
        vRes[rowFinal] = temp % 2; //On fait %2 pour rester en binaire
        temp = 0;
    }
    return vRes;
}

//Utilisé pour convertir un caractère (byte = octet) en tableau de bits (ici considerés comme entiers)
int *ByteToBits(unsigned char byte){
    static int bits[8];
    int i=0;
    while (byte > 1){
        bits[i] = byte % 2;
        byte /= 2;
        i++;
    }
    bits[i] = (int)byte;
    for (int j = i+1; j < 8; j++) bits[j] = 0;
    //On a maintenant le byte complet sous forme binaire, avec bit de poid le plus faible = bits[0]
    return bits;
}

//Utilisé pour convertir un tableau de bits en caractère
unsigned char BitsToByte(const int *bits){
    unsigned char byte = 0;
    for (int i = 0, powerOf2 = 1; i < 8; i++) {
        byte += bits[i] * powerOf2;
        powerOf2 *= 2;
    }
    return byte;
}

//Fonction 3 : application de l'opération H x v + c
unsigned char CharApplyMatrix(unsigned char byte){
    int *bits = ByteToBits(byte), H[][8] = {
            1, 0, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 1, 1
    };
    int *Xi = multiplyMatrices(bits, H); //Ce qui correspond à H x vi
    int c[] = {1, 1, 0, 0, 0, 1, 1, 0};
    for (int j = 0; j < 8; j++) Xi[j] = (Xi[j] + c[j]) % 2; //Ce qui correspond à Xi = H x vi + c
    return BitsToByte(Xi);
}
//Fonction inverse de celle ci-dessus (H' x Xi + c')
unsigned char CharApplyMatrixReverse(unsigned char byte){
    int *bits = ByteToBits(byte), HPrime[][8] = {
            0, 0, 1, 0, 0, 1, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 0,
            0, 1, 0, 0, 1, 0, 0, 1,
            1, 0, 1, 0, 0, 1, 0, 0,
            0, 1, 0, 1, 0, 0, 1, 0,
            0, 0, 1, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 1, 0
    };
    int *vi = multiplyMatrices(bits, HPrime); //Ce qui correspond à H' x Xi
    int cPrime[] = {1, 0, 1, 0, 0, 0, 0, 0};
    for (int j = 0; j < 8; j++) vi[j] = (vi[j] + cPrime[j]) % 2; //Ce qui correspond à vi = H' x Xi + c'
    return BitsToByte(vi);
}

int main() {
    int iterationkey1 = 0, i = 0;
    //Création d'une valeur utilisée pour la permutation des charactères dépendante d'EncryptionKey
    while (EncryptionKey[i] != '\0'){ iterationkey1 += EncryptionKey[i]; i++;}
    printf("Cle d'iteration K1 : %d\n", iterationkey1);
    int size;
    unsigned char *file;
    if((file = readFile(&size)) == NULL) return EXIT_FAILURE;

    printf("\n============Depart============\n");
    for (i = 0; i < size; i++) printf("%c", file[i]);
    printf("\n==============================\n");

    //Etape 1
    for (int j = 0; j < size; j++) file[j] = CharPermutation(file[j], iterationkey1);

    //Affichage du fichier après encodage
    printf("\n==========Permutation==========\n");
    for (i = 0; i < size; i++) printf("%c", file[i]);
    printf("\n===============================\n");

    for (int j = 0; j < size; j++) file[j] = CharApplyMatrix(file[j]);

    //Affichage du fichier après encodage
    printf("\n============Matrice============\n");
    for (i = 0; i < size; i++) printf("%c", file[i]);
    printf("\n===============================\n");

    //Etape 3
    for (int j = 0; j < size; j++) file[j] = CharApplyMatrixReverse(file[j]);
    for (int j = 0; j < size; j++) file[j] = CharPermutationReverse(file[j], iterationkey1);

    //Affichage du fichier après encodage
    printf("\n===========Decodage===========\n");
    for (i = 0; i < size; i++) printf("%c", file[i]);
    printf("\n==============================");

    return 0;
}
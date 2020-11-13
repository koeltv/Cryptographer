#include <stdio.h>
#include <stdlib.h>
#define EncryptionKey "NF05 Rules" //TODO Demander la clé de chiffrement

///Lit un fichier.
///La fonction readFile() permet de lire un fichier quelconque dont on lui donne le lien (relatif au projet)
///@warning Si la fonction ne peut pas ouvrir le fichier (par exemple un chemin non valide), elle renvoie un NULL
///@param size - pointeur vers un entier qui recevra la taille du fichier lu
///@return pointeur vers caractère correspondant au 1er octet du fichier lu
///### Example
///~~~~~~~~~~~~~~.c
/// //This will print the content of the file
/// unsigned char *file = readFile(size);
/// for (i = 0; i < size; i++) printf("%c", file[i]);
///~~~~~~~~~~~~~~
unsigned char *readFile(int *size){
    FILE *fp;
    char link[] = "../test.txt"; //TODO Demander le lien du fichier à encoder + allocation dynamique
    //printf("Entrer le lien du fichier relatif au programme (ex: \"../test.txt\") : ");
    //scanf("%s", link);
    if ((fp = fopen(link, "rb")) == NULL) {//Gestion d'erreurs
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", link);
        return NULL;
    } // On accède au fichier à encoder

    fseek(fp, 0L, SEEK_END); // Recherche de la fin du fichier
    *size = ftell(fp); // Stockage de la taille
    rewind(fp); // Replacement du pointeur au début du fichier

    unsigned char *file = (unsigned char*)malloc(*size * sizeof(char)); //Création d'un tableau pour contenir les octets du fichier
    for (int i = 0; i < *size; i++) fscanf(fp, "%c", &file[i]);
    fclose(fp);
    return file;
}

///Permutation d'un octet.
///La fonction CharPermutation() va prendre un octet et le décaler vers la droite dans la table ASCII
///@note Si la valeur dépasse 255, on repart de 0
///@param car - caractère/octet à permuter
///@param key - valeur selon laquel car sera décalé
///@return caractère/octet permuté
///### Example
///~~~~~~~~~~~~~~.c
/// unsigned char octet = 'a';
/// printf("%c", CharPermutation(car, 2)); //Le résultat sera 'c'
///~~~~~~~~~~~~~~
unsigned char CharPermutation(unsigned char car, int key){
    return (char)(((int)car+key)%255);
}

///Inversion de la permutation d'un octet.
///La fonction CharPermutationReverse() va prendre un octet et le décaler vers la gauche dans la table ASCII
///@note Si la valeur atteint 0, on repart de 255
///@param car - caractère/octet à permuter
///@param key - valeur selon laquel car sera décalé
///@return caractère/octet permuté
///### Example
///~~~~~~~~~~~~~~.c
/// unsigned char octet = 'c';
/// printf("%c", CharPermutationReverse(car, 2)); //Le résultat sera 'a'
///~~~~~~~~~~~~~~
unsigned char CharPermutationReverse(unsigned char car, int key){
    return (char)((int)car - key%255);
}

///Multiplication matrice par vecteur.
///La fonction multiplyMatrices() effectue une multiplication entre une matrice et un vecteur dans cet ordre
///@warning La matrice doit être de dimension 8x8 et le vecteur de dimension 8x1
///@param v - vecteur V
///@param H - matrice M
///@return résultat de M x V
int *multiplyMatrices(const int *v, int H[][8]){
    static int vRes[8];
    for (int rowFinal = 0, temp = 0; rowFinal < 8; rowFinal++) {
        for (int k = 0; k < 8; k++) temp += H[rowFinal][k] * v[k];
        vRes[rowFinal] = temp % 2; //On fait %2 pour rester en binaire
        temp = 0;
    }
    return vRes;
}

///Conversion d'un octet/caractère vers un tableau de bits
///La fonction ByteToBits() prend un octet/caractère et donne en sorti un tableau de bits correspondant à sa valeur en binaire
///@note Le bit de poids le plus faible se trouve en position 0
///@param byte - caractère/octet à convertir
///@return tableau de bits correspondant à la valeur en binaire de l'octet
///### Example
///~~~~~~~~~~~~~~.c
/// int byte[8] = ByteToBits('c');
/// for (int i=7; i>=0; i--) printf("%d", byte[i]); //On aura 01100011
///~~~~~~~~~~~~~~
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
    return bits;
}

///Conversion d'un tableau de bits vers un octet/caractère
///La fonction BitsToByte() prend un tableau de bits et donne en sorti un octet/caractère correspondant à sa valeur base 10
///@param bits - pointeur vers un tableau de 8 bits
///@return octet correspondant à la valeur en base 10 du tableau de bits
///### Example
///~~~~~~~~~~~~~~.c
/// int bits[] = {1, 1, 0, 0, 0, 1, 1, 0};
/// printf("%c", BitsToByte(bits); //On aura 'c'
///~~~~~~~~~~~~~~
unsigned char BitsToByte(const int *bits){
    unsigned char byte = 0;
    for (int i = 0, powerOf2 = 1; i < 8; i++) {
        byte += bits[i] * powerOf2;
        powerOf2 *= 2;
    }
    return byte;
}

///Fonction Matriciel
///La fonction CharApplyMatrix() prend un octet/caractère et va le multiplier par une matrice H puis effectuer une addition avec C pour encoder cet octet/caractère
///@note La matrice H et le vecteur C sont connu auparavant
///@param byte - caractère/octet à encoder
///@return caractère/octet encodé
///@see BitsToByte() ByteToBits multiplyMatrices()
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

///Fonction Matriciel Inverse
///La fonction CharApplyMatrixReverse() prend un octet/caractère et va le multiplier par une matrice H' puis effectuer une addition avec C' pour décoder cet octet/caractère après passage dans CharApplyMatrix()
///@note La matrice H' et le vecteur C' sont connu auparavant
///@param byte - caractère/octet à encoder
///@return caractère/octet décodé
///@see BitsToByte() ByteToBits multiplyMatrices()
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define encryptionKey "NF05 Rules" //A enlever, remplacer par demande à l'utilisateur début de main()
#define link "../test.txt" //Pareil, juste la pour les tests

/**
 * Lit une chaîne de caractère dans un fichier donné
 * La fonction writeString() permet de lire une chaîne de caractère quelconque terminée par un retour à la ligne dans un fichier
 * @note Il est possible de choisir stdin comme fichier pour une entrée manuelle
 * @param fp - fichier dans lequel lire la chaîne de caractère
 * @return addresse où est stocké la chaîne de caractère
 */
char *writeString(){
    int i=0; char temp;
    char *string = (char*) malloc (20 * sizeof(char)); //On part d'une chaîne de 20 caractères
    scanf(" %c", &temp);
    while (temp >= ' ' && temp <= '~'){
        if (i % 19 == 1 && i > 19) string = (char*) realloc (string, (i+20) * sizeof(char)); //Si on dépasse 20 caractères, on ajoute un espace de 20 caractères à la chaîne
        string[i] = temp;
        scanf("%c", &temp);
        i++;
    } string[i] = '\0';
    return string;
}

/**
 * Lit le contenu d'un fichier
 * La fonction readFile() permet de lire le contenu d'un fichier quelconque dont on lui donne le lien (relatif au projet)
 * @param fileLink - lien vers le fichier à lire
 * @param fileData - pointeur sur les données récuperées du fichier
 * @param size - pointeur vers un entier qui recevra la taille du fichier lu
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  //Ceci affiche le contenu du fichier test.txt à l'emplacement du programme
 *  unsigned char *file = NULL;
 *  file = readFile("./test.txt", &file, &size);
 *  for (i = 0; i < size; i++) printf("%c", file[i]);
 * ~~~~~~~~~~~~~~
 */
void readFile(char *fileLink, unsigned char **fileData, int *size){
    FILE *sourceFile = NULL;
    if ((sourceFile = fopen(fileLink, "rb")) == NULL) fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", link);
    else {
        fseek(sourceFile, 0L, SEEK_END); // Recherche de la fin du fichier
        *size = ftell(sourceFile);
        rewind(sourceFile);// Stockage de la taille
        unsigned char *temp = (unsigned char *) malloc(*size * sizeof(unsigned char)); //Création d'un tableau pour contenir les octets du fichier
        for (int i = 0; i < *size; i++) fscanf(sourceFile, "%c", &temp[i]);
        fclose(sourceFile);
        *fileData = temp;
    }
}
/**
 * Ecrit dans un fichier
 * La fonction writeInFile() permet d'écrire des données dans un fichier basé sur le fichier d'entrée selon le mode choisi
 * @note l'extension "_encrypted" ou "decrypted" sera ajouté à la fin du nom du fichier selon le mode choisi
 * @warning Si un fichier du même nom existe déjà, il sera écrasé et remplacé par celui généré par la fonction
 * @param data - pointeur sur les données à écrire dans le fichier
 * @param size - quantité de données à écrire
 * @param sourceLink - lien vers le fichier source (utilisé pour le nom du fichier de destination)
 * @param action - sélectionne l'extension "_encrypted" ou "decrypted" (0 pour la 1ère, toute autre valeur pour la 2ème)
 */
void writeInFile(unsigned char *data, const int *size, const char *sourceLink, const int *action){
    int pointPosition=0, linkLenght=0;
    //Recherche taille du lien et position du point de l'extension de fichier
    while (sourceLink[linkLenght] != '\0'){
        if (sourceLink[linkLenght] == '.') pointPosition = linkLenght;
        linkLenght++;
    }
    char *destlink = (char *) malloc((linkLenght + 10) * sizeof(char));
    //Copie du début du lien jusqu'au point
    for (int i = 0; i < pointPosition; i++) destlink[i] = sourceLink[i];
    destlink[pointPosition] = '\0';
    if (*action == 0) strcat(destlink, "_encrypted");
    else strcat(destlink, "_decrypted");
    //Ajout de l'extension du lien original
    for (int i = pointPosition; i <= linkLenght; i++) destlink[i + 10] = sourceLink[i];
    //Ecriture des données
    FILE *destinationFile = fopen(destlink, "wb");
    for (int i = 0; i < *size; i++) fprintf(destinationFile, "%c", data[i]);
    fclose(destinationFile);
}

/**
 * Permutation d'un octet
 * La fonction CharPermutation() va prendre un octet et le décaler vers la droite dans la table ASCII
 * @note Si la valeur dépasse 255, on repart de 0
 * @param car - caractère/octet à permuter
 * @param key - valeur selon laquel car sera décalé
 * @return caractère/octet permuté
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char octet = 'a';
 *  printf("%c", CharPermutation(car, 2)); //Le résultat sera 'c'
 * ~~~~~~~~~~~~~~
 */
void CharPermutation(unsigned char *car, const unsigned char *key){
    *car += *key % 256;
}

/**
 * Inversion de la permutation d'un octet
 * La fonction CharPermutationReverse() va prendre un octet et le décaler vers la gauche dans la table ASCII
 * @note Si la valeur atteint 0, on repart de 255
 * @param car - caractère/octet à permuter
 * @param key - valeur selon laquel car sera décalé
 * @return caractère/octet permuté
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char octet = 'c';
 *  printf("%c", CharPermutationReverse(car, 2)); //Le résultat sera 'a'
 * ~~~~~~~~~~~~~~
 */
void CharPermutationReverse(unsigned char *car, const unsigned char *key){
    *car -= *key % 256;
}

/**
 * Multiplication matrice par vecteur
 * La fonction multiplyMatrices() effectue une multiplication entre une matrice et un vecteur dans cet ordre
 * @warning La matrice doit être de dimension 8x8 et le vecteur de dimension 8x1
 * @param v - vecteur V, mis à jour suite à la multiplication
 * @param H - matrice M
 */
void multiplyMatrices(int **v, int H[][8]){
    int vRes[8];
    for (int rowFinal = 0, temp = 0; rowFinal < 8; rowFinal++) {
        for (int k = 0; k < 8; k++) temp += H[rowFinal][k] * (*v)[k];
        vRes[rowFinal] = temp % 2; //On fait %2 pour rester en binaire
        temp = 0;
    } for (int i = 0; i < 8; i++) (*v)[i] = vRes[i];
}

/**
 * Conversion d'un octet/caractère vers un tableau de bits
 * La fonction ByteToBits() prend un octet/caractère et donne en sortie un tableau de bits correspondant à sa valeur en binaire
 * @note Le bit de poids le plus faible se trouve en position 0
 * @param byte - caractère/octet à convertir
 * @return tableau de bits correspondant à la valeur en binaire de l'octet
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  int byte[8] = ByteToBits('c');
 *  for (int i=7; i>=0; i--) printf("%d", byte[i]); //On aura 01100011
 * ~~~~~~~~~~~~~~
 */
int *ByteToBits(unsigned char byte){
    int *bits = (int *) malloc(8 * sizeof(int));
    int i=0;
    while (byte > 1){
        bits[i] = byte % 2;
        byte /= 2;
        i++;
    } bits[i] = byte;
    for (int j = i+1; j < 8; j++) bits[j] = 0;
    return bits;
}

/**
 * Conversion d'un tableau de bits vers un octet/caractère
 * La fonction BitsToByte() prend un tableau de bits et donne en sorti un octet/caractère correspondant à sa valeur base 10
 * @param bits - pointeur vers un tableau de 8 bits
 * @return octet correspondant à la valeur en base 10 du tableau de bits
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  int bits[] = {1, 1, 0, 0, 0, 1, 1, 0};
 *  printf("%c", BitsToByte(bits); //On aura 'c'
 * ~~~~~~~~~~~~~~
 */
unsigned char BitsToByte(const int *bits){
    unsigned char byte = 0;
    for (int i = 0, powerOf2 = 1; i < 8; i++) {
        byte += bits[i] * powerOf2;
        powerOf2 *= 2;
    } return byte;
}

/**
 * Fonction Matriciel
 * La fonction CharApplyMatrix() prend un octet/caractère et va le multiplier par une matrice H puis effectuer une addition avec C pour encoder cet octet/caractère
 * @note La matrice H et le vecteur C sont connus auparavant
 * @param byte - caractère/octet à encoder, mis à jour suite à l'application de la fonction
 * @see BitsToByte() ByteToBits() multiplyMatrices()
 */
void CharApplyMatrix(unsigned char *byte){
    int *bits = ByteToBits(*byte), H[][8] = {
            1, 0, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 1, 1
    };
    multiplyMatrices(&bits, H); //Ce qui correspond à H x vi
    int c[] = {1, 1, 0, 0, 0, 1, 1, 0};
    for (int j = 0; j < 8; j++) bits[j] = (bits[j] + c[j]) % 2; //Ce qui correspond à Xi = H x vi + c
    *byte = BitsToByte(bits);
}

/**
 * Fonction Matriciel Inverse
 * La fonction CharApplyMatrixReverse() prend un octet/caractère et va le multiplier par une matrice H' puis effectuer une addition avec C' pour décoder cet octet/caractère après passage dans CharApplyMatrix()
 * @note La matrice H' et le vecteur C' sont connu auparavant
 * @param byte - caractère/octet à décoder, mis à jour suite à l'application de la fonction
 * @see BitsToByte() ByteToBits() multiplyMatrices()
 */
void CharApplyMatrixReverse(unsigned char *byte){
    int *bits = ByteToBits(*byte), HPrime[][8] = {
            0, 0, 1, 0, 0, 1, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 0,
            0, 1, 0, 0, 1, 0, 0, 1,
            1, 0, 1, 0, 0, 1, 0, 0,
            0, 1, 0, 1, 0, 0, 1, 0,
            0, 0, 1, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 1, 0
    };
    multiplyMatrices(&bits, HPrime); //Ce qui correspond à H' x Xi
    int cPrime[] = {1, 0, 1, 0, 0, 0, 0, 0};
    for (int j = 0; j < 8; j++) bits[j] = (bits[j] + cPrime[j]) % 2; //Ce qui correspond à vi = H' x Xi + c'
    *byte = BitsToByte(bits);
}

/**
 * Fonction XOR logique
 * La fonction ApplyXOROnByte() prend un octet/caractère et va appliquer l'opération logique XOR individuellement entre chaque bit de l'octet et la clé d'itération n°2
 * @param byte - caractère/octet à encoder, mis à jour suite à la fonction
 * @param key - 2ème valeur (sous forme d'entier) pour faire le XOR
 * @see BitsToByte() ByteToBits()
 */
void ApplyXOROnByte(unsigned char *byte, const unsigned char *key){
    int *bits = ByteToBits(*byte), *keyBits = ByteToBits(*key);
    for (int i = 0; i < 8; i++) bits[i] = (bits[i] + keyBits[i]) % 2;
    free(keyBits);
    *byte = BitsToByte(bits);
    free(bits);
}

/**
 * Fonction de Concaténation
 * La fonction concatenate() prend 4 octets/caractères et va les concaténer en respectant un ordre d'opération précis (voir ci-dessous)
 * @note opération effectuées :
 * Z[0] = Y[0] + Y[1]
 * Z[1] = Y[0] + Y[1] + Y[2]
 * Z[2] = Y[1] + Y[2] + Y[3]
 * Z[3] = Y[2] + Y[3]
 * Y étant l'entrée et Z la sortie
 * @param bytes - caractère/octet à encoder, mis à jour suite à la fonction
 */
void concatenate(unsigned char (*bytes)[4]){
    unsigned char temp0 = (*bytes)[0], temp1 = (*bytes)[1], temp2 = (*bytes)[2], temp3 = (*bytes)[3];
    (*bytes)[0] = (temp0 + temp1) % 256;
    (*bytes)[1] = (temp0 + temp1 + temp2) % 256;
    (*bytes)[2] = (temp1 + temp2 + temp3) % 256;
    (*bytes)[3] = (temp2 + temp3) % 256;
}

/**
 * Fonction de Concaténation Inverse
 * La fonction concatenateReverse() prend 4 octets/caractères et va inverser la concaténation de concatenate() en respectant un ordre d'opération précis (voir ci-dessous)
 * @note opération effectuées :
 * Y[0] = Z[0] - Z[2] + Z[3]
 * Y[1] = Z[2] - Z[3]
 * Y[2] = Z[1] - Z[0]
 * Y[3] = Z[0] - Z[1] + Z[3]
 * Z étant l'entrée et Y la sortie
 * @param bytes - caractère/octet à décoder, mis à jour suite à la fonction
 */
void concatenateReverse(unsigned char (*bytes)[4]){
    unsigned char temp0 = (*bytes)[0], temp1 = (*bytes)[1], temp2 = (*bytes)[2], temp3 = (*bytes)[3];
    (*bytes)[0] = (temp0 - temp2 + temp3) % 256;
    (*bytes)[1] = (temp2 - temp3) % 256;
    (*bytes)[2] = (temp1 - temp0) % 256;
    (*bytes)[3] = (temp0 - temp1 + temp3) % 256;
}

//utiliser "doxygen Doxyfile" pour mettre à jour la documentation
int main() { //TODO Mettre en place les N répétitions et varier clé d'itération selon l'itération
    /*
    printf("==========================================================================================================\n\n");
    printf("#######  #######  #     #  #######  #######  #######  #######  #######  #######  #######  #     #  #######\n");
    printf("#        #     #  #     #  #     #     #     #     #  #        #     #  #     #  #        #     #  #\n");
    printf("#        #######   #   #   #######     #     #     #  #        #######  #######  #        #######  #\n");
    printf("#        #    #      #     #           #     #     #  #   ###  #    #   #     #  #   ###  #     #  #####\n");
    printf("#        #     #     #     #           #     #     #  #     #  #     #  #     #  #     #  #     #  #\n");
    printf("#######  #     #     #     #           #     #######  #######  #     #  #     #  #######  #     #  #######\n");
    printf("\n==========================================================================================================\n");
    printf("Bonjour ! Cette application vous permet d'encrypter/decrypter tous types de fichier.\nQue voulez-vous faire ? (0 pour encryptage, 1 pour decryptage)\n");
    int action, N;
    scanf("%d", &action);
    if (action == 0) printf("Encryptage selectionne, ");
    else printf("Decryptage selectionne, ");
    printf("veuillez entrer le lien du fichier relatif au programme (ex: ../toto.txt)\n");
    char *link = writeString();
    printf("Entrer la cle de chiffrement\n");
    char *encryptionKey = writeString();
    printf("Enfin, entrez le nombre d'iterations a effectuer (nombre plus grand, plus de securite mais processus plus long)\n");
    scanf("%d", &N);
    for (int i = 1; i <= N; i++);
    printf("Termine ! Retrouvez le resultat au meme emplacement que le fichier de depart.");
     */

    int size = 0; unsigned char *fileData = NULL;
    readFile(link, &fileData, &size);
    if (size < 1) return EXIT_FAILURE;

    int i = 0; unsigned char iterationKey1 = 0, iterationKey2 = 0;
    //Création d'une valeur utilisée pour la permutation des charactères dépendante d'encryptionKey
    while (encryptionKey[i] != '\0'){
        iterationKey1 += encryptionKey[i];
        iterationKey2 *= encryptionKey[i];
        i++;
    }

    //if (action == 0){ //Encodage if else désactivé pour les tests
        for (int j = 0; j < size; j++) {
            CharPermutation(&fileData[j], &iterationKey1);
            CharApplyMatrix(&fileData[j]);
            ApplyXOROnByte(&fileData[j], &iterationKey2);
            if ((j+1) % 4 == 0 && j > 0){
                unsigned char bytes[4] = {fileData[j-3], fileData[j-2], fileData[j-1], fileData[j]};
                concatenate(&bytes);
                fileData[j-3] = bytes[0]; fileData[j-2] = bytes[1]; fileData[j-1] = bytes[2]; fileData[j] = bytes[3];
            }
        }
        printf("Encodage termine !");
        printf("\n======================Encode======================\n");
        for (int j = 0; j < size; j++) printf("%c", fileData[j]);
        printf("\n==================================================\n");
    //} else{ //Decodage
        for (int j = 0; j < size; j++){
            if (j % 4 == 0){
                unsigned char bytes[4] = {fileData[j], fileData[j+1], fileData[j+2], fileData[j+3]};
                concatenateReverse(&bytes);
                fileData[j] = bytes[0]; fileData[j+1] = bytes[1]; fileData[j+2] = bytes[2]; fileData[j+3] = bytes[3];
            }
            ApplyXOROnByte(&fileData[j], &iterationKey2);
            CharApplyMatrixReverse(&fileData[j]);
            CharPermutationReverse(&fileData[j], &iterationKey1);
        }
        printf("Decodage termine !");
        printf("\n======================Decode======================\n");
        for (int j = 0; j < size; j++) printf("%c", fileData[j]);
        printf("\n==================================================");
    /*}
    writeInFile(fileData, size, link, action); fonction pour écrire les résultats dans un fichier, désactivé pour les test
    printf("Termine ! Retrouvez le resultat au meme emplacement que le fichier de depart.");*/
    return 0; //TODO utiliser free() pour libérer les allocations dynamiques
}
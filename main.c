#include <stdio.h>
#include <stdlib.h>

/**
 * Lit une chaîne de caractère
 * La fonction writeString() permet de lire une chaîne de caractère quelconque terminée par un retour à la ligne
 * @note Cette fonction prend tout caractère visible à l'exception des retours à la ligne
 * @attention Cette fonction ne prendra pas un espace situé en début de chaîne (ex: " exemple" donnera "exemple")
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
 * La fonction readFile() permet de lire le contenu d'un fichier quelconque dont on lui donne le lien (relatif au programme)
 * @warning Si le fichier n'existe pas ou que la fonction n'arrive pas à le lire, elle affiche l'erreur suivante : "Erreur: Impossible d'ouvrir le fichier <fichier>", <fichier> étant le fichier que la fonction à essayer d'ouvrir
 * @note Dans le cas d'une erreur de lecture la fonction ne change pas size
 * @param fileLink - lien vers le fichier à lire
 * @param fileData - données récuperées du fichier
 * @param size - entier qui recevra la taille du fichier lu
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  //Ceci affiche le contenu du fichier test.txt à l'emplacement du programme
 *  unsigned char *file = NULL; int size = 0;
 *  readFile("./test.txt", &file, &size);
 *  if (size > 0) for (i = 0; i < size; i++) printf("%c", file[i]);
 * ~~~~~~~~~~~~~~
 */
void readFile(char *fileLink, unsigned char **fileData, int *size){
    FILE *sourceFile = NULL;
    if ((sourceFile = fopen(fileLink, "rb")) == NULL) fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s", fileLink);
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
 * La fonction writeInFile() permet d'écrire des données dans un fichier basé sur le fichier d'entrée et le mode choisi
 * @note l'extension "_encrypted" ou "decrypted" sera ajouté à la fin du nom du fichier selon le mode choisi
 * @warning Si un fichier du même nom existe déjà, il sera écrasé et remplacé par celui généré par la fonction
 * @param data - données à écrire dans le fichier
 * @param size - quantité de données à écrire
 * @param sourceLink - lien vers le fichier source (utilisé pour le nom du fichier de destination)
 * @param action - sélectionne l'extension "_encrypted" ou "decrypted" (0 pour la 1ère, toute autre valeur pour la 2ème)
 */
void writeInFile(unsigned char *data, const int *size, const char *sourceLink, const unsigned char *action){
    int pointPosition=0, linkLenght=0;
    //Recherche taille du lien et position du point de l'extension de fichier
    while (sourceLink[linkLenght] != '\0'){
        if (sourceLink[linkLenght] == '.') pointPosition = linkLenght;
        linkLenght++;
    }
    char *destlink = (char *) malloc((linkLenght + 11) * sizeof(char));
    //Copie du début du lien jusqu'au point
    for (int i = 0; i < pointPosition; i++) destlink[i] = sourceLink[i];
    if (*action == 0) {
        char encrypted[] = "_encrypted";
        for (int j = 0; j <= 10; j++) destlink[pointPosition + j] = encrypted[j];
    } else {
        char decrypted[] = "_decrypted";
        for (int j = 0; j <= 10; j++) destlink[pointPosition + j] = decrypted[j];
    }
    //Ajout de l'extension du lien original
    for (int i = pointPosition; i <= linkLenght; i++) destlink[10 + i] = sourceLink[i];
    //Ecriture des données
    FILE *destinationFile = fopen(destlink, "wb");
    for (int i = 0; i < *size; i++) fprintf(destinationFile, "%c", data[i]);
    fclose(destinationFile);
    printf("Termine ! Retrouvez le resultat a l'emplacement suivant: %s", destlink);
    free(destlink);
}

/**
 * Permutation d'un octet
 * La fonction CharPermutation() va prendre un octet et le décaler vers la droite dans la table ASCII
 * @note Si la valeur dépasse 255, on repart de 0
 * @param byte - caractère/octet à permuter, mis à jour suite à la permutation
 * @param key - valeur selon laquel byte sera décalé
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char octet = 'a';
 *  printf("%c", CharPermutation(byte, 2)); //Le résultat sera 'c'
 * ~~~~~~~~~~~~~~
 */
void CharPermutation(unsigned char *byte, const unsigned char *key){
    *byte += *key % 256;
}

/**
 * Inversion de la permutation d'un octet
 * La fonction CharPermutationReverse() va prendre un octet et le décaler vers la gauche dans la table ASCII
 * @note Si la valeur atteint 0, on repart de 255
 * @param byte - caractère/octet à permuter, mis à jour suite à la permutation
 * @param key - valeur selon laquel byte sera décalé
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char octet = 'c';
 *  printf("%c", CharPermutationReverse(byte, 2)); //Le résultat sera 'a'
 * ~~~~~~~~~~~~~~
 */
void CharPermutationReverse(unsigned char *byte, const unsigned char *key){
    *byte -= *key % 256;
}

/**
 * Multiplication matrice par vecteur
 * La fonction multiplyMatrices() effectue une multiplication entre une matrice et un vecteur dans cet ordre
 * @warning La matrice doit être de dimension 8x8 et le vecteur de dimension 8x1
 * @param v - vecteur V de taille 8x1, mis à jour suite à la multiplication
 * @param H - matrice de taille 8x8
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
    int i = 0, *bits = (int *) calloc(8, sizeof(int));
    while (byte > 1){
        bits[i] = byte % 2;
        byte /= 2;
        i++;
    } bits[i] = byte;
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
    for (int i = 0, powerOf2 = 1; i < 8; i++, powerOf2 *= 2) byte += bits[i] * powerOf2;
    return byte;
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
    free(bits);
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
    free(bits);
}

/**
 * Fonction XOR logique
 * La fonction ApplyXOROnByte() prend un octet/caractère et va appliquer l'opération logique XOR individuellement entre chaque bit de l'octet et la clé d'itération n°2
 * @param byte - caractère/octet à encoder, mis à jour suite à la fonction
 * @param key - 2ème valeur (sous forme d'entier) pour faire le XOR
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char example = 'c'; //Valeur ASCII de 'c': 99
 *  ApplyXOROnByte(&example, ' '); //Valeur ASCII de ' ': 32
 *  printf("%c", example); //On aura 'C' (écart de 32 entre majuscule et minuscule)
 * ~~~~~~~~~~~~~~
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
 * La fonction concatenate() prend 4 octets/caractères et va les concaténer en respectant un ordre d'opération précis (voir ci-dessous).
 * Opération effectuées :
 *
 * Z[0] = Y[0] + Y[1]
 *
 * Z[1] = Y[0] + Y[1] + Y[2]
 *
 * Z[2] = Y[1] + Y[2] + Y[3]
 *
 * Z[3] = Y[2] + Y[3]
 *
 * Y étant l'entrée et Z la sortie
 * @param byte0 - correspond à Y[0], devient Z[0] suite à l'appel de la fonction
 * @param byte1 - correspond à Y[1], devient Z[1] suite à l'appel de la fonction
 * @param byte2 - correspond à Y[2], devient Z[2] suite à l'appel de la fonction
 * @param byte3 - correspond à Y[3], devient Z[3] suite à l'appel de la fonction
 */
void concatenate(unsigned char *byte0, unsigned char *byte1, unsigned char *byte2, unsigned char *byte3){
    unsigned char temp0 = *byte0, temp1 = *byte1, temp2 = *byte2, temp3 = *byte3;
    *byte0 = (temp0 + temp1) % 256;
    *byte1 = (temp0 + temp1 + temp2) % 256;
    *byte2 = (temp1 + temp2 + temp3) % 256;
    *byte3 = (temp2 + temp3) % 256;
}

/**
 * Fonction de Concaténation Inverse
 * La fonction concatenateReverse() prend 4 octets/caractères et va inverser la concaténation de concatenate() en respectant un ordre d'opération précis (voir ci-dessous).
 * Opération effectuées :
 *
 * Y[0] = Z[0] - Z[2] + Z[3]
 *
 * Y[1] = Z[2] - Z[3]
 *
 * Y[2] = Z[1] - Z[0]
 *
 * Y[3] = Z[0] - Z[1] + Z[3]
 *
 * Z étant l'entrée et Y la sortie
 * @param byte0 - correspond à Z[0], devient Y[0] suite à l'appel de la fonction
 * @param byte1 - correspond à Z[1], devient Y[1] suite à l'appel de la fonction
 * @param byte2 - correspond à Z[2], devient Y[2] suite à l'appel de la fonction
 * @param byte3 - correspond à Z[3], devient Y[3] suite à l'appel de la fonction
 */
void concatenateReverse(unsigned char *byte0, unsigned char *byte1, unsigned char *byte2, unsigned char *byte3){
    unsigned char temp0 = *byte0, temp1 = *byte1, temp2 = *byte2, temp3 = *byte3;
    *byte0 = (temp0 - temp2 + temp3) % 256;
    *byte1 = (temp2 - temp3) % 256;
    *byte2 = (temp1 - temp0) % 256;
    *byte3 = (temp0 - temp1 + temp3) % 256;
}

//utiliser "doxygen Doxyfile" pour mettre à jour la documentation
int main() {
    printf("===================================================================================================================\n\n");
    printf("#######  #######  #     #  #######  #######  #######  #######  #######  #######  #######  #     #  #######  #######\n");
    printf("#        #     #  #     #  #     #     #     #     #  #        #     #  #     #  #     #  #     #  #        #     #\n");
    printf("#        #######   #   #   #######     #     #     #  #        #######  #######  #######  #######  #        #######\n");
    printf("#        #    #      #     #           #     #     #  #   ###  #    #   #     #  #        #     #  #####    #    #\n");
    printf("#        #     #     #     #           #     #     #  #     #  #     #  #     #  #        #     #  #        #     #\n");
    printf("#######  #     #     #     #           #     #######  #######  #     #  #     #  #        #     #  #######  #     #\n");
    printf("\n===================================================================================================================\n");
    printf("Bonjour ! Cette application vous permet d'encrypter/decrypter tous types de fichier.\n");
    unsigned char action; int N;
    do {
        printf("Que voulez-vous faire ? (0 pour encryptage, 1 pour decryptage)\n");
        scanf(" %c", &action); action -= '0';
    } while (action < 0 || action > 1);
    if (action == 0) printf("Encryptage selectionne, ");
    else printf("Decryptage selectionne, ");
    int size = 0; char *link; unsigned char *fileData = NULL, temp = 1;
    do {
        printf("veuillez entrer le lien du fichier relatif au programme (ex: ../toto.txt)\n");
        link = writeString();
        readFile(link, &fileData, &size);
        if (size < 1) {
            fprintf(stderr, ", voulez-vous reessayer ? (0 pour oui, 1 pour non)\n");
            scanf(" %c", &temp); temp -= '0';
            if (temp == 1) return EXIT_SUCCESS;
        }
    } while (size < 1);

    printf("Entrer la cle de chiffrement\n");
    char *encryptionKey = writeString();
    do {
        char temp1[10], *temp2;
        printf("Enfin, entrez le nombre d'iterations a effectuer (nombre plus grand, plus de securite mais processus plus long)\n");
        scanf(" %s", temp1);
        N = strtol(temp1, &temp2, 10);
    } while (N <= 0);

    for (int i = 1; i <= N; i++) {
        unsigned char iterationKey1 = 0, iterationKey2 = 0;
        int keyGeneratorIterator = 0;
        while (encryptionKey[keyGeneratorIterator] != '\0') { //Création des clés d'itérations
            int iterationVariation = i;
            if (action == 1) iterationVariation = N - i + 1;
            iterationKey1 += encryptionKey[keyGeneratorIterator] + iterationVariation;
            iterationKey2 *= encryptionKey[keyGeneratorIterator] + iterationVariation;
            keyGeneratorIterator++;
        }
        if (action == 0) { //Encodage
            for (int j = 0; j < size; j++) {
                CharPermutation(&fileData[j], &iterationKey1);
                CharApplyMatrix(&fileData[j]);
                ApplyXOROnByte(&fileData[j], &iterationKey2);
                if ((j + 1) % 4 == 0 && j > 0) concatenate(&fileData[j - 3], &fileData[j - 2], &fileData[j - 1], &fileData[j]);
            }
        } else { //Decodage
            for (int j = 0; j < size; j++) {
                if (j % 4 == 0) concatenateReverse(&fileData[j], &fileData[j + 1], &fileData[j + 2], &fileData[j + 3]);
                ApplyXOROnByte(&fileData[j], &iterationKey2);
                CharApplyMatrixReverse(&fileData[j]);
                CharPermutationReverse(&fileData[j], &iterationKey1);
            }
        }
    }
    writeInFile(fileData, &size, link, &action); //fonction pour écrire les résultats dans un fichier, désactivé pour les test
    //free(fileData); //TODO gérer problème en cas de présence de caractère table ASCII étendue
    return EXIT_SUCCESS;
}
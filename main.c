/**
 * @file main.c Fichier principal du programme
 *
 * Ce programme n'est constitué que du fichier main.c qui contient toutes les fonctions nécessaires à son fonctionnement
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Affiche une barre de progression
 * La fonction printProgress() affiche une barre de progression qui s'actualise à chaque appel
 * @date 17 Décembre 2020
 * @note inspiration: https://gist.github.com/amullins83/24b5ef48657c08c4005a8fab837b7499
 * @param currentIteration - numéro de l'itération en cours
 * @param total - nombre total d'itérations
 */
void printProgress(const int *currentIteration, const int *total){
    const char prefix[] = "Progression: [", suffix[] = "]";
    const unsigned char prefixLength = sizeof(prefix) - 1, suffixLength = sizeof(suffix) - 1;
    char *progressBar = calloc(prefixLength + 100 + suffixLength + 1, 1);

    for (int i = 0; prefix[i] != '\0'; i++) progressBar[i] = prefix[i];
    for (int i = 0; i < 100; i++) progressBar[prefixLength + i] = i < (100 - (((*total - *currentIteration) * 100) / *total)) ? '#' : ' ';
    for (int i = 0; suffix[i] != '\0'; i++) progressBar[prefixLength + 100 + i] = suffix[i];

    printf("\b\r%c[2K\r%s", 27, progressBar);
    fflush(stdout); free(progressBar);
}

/**
 * Lit une chaîne de caractère
 * La fonction writeString() permet de lire une chaîne de caractère quelconque terminée par un retour à la ligne
 * @date 21 Novembre 2020
 * @note Cette fonction prend tout caractère visible à l'exception des retours à la ligne
 * @attention Cette fonction ne prendra pas un espace situé en début de chaîne (ex: " exemple" donnera "exemple")
 * @return addresse où est stocké la chaîne de caractère
 */
char *writeString(){
    int i = 0;
    char temp, *string = (char*) malloc(20 * sizeof(char)); //On part d'une chaîne de 20 caractères
    scanf(" %c", &temp);
    while (temp >= ' ' && temp <= '~'){
        if (i % 19 == 1 && i > 19) string = (char *) realloc (string, (i + 20) * sizeof(char)); //Si on dépasse 20 caractères, on ajoute un espace de 20 caractères à la chaîne
        string[i] = temp;
        scanf("%c", &temp);
        i++;
    } string[i] = '\0';
    return string;
}

/**
 * Lit le contenu d'un fichier
 * La fonction readFile() permet de lire le contenu d'un fichier quelconque dont on lui donne le lien (relatif au programme)
 * @date 29 Octobre 2020
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
        fseek(sourceFile, 0L, SEEK_END); //Recherche de la fin du fichier
        *size = ftell(sourceFile); //Stockage de la taille
        rewind(sourceFile);
        unsigned char *temp = (unsigned char *) malloc(*size * sizeof(unsigned char)); //Création d'un tableau pour contenir les octets du fichier
        for (int i = 0; i < *size; i++) fscanf(sourceFile, "%c", &temp[i]);
        fclose(sourceFile);
        *fileData = temp;
    }
}

/**
 * Ecrit dans un fichier
 * La fonction writeInFile() permet d'écrire des données dans un fichier basé sur le fichier d'entrée et le mode choisi
 * @date 21 Novembre 2020
 * @note l'extension "_encrypted" ou "decrypted" sera ajouté à la fin du nom du fichier selon le mode choisi
 * @warning Si un fichier du même nom existe déjà, il sera écrasé et remplacé par celui généré par la fonction
 * @param data - données à écrire dans le fichier
 * @param size - quantité de données à écrire
 * @param sourceLink - lien vers le fichier source (utilisé pour le nom du fichier de destination)
 * @param action - sélectionne l'extension "_encrypted" ou "_decrypted" (0 pour la 1ère, toute autre valeur pour la 2ème)
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
    printf("\nTermine ! Retrouvez le resultat a l'emplacement suivant: %s", destlink);
    free(destlink);
}

/**
 * Permutation d'un octet
 * La fonction charPermutation() va prendre un octet et le décaler vers la droite dans la table ASCII
 * @date 29 Octobre 2020
 * @note Si la valeur dépasse 255, on repart de 0
 * @param byte - caractère/octet à permuter, mis à jour suite à la permutation
 * @param key - valeur selon laquel byte sera décalé
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char octet = 'a';
 *  printf("%c", charPermutation(byte, 2)); //Le résultat sera 'c'
 * ~~~~~~~~~~~~~~
 */
void charPermutation(unsigned char *byte, const unsigned char *key){
    *byte += *key % 256;
}

/**
 * Inversion de la permutation d'un octet
 * La fonction charPermutationReverse() va prendre un octet et le décaler vers la gauche dans la table ASCII
 * @date 29 Octobre 2020
 * @note Si la valeur atteint 0, on repart de 255
 * @param byte - caractère/octet à permuter, mis à jour suite à la permutation
 * @param key - valeur selon laquel byte sera décalé
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char octet = 'c';
 *  printf("%c", charPermutationReverse(byte, 2)); //Le résultat sera 'a'
 * ~~~~~~~~~~~~~~
 */
void charPermutationReverse(unsigned char *byte, const unsigned char *key){
    *byte -= *key % 256;
}

/**
 * lit un fichier d'entier formaté
 * la fonction readTable lit un fichier d'entier formatée suivant les conditions suivantes :
 *
 * - les entiers sont compris entre 0 et 255 inclus
 * - chaque entier doit être différent
 * - Ils sont séparés par un caractère différent d’un nombre
 * - il y a au moins 256 entiers (les entiers après les 256 premiers ne seront pas lus)
 *
 * @date 29 Décembre 2020
 * @attention Si le fichier n'est pas conforme aux points ci-dessus, la fonction s'arrête et indique le problème
 * @warning Si le fichier n'existe pas la fonction affiche "Impossible d'ouvrir la table de permutation" et s'arrête
 * @param link - Lien vers le fichier contenant la table de permutation
 * @param permutationTable - table de permutation mise à jour suite à la fonction
 */
void readTable(char *link, unsigned char **permutationTable){
    FILE *tableFile = NULL;
    if ((tableFile = fopen(link, "rb")) == NULL) printf("Impossible d'ouvrir la table de permutation, ");
    else{
        fseek(tableFile, 0L, SEEK_END); //Recherche de la fin du fichier
        int size = ftell(tableFile); //Stockage de la taille
        rewind(tableFile);

        if (size >= 913){
            *permutationTable = (unsigned char *) calloc(256, sizeof(unsigned char));
            int i = 0, positionInFile = 0;
            while (i < 256 && *permutationTable != NULL){
                char temp;
                fscanf(tableFile, "%c", &temp); positionInFile++;
                while (temp >= '0' && temp <= '9' && positionInFile <= size){ //Lecture et assemblage des chiffres
                    (*permutationTable)[i] = (*permutationTable)[i] * 10 + temp - '0';
                    fscanf(tableFile, "%c", &temp); positionInFile++;
                }
                for (int j = 0; j < i; j++){ //Si valeur dupliquée ou sortant de l'intervalle
                    if ((*permutationTable)[i] == (*permutationTable)[j]){
                        printf("L'entier n%d et/ou l'entier n%d est/sont duplique(s) dans la table ou sort(ent) de l'intervalle [0, 255], ", i, j);
                        free(*permutationTable); *permutationTable = NULL;
                        break;
                    }
                } i++;
            }
        } else printf("La table de permutation est incomplete ou mal formatee, "); //Il manque des valeurs
        fclose(tableFile);
    }
}

/**
 * Permutation d'un octet selon une table de permutation
 * La fonction permutationWithTable() prend un octet et le modifie en prenant la valeur à la position donnée par l'octet dans une table de permutation fournie par un fichier externe nommé "permutation.txt"
 * @date 29 Décembre 2020
 * @warning le fichier "permutation.txt" doit être ua même emplacement que le programme, sans quoi on obtient une erreur
 * @param byte - caractère/octet à permuter, mis à jour à la suite de la permutation
 */
void permutationWithTable(unsigned char *byte, const unsigned char *permutationTable){
    *byte = permutationTable[*byte];
}

/**
 * Inversion de la permutation d'un octet selon une table de permutation
 * La fonction permutationWithTableReverse() prend un octet et le modifie en prenant l'indice associé à la valeur donnée par l'octet dans une table de permutation fournie par un fichier externe nommé "permutation.txt"
 * @date 29 Décembre 2020
 * @warning le fichier "permutation.txt" doit être ua même emplacement que le programme, sans quoi on obtient une erreur
 * @param byte - caractère/octet à permuter, mis à jour à la suite de la permutation
 */
void permutationWithTableReverse(unsigned char *byte, const unsigned char *permutationTable){
    int i = 0;
    while (permutationTable[i] != *byte && i < 256) i++;
    *byte = i;
}

/**
 * Multiplication matrice par vecteur
 * La fonction multiplyMatrices() effectue une multiplication entre une matrice et un vecteur dans cet ordre
 * @date 09 Novembre 2020
 * @warning La matrice doit être de dimension 8x8 et le vecteur de dimension 8x1
 * @param H - matrice de taille 8x8
 * @param v - vecteur V de taille 8x1, mis à jour suite à la multiplication
 */
void multiplyMatrices(unsigned char (*H)[8], unsigned char **v){
    unsigned char vRes[8];
    for (int rowFinal = 0, temp = 0; rowFinal < 8; rowFinal++){
        for (int k = 0; k < 8; k++) temp += H[rowFinal][k] * (*v)[k];
        vRes[rowFinal] = temp % 2; //On fait %2 pour rester en binaire
        temp = 0;
    } for (int i = 0; i < 8; i++) (*v)[i] = vRes[i];
}

/**
 * Conversion d'un octet/caractère vers un tableau de bits
 * La fonction byteToBits() prend un octet/caractère et donne en sortie un tableau de bits correspondant à sa valeur en binaire
 * @date 09 Novembre 2020
 * @note Le bit de poids le plus faible se trouve en position 0
 * @param byte - caractère/octet à convertir
 * @param bits - tableau de bits correspondant à la valeur en binaire de l'octet
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  int byte[8] = byteToBits('c');
 *  for (int i=7; i>=0; i--) printf("%d", byte[i]); //On aura 01100011
 * ~~~~~~~~~~~~~~
 */
void byteToBits(unsigned char *byte, unsigned char **bits){
    *bits = (unsigned char *) calloc(8, sizeof(unsigned char));
    for (int i = 0; (*byte) > 0; i++, *(byte) /= 2) (*bits)[i] = *byte % 2;
}

/**
 * Conversion d'un tableau de bits vers un octet/caractère
 * La fonction bitsToByte() prend un tableau de bits et donne en sortie un octet/caractère correspondant à sa valeur base 10
 * @date 09 Novembre 2020
 * @param bits - tableau de bits correspondant à la valeur en binaire de l'octet
 * @param byte - octet correspondant à la valeur en base 10 du tableau de bits
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  int bits[] = {1, 1, 0, 0, 0, 1, 1, 0};
 *  printf("%c", bitsToByte(bits); //On aura 'c'
 * ~~~~~~~~~~~~~~
 */
void bitsToByte(const unsigned char *bits, unsigned char *byte){
    for (int i = 0, powerOf2 = 1; i < 8; i++, powerOf2 *= 2) *byte += bits[i] * powerOf2;
}

/**
 * Fonction Matriciel
 * La fonction charApplyMatrix() prend un octet/caractère et va le multiplier par une matrice H puis effectuer une addition avec C pour encoder cet octet/caractère
 * @date 09 Novembre 2020
 * @note La matrice H et le vecteur C sont connus auparavant
 * @param byte - caractère/octet à encoder, mis à jour suite à l'application de la fonction
 * @see bitsToByte() byteToBits() multiplyMatrices()
 */
void charApplyMatrix(unsigned char *byte){
    unsigned char *bits = NULL, H[][8] = {
            1, 0, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 1, 1
    };
    byteToBits(byte, &bits);
    multiplyMatrices(H, &bits); //Ce qui correspond à H x vi
    unsigned char c[] = {1, 1, 0, 0, 0, 1, 1, 0};
    for (int j = 0; j < 8; j++) bits[j] = (bits[j] + c[j]) % 2; //Ce qui correspond à Xi = H x vi + c
    bitsToByte(bits, byte);
    free(bits);
}

/**
 * Fonction Matriciel Inverse
 * La fonction charApplyMatrixReverse() prend un octet/caractère et va le multiplier par une matrice H' puis effectuer une addition avec C' pour décoder cet octet/caractère après passage dans charApplyMatrix()
 * @date 09 Novembre 2020
 * @note La matrice H' et le vecteur C' sont connu auparavant
 * @param byte - caractère/octet à décoder, mis à jour suite à l'application de la fonction
 * @see bitsToByte() byteToBits() multiplyMatrices()
 */
void charApplyMatrixReverse(unsigned char *byte){
    unsigned char *bits = NULL, HPrime[][8] = {
            0, 0, 1, 0, 0, 1, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 0,
            0, 1, 0, 0, 1, 0, 0, 1,
            1, 0, 1, 0, 0, 1, 0, 0,
            0, 1, 0, 1, 0, 0, 1, 0,
            0, 0, 1, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 1, 0
    };
    byteToBits(byte, &bits);
    multiplyMatrices(HPrime, &bits); //Ce qui correspond à H' x Xi
    unsigned char cPrime[] = {1, 0, 1, 0, 0, 0, 0, 0};
    for (int j = 0; j < 8; j++) bits[j] = (bits[j] + cPrime[j]) % 2; //Ce qui correspond à vi = H' x Xi + c'
    bitsToByte(bits, byte);
    free(bits);
}

/**
 * Fonction XOR logique
 * La fonction applyXOROnByte() prend un octet/caractère et va appliquer l'opération logique XOR individuellement entre chaque bit de l'octet et la clé d'itération n°2
 * @date 05 Décembre 2020
 * @param byte - caractère/octet à encoder, mis à jour suite à la fonction
 * @param key - 2ème valeur (sous forme d'entier) pour faire le XOR
 * ### Exemple
 * ~~~~~~~~~~~~~~.c
 *  unsigned char example = 'c'; //Valeur ASCII de 'c': 99
 *  applyXOROnByte(&example, ' '); //Valeur ASCII de ' ': 32
 *  printf("%c", example); //On aura 'C' (écart de 32 entre majuscule et minuscule)
 * ~~~~~~~~~~~~~~
 * @see bitsToByte() byteToBits()
 */
void applyXOROnByte(unsigned char *byte, const unsigned char *key){
    unsigned char *bits = NULL, *keyBits = NULL, localKey = *key;
    byteToBits(byte, &bits); byteToBits(&localKey, &keyBits);
    for (int i = 0; i < 8; i++) bits[i] = (bits[i] + keyBits[i]) % 2;
    bitsToByte(bits, byte);
    free(keyBits); free(bits);
}

/**
 * Fonction de Concaténation
 * La fonction concatenate() prend 4 octets/caractères et va les concaténer en respectant un ordre d'opération précis (voir ci-dessous).
 * Opération effectuées :
 *
 * - Z[0] = Y[0] + Y[1]
 * - Z[1] = Y[0] + Y[1] + Y[2]
 * - Z[2] = Y[1] + Y[2] + Y[3]
 * - Z[3] = Y[2] + Y[3]
 *
 * Y étant l'entrée et Z la sortie
 * @date 14 Décembre 2020
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
 * - Y[0] = Z[0] - Z[2] + Z[3]
 * - Y[1] = Z[2] - Z[3]
 * - Y[2] = Z[1] - Z[0]
 * - Y[3] = Z[0] - Z[1] + Z[3]
 *
 * Z étant l'entrée et Y la sortie
 * @date 14 Décembre 2020
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
int main(){
    printf("===================================================================================================================\n\n");
    printf("#######  #######  #     #  #######  #######  #######  #######  #######  #######  #######  #     #  #######  #######\n");
    printf("#        #     #  #     #  #     #     #     #     #  #        #     #  #     #  #     #  #     #  #        #     #\n");
    printf("#        #######   #   #   #######     #     #     #  #        #######  #######  #######  #######  #        #######\n");
    printf("#        #    #      #     #           #     #     #  #   ###  #    #   #     #  #        #     #  #####    #    #\n");
    printf("#        #     #     #     #           #     #     #  #     #  #     #  #     #  #        #     #  #        #     #\n");
    printf("#######  #     #     #     #           #     #######  #######  #     #  #     #  #        #     #  #######  #     #\n");
    printf("\n===================================================================================================================\n");
    printf("Bonjour ! Ce programme vous permet d'encrypter/decrypter tous types de fichier.\n");
    unsigned char action; int N;
    do{
        printf("Que voulez-vous faire ? (0 pour encryptage, 1 pour decryptage)\n");
        scanf(" %c", &action); action -= '0';
    } while (action < 0 || action > 1);
    if (action == 0) printf("Encryptage selectionne, ");
    else printf("Decryptage selectionne, ");

    int size = 0; char *link; unsigned char *fileData = NULL, temp = 1;
    do{
        printf("veuillez entrer le lien du fichier relatif au programme (ex: ../toto.txt)\n");
        link = writeString();
        readFile(link, &fileData, &size);
        if (size < 1){
            fprintf(stderr, ", voulez-vous reessayer ? (0 pour oui, 1 pour non)\n");
            scanf(" %c", &temp); temp -= '0';
            free(link);
            if (temp == 1) return EXIT_SUCCESS;
        }
    } while (size < 1);

    printf("Entrer la cle de chiffrement\n");
    char *encryptionKey = writeString();
    do{
        char temp1[10], *temp2;
        printf("Enfin, entrez le nombre d'iterations a effectuer entre 0 et 999 999 (nombre plus grand, plus de securite mais processus plus long)\n");
        scanf(" %s", temp1);
        if ((N = strtol(temp1, &temp2, 10)) > 999999) N = 999999;
        getchar(); //Elimine le retour à la ligne
    } while (N < 1);

    unsigned char *permutationTable = NULL;
    char *tableLink = NULL;
    readTable("permutation.txt", &permutationTable);
    while (permutationTable == NULL){
        printf("entrer le lien de la table de permutation (ou 0 pour stopper le programme)\n");
        tableLink = writeString();
        if (tableLink[0] == '0') exit(EXIT_SUCCESS);
        else readTable(tableLink, &permutationTable);
    } free(tableLink);

    int i = 0;
    printProgress(&i, &N);
    for (i = 1; i <= N; i++){
        unsigned char iterationKey1 = 0, iterationKey2 = 0;
        int keyGeneratorIterator = 0;
        while (encryptionKey[keyGeneratorIterator] != '\0'){ //Création des clés d'itérations
            int iterationVariation = action == 1 ? N - i + 1 : i;
            iterationKey1 += encryptionKey[keyGeneratorIterator] + iterationVariation;
            iterationKey2 *= encryptionKey[keyGeneratorIterator] + iterationVariation;
            keyGeneratorIterator++;
        }
        if (action == 0){ //Encodage
            for (int j = 0; j < size; j++){
                charPermutation(&fileData[j], &iterationKey1);
                permutationWithTable(&fileData[j], permutationTable);
                charApplyMatrix(&fileData[j]);
                applyXOROnByte(&fileData[j], &iterationKey2);
                if ((j + 1) % 4 == 0) concatenate(&fileData[j - 3], &fileData[j - 2], &fileData[j - 1], &fileData[j]);
            }
        } else{ //Decodage
            for (int j = 0; j < size; j++){
                if (j % 4 == 0 && j + 3 < size) concatenateReverse(&fileData[j], &fileData[j + 1], &fileData[j + 2], &fileData[j + 3]);
                applyXOROnByte(&fileData[j], &iterationKey2);
                charApplyMatrixReverse(&fileData[j]);
                permutationWithTableReverse(&fileData[j], permutationTable);
                charPermutationReverse(&fileData[j], &iterationKey1);
            }
        }
        printProgress(&i, &N);
    }
    free(permutationTable); free(encryptionKey);
    writeInFile(fileData, &size, link, &action); //écriture des résultats dans un fichier
    free(link); free(fileData);
    printf("\nAppuyer sur entree pour mettre fin au programme\n"); getchar();
    return EXIT_SUCCESS;
}
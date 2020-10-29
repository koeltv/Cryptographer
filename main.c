#include <stdio.h>

int main() {
    int k=0;
    char tempchar, input[101] = ""; //limite à 100 caractères (avec '\0')
    printf("Saisir une chaine de caracteres : ");
    scanf("%c", &tempchar);
    while (tempchar > 31 && tempchar < 127){ //On vérifie que les caractères soit des caractères visbles (voir table ASCII)
        input[k] = tempchar;
        scanf("%c", &tempchar);
        k++;
    }
    printf("La chaine de caracteres encodee vaut : %s", input);
    return 0;
}

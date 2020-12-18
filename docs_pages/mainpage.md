@mainpage Manuel du Cryptographer
###Présentation
Le but de ce projet est de réaliser un programme permettant d’encrypter des fichiers quelconques avec une clé que l’on lui fournit ainsi que de déchiffrer ces fichiers (avec cette même clé).

Pour le cryptage, on utilise 5 méthodes différentes de cryptage symétrique (qui sont réversibles) que l’on va répéter N fois (N étant choisi par l’utilisateur) afin d’augmenter la sécurité du cryptage. Ce projet se fera purement en langage C avec une équipe de 2 développeurs.

Prenons l’exemple suivant :
 ![](schema.png)
 
Un fichier est fourni au programme avec une « clé » et le nombre de répétitions N, le programme va encrypter le fichier puis envoyer ce nouveau fichier au même endroit que le fichier fourni. 

Inversement on peut demander au programme de décrypter un fichier en lui fournissant le fichier, la « clé » et le nombre de répétitions.

C’est pour cela que l’on appelle cette méthode la cryptographie symétrique, on va passer les mêmes paramètres en entrée pour crypter ou décrypter un fichier (à part le fait que pour décrypter, il faudra le fichier crypté, bien entendu).
###Mode d'emploi
A l’ouverture du programme, celui-ci commencera par vous demander si vous voulez crypter ou décrypter un fichier, après quoi 3 informations vous seront demandées dans cette ordre :
1.	L’action à effectuer (encryptage/décryptage),
2.	Le lien du fichier à encrypter, relatif à l’emplacement du programme (ex : ../fichier/toto.txt),
3.	La clé d’encryptage, sous forme de chaîne de caractère (ex : Ceci est une cle d’encryptage),
4.	Le nombre d’itérations voulues (plus d’itérations donne un meilleur encryptage mais cela est plus long).

Une fois toutes ces informations saisies, le programme va commencer le cryptage/décryptage. Lors de ce processus, une barre de progression sera affichée et actualisé tout au long de l’exécution pour vous tenir au courant de l’état de l’encryptage/décryptage. Quand il aura fini vous serez informé par le texte suivant « Termine ! Retrouvez le resultat a l'emplacement suivant: <emplacement> », <emplacement> étant l’emplacement ou à été enregistré le fichier.

En cas d’erreur, si le fichier à encrypter n’existe pas par exemple, le programme vous indiquera cette erreur et vous donnera la possibilité de la corriger (dans le cas d’un fichier non existant, il vous proposera d’entrer à nouveau un lien vers un fichier).

Une fois que vous avez trouvé votre fichier, au même emplacement que le fichier de départ (cet emplacement vous sera indiqué par le programme), vous pouvez fermer la fenêtre du programme sans problème en appuyant sur la touche entrée.

####Attention ! 
Le programme fonctionne avec tout type de caractères, y compris les caractères accentués, néanmoins cette liste de caractère reste non exhaustive (limitée aux caractères de la table ASCII étendue, voir ci-dessous), aussi faites attention au caractères spécifiques « trop exotiques ». Même si ceux si n’entraîneront pas la fermeture du programme et vous serons signalés, ils provoqueront des modifications qui seront visibles lors du décodage, par exemple le(s) caractère(s) problématique(s) remplacés par des chaînes de caractères illisibles, comme ceci : « Ã§Ã© ».

Table ASCII standard :

 ![](asciitable.png)
 
Ajouts de la table ASCII étendue :

 ![](extendedasciitable.png)
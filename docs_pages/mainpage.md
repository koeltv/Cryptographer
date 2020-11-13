@mainpage Manuel du Cryptographer

Le but de ce projet est de réaliser un programme permettant d’encrypter des fichiers quelconques avec une clé que l’on lui fournit ainsi que de déchiffrer ces fichiers (avec cette même clé). Pour le cryptage, on utilise 5 méthodes différentes de cryptage symétrique (qui sont réversibles) que l’on va répéter N fois (N étant choisi par l’utilisateur) afin d’augmenter la sécurité du cryptage. Ce projet se fera purement en langage C avec une équipe de 2 développeurs.

Prenons l’exemple suivant :
 ![](schema.png)
 
Un fichier est fourni au programme avec une « clé » et le nombre de répétitions N, le programme va encrypter le fichier puis envoyer ce nouveau fichier au même endroit que le fichier fourni. 
Inversement on peut demander au programme de décrypter un fichier en lui fournissant le fichier, la « clé » et le nombre de répétitions.
C’est pour cela que l’on appelle cette méthode la cryptographie symétrique, on va passer les mêmes paramètres en entrée pour crypter ou décrypter un fichier (à part le fait que pour décrypter, il faudra le fichier crypté, bien entendu).
Dans la prochaine partie nous vous décrieront les algorithmes utilisés pour créer ce programme, puis nous vous parlerons des problèmes que nous avons rencontré et des solutions que nous avons choisies. Nous vous donnerons ensuite un mode d’emploi expliquant l’utilisation de notre programme, puis nous finiront sur une conclusion synthétisant les différents aspects de ce projet et quelques points qui pourrait être améliorés.
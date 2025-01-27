# IN204
Projet tetris pour le cours IN204  
Binômes: Thaddée MORGAND, Ferdinand ALAIN

## Présentation du projet
Le but du projet est d'implémenter complètement le jeu TETRIS. Des pièces de couleur descendent du haut de l'écran. Le joueur ne peut pas ralentir ou empêcher cette chute mais peut décider à quel angle de rotation (0°, 90°, 180°, 270°) et à quel emplacement latéral l'objet peut atterrir. Lorsqu'une ligne horizontale est complétée sans vide, elle disparaît et les blocs supérieurs tombent. Si le joueur ne parvient pas à faire disparaître les lignes assez vite et que l'écran se remplit jusqu'en haut, il est submergé et la partie est finie.

## Distribution et bibliothèque utilisées
Pour ce projet, nous avons travaillé sur une distribution Linux (Ubuntu pour être précis) et nous avons utilisé la bibliothèque graphique SFML pour pouvoir afficher le plateau de jeu. Pour installer SFML, nous avons suivi le guide proposé par le site de SFML. Nous avons également utilisé un fichier de font afin de générer une police pour les textes affichés: vous pouvez le trouver dans le dossier font.

## Fichiers nécessaires au bon fonctionnement du programme
Assurez-vous tout d'abord d'avoir bien installé la bibliothèque SFML. Le fichier main.cpp contient la boucle principale du programme faisant tourner le jeu et doit être compilé avec game.cpp. Les fichiers game.cpp et game.hpp contiennent le code de toutes les classes et fonctions utilisées par le main. Le dossier font doit être situé au même emplacement que le fichier main.cpp au moment de la compilation. Pour compiler avec notre makefile, il suffit d'utiliser la commande make run.

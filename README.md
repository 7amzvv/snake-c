# Snake en C

Un jeu Snake avec une interface graphique, programme en C avec raylib.

## Le jeu

Le serpent commence avec un seul carre et avance avec les fleches du clavier.
Le but est de manger la nourriture rouge sans toucher les murs ou son propre corps.

Chaque nourriture rapporte 10 points et ajoute un carre au serpent.
La vitesse est fixe : une case toutes les 0,30 seconde.

Apres une partie, on peut rejouer ou consulter l'historique des scores.
Le record et l'historique sont conserves uniquement pendant la session :
ils disparaissent quand on ferme le jeu. Une partie interrompue en fermant
la fenetre n'est pas ajoutee a l'historique.

## Commandes

| Commande | Action |
| --- | --- |
| Fleches du clavier | Changer de direction |
| R, apres une partie | Rejouer |
| Souris | Utiliser les boutons |
| Molette ou haut/bas dans l'historique | Faire defiler les scores |
| Echap | Fermer le jeu |

## Installation sous Windows

La configuration fournie cible Windows 64 bits avec GCC MinGW-w64.
GCC doit etre accessible avec la commande `gcc` dans le terminal.
VS Code est facultatif.

1. Telecharger ou cloner ce depot.
2. Ouvrir un terminal PowerShell dans le dossier du projet.
3. Installer raylib avec :

```powershell
.\setup-raylib.ps1
```

Le script telecharge raylib 6.2 depuis son depot officiel et verifie
l'empreinte de l'archive. Les fichiers sont places dans `lib/raylib`.

Si Windows bloque un script ou un executable, consulter le message de
Securite Windows avant de poursuivre : le projet ne modifie aucune protection.

## Compiler et jouer

Dans VS Code, ouvrir le dossier puis utiliser **Ctrl + Maj + B** pour compiler.
La tache **Jouer**, accessible par **Terminal > Executer la tache**, compile
et lance le jeu.

On peut aussi utiliser le terminal :

```powershell
gcc -std=c11 -Wall -Wextra -Werror -pedantic main.c -Ilib/raylib/include -Llib/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -o snake.exe
.\snake.exe
```

Fermer le jeu avant de le recompiler.

## Tests

La tache VS Code **Tester Snake** verifie la nourriture, la croissance,
le score, les collisions, la victoire et la gestion de l'historique.
Ces tests ne verifient pas visuellement les boutons.

## Organisation

- `main.c` : logique du jeu et affichage.
- `tests/test_snake.c` : tests de la logique.
- `setup-raylib.ps1` : installation locale de raylib.
- `.vscode/tasks.json` : compilation, lancement et tests.
- `.gitignore` : exclusion des fichiers generes.

Ce projet travaille les structures, les tableaux, les pointeurs,
l'allocation dynamique et une boucle de jeu.

La partie graphique utilise [raylib](https://github.com/raysan5/raylib).
Les fichiers de cette bibliotheque conservent leurs mentions de licence.

## Credits

Hamza Fadli

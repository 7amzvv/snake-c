#include <stdio.h>

#define LARGEUR 20
#define HAUTEUR 10

void afficher_terrain(int serpent_x, int serpent_y)
{
    for (int y = 0; y < HAUTEUR; y++) {
        for (int x = 0; x < LARGEUR; x++) {
            /* Les cases du bord forment les murs. */
            if (y == 0 || y == HAUTEUR - 1 || x == 0 || x == LARGEUR - 1) {
                putchar('#');
            } else if (x == serpent_x && y == serpent_y) {
                putchar('O');
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
}

int main(void)
{
    /* On place la tete du serpent au centre du terrain. */
    int serpent_x = LARGEUR / 2;
    int serpent_y = HAUTEUR / 2;

    printf("Snake - etape 1 : affichage du terrain\n\n");
    afficher_terrain(serpent_x, serpent_y);

    return 0;
}

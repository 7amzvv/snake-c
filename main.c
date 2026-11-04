#include "raylib.h"

#define COLONNES 20
#define LIGNES 15
#define TAILLE_CASE 30
#define MARGE_HAUT 60

int main(void)
{
    int serpent_x = COLONNES / 2;
    int serpent_y = LIGNES / 2;

    InitWindow(COLONNES * TAILLE_CASE,
               LIGNES * TAILLE_CASE + MARGE_HAUT, "Snake");
    SetTargetFPS(60);

    /* La fenetre reste ouverte jusqu'a Echap ou un clic sur la croix. */
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){20, 24, 32, 255});
        DrawText("SNAKE", 20, 18, 28, LIME);
        DrawText("Etape 2 : le terrain", 170, 25, 18, RAYWHITE);

        for (int y = 0; y < LIGNES; y++) {
            for (int x = 0; x < COLONNES; x++) {
                int pixel_x = x * TAILLE_CASE;
                int pixel_y = y * TAILLE_CASE + MARGE_HAUT;

                /* On convertit les cases de la grille en pixels. */
                if (x == 0 || x == COLONNES - 1 ||
                    y == 0 || y == LIGNES - 1) {
                    DrawRectangle(pixel_x, pixel_y,
                                  TAILLE_CASE, TAILLE_CASE, DARKGRAY);
                } else {
                    DrawRectangleLines(pixel_x, pixel_y,
                                       TAILLE_CASE, TAILLE_CASE,
                                       (Color){32, 38, 48, 255});
                }
            }
        }

        DrawRectangle(serpent_x * TAILLE_CASE + 2,
                      serpent_y * TAILLE_CASE + MARGE_HAUT + 2,
                      TAILLE_CASE - 4, TAILLE_CASE - 4, LIME);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

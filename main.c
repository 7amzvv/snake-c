#include "raylib.h"

#define COLONNES 20
#define LIGNES 15
#define TAILLE_CASE 30
#define MARGE_HAUT 60
#define DELAI_DEPLACEMENT 0.15f

int main(void)
{
    int serpent_x = COLONNES / 2;
    int serpent_y = LIGNES / 2;
    int direction_x = 0;
    int direction_y = 0;
    float temps = 0.0f;
    bool perdu = false;

    InitWindow(COLONNES * TAILLE_CASE,
               LIGNES * TAILLE_CASE + MARGE_HAUT, "Snake");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (perdu && IsKeyPressed(KEY_R)) {
            serpent_x = COLONNES / 2;
            serpent_y = LIGNES / 2;
            direction_x = 0;
            direction_y = 0;
            temps = 0.0f;
            perdu = false;
        }

        if (!perdu) {
            if (IsKeyPressed(KEY_RIGHT)) {
                direction_x = 1;
                direction_y = 0;
            } else if (IsKeyPressed(KEY_LEFT)) {
                direction_x = -1;
                direction_y = 0;
            } else if (IsKeyPressed(KEY_UP)) {
                direction_x = 0;
                direction_y = -1;
            } else if (IsKeyPressed(KEY_DOWN)) {
                direction_x = 0;
                direction_y = 1;
            }

            if (direction_x != 0 || direction_y != 0) {
                temps += GetFrameTime();

                while (temps >= DELAI_DEPLACEMENT && !perdu) {
                    temps -= DELAI_DEPLACEMENT;
                    int prochain_x = serpent_x + direction_x;
                    int prochain_y = serpent_y + direction_y;

                    if (prochain_x <= 0 || prochain_x >= COLONNES - 1 ||
                        prochain_y <= 0 || prochain_y >= LIGNES - 1) {
                        perdu = true;
                    } else {
                        serpent_x = prochain_x;
                        serpent_y = prochain_y;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground((Color){20, 24, 32, 255});
        DrawText("SNAKE", 20, 18, 28, LIME);

        if (perdu) {
            DrawText("Perdu ! R pour rejouer", 170, 25, 18, RED);
        } else {
            DrawText("Fleches pour se deplacer", 170, 25, 18, RAYWHITE);
        }

        for (int y = 0; y < LIGNES; y++) {
            for (int x = 0; x < COLONNES; x++) {
                int pixel_x = x * TAILLE_CASE;
                int pixel_y = y * TAILLE_CASE + MARGE_HAUT;

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

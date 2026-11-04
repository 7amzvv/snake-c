#include "raylib.h"
#include <stdlib.h>

#define COLONNES 20
#define LIGNES 15
#define TAILLE_CASE 30
#define MARGE_HAUT 60
#define MAX_CASES ((COLONNES - 2) * (LIGNES - 2))
#define DELAI_DEPLACEMENT 0.30f

typedef struct { int x; int y; } Case;

typedef struct {
    Case corps[MAX_CASES];
    Case nourriture;
    int taille;
    int dx;
    int dy;
    int score;
    bool termine;
    bool gagne;
} Jeu;

typedef struct Score {
    int valeur;
    struct Score *suivant;
} Score;

bool meme_case(Case a, Case b)
{
    return a.x == b.x && a.y == b.y;
}

void placer_nourriture(Jeu *jeu)
{
    int choix = GetRandomValue(0, MAX_CASES - jeu->taille - 1);
    for (int y = 1; y < LIGNES - 1; y++) {
        for (int x = 1; x < COLONNES - 1; x++) {
            Case position = {x, y};
            bool occupee = false;
            for (int i = 0; i < jeu->taille; i++) {
                if (meme_case(position, jeu->corps[i])) {
                    occupee = true;
                    break;
                }
            }
            if (!occupee && choix-- == 0) {
                jeu->nourriture = position;
                return;
            }
        }
    }
}

void nouvelle_partie(Jeu *jeu)
{
    *jeu = (Jeu){0};
    jeu->taille = 1;
    for (int i = 0; i < jeu->taille; i++)
        jeu->corps[i] = (Case){COLONNES / 2 - i, LIGNES / 2};
    placer_nourriture(jeu);
}

void avancer(Jeu *jeu)
{
    if (jeu->termine || (jeu->dx == 0 && jeu->dy == 0)) return;
    Case tete = {jeu->corps[0].x + jeu->dx, jeu->corps[0].y + jeu->dy};
    bool mange = meme_case(tete, jeu->nourriture);
    if (tete.x <= 0 || tete.x >= COLONNES - 1 ||
        tete.y <= 0 || tete.y >= LIGNES - 1) {
        jeu->termine = true;
        return;
    }
    int limite = mange ? jeu->taille : jeu->taille - 1;
    for (int i = 0; i < limite; i++) {
        if (meme_case(tete, jeu->corps[i])) {
            jeu->termine = true;
            return;
        }
    }
    if (mange) {
        jeu->taille++;
        jeu->score += 10;
    }
    for (int i = jeu->taille - 1; i > 0; i--)
        jeu->corps[i] = jeu->corps[i - 1];
    jeu->corps[0] = tete;
    if (jeu->taille == MAX_CASES) {
        jeu->termine = true;
        jeu->gagne = true;
    } else if (mange) placer_nourriture(jeu);
}

bool enregistrer_score(Score **historique, int valeur)
{
    Score *resultat = malloc(sizeof *resultat);
    if (resultat == NULL) return false;
    *resultat = (Score){valeur, *historique};
    *historique = resultat;
    return true;
}

void liberer_historique(Score **historique)
{
    while (*historique != NULL) {
        Score *suivant = (*historique)->suivant;
        free(*historique);
        *historique = suivant;
    }
}

bool bouton(Rectangle zone, const char *texte)
{
    bool survole = CheckCollisionPointRec(GetMousePosition(), zone);
    DrawRectangleRec(zone, survole ? (Color){65, 95, 80, 255}
                                  : (Color){42, 58, 53, 255});
    DrawRectangleLinesEx(zone, 1, LIME);
    DrawText(texte, (int)(zone.x + (zone.width - MeasureText(texte, 20)) / 2),
             (int)zone.y + 13, 20, RAYWHITE);
    return survole && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

int main(void)
{
    InitWindow(COLONNES * TAILLE_CASE,
               LIGNES * TAILLE_CASE + MARGE_HAUT, "Snake");
    SetTargetFPS(60);
    Jeu jeu;
    nouvelle_partie(&jeu);
    Score *historique = NULL;
    int nombre_parties = 0, meilleur = 0, decalage = 0;
    float temps = 0;
    bool direction_changee = false, voir_historique = false;
    bool erreur_memoire = false;

    while (!WindowShouldClose()) {
        bool rejouer = false;
        if (!jeu.termine) {
            if (!direction_changee) {
                int dx = jeu.dx, dy = jeu.dy;
                if (IsKeyPressed(KEY_RIGHT)) { dx = 1; dy = 0; }
                else if (IsKeyPressed(KEY_LEFT)) { dx = -1; dy = 0; }
                else if (IsKeyPressed(KEY_UP)) { dx = 0; dy = -1; }
                else if (IsKeyPressed(KEY_DOWN)) { dx = 0; dy = 1; }
                Case prochaine = {jeu.corps[0].x + dx, jeu.corps[0].y + dy};
                if ((dx != jeu.dx || dy != jeu.dy) &&
                    (jeu.taille == 1 || !meme_case(prochaine, jeu.corps[1]))) {
                    jeu.dx = dx;
                    jeu.dy = dy;
                    direction_changee = true;
                }
            }
            if (jeu.dx != 0 || jeu.dy != 0) {
                temps += GetFrameTime();
                while (temps >= DELAI_DEPLACEMENT && !jeu.termine) {
                    temps -= DELAI_DEPLACEMENT;
                    avancer(&jeu);
                    direction_changee = false;
                }
            }
            if (jeu.termine) {
                if (enregistrer_score(&historique, jeu.score)) nombre_parties++;
                else erreur_memoire = true;
                if (jeu.score > meilleur) meilleur = jeu.score;
            }
        }

        BeginDrawing();
        ClearBackground((Color){20, 24, 32, 255});
        DrawText("SNAKE", 20, 18, 28, LIME);
        DrawText(TextFormat("Score : %d", jeu.score), 180, 23, 22, RAYWHITE);
        DrawText(TextFormat("Record : %d", meilleur), 380, 23, 22, GOLD);

        if (voir_historique) {
            DrawText("Historique de cette session", 50, 85, 26, RAYWHITE);
            DrawText("Efface a la fermeture du jeu", 50, 122, 18, GRAY);
            int mouvement = (int)GetMouseWheelMove();
            if (IsKeyPressed(KEY_DOWN)) mouvement--;
            if (IsKeyPressed(KEY_UP)) mouvement++;
            decalage -= mouvement;
            int maximum = nombre_parties > 7 ? nombre_parties - 7 : 0;
            if (decalage < 0) decalage = 0;
            if (decalage > maximum) decalage = maximum;
            Score *courant = historique;
            for (int i = 0; i < decalage && courant; i++) courant = courant->suivant;
            for (int i = 0; i < 7 && courant; i++, courant = courant->suivant)
                DrawText(TextFormat("Partie %d : %d points",
                         nombre_parties - decalage - i, courant->valeur),
                         65, 170 + i * 32, 22, RAYWHITE);
            DrawText("Molette ou haut/bas pour defiler", 65, 405, 17, GRAY);
            if (bouton((Rectangle){170, 445, 260, 46}, "Retour"))
                voir_historique = false;
        } else {
            for (int y = 0; y < LIGNES; y++) {
                for (int x = 0; x < COLONNES; x++) {
                    int px = x * TAILLE_CASE, py = y * TAILLE_CASE + MARGE_HAUT;
                    if (x == 0 || x == COLONNES - 1 || y == 0 || y == LIGNES - 1)
                        DrawRectangle(px, py, TAILLE_CASE, TAILLE_CASE, DARKGRAY);
                    else
                        DrawRectangleLines(px, py, TAILLE_CASE, TAILLE_CASE,
                                           (Color){32, 38, 48, 255});
                }
            }
            if (!jeu.gagne)
                DrawRectangle(jeu.nourriture.x * TAILLE_CASE + 5,
                              jeu.nourriture.y * TAILLE_CASE + MARGE_HAUT + 5,
                              TAILLE_CASE - 10, TAILLE_CASE - 10, RED);
            for (int i = jeu.taille - 1; i >= 0; i--)
                DrawRectangle(jeu.corps[i].x * TAILLE_CASE + 2,
                              jeu.corps[i].y * TAILLE_CASE + MARGE_HAUT + 2,
                              TAILLE_CASE - 4, TAILLE_CASE - 4, i == 0 ? LIME : GREEN);

            if (jeu.termine) {
                DrawRectangle(60, 150, 480, 300, (Color){15, 20, 28, 245});
                DrawText(jeu.gagne ? "Bravo !" : "Partie terminee", 150, 175, 30, LIME);
                DrawText(TextFormat("Score : %d points", jeu.score), 170, 225, 24, RAYWHITE);
                if (bouton((Rectangle){150, 280, 300, 46}, "Rejouer (R)")) rejouer = true;
                if (bouton((Rectangle){150, 340, 300, 46}, "Historique des scores")) {
                    voir_historique = true;
                    decalage = 0;
                }
                if (erreur_memoire)
                    DrawText("Memoire insuffisante pour ce score", 95, 410, 18, RED);
                if (IsKeyPressed(KEY_R)) rejouer = true;
            } else if (jeu.dx == 0 && jeu.dy == 0)
                DrawText("Appuie sur une fleche pour commencer", 75, 455, 20, RAYWHITE);
        }
        EndDrawing();
        if (rejouer) {
            nouvelle_partie(&jeu);
            temps = 0;
            direction_changee = false;
        }
    }
    liberer_historique(&historique);
    CloseWindow();
    return 0;
}

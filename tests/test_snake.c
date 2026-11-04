#define main snake_main
#include "../main.c"
#undef main
#include <assert.h>
#include <stdio.h>

int main(void)
{
    Jeu jeu;
    for (int n = 0; n < 100; n++) {
        nouvelle_partie(&jeu);
        assert(jeu.taille == 1 && jeu.score == 0 && !jeu.termine);
        assert(jeu.nourriture.x > 0 && jeu.nourriture.x < COLONNES - 1);
        assert(jeu.nourriture.y > 0 && jeu.nourriture.y < LIGNES - 1);
        for (int i = 0; i < jeu.taille; i++)
            assert(!meme_case(jeu.corps[i], jeu.nourriture));
    }
    jeu.dx = 1;
    jeu.nourriture = (Case){jeu.corps[0].x + 1, jeu.corps[0].y};
    avancer(&jeu);
    assert(jeu.score == 10 && jeu.taille == 2 && !jeu.termine);
    for (int i = 0; i < jeu.taille; i++)
        assert(!meme_case(jeu.corps[i], jeu.nourriture));

    jeu.corps[0] = (Case){COLONNES - 2, 5};
    avancer(&jeu);
    assert(jeu.termine && jeu.score == 10);
    avancer(&jeu);
    assert(jeu.score == 10);

    nouvelle_partie(&jeu);
    jeu.taille = 2;
    jeu.corps[1] = (Case){jeu.corps[0].x - 1, jeu.corps[0].y};
    jeu.nourriture = jeu.corps[1];
    jeu.dx = -1;
    avancer(&jeu);
    assert(jeu.termine);

    jeu = (Jeu){.taille = 4, .dx = -1};
    jeu.corps[0] = (Case){3, 3};
    jeu.corps[1] = (Case){3, 4};
    jeu.corps[2] = (Case){2, 4};
    jeu.corps[3] = (Case){2, 3};
    jeu.nourriture = (Case){8, 8};
    avancer(&jeu);
    assert(!jeu.termine && jeu.corps[0].x == 2);

    jeu = (Jeu){.taille = MAX_CASES - 1, .dy = -1};
    jeu.corps[0] = (Case){18, 13};
    jeu.nourriture = (Case){18, 12};
    int indice = 1;
    for (int y = 1; y < LIGNES - 1; y++) {
        for (int x = 1; x < COLONNES - 1; x++) {
            Case position = {x, y};
            if (!meme_case(position, jeu.corps[0]) &&
                !meme_case(position, jeu.nourriture)) jeu.corps[indice++] = position;
        }
    }
    avancer(&jeu);
    assert(jeu.termine && jeu.gagne && jeu.taille == MAX_CASES);

    Score *historique = NULL;
    assert(enregistrer_score(&historique, 10));
    assert(enregistrer_score(&historique, 0));
    nouvelle_partie(&jeu);
    assert(jeu.score == 0 && historique->valeur == 0);
    assert(historique->suivant->valeur == 10);
    liberer_historique(&historique);
    assert(historique == NULL);
    puts("OK : nourriture, croissance, score, collisions, victoire, historique et nettoyage.");
    return 0;
}


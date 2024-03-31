#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct
{
    int *etatsdepa;
    int *etatssuiv;
    char *etiquettes;
    char *ettiquettesUniques;
    int *etatsInitial;
    int *etatsFinal;
    int *etatsAccessibles;
    int *etatsAccessibles1;
    int nbrEtatsAccessible1;
    int nbrEtatsAccessible;
    int nbrEtiquettesUniques;
    int nbrtransitions;
    int nbrEtatsInit;
    int nbrEtatsFin;
} automate;



//-------------fonction1
// Lecture et stockage des informations de l'automate à partir d'un fichier
void stockerautomate(char nomfichier[], automate *a)
{ // Ouverture du fichier en mode lecture
    FILE *pointeurfich;
    pointeurfich = fopen(nomfichier, "r");
    if (pointeurfich == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[100];
    int i, j;
    char alpha;
    int tempTransitions = 0;
    // compter le nombre de transitions
    while (fgets(ligne, 100, pointeurfich) != NULL)
    {
        tempTransitions++;
    }
    tempTransitions -= 2;
    // Allocation dynamique de la mémoire pour les transitions
    a->etatsdepa = (int *)malloc(tempTransitions * sizeof(int));
    if (a->etatsdepa == NULL)
    {
        printf("Alloue echouee pour etatsdepa\n");
        exit(EXIT_FAILURE);
    }
    a->etatssuiv = (int *)malloc(tempTransitions * sizeof(int));
    if (a->etatssuiv == NULL)
    {
        printf("Alloue echouee pour etatssuiv\n");
        free(a->etatsdepa);
        exit(EXIT_FAILURE);
    }
    a->etiquettes = (char *)malloc(tempTransitions * sizeof(char));
    if (a->etiquettes == NULL)
    {
        printf("Alloue echouee pour etiquettes\n");
        free(a->etatsdepa);
        free(a->etatssuiv);
        exit(EXIT_FAILURE);
    }
    // Réinitialisation du pointeur de fichier pour la deuxième lecture
    rewind(pointeurfich);
    // Lecture et stockage des transitions et etats initiaux
    a->nbrtransitions = 0;
    while (fgets(ligne, 100, pointeurfich) != NULL)
    {
        if (sscanf(ligne, "%d %d %c", &i, &j, &alpha) == 3)
        {
            if (!isalpha(alpha))
            { // Ce bloc est exécuté si la ligne actuelle ne correspond pas à une transition
              // Il est utilisé pour traiter les cas où il y a plus qu'un ou deux états initiaux

                int i = 0;
                int initial;
                int offset;
                char *ptr = ligne;

                while (sscanf(ptr, "%d%n", &initial, &offset) == 1)
                {
                    a->etatsInitial = (int *)(realloc(a->etatsInitial, (i + 1) * sizeof(int)));
                    a->etatsInitial[i] = initial;
                    i++;
                    ptr += offset;
                }
                a->nbrEtatsInit = i;
                break;
            }
            a->etatsdepa[a->nbrtransitions] = i;
            a->etatssuiv[a->nbrtransitions] = j;
            a->etiquettes[a->nbrtransitions] = alpha;
            a->nbrtransitions++;
        }
        else
        { // Ce bloc est exécuté si la ligne actuelle ne correspond pas à une transition
            // Il est utilisé pour traiter les cas où il y a seulement un ou deux états initiaux
            int i = 0;
            int initial;
            int offset;
            char *ptr = ligne;
            while (sscanf(ptr, "%d%n", &initial, &offset) == 1)
            {
                a->etatsInitial = (int *)(realloc(a->etatsInitial, (i + 1) * sizeof(int)));
                a->etatsInitial[i] = initial;
                i++;
                ptr += offset;
            }
            a->nbrEtatsInit = i;
            break;
        }
    }
    if (a->etatsInitial == NULL && i > 0)
    {
        printf("alloue echouee pour etatsInitial\n");
        free(a->etatsdepa);
        free(a->etatssuiv);
        free(a->etiquettes);
        exit(EXIT_FAILURE);
    }
    // Lecture et stockage des états finaux
    if (fgets(ligne, 100, pointeurfich) != NULL)
    {
        int i = 0;
        int final;
        int offset;
        char *ptr = ligne;
        while (sscanf(ptr, "%d%n", &final, &offset) == 1)
        {
            a->etatsFinal = (int *)(realloc(a->etatsFinal, (i + 1) * sizeof(int)));
            a->etatsFinal[i] = final;
            i++;
            ptr += offset;
        }
        a->nbrEtatsFin = i;
    }
    if (a->etatsFinal == NULL && i > 0)
    {
        printf("allou echouee pour etatsFinal\n");
        free(a->etatsInitial);
        free(a->etatsdepa);
        free(a->etatssuiv);
        free(a->etiquettes);
        exit(EXIT_FAILURE);
    }
    // Fermeture du fichier
    fclose(pointeurfich);
}
//-------------fonction2
// Affichage des transitions de l'automate et leurs nombre total
void afficherTransitions(automate *a)
{
    printf("\n");
    for (int i = 0; i < a->nbrtransitions; i++)
    {
        printf("(%d --> %d) %c ", a->etatsdepa[i], a->etatssuiv[i], a->etiquettes[i]);
        printf("\n");
    }
    printf("\n");
    printf("nombre de transitions : %d\n", a->nbrtransitions);
    printf("\n");
}
//-------------fonction3
// Stockage de l'alphabet unique à partir des étiquettes des transitions
void stockerAlphabet(automate *a)
{
    // Initialisation du compteur d'étiquettes uniques
    a->nbrEtiquettesUniques = a->nbrtransitions;
    // Élimination des doublons pour trouver le nombre réel d'étiquettes uniques
    for (char *P1 = a->etiquettes; P1 < a->etiquettes + a->nbrtransitions; P1++)
    {
        for (char *P2 = a->etiquettes; P2 < P1; P2++)
        {
            if (*P1 == *P2)
            {
                a->nbrEtiquettesUniques--;
                break;
            }
        }
    }
    // Allocation dynamique pour stocker l'alphabet unique
    a->ettiquettesUniques = (char *)(malloc(a->nbrEtiquettesUniques * sizeof(char)));
    // Parcours des étiquettes pour remplir l'alphabet unique, en évitant les doublons
    int i = 0;
    for (char *P1 = a->etiquettes; P1 < a->etiquettes + a->nbrtransitions; P1++)
    {
        if (!strchr(a->ettiquettesUniques, *P1))
        {
            a->ettiquettesUniques[i] = *P1;
            i++;
        }
    }
}
//-------------fonction4
// Affichage de l'alphabet de l'automate
void afficherAlphabet(automate *a)
{
    printf("\nl'alphabet :\t");
    for (char *P1 = a->ettiquettesUniques; P1 < a->ettiquettesUniques + a->nbrEtiquettesUniques; P1++)
    {
        printf("%c\t", *P1);
    }
    printf("\n");
    printf("\nle nombre d'alphabet unique est : %d\n", a->nbrEtiquettesUniques);
    printf("\n");
}
//-------------fonction5
// Affichage des états initiaux de l'automate
void afficherEtatsinitiaux(automate *a)
{
    printf("\nEtats initiaux :\t");
    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        printf("%d\t", a->etatsInitial[i]);
    }
    printf("\n");
}
//-------------fonction6
// Affichage des états finaux de l'automate
void afficherEtatsfinaux(automate *a)
{
    printf("\nEtats Finaux :\t\t");
    for (int i = 0; i < a->nbrEtatsFin; i++)
    {
        printf("%d\t", a->etatsFinal[i]);
    }
    printf("\n");
}
//-------------fonction7
bool estDansTableau(int etat, int *tableau, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (tableau[i] == etat)
        {
            return true;
        }
    }
    return false;
}
//-------------fonction8
void EtatsAccessibles(int etatActuel, automate *a)
{
    if (!estDansTableau(etatActuel, a->etatsAccessibles, a->nbrEtatsAccessible))
    {
        a->etatsAccessibles[a->nbrEtatsAccessible++] = etatActuel;

        for (int i = 0; i < a->nbrtransitions; i++)
        {
            if (a->etatsdepa[i] == etatActuel && !estDansTableau(a->etatsdepa[i], a->etatsFinal, a->nbrEtatsFin))
            {
                EtatsAccessibles(a->etatssuiv[i], a);
            }
        }
    }
}
//-------------fonction9
void EtatsAccessibles0(automate *a)
{
    a->nbrEtatsAccessible = 0;
    a->etatsAccessibles = (int *)malloc(a->nbrtransitions * 2 * sizeof(int));
    if (a->etatsAccessibles == NULL)
    {
        printf("Allocation échouée pour etatsAccessibles\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        EtatsAccessibles(a->etatsInitial[i], a);
    }
}
//-------------fonction10
void afficherEtatsAccessibles(automate *a)
{
    printf("États accessibles :\n");
    for (int i = 0; i < a->nbrEtatsAccessible; i++)
    {
        printf("%d\t", a->etatsAccessibles[i]);
    }
    printf("\n");
}
//-------------fonction11
void genererFichierDOT(automate *a)
{
    FILE *ptrdot = fopen("Automate.dot", "w");
    if (ptrdot == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    fprintf(ptrdot, "digraph automate {\n");
    fprintf(ptrdot, "\trankdir=LR;\n");
    fprintf(ptrdot, "\tnode [shape = circle, color = gray];\n");

    for (int i = 0; i < a->nbrtransitions; i++)
    {
        fprintf(ptrdot, "\t%d -> %d [label=%c];\n", a->etatsdepa[i], a->etatssuiv[i], a->etiquettes[i]);
    }

    for (int i = 0; i < a->nbrEtatsAccessible; i++)
    {
        fprintf(ptrdot, "\t%d [color=black];\n", a->etatsAccessibles[i]);
    }

    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        fprintf(ptrdot, "\t%d [color=green];\n", a->etatsInitial[i]);
    }

    for (int i = 0; i < a->nbrEtatsFin; i++)
    {
        fprintf(ptrdot, "\t%d [color=blue];\n", a->etatsFinal[i]);
    }

    fprintf(ptrdot, "}\n");

    fclose(ptrdot);
    system("dot -Tpng Automate.dot -o Automate.png");
    system("Automate.png");
}
//-------------fonction12
int testerMot(char mot[], automate *a)
{
    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        int etatActuel = a->etatsInitial[i];
        // on suppose que le mot est engendree
        int engendre = 1;
        for (int j = 0; mot[j] != '\0'; j++)
        {
            char charActuel = mot[j];
            int transtrouve = 0;
            for (int k = 0; k < a->nbrtransitions; k++)
            {
                if (etatActuel == a->etatsdepa[k] && a->etiquettes[k] == charActuel)
                {
                    etatActuel = a->etatssuiv[k];
                    transtrouve = 1;
                    break;
                }
            }

            if (!transtrouve)
            {
                engendre = 0;
                break;
            }
        }
        if (engendre == 1 && estDansTableau(etatActuel, a->etatsFinal, a->nbrEtatsFin))
        {
            return 1;
        }
    }
    return 0;
}
//-------------fonction13
void lireMots(automate *a)
{
    char Mots[20];
    printf("entrer le nom du fichier Mots : ");
    scanf("%s", Mots);
    FILE *ptr = fopen(Mots, "r");
    char mot[100];
    if (ptr == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier Mots.txt\n");
        exit(EXIT_FAILURE);
    }
    printf("\nListe des mots engendre par l'Automate\n");
    while (fgets(mot, sizeof(mot), ptr) != NULL)
    {
        mot[strcspn(mot, "\n")] = 0;

        if (testerMot(mot, a))
        {

            printf("%s \n", mot);
        }
    }
    fclose(ptr);
}
//-------------fonction14
void Union_Automates(automate *a, automate *b)
{
    FILE *p = fopen("Union.txt", "w");
    for (int i = 0; i < a->nbrtransitions; i++)
    {
        fprintf(p, "%d %d %c\n", a->etatsdepa[i], a->etatssuiv[i], a->etiquettes[i]);
    }
    for (int i = 0; i < b->nbrtransitions; i++)
    {
        fprintf(p, "%d %d %c\n", b->etatsdepa[i], b->etatssuiv[i], b->etiquettes[i]);
    }
    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        fprintf(p, "0 %d E\n", a->etatsInitial[i]);
    }
    for (int i = 0; i < a->nbrEtatsFin; i++)
    {
        fprintf(p, "%d 9 E\n", a->etatsFinal[i]);
    }
    for (int i = 0; i < b->nbrEtatsInit; i++)
    {
        fprintf(p, "0 %d E\n", b->etatsInitial[i]);
    }
    for (int i = 0; i < b->nbrEtatsFin; i++)
    {
        fprintf(p, "%d 9 E\n", b->etatsFinal[i]);
    }
    fprintf(p, "0\n");
    fprintf(p, "9");
    fclose(p);
}
//-------------fonction15
void EtatsAccessibles_depuis_etatinitial(int etatActuel, automate *a)
{

    if (!estDansTableau(etatActuel, a->etatsAccessibles1, a->nbrEtatsAccessible1))
    {
        a->etatsAccessibles1[a->nbrEtatsAccessible1++] = etatActuel;

        for (int i = 0; i < a->nbrtransitions; i++)
        {
            if (a->etatsdepa[i] == etatActuel && !estDansTableau(a->etatsdepa[i], a->etatsFinal, a->nbrEtatsFin))
            {
                EtatsAccessibles_depuis_etatinitial(a->etatssuiv[i], a);
            }
        }
    }
}
//-------------fonction16
void Etoile_Automates(automate *a)
{
    FILE *p = fopen("Etoile.txt", "w");
    for (int i = 0; i < a->nbrtransitions; i++)
    {
        fprintf(p, "%d %d %c\n", a->etatsdepa[i], a->etatssuiv[i], a->etiquettes[i]);
    }
    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        fprintf(p, "0 %d E\n", a->etatsInitial[i]);
    }
    for (int i = 0; i < a->nbrEtatsFin; i++)
    {
        fprintf(p, "%d 9 E\n", a->etatsFinal[i]);
    }
    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        a->nbrEtatsAccessible1 = 0;
        a->etatsAccessibles1 = (int *)malloc(a->nbrtransitions * 2 * sizeof(int));
        if (a->etatsAccessibles1 == NULL)
        {
            printf("Allocation échouée pour etatsAccessibles\n");
            exit(EXIT_FAILURE);
        }
        EtatsAccessibles_depuis_etatinitial(a->etatsInitial[i], a);
        for (int j = 0; j < a->nbrEtatsFin; j++)
        {
            if (estDansTableau(a->etatsFinal[j], a->etatsAccessibles1, a->nbrEtatsAccessible1))
            {
                fprintf(p, "%d %d E\n", a->etatsFinal[j], a->etatsInitial[i]);
            }
        }

        free(a->etatsAccessibles1);
        a->etatsAccessibles1 = NULL;
    }

    fprintf(p, "0 9 E\n");
    fprintf(p, "0\n");
    fprintf(p, "9");
    fclose(p);
}
//-------------fonction17
typedef struct
{
    int etat1[25];
    int etat2[25];
    int etatsdep[25];
    int etatsuiv[25];
    int etatinit[25];
    int etatfin[25];
} etatspaire;
void Produit_Automates(automate *a, automate *b, etatspaire et)
{
    FILE *p = fopen("Produit.txt", "w");
    if (p == NULL)
    {
        printf("Erreur lors de la création du fichier Produit.txt\n");
        exit(EXIT_FAILURE);
    }

    int k = 0;
    int L = 0;
    int Q = 0;
    int M = 0;
    int N = 0;

    for (int i = 0; i < a->nbrtransitions; i++)
    {
        for (int j = 0; j < b->nbrtransitions; j++)
        {
            // Si les transitions partent du même symbole
            if (a->etiquettes[i] == b->etiquettes[j])
            {

                if (estDansTableau(a->etatsdepa[i], a->etatsInitial, a->nbrEtatsInit) && estDansTableau(b->etatsdepa[j], b->etatsInitial, b->nbrEtatsInit))
                {
                    et.etat1[k] = a->etatsdepa[i];
                    et.etat2[k] = b->etatsdepa[j];
                    et.etatinit[L] = k;
                    k++;
                    L++;
                }
                else
                {
                    et.etat1[k] = a->etatsdepa[i];
                    et.etat2[k] = b->etatsdepa[j];
                    et.etatsdep[M] = k;
                    k++;
                    M++;
                }

                if (estDansTableau(a->etatssuiv[i], a->etatsFinal, a->nbrEtatsFin) && estDansTableau(b->etatssuiv[j], b->etatsFinal, b->nbrEtatsFin))
                {
                    et.etat1[k] = a->etatssuiv[i];
                    et.etat2[k] = b->etatssuiv[j];
                    et.etatfin[Q] = k;
                    Q++;
                }
                else
                {
                    et.etat1[k] = a->etatssuiv[i];
                    et.etat2[k] = b->etatssuiv[j];
                    et.etatsuiv[N] = k;
                    N++;
                }
                printf("\n\n---  (%d, %d)->(%d, %d) %c  ---\n", a->etatsdepa[i], b->etatsdepa[j], a->etatssuiv[i], b->etatssuiv[j], a->etiquettes[i]);
                fprintf(p, "%d %d %c\n", k - 1, k, a->etiquettes[i]);
                k++;
            }
        }
    }
    

    fclose(p);
    FILE *pr = fopen("Produit.dot", "w");
    if (pr == NULL)
    {
        printf("Erreur lors de la création du fichier Produit.dot\n");
        exit(EXIT_FAILURE);
    }

    fprintf(pr, "digraph produit {\n");
    fprintf(pr, "\trankdir=LR;\n");
    fprintf(pr, "\tnode [shape = circle];\n");

    for (int i = 0; i < a->nbrtransitions; i++)
    {
        for (int j = 0; j < b->nbrtransitions; j++)
        {
            // Si les transitions partent du même symbole
            if (a->etiquettes[i] == b->etiquettes[j])
            {
                fprintf(p, "\t\"%d,%d\" -> \"%d,%d\" [label=\"%c\"];\n", a->etatsdepa[i], b->etatsdepa[j], a->etatssuiv[i], b->etatssuiv[j], a->etiquettes[i]);

                // Vérification des états initiaux dans la paire
                if (estDansTableau(a->etatsdepa[i], a->etatsInitial, a->nbrEtatsInit) && estDansTableau(b->etatsdepa[j], b->etatsInitial, b->nbrEtatsInit))
                {
                    fprintf(p, "\t\"%d,%d\" [color=green];\n", a->etatsdepa[i], b->etatsdepa[j]);
                }

                // Vérification des états finaux dans la paire
                if (estDansTableau(a->etatssuiv[i], a->etatsFinal, a->nbrEtatsFin) && estDansTableau(b->etatssuiv[j], b->etatsFinal, b->nbrEtatsFin))
                {
                    fprintf(p, "\t\"%d,%d\" [color=blue];\n", a->etatssuiv[i], b->etatssuiv[j]);
                }
            }
        }
    }

    fprintf(pr, "}\n");
    fclose(pr);
}

void EtatsAccessibles_utilisant_epsilons(int etatActuel, automate *a)
{

    if (!estDansTableau(etatActuel, a->etatsAccessibles1, a->nbrEtatsAccessible1))
    {
        a->etatsAccessibles1[a->nbrEtatsAccessible1++] = etatActuel;

        for (int i = 0; i < a->nbrtransitions; i++)
        {
            if (a->etatsdepa[i] == etatActuel && a->etiquettes[i] == 'e')
            {
                EtatsAccessibles_utilisant_epsilons(a->etatssuiv[i], a);
            }
        }
    }
}
int si_elle_exist_deja(int debut, int fin, char symbole, int *depa, int *suiv, char *etiq, int compt)
{
    for (int k = 0; k < compt; k++)
    {
        if (depa[k] == debut && suiv[k] == fin && etiq[k] == symbole)
        {
            return 1;
        }
    }
    return 0;
}
void Suppression_epsilon(automate *a)
{
    FILE *p = fopen("Sans_epsilons.txt", "w");
    if (p == NULL)
    {
        perror("Error opening file");
        return;
    }

    int *tempDepa = (int *)malloc(a->nbrtransitions * sizeof(int) * 2);
    int *tempSuiv = (int *)malloc(a->nbrtransitions * sizeof(int) * 2);
    char *tempEtiquettes = (char *)malloc(a->nbrtransitions * sizeof(char) * 2);
    int tempCount = 0;

    for (int i = 0; i < a->nbrtransitions; i++)
    {
        if (a->etiquettes[i] != 'e')
        {
            if (!si_elle_exist_deja(a->etatsdepa[i], a->etatssuiv[i], a->etiquettes[i], tempDepa, tempSuiv, tempEtiquettes, tempCount))
            {
                fprintf(p, "%d %d %c\n", a->etatsdepa[i], a->etatssuiv[i], a->etiquettes[i]);
                tempDepa[tempCount] = a->etatsdepa[i];
                tempSuiv[tempCount] = a->etatssuiv[i];
                tempEtiquettes[tempCount] = a->etiquettes[i];
                tempCount++;
            }
        }

        else
        {
            a->nbrEtatsAccessible1 = 0;
            a->etatsAccessibles1 = (int *)malloc(a->nbrtransitions * 2 * sizeof(int));
            EtatsAccessibles_utilisant_epsilons(a->etatsdepa[i], a);
            for (int j = 0; j < a->nbrEtatsAccessible1; j++)
            {
                int accessibleState = a->etatsAccessibles1[j];
                for (int l = 0; l < a->nbrtransitions; l++)
                {
                    if (a->etatsdepa[l] == accessibleState && a->etiquettes[l] != 'e')
                    {
                        if (!si_elle_exist_deja(a->etatsdepa[i], a->etatssuiv[l], a->etiquettes[l], tempDepa, tempSuiv, tempEtiquettes, tempCount))
                        {
                            fprintf(p, "%d %d %c\n", a->etatsdepa[i], a->etatssuiv[l], a->etiquettes[l]);
                            tempDepa[tempCount] = a->etatsdepa[i];
                            tempSuiv[tempCount] = a->etatssuiv[l];
                            tempEtiquettes[tempCount] = a->etiquettes[l];
                            tempCount++;
                        }
                    }
                }
            }
            free(a->etatsAccessibles1);
            a->etatsAccessibles1 = NULL;
        }
    }

    for (int i = 0; i < a->nbrEtatsInit; i++)
    {
        fprintf(p, "%d\n", a->etatsInitial[i]);
    }
    for (int i = 0; i < a->nbrEtatsFin; i++)
    {
        fprintf(p, "%d ", a->etatsFinal[i]);
    }

    int *isFinalDueToEpsilon = calloc(a->nbrtransitions, sizeof(int));

    for (int i = 0; i < a->nbrtransitions; i++)
    {
        if (a->etiquettes[i] == 'e')
        {
            a->nbrEtatsAccessible1 = 0;
            a->etatsAccessibles1 = (int *)malloc(a->nbrtransitions * 2 * sizeof(int));
            if (!a->etatsAccessibles1)
            {
                perror("Memory allocation failed");
                return;
            }
            EtatsAccessibles_utilisant_epsilons(a->etatsdepa[i], a);

            for (int k = 0; k < a->nbrEtatsFin; k++)
            {
                if (estDansTableau(a->etatsFinal[k], a->etatsAccessibles1, a->nbrEtatsAccessible1))
                {
                    if (!estDansTableau(a->etatsdepa[i], a->etatsFinal, a->nbrEtatsFin) && !isFinalDueToEpsilon[a->etatsdepa[i]])
                    {
                        fprintf(p, "%d ", a->etatsdepa[i]);
                        isFinalDueToEpsilon[a->etatsdepa[i]] = 1;
                    }
                }
            }

            free(a->etatsAccessibles1);
            a->etatsAccessibles1 = NULL;
        }
    }
    fprintf(p, "\n");

    free(isFinalDueToEpsilon);
    fclose(p);
    free(tempDepa);
    free(tempSuiv);
    free(tempEtiquettes);
}

int main()
{
    char Automate[20];
    char motATester[100];
    etatspaire n;

    automate au = {0};
    au.etatsdepa = NULL;
    au.etatssuiv = NULL;
    au.etiquettes = NULL;
    au.ettiquettesUniques = NULL;
    au.etatsInitial = NULL;
    au.etatsFinal = NULL;
    au.etatsAccessibles = NULL;
    au.nbrEtatsAccessible = 0;
    au.nbrEtiquettesUniques = 0;
    au.nbrtransitions = 0;
    au.nbrEtatsInit = 0;
    au.nbrEtatsFin = 0;

    automate e = {0};
    e.etatsdepa = NULL;
    e.etatssuiv = NULL;
    e.etiquettes = NULL;
    e.ettiquettesUniques = NULL;
    e.etatsInitial = NULL;
    e.etatsFinal = NULL;
    e.etatsAccessibles = NULL;
    e.nbrEtatsAccessible = 0;
    e.nbrEtiquettesUniques = 0;
    e.nbrtransitions = 0;
    e.nbrEtatsInit = 0;
    e.nbrEtatsFin = 0;

    automate p = {0};
    p.etatsdepa = NULL;
    p.etatssuiv = NULL;
    p.etiquettes = NULL;
    p.ettiquettesUniques = NULL;
    p.etatsInitial = NULL;
    p.etatsFinal = NULL;
    p.etatsAccessibles = NULL;
    p.nbrEtatsAccessible = 0;
    p.nbrEtiquettesUniques = 0;
    p.nbrtransitions = 0;
    p.nbrEtatsInit = 0;
    p.nbrEtatsFin = 0;

    automate u = {0};
    u.etatsdepa = NULL;
    u.etatssuiv = NULL;
    u.etiquettes = NULL;
    u.ettiquettesUniques = NULL;
    u.etatsInitial = NULL;
    u.etatsFinal = NULL;
    u.etatsAccessibles = NULL;
    u.nbrEtatsAccessible = 0;
    u.nbrEtiquettesUniques = 0;
    u.nbrtransitions = 0;
    u.nbrEtatsInit = 0;
    u.nbrEtatsFin = 0;

    automate a = {0};
    a.etatsdepa = NULL;
    a.etatssuiv = NULL;
    a.etiquettes = NULL;
    a.ettiquettesUniques = NULL;
    a.etatsInitial = NULL;
    a.etatsFinal = NULL;
    a.etatsAccessibles = NULL;
    a.nbrEtatsAccessible = 0;
    a.nbrEtiquettesUniques = 0;
    a.nbrtransitions = 0;
    a.nbrEtatsInit = 0;
    a.nbrEtatsFin = 0;

    automate b = {0};
    b.etatsdepa = NULL;
    b.etatssuiv = NULL;
    b.etiquettes = NULL;
    b.ettiquettesUniques = NULL;
    b.etatsInitial = NULL;
    b.etatsFinal = NULL;
    b.etatsAccessibles = NULL;
    b.nbrEtatsAccessible = 0;
    b.nbrEtiquettesUniques = 0;
    b.nbrtransitions = 0;
    b.nbrEtatsInit = 0;
    b.nbrEtatsFin = 0;

    automate s = {0};
    s.etatsdepa = NULL;
    s.etatssuiv = NULL;
    s.etiquettes = NULL;
    s.ettiquettesUniques = NULL;
    s.etatsInitial = NULL;
    s.etatsFinal = NULL;
    s.etatsAccessibles = NULL;
    s.nbrEtatsAccessible = 0;
    s.nbrEtiquettesUniques = 0;
    s.nbrtransitions = 0;
    s.nbrEtatsInit = 0;
    s.nbrEtatsFin = 0;

    int choix;
    do
    {
        printf("\nMenu1:\n\n");
        printf("\n1. operer sur une seul automate\n");
        printf("\n2. manipuler automate\n");
        printf("\n3. quitter\n");
        printf("\nvotre choix : ");
        scanf("\n%d", &choix);
        switch (choix)
        {
        case 1:
        {
            int choix1;
            do
            {
                printf("\nMenu1-1:\n\n");
                printf("0. entrer le nom du fichier de l'Automate principal\n");
                printf("1. Afficher ces transitions\n");
                printf("2. Afficher les etats initiaux\n");
                printf("3. Afficher les etats finaux\n");
                printf("4. Afficher l'alphabet des etiquettes\n");
                printf("5. Afficher les etats accessibles\n");
                printf("6. Generer le fichier DOT et afficher le PNG\n");
                printf("7. Tester si un mot est engendré par l'automate\n");
                printf("8. Lire et afficher les mots engendrés par l'automate\n");
                printf("9. Quitter\n");
                printf("\nvotre choix : ");
                scanf("%d", &choix1);
                switch (choix1)
                {
                case 0:
                    printf("\nentrer nom fichier text :\t");
                    scanf("%s", Automate);
                    stockerautomate(Automate, &au);
                    stockerAlphabet(&au);
                    EtatsAccessibles0(&au);

                    break;
                case 1:
                    afficherTransitions(&au);
                    break;
                case 2:
                    afficherEtatsinitiaux(&au);
                    break;
                case 3:
                    afficherEtatsfinaux(&au);
                    break;
                case 4:
                    afficherAlphabet(&au);
                    break;
                case 5:
                    afficherEtatsAccessibles(&au);
                    break;
                case 6:
                    genererFichierDOT(&au);
                    break;
                case 7:
                    printf("Entrez le mot à tester : ");
                    scanf("%s", motATester);
                    if (testerMot(motATester, &au))
                    {
                        printf("\nLe mot est engendré par l'automate.\n");
                    }
                    else
                    {
                        printf("\nLe mot n'est pas engendré par l'automate.\n");
                    }
                    break;
                case 8:

                    lireMots(&au);

                    break;
                case 9:
                    break;

                default:
                    printf("Option invalide, veuillez réessayer.\n");
                }

            } while (choix1 != 9);
            break;
        }

        case 2:
        {
            int choix2;
            do
            {
                printf("\nMenu1-2:\n\n");
                printf("1. Union deux automates\n");
                printf("2. Produit deux Automates\n");
                printf("3. Etoile d'une automate\n");
                printf("4. Suprimer les epsilons d'une automate\n");
                printf("5. Quitter\n");
                printf("\nvotre choix : ");
                scanf("%d", &choix2);
                switch (choix2)
                {
                case 1:
                    printf("\nentrer le nom du fichier text de l'automate A :\t");
                    scanf("%s", Automate);
                    stockerautomate(Automate, &a);
                    stockerAlphabet(&a);
                    EtatsAccessibles0(&a);
                    printf("\nentrer le nom du fichier text de l'automate B :\t");
                    scanf("%s", Automate);
                    stockerautomate(Automate, &b);
                    stockerAlphabet(&b);
                    EtatsAccessibles0(&b);
                    Union_Automates(&a, &b);
                    stockerautomate("Union.txt", &u);
                    stockerAlphabet(&u);
                    EtatsAccessibles0(&u);
                    genererFichierDOT(&u);
                    break;
                case 2:

                    printf("\nentrer le nom du fichier text de l'automate A :\t");
                    scanf("%s", Automate);
                    stockerautomate(Automate, &a);
                    stockerAlphabet(&a);
                    EtatsAccessibles0(&a);
                    printf("\nentrer le nom du fichier text de l'automate B :\t");
                    scanf("%s", Automate);
                    stockerautomate(Automate, &b);
                    stockerAlphabet(&b);
                    EtatsAccessibles0(&b);
                    Produit_Automates(&a, &b, n);
                    

                    break;
                case 3:
                    printf("\nentrer le nom du fichier text de l'automate A :\t");
                    scanf("\n%s", Automate);
                    stockerautomate(Automate, &a);
                    stockerAlphabet(&a);
                    EtatsAccessibles0(&a);
                    Etoile_Automates(&a);
                    stockerautomate("Etoile.txt", &e);
                    genererFichierDOT(&e);
                    break;
                case 4:
                    printf("\nentrer le nom du fichier text de l'automate a epsilon transition :\t");
                    scanf("\n%s", Automate);
                    stockerautomate(Automate, &a);
                    stockerAlphabet(&a);
                    EtatsAccessibles0(&a);
                    Suppression_epsilon(&a);
                    stockerautomate("Sans_epsilons.txt", &s);
                    genererFichierDOT(&s);

                    break;
                case 5:
                    break;
                default:
                    printf("Option invalide, veuillez réessayer.\n");
                }

            } while (choix2 != 5);
            break;
        }

        case 3:
            break;
        default:
            printf("Option invalide, veuillez réessayer.\n");
        }

    } while (choix != 3);

    free(au.etatsdepa);
    au.etatsdepa = NULL;

    free(au.etatssuiv);
    au.etatssuiv = NULL;

    free(au.etiquettes);
    au.etiquettes = NULL;

    free(au.ettiquettesUniques);
    au.ettiquettesUniques = NULL;

    free(au.etatsInitial);
    au.etatsInitial = NULL;

    free(au.etatsFinal);
    au.etatsFinal = NULL;

    free(au.etatsAccessibles);
    au.etatsAccessibles = NULL;

    free(au.etatsAccessibles1);
    au.etatsAccessibles1 = NULL;

    free(u.etatsdepa);
    u.etatsdepa = NULL;

    free(u.etatssuiv);
    u.etatssuiv = NULL;

    free(u.etiquettes);
    u.etiquettes = NULL;

    free(u.ettiquettesUniques);
    u.ettiquettesUniques = NULL;

    free(u.etatsInitial);
    u.etatsInitial = NULL;

    free(u.etatsFinal);
    u.etatsFinal = NULL;

    free(u.etatsAccessibles);
    u.etatsAccessibles = NULL;

    free(a.etatsdepa);
    a.etatsdepa = NULL;

    free(a.etatssuiv);
    a.etatssuiv = NULL;

    free(a.etiquettes);
    a.etiquettes = NULL;

    free(a.ettiquettesUniques);
    a.ettiquettesUniques = NULL;

    free(a.etatsInitial);
    a.etatsInitial = NULL;

    free(a.etatsFinal);
    a.etatsFinal = NULL;

    free(a.etatsAccessibles);
    a.etatsAccessibles = NULL;

    free(a.etatsAccessibles1);
    a.etatsAccessibles1 = NULL;

    free(b.etatsdepa);
    b.etatsdepa = NULL;

    free(b.etatssuiv);
    b.etatssuiv = NULL;

    free(b.etiquettes);
    b.etiquettes = NULL;

    free(b.ettiquettesUniques);
    b.ettiquettesUniques = NULL;

    free(b.etatsInitial);
    b.etatsInitial = NULL;

    free(b.etatsFinal);
    b.etatsFinal = NULL;

    free(b.etatsAccessibles);
    b.etatsAccessibles = NULL;

    free(b.etatsAccessibles1);
    b.etatsAccessibles1 = NULL;

    free(e.etatsdepa);
    e.etatsdepa = NULL;

    free(e.etatssuiv);
    e.etatssuiv = NULL;

    free(e.etiquettes);
    e.etiquettes = NULL;

    free(e.ettiquettesUniques);
    e.ettiquettesUniques = NULL;

    free(e.etatsInitial);
    e.etatsInitial = NULL;

    free(e.etatsFinal);
    e.etatsFinal = NULL;

    free(e.etatsAccessibles);
    e.etatsAccessibles = NULL;

    free(e.etatsAccessibles1);
    e.etatsAccessibles1 = NULL;

    free(p.etatsdepa);
    p.etatsdepa = NULL;

    free(p.etatssuiv);
    p.etatssuiv = NULL;

    free(p.etiquettes);
    p.etiquettes = NULL;

    free(p.ettiquettesUniques);
    p.ettiquettesUniques = NULL;

    free(p.etatsInitial);
    p.etatsInitial = NULL;

    free(p.etatsFinal);
    p.etatsFinal = NULL;

    free(p.etatsAccessibles);
    p.etatsAccessibles = NULL;

    free(p.etatsAccessibles1);
    p.etatsAccessibles1 = NULL;

    free(s.etatsdepa);
    s.etatsdepa = NULL;

    free(s.etatssuiv);
    s.etatssuiv = NULL;

    free(s.etiquettes);
    s.etiquettes = NULL;

    free(s.ettiquettesUniques);
    s.ettiquettesUniques = NULL;

    free(s.etatsInitial);
    s.etatsInitial = NULL;

    free(s.etatsFinal);
    s.etatsFinal = NULL;

    free(s.etatsAccessibles);
    s.etatsAccessibles = NULL;

    free(s.etatsAccessibles1);
    s.etatsAccessibles1 = NULL;

    return 0;
}

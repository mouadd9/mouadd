#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int *etatsdepa;
int *etatssuiv;
char *etiquettes;
char *ettiquettesUniques;
int *etatsInitial;
int *etatsFinal;
int *etatsAccessibles;
char listdesmots[100];
int nbrEtatsAccessible;
int nbrEtiquettesUniques;
int nbrtransitions;
int nbrEtatsInit;
int nbrEtatsFin;
//-------------fonction1
// Lecture et stockage des informations de l'automate à partir d'un fichier
void stockerautomate(char nomfichier[])
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
    etatsdepa = (int *)malloc(tempTransitions * sizeof(int));
    if (etatsdepa == NULL)
    {
        printf("Alloue echouee pour etatsdepa\n");
        exit(EXIT_FAILURE);
    }
    etatssuiv = (int *)malloc(tempTransitions * sizeof(int));
    if (etatssuiv == NULL)
    {
        printf("Alloue echouee pour etatssuiv\n");
        free(etatsdepa);
        exit(EXIT_FAILURE);
    }
    etiquettes = (char *)malloc(tempTransitions * sizeof(char));
    if (etiquettes == NULL)
    {
        printf("Alloue echouee pour etiquettes\n");
        free(etatsdepa);
        free(etatssuiv);
        exit(EXIT_FAILURE);
    }
    // Réinitialisation du pointeur de fichier pour la deuxième lecture
    rewind(pointeurfich);
    // Lecture et stockage des transitions et etats initiaux
    nbrtransitions = 0;
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
                    etatsInitial = (int *)(realloc(etatsInitial, (i + 1) * sizeof(int)));
                    etatsInitial[i] = initial;
                    i++;
                    ptr += offset;
                }
                nbrEtatsInit = i;
                break;
            }
            etatsdepa[nbrtransitions] = i;
            etatssuiv[nbrtransitions] = j;
            etiquettes[nbrtransitions] = alpha;
            nbrtransitions++;
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
                etatsInitial = (int *)(realloc(etatsInitial, (i + 1) * sizeof(int)));
                etatsInitial[i] = initial;
                i++;
                ptr += offset;
            }
            nbrEtatsInit = i;
            break;
        }
    }
    if (etatsInitial == NULL && i > 0)
    {
        printf("alloue echouee pour etatsInitial\n");
        free(etatsdepa);
        free(etatssuiv);
        free(etiquettes);
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
            etatsFinal = (int *)(realloc(etatsFinal, (i + 1) * sizeof(int)));
            etatsFinal[i] = final;
            i++;
            ptr += offset;
        }
        nbrEtatsFin = i;
    }
    if (etatsFinal == NULL && i > 0)
    {
        printf("allou echouee pour etatsFinal\n");
        free(etatsInitial);
        free(etatsdepa);
        free(etatssuiv);
        free(etiquettes);
        exit(EXIT_FAILURE);
    }
    // Fermeture du fichier
    fclose(pointeurfich);
}
//-------------fonction2
// Affichage des transitions de l'automate et leurs nombre total
void afficherTransitions()
{
    printf("\n");
    for (int i = 0; i < nbrtransitions; i++)
    {
        printf("(%d --> %d) %c ", etatsdepa[i], etatssuiv[i], etiquettes[i]);
        printf("\n");
    }
    printf("\n");
    printf("nombre de transitions : %d\n", nbrtransitions);
    printf("\n");
}
//-------------fonction3
// Stockage de l'alphabet unique à partir des étiquettes des transitions
void stockerAlphabet()
{
    // Initialisation du compteur d'étiquettes uniques
    nbrEtiquettesUniques = nbrtransitions;
    // Élimination des doublons pour trouver le nombre réel d'étiquettes uniques
    for (char *P1 = etiquettes; P1 < etiquettes + nbrtransitions; P1++)
    {
        for (char *P2 = etiquettes; P2 < P1; P2++)
        {
            if (*P1 == *P2)
            {
                nbrEtiquettesUniques--;
                break;
            }
        }
    }
    // Allocation dynamique pour stocker l'alphabet unique
    ettiquettesUniques = (char *)(malloc(nbrEtiquettesUniques * sizeof(char)));
    // Parcours des étiquettes pour remplir l'alphabet unique, en évitant les doublons
    int i = 0;
    for (char *P1 = etiquettes; P1 < etiquettes + nbrtransitions; P1++)
    {
        if (!strchr(ettiquettesUniques, *P1))
        {
            ettiquettesUniques[i] = *P1;
            i++;
        }
    }
}
//-------------fonction4
// Affichage de l'alphabet de l'automate
void afficherAlphabet()
{
    printf("\nl'alphabet :\t");
    for (char *P1 = ettiquettesUniques; P1 < ettiquettesUniques + nbrEtiquettesUniques; P1++)
    {
        printf("%c\t", *P1);
    }
    printf("\n");
    printf("\nle nombre d'alphabet unique est : %d\n", nbrEtiquettesUniques);
    printf("\n");
}
//-------------fonction5
// Affichage des états initiaux de l'automate
void afficherEtatsinitiaux()
{
    printf("\nEtats initiaux :\t");
    for (int i = 0; i < nbrEtatsInit; i++)
    {
        printf("%d\t", etatsInitial[i]);
    }
    printf("\n");
}
//-------------fonction6
// Affichage des états finaux de l'automate
void afficherEtatsfinaux()
{
    printf("\nEtats Finaux :\t\t");
    for (int i = 0; i < nbrEtatsFin; i++)
    {
        printf("%d\t", etatsFinal[i]);
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
void EtatsAccessibles(int etatActuel)
{
    if (!estDansTableau(etatActuel, etatsAccessibles, nbrEtatsAccessible))
    {
        etatsAccessibles[nbrEtatsAccessible++] = etatActuel;

        for (int i = 0; i < nbrtransitions; i++)
        {
            if (etatsdepa[i] == etatActuel && !estDansTableau(etatsdepa[i], etatsFinal, nbrEtatsFin))
            {
                EtatsAccessibles(etatssuiv[i]);
            }
        }
    }
}
//-------------fonction9
void EtatsAccessibles0()
{
    nbrEtatsAccessible = 0;
    etatsAccessibles = (int *)malloc(nbrtransitions * 2 * sizeof(int));
    if (etatsAccessibles == NULL)
    {
        printf("Allocation échouée pour etatsAccessibles\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbrEtatsInit; i++)
    {
        EtatsAccessibles(etatsInitial[i]);
    }
}
//-------------fonction10
void afficherEtatsAccessibles()
{
    printf("États accessibles :\n");
    for (int i = 0; i < nbrEtatsAccessible; i++)
    {
        printf("%d\t", etatsAccessibles[i]);
    }
    printf("\n");
}
//-------------fonction11
void genererFichierDOT()
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

    for (int i = 0; i < nbrtransitions; i++)
    {
        fprintf(ptrdot, "\t%d -> %d [label=%c];\n", etatsdepa[i], etatssuiv[i], etiquettes[i]);
    }

    for (int i = 0; i < nbrEtatsAccessible; i++)
    {
        fprintf(ptrdot, "\t%d [color=black];\n", etatsAccessibles[i]);
    }

    for (int i = 0; i < nbrEtatsInit; i++)
    {
        fprintf(ptrdot, "\t%d [color=green];\n", etatsInitial[i]);
    }

    for (int i = 0; i < nbrEtatsFin; i++)
    {
        fprintf(ptrdot, "\t%d [color=blue];\n", etatsFinal[i]);
    }

    fprintf(ptrdot, "}\n");

    fclose(ptrdot);
    system("dot -Tpng Automate.dot -o Automate.png");
    system("Automate.png");
}
//-------------fonction12
int testerMot(char mot[])
{
    for (int i = 0; i < nbrEtatsInit; i++)
    {
        int etatActuel = etatsInitial[i];
        // on suppose que le mot est engendree
        int engendre = 1;
        for (int j = 0; mot[j] != '\0'; j++)
        {
            char charActuel = mot[j];
            int transtrouve = 0;
            for (int k = 0; k < nbrtransitions; k++)
            {
                if (etatActuel == etatsdepa[k] && etiquettes[k] == charActuel)
                {
                    etatActuel = etatssuiv[k];
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
        if (engendre == 1 && estDansTableau(etatActuel, etatsFinal, nbrEtatsFin))
        {
            return 1;
        }
    }
    return 0;
}
//-------------fonction13
void lireMots()
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

        if (testerMot(mot))
        {

            printf("%s \n", mot);
        }
    }
    fclose(ptr);
}

int main()
{
    char Automate[20];
    char motATester[100];

    printf("entrer le non du fichier de l'Automate : ");
    scanf("%s", Automate);

    stockerautomate(Automate);
    stockerAlphabet();
    EtatsAccessibles0();

    int choix;
    do
    {
        printf("\nMenu:\n");
        printf("1. Afficher les transitions\n");
        printf("2. Afficher les etats initiaux\n");
        printf("3. Afficher les etats finaux\n");
        printf("4. Afficher l'alphabet des etiquettes\n");
        printf("5. Afficher les etats accessibles\n");
        printf("6. Generer le fichier DOT et afficher le PNG\n");
        printf("7. Tester si un mot est engendré par l'automate\n");
        printf("8. Lire et afficher les mots engendrés par l'automate\n");
        printf("9. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            afficherTransitions();
            break;
        case 2:
            afficherEtatsinitiaux();
            break;
        case 3:
            afficherEtatsfinaux();
            break;
        case 4:
            afficherAlphabet();
            break;
        case 5:
            afficherEtatsAccessibles();
            break;
        case 6:
            genererFichierDOT();
            break;
        case 7:
            printf("Entrez le mot à tester : ");
            scanf("%s", motATester);
            if (testerMot(motATester))
            {
                printf("\nLe mot est engendré par l'automate.\n");
            }
            else
            {
                printf("\nLe mot n'est pas engendré par l'automate.\n");
            }
            break;
        case 8:

            lireMots();

            break;
        case 9:
            printf("-------------------Au revoir!----------------\n");
            printf("-------------------- ^._.^ -------------------");
            break;
        default:
            printf("Option invalide, veuillez réessayer.\n");
        }

    } while (choix != 9);

    free(etatsdepa);
    free(etatssuiv);
    free(ettiquettesUniques);
    free(etiquettes);
    free(etatsInitial);
    free(etatsFinal);
    free(etatsAccessibles);

    return 0;
}

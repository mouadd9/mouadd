#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    long num;
    double solde;
    char nom[30];
    char prenom[30];
} compte;

char menu()
{
    char choix;
    printf("_______Ajouter un compte ______A \n");
    printf("_______Supprimer un compte ______S\n");
    printf("_______Opérations sur un compte ______ O\n");
    printf("_______lister les débiteurs ______D\n");
    printf("_______Lister les comptes ______L\n");
    printf("_______Quitter ______Q\n");
    printf("Entrer votre choix : ");
    scanf(" %c", &choix);
    return choix;
}
char sous_menu()
{
    char choix;

    printf(" * *Affichage * *A\n");
    printf(" * *virement * *V\n");
    printf(" * *Retrait * *R\n");
    printf(" * pour quitter taper * *Q\n");
    printf("Entrer votre choix : ");
    scanf(" %c", &choix);
    return choix;
}
compte *saisir()
{
    compte *nouveauCompte = (compte *)malloc(sizeof(compte));
    if (nouveauCompte == NULL)
    {
        printf("allocation pour nouveauCompte echouee");
        return NULL;
    }
    printf("entrer le numero du compte:\n");
    scanf("%ld", &nouveauCompte->num);
    printf("entrer votre solde\n");
    scanf("%lf", &nouveauCompte->solde);
    printf("entrer votre nom\n");
    scanf("%29s", nouveauCompte->nom);
    printf("entrer votre prenom\n");
    scanf("%29s", nouveauCompte->prenom);
    return (nouveauCompte);
}
void ajouter(char *fich, compte C)
{
    FILE *p = fopen(fich, "ab");
    if (p == NULL)
    {
        return;
    }
    fwrite(&C, sizeof(compte), 1, p);
    fclose(p);
}
void afficher(char *fich, long num)
{
    FILE *p = fopen(fich, "rb");
    if (p == NULL)
    {
        fclose(p);
        return;
    }
    compte *c = (compte *)malloc(sizeof(compte));
    if (c == NULL)
    {
        printf("Échec de l'allocation mémoire.\n");
        fclose(p);
        return;
    }
    while (fread(c, sizeof(compte), 1, p) == 1)
    {
        if (c->num == num)
        {
            printf("numero du compte: %ld\n", c->num);
            printf("solde du compte: %lf\n", c->solde);
            printf("nom: %s\n", c->nom);
            printf("prenom: %s\n", c->prenom);
        }
    }
    free(c);
    fclose(p);
}
int position(char *fich, long num)
{
    FILE *p = fopen(fich, "rb");
    if (p == NULL)
    {
        return 0;
    }
    compte *c;
    c = (compte *)malloc(sizeof(compte));
    int compteur = 0;
    int trouvee = 0;

    while (fread(c, sizeof(compte), 1, p) == 1)
    {
        compteur++;
        if (num == c->num)
        {
            return compteur;
            trouvee = 1;
            break;
        }
    }
    if (!trouvee)
    {
        return -1;
    }
    free(c);
    fclose(p);
}
void verser(char *fich, long num, double solde)
{
    FILE *p = fopen(fich, "r+b");
    if (p == NULL)
    {
        return;
    }
    compte *c = (compte *)malloc(sizeof(compte));
    if (c == NULL)
    {
        fclose(p);
        return;
    }
    while (fread(c, sizeof(compte), 1, p) == 1)
    {
        if (num == c->num)
        {
            c->solde = c->solde + solde;
            fseek(p, -sizeof(compte), 1);
            fwrite(c, sizeof(compte), 1, p);
        }
    }

    free(c);
    fclose(p);
}
void retirer(char *fich, long num, double solde)
{
    FILE *p = fopen(fich, "r+b");
    if (p == NULL)
    {
        return;
    }
    compte *c = (compte *)malloc(sizeof(compte));
    if (c == NULL)
    {
        fclose(p);
        return;
    }
    while (fread(c, sizeof(compte), 1, p) == 1)
    {
        if (num == c->num)
        {
            c->solde = c->solde-solde;
            fseek(p, -sizeof(compte), 1);
            fwrite(c, sizeof(compte), 1, p);
        }
    }

    free(c);
    fclose(p);
}
void debiteur(char *fich)
{
    FILE *p = fopen(fich, "rb");
    if (p == NULL)
    {
        return;
    }
    compte *c = (compte *)malloc(sizeof(compte));
    if (c == NULL)
    {
        fclose(p);
        return;
    }
    while (fread(c, sizeof(compte), 1, p) == 1)
    {
        if (c->solde < 0)
        {
            afficher(fich, c->num);
        }
    }
    free(c);
    fclose(p);
}
void comptes(char *fich)
{
    FILE *p = fopen(fich, "rb");
    if (p == NULL)
    {
        return;
    }
    compte *c = (compte *)malloc(sizeof(compte));
    if (c == NULL)
    {
        fclose(p);
        return;
    }
    while (fread(c, sizeof(compte), 1, p) == 1)
    {
        afficher(fich, c->num);
    }
    free(c);
    fclose(p);
}
void suppression(char *fich, long num)
{
    FILE *p = fopen(fich, "rb");
    if (p == NULL)
    {
        return;
    }
    char fichier[] = "temp.bin";
    FILE *f = fopen(fichier, "wb");
    if (f == NULL)
    {
        return;
    }
    compte *c = (compte *)malloc(sizeof(compte));
    if (c == NULL)
    {
        fclose(p);
        fclose(f);
        return;
    }
    while (fread(c, sizeof(compte), 1, p) == 1)
    {
        if (c->num != num)
        {
            fwrite(c, sizeof(compte), 1, f);
        }
    }
    free(c);
    fclose(p);
    fclose(f);
    remove(fich);
    rename(fichier, fich);
}
void operations(char *fich)
{
    char choix;
    do
    {
        choix = sous_menu();
        switch (choix)
        {
        case 'A':
            long numero;
            printf("entrer le numero du compte: ");
            scanf("%ld", &numero);
            afficher(fich, numero);
            break;
        case 'V':
            long numero1;
            printf("entrer le numero du compte: ");
            scanf("%ld", &numero1);
            double solde1;
            printf("entrer le solde a verser: ");
            scanf("%lf", &solde1);
            verser(fich, numero1, solde1);
            break;
        case 'R':
            long numero2;
            printf("entrer le numero du compte: ");
            scanf("%ld", &numero2);
            double solde2;
            printf("entrer le solde a retirer: ");
            scanf("%lf", &solde2);
            retirer(fich, numero2, solde2);
            break;
        case 'Q':
            break;
        }
    } while (choix != 'Q');
}
int main()
{
    char fichier[20];
    char choix;
    printf("donner le nom du fichier: ");
    scanf("%19s", fichier);

    do
    {
        choix = menu();
        switch (choix)
        {
        case 'A':
            compte *nouveauCompte = saisir();
            if (nouveauCompte == NULL)
            {
                return 0;
            }
            ajouter(fichier, *nouveauCompte);
            free(nouveauCompte);

            break;
        case 'S':
            long numero;
            printf("entrer le numero du compte a supprimer: ");
            scanf("%ld", &numero);
            suppression(fichier, numero);
            break;
        case 'O':
            operations(fichier);
            break;
        case 'D':
            debiteur(fichier);
            break;
        case 'L':
            comptes(fichier);
            break;
        case 'Q':
            break;
        }
    } while (choix != 'Q');
}
#include <stdio.h>
#include <string.h>

typedef struct
{
    char adresseExpediteur[100];
    char sujet[150];
    struct
    {
        int jour;
        int mois;
        int annee;
    } dateEnvoi;
    char contenu[1000];
    int etatDeLecture; 
} Courrier;


void saisirCourrier(Courrier *courrier) {
    printf("Adresse de l'expediteur: ");
    fgets(courrier->adresseExpediteur, 100, stdin);
    courrier->adresseExpediteur[strcspn(courrier->adresseExpediteur, "\n")] = 0; 

    printf("Sujet: ");
    fgets(courrier->sujet, 150, stdin);
    courrier->sujet[strcspn(courrier->sujet, "\n")] = 0;

    printf("Date d'envoi (jj mm aaaa): ");
    scanf("%d %d %d", &courrier->dateEnvoi.jour, &courrier->dateEnvoi.mois, &courrier->dateEnvoi.annee);
    getchar(); 

    printf("Contenu: ");
    fgets(courrier->contenu, 1000, stdin);
    courrier->contenu[strcspn(courrier->contenu, "\n")] = 0;

    courrier->etatDeLecture = 0; 
}

void ajouterCourrier(const Courrier *courrier)
{
    FILE *fichier = fopen("Mail.txt", "a"); 
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    fprintf(fichier, "Adresse de l'expéditeur: %s\n", courrier->adresseExpediteur);
    fprintf(fichier, "Sujet: %s\n", courrier->sujet);
    fprintf(fichier, "Date d'envoi: %02d/%02d/%04d\n", courrier->dateEnvoi.jour,
            courrier->dateEnvoi.mois, courrier->dateEnvoi.annee);
    fprintf(fichier, "Contenu: %s\n", courrier->contenu);
    fprintf(fichier, "État de lecture: %d\n\n", courrier->etatDeLecture);

    fclose(fichier);
}
void numeroterEnregistrements(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r"); 
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    
    FILE *fichierTemp = fopen("MailTemp.txt", "w");
    if (fichierTemp == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(fichier);
        return;
    }

    char ligne[1024];
    int numero = 1; 
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        
        if (strncmp(ligne, "Adresse de l'expéditeur:", strlen("Adresse de l'expéditeur:")) == 0) {
            fprintf(fichierTemp, "Enregistrement numéro: %d\n", numero++);
        }
        
        fputs(ligne, fichierTemp);
    }

    fclose(fichier); 
    fclose(fichierTemp); 

    remove(nomFichier);
    rename("MailTemp.txt", nomFichier);
}
void copierEnregistrementsImpairsEtPairs(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r"); 
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    FILE *fichierImpairs = fopen("Mail_Impairs.txt", "w");
    FILE *fichierPairs = fopen("Mail_Pairs.txt", "w");

    if (fichierImpairs == NULL || fichierPairs == NULL) {
        perror("Erreur lors de la création des fichiers impairs et pairs");
        fclose(fichier);
        if (fichierImpairs != NULL) fclose(fichierImpairs);
        if (fichierPairs != NULL) fclose(fichierPairs);
        return;
    }
      char ligne[1024];
    int index = 1; 
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        
        if (index % 2 == 0) {
            fputs(ligne, fichierPairs); 
        } else {
            fputs(ligne, fichierImpairs); 
        }

        if (strcmp(ligne, "\n") == 0) {
            index++; 
        }
    }

    fclose(fichier);
    fclose(fichierImpairs);
    fclose(fichierPairs);
}
int Chercher_Mail_Exped(const char *nomFichier, char *adresse) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    char ligne[1024];
    int numeroEnregistrement = 0;
    int trouve = 0;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
       
        if (strstr(ligne, adresse) != NULL) {
            trouve = 1; 
            break;
        }
        
        if (strcmp(ligne, "\n") == 0) {
            numeroEnregistrement++; 
        }
    }

    fclose(fichier); 

    return trouve ? numeroEnregistrement : -1;
}
void Afficher_Mail_Exped(const char *nomFichier, const char *adresseExp) {
    FILE *fichier = fopen(nomFichier, "r"); 
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Courrier courrier;
    int finDeMail = 0;

    while (!feof(fichier)) {
        
        fscanf(fichier, "Adresse de l'expéditeur: %[^\n]\n", courrier.adresseExpediteur);
        fscanf(fichier, "Sujet: %[^\n]\n", courrier.sujet);
        fscanf(fichier, "Date d'envoi: %d/%d/%d\n", &courrier.dateEnvoi.jour, &courrier.dateEnvoi.mois, &courrier.dateEnvoi.annee);
        fscanf(fichier, "Contenu: %[^\n]\n", courrier.contenu);
        fscanf(fichier, "État de lecture: %d\n\n", &courrier.etatDeLecture);

       
        finDeMail = fgetc(fichier);
        if (finDeMail == '\n') {

            if (courrier.etatDeLecture == 0 && strcmp(courrier.adresseExpediteur, adresseExp) == 0) {
                printf("Sujet: %s\n", courrier.sujet);
                printf("Date d'envoi: %02d/%02d/%04d\n", courrier.dateEnvoi.jour,
                       courrier.dateEnvoi.mois, courrier.dateEnvoi.annee);
                printf("Contenu: %s\n\n", courrier.contenu);
            }
        } else {

            ungetc(finDeMail, fichier);
        }
    }

    fclose(fichier);
}
void afficherMenu() {
    printf("\nMenu Principal\n");
    printf("1. Ajouter un courrier\n");
    printf("2. Numéroter les enregistrements\n");
    printf("3. Copier les enregistrements impairs et pairs\n");
    printf("4. Chercher un courrier par adresse d'expediteur\n");
    printf("5. Afficher tous les courriers non lus d'un expediteur\n");
    printf("6. Quitter\n");
    printf("Veuillez choisir une option: ");
}

int main() {
    int choix;
    Courrier courrier;
    char adresseExp[100];

    do {
        afficherMenu();
        scanf("%d", &choix);
        getchar(); 

        switch(choix) {
            case 1:
                saisirCourrier(&courrier);
                ajouterCourrier(&courrier);
                break;
            case 2:
                numeroterEnregistrements("Mail.txt");
                break;
            case 3:
                copierEnregistrementsImpairsEtPairs("Mail.txt");
                break;
            case 4:
                printf("Entrez l'adresse de l'expediteur: ");
                fgets(adresseExp, 100, stdin);
                adresseExp[strcspn(adresseExp, "\n")] = 0; // Supprimer le caractère de nouvelle ligne
                int resultat = Chercher_Mail_Exped("Mail.txt", adresseExp);
                if (resultat != -1) {
                    printf("Mail trouvé à l'enregistrement %d.\n", resultat + 1);
                } else {
                    printf("Aucun mail trouvé.\n");
                }
                break;
            case 5:
                printf("Entrez l'adresse de l'expediteur: ");
                fgets(adresseExp, 100, stdin);
                adresseExp[strcspn(adresseExp, "\n")] = 0;
                Afficher_Mail_Exped("Mail.txt", adresseExp);
                break;
            case 6:
                printf("Quitter\n");
                break;
            default:
                printf("Choix invalide. Veuillez choisir une option valide.\n");
        }
    } while(choix != 6);

    return 0;
}


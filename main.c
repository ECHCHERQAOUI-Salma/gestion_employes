#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//structure date
struct Date {
    int day;
    int month;
    int year;
};

//structure employee
struct Employe {
    int code;
    char fName[50];
    char lName[50];
    struct Date birthDate;
    char jobTitle[50];
    float salary;
    struct Employe* next;
};

//structure list
struct listeEmployees {
    struct Employe* head;
};


//initialiser la liste
struct listeEmployees* initList() {
    struct listeEmployees* liste = malloc(sizeof(struct listeEmployees));
    liste->head = NULL;

    return liste;
}

//si la liste est vide
int isEmpty(struct listeEmployees* liste) {
    return liste->head == NULL;
}

//sauvgarder les employees dans un fichier texte
void saveEmployeesToFile(const struct listeEmployees* liste, const char* fileName) {
    FILE* file = fopen(fileName, "w+");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour l'�criture.\n");
        return;
    }

    struct Employe* current = liste->head;

    while (current != NULL) {
        fprintf(file, "%d %s %s %d/%d/%d %s %f\n", current->code, current->fName, current->lName,
                current->birthDate.day, current->birthDate.month, current->birthDate.year,
                current->jobTitle, current->salary);

        current = current->next;
    }

    fclose(file);
}

//Charger les employees depuis un fichier texte.
void loadEmployeesFromFile(struct listeEmployees* liste,const char* fileName){
    FILE* file = fopen (fileName,"r");

    if (file == NULL){
        printf("Erreur lors d'ouverture.\n\n");
    }

    while(1){
         struct Employe* newEmploye = (struct Employe*)malloc(sizeof(struct Employe));

          if (fscanf(file, "%d %s %s %d/%d/%d %s %f",
                               &newEmploye->code,
                               newEmploye->fName,
                               newEmploye->lName,
                               &newEmploye->birthDate.day,
                               &newEmploye->birthDate.month,
                               &newEmploye->birthDate.year,
                               newEmploye->jobTitle,
                               &newEmploye->salary) != 8){

                    free(newEmploye);
                    if (feof(file)) {
                        break;
                    } else {
                        fclose(file);
                        return;
                    }

                   }

        newEmploye->next = NULL;

        if (isEmpty(liste)) {

        liste->head = newEmploye;

        }  else {
            struct Employe* it = liste->head;

            while (it->next != NULL) {
                it = it->next;
            }

            it->next = newEmploye;
        }
    }

}

//afficher les employes
void displayEmployees(const struct listeEmployees* liste) {
    struct Employe* current = liste->head;
    printf("Afficher la liste des employes (Pour quitter le programme tapez alt+F4) \n");
    while (current != NULL) {

        printf("Code : %d\n", current->code);
        printf("Pr�nom : %s\n", current->fName);
        printf("Nom : %s\n", current->lName);
        printf("Date de naissance : %d/%d/%d\n", current->birthDate.day, current->birthDate.month, current->birthDate.year);
        printf("Titre de poste : %s\n", current->jobTitle);
        printf("Salaire : %.2f\n", current->salary);

        current = current->next;
        printf("\n");
    }
}

void addEmploye(struct listeEmployees* liste) {
    struct Employe* newEmploye = (struct Employe*)malloc(sizeof(struct Employe));

    printf("Ajouter un employe (Pour quitter le programme tapez alt+F4) \n");
    printf("Entrez le code: ");
    scanf("%d", &newEmploye->code);

     struct Employe* it = liste->head;

        while (it->next != NULL) {
            if(newEmploye->code == it->code){
                printf("ce code exsiste deja\n");
                printf("Entrez le code: ");
                scanf("%d", &newEmploye->code);
            }
            it = it->next;
        }


    printf("Entrez le nom: ");
    scanf("%s", newEmploye->fName);

    printf("Entrez le pr�nom: ");
    scanf("%s", newEmploye->lName);

    printf("Entrez la date de naissance (j/m/aaaa):");
    scanf("%d/%d/%d", &newEmploye->birthDate.day, &newEmploye->birthDate.month, &newEmploye->birthDate.year);

    printf("Entrez le titre de poste: ");
    scanf("%s", newEmploye->jobTitle);

    printf("Entrez le salaire: ");
    scanf("%f", &newEmploye->salary);

    newEmploye->next = NULL;

    // Ajouter � la fin de la liste cha�n�e
    if (isEmpty(liste)) {

        liste->head = newEmploye;

        } else {
        struct Employe* it = liste->head;

        while (it->next != NULL) {
            it = it->next;
        }

        it->next = newEmploye;
    }
    printf("\nEmploye bien ajoute");
    saveEmployeesToFile(liste, "employees.txt");
}

void deleteEmploye(struct listeEmployees* liste){

    int code;
    printf("Supprimer un employe (Pour quitter le programme tapez alt+F4) \n");
    printf("Entrez le code de l'employ� � supprimer : ");
    scanf("%d", &code);

    struct Employe* current= liste->head;
    struct Employe* previous = NULL;
    int employeeFound = 0;

    while (current != NULL) {
        if (current->code == code) {

            printf("Les informations de l'employe.\n");
            printf("Code : %d\n", current->code);
            printf("Prenom : %s\n", current->fName);
            printf("Nom : %s\n", current->lName);
            printf("Date de naissance : %d/%d/%d\n", current->birthDate.day, current->birthDate.month, current->birthDate.year);
            printf("Titre de poste : %s\n", current->jobTitle);
            printf("Salaire : %f\n", current->salary);


            char confirmation;
            printf("Voulez-vous vraiment supprimer cet employ� ? (O/N) : ");
            scanf(" %c", &confirmation);

            if (confirmation == 'O' || confirmation == 'o') {

                if (previous == NULL) {

                    liste->head = current->next;
                } else {

                    previous->next = current->next;
                }

                free(current);
                printf("L'employe avec le code %d a ete supprime .\n", code);
            } else {
                printf("La suppression est annulee.\n");
            }

            employeeFound = 1;
            break;
        }

        previous = current;
        current = current->next;
    }
    if (employeeFound == 0) {
        printf("Aucun employ� avec le code %d n'a �t� trouv�.\n", code);
        deleteEmploye(liste);
    }
    free(current);
    saveEmployeesToFile(liste,"../employees.txt");
}

void searchEmploye(struct listeEmployees* liste){

    int choix;
    int count=0;
    printf("Chercher un employe par son code, son nom ou bien par son prenom:\n");
    printf("Si vous souhaitez chercher un employe par:\n 1- CODE \n 2- NOM \n 3- PRENOM\n");
    scanf("%d",&choix);

    if (choix == 1){
        int code;
        printf("Entrez le code de l'employ� � rechercher : ");
        scanf("%d",&code);
        struct Employe *current= liste->head;
        while(current != NULL){

            if ( current->code == code){
                printf("Les informations de l'employe.\n");
                printf("Code : %d\n", current->code);
                printf("Prenom : %s\n", current->fName);
                printf("Nom : %s\n", current->lName);
                printf("Date de naissance : %d/%d/%d\n", current->birthDate.day, current->birthDate.month, current->birthDate.year);
                printf("Titre de poste : %s\n", current->jobTitle);
                printf("Salaire : %f\n", current->salary);
                count++;

            }

            current= current->next;

        }
        if (count == 0){
            printf("Aucun employ� avec le code \"%d\" n'a �t� trouv�.\n",code);
        }
    }
    if (choix == 2){
        char fName[50];
        printf("Entrez le nom de l'employ� � rechercher : ");
        scanf("%s", &fName);
        struct Employe *current= liste->head;
        while(current != NULL){

            if (strcmp(current->fName, fName) == 0){
                printf("Les informations de l'employe.\n");
                printf("Code : %d\n", current->code);
                printf("Prenom : %s\n", current->fName);
                printf("Nom : %s\n", current->lName);
                printf("Date de naissance : %d/%d/%d\n", current->birthDate.day, current->birthDate.month, current->birthDate.year);
                printf("Titre de poste : %s\n", current->jobTitle);
                printf("Salaire : %f\n", current->salary);
                count++;
            }
            current= current->next;
        }
        if (count == 0){
            printf("Aucun employ� avec le nom \"%d\" n'a �t� trouv�.\n",fName);
        }
    }

        if (choix == 3){
        char lName[50];
        printf("Entrez le nom de l'employ� � supprimer : ");
        scanf("%s", &lName);
        struct Employe *current= liste->head;
        while(current != NULL){

            if (strcmp(current->lName, lName) == 0){
                printf("Les informations de l'employe.\n");
                printf("Code : %d\n", current->code);
                printf("Prenom : %s\n", current->fName);
                printf("Nom : %s\n", current->lName);
                printf("Date de naissance : %d/%d/%d\n", current->birthDate.day, current->birthDate.month, current->birthDate.year);
                printf("Titre de poste : %s\n", current->jobTitle);
                printf("Salaire : %f\n", current->salary);
                count++;
            }
            current= current->next;
        }
        if (count == 0){
            printf("Aucun employ� avec le prenom \"%d\" n'a �t� trouv�.\n",lName);
        }
    }
}

void updateEmployee(struct listeEmployees* liste) {
    int codeToFind;
    printf("Entrez le code de l'employ� � mettre � jour : ");
    scanf("%d", &codeToFind);

    struct Employe* current = liste->head;

    while (current != NULL) {
        if (current->code == codeToFind) {
            // Afficher les informations actuelles de l'employ�
            printf("\nInformations actuelles de l'employ� :\n");
            printf("Code: %d\n", current->code);
            printf("Pr�nom: %s\n", current->fName);
            printf("Nom: %s\n", current->lName);
            printf("Date de naissance: %d/%d/%d\n", current->birthDate.day, current->birthDate.month, current->birthDate.year);
            printf("Titre de poste: %s\n", current->jobTitle);
            printf("Salaire: %.2f\n", current->salary);

            // Demander les nouvelles informations
            do{
                printf("\nEntrez le nouveau titre de poste (President/VP-Sales/VP-Marketing/Sales-Manager/Sales-Rep) : ");
                scanf("%s", current->jobTitle);
            }while( strcmp(current->jobTitle, "President") != 0 &&
                     strcmp(current->jobTitle, "VP-Sales") != 0 &&
                     strcmp(current->jobTitle, "VP-Marketing") != 0 &&
                    strcmp(current->jobTitle, "Sales-Rep") != 0&&
                     strcmp(current->jobTitle, "Sales-Manager") != 0);


            // Limiter la mise � jour au poste et au salaire
            printf("Entrez le nouveau salaire : ");
            scanf("%f", &current->salary);

            // Afficher un message de confirmation
            printf("\nLes informations de l'employ� ont �t� mises � jour avec succ�s.\n");
saveEmployeesToFile(liste, "../employees.txt");
            return;
        }

        current = current->next;
    }

    // Aucun employ� trouv� avec le code donn�
    printf("\nAucun employ� trouv� avec le code %d.\n", codeToFind);
}


int main()
{
    int n;
    struct listeEmployees* liste = initList();
    loadEmployeesFromFile(liste, "employees.txt");
    printf("\nChoisissez le numero l'operation convenable:\n");
    printf("1: Afficher la liste des employes\n");
    printf("2: Ajouter un employe\n");
    printf("3: Supprimer un employe\n");
    printf("4: Rechercher un employe\n");
    printf("5: mettre a jour un employe\n");
    printf("6: Quitter\n");
    scanf("%d", &n);
    switch(n)
    {
    case (1):
        displayEmployees(liste);
        main();
        break;
    case (2):
        addEmploye(liste);
        main();
        break;
    case (3):
        deleteEmploye(liste);
        main();
        break;
    case (4):
        searchEmploye(liste);
        main();
         break;
    case (5):
        updateEmployee(liste);
        main();
         break;
    case (6):
        return 0;
    default:
        main();
    }
    return 0;
}

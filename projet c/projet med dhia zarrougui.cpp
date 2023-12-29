#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Voyageur {
    char nom[50];
    char prenom[50];
    int age;
    struct Voyageur *suivant;
} Voyageur;

typedef struct Bus {
    int numero;
    char destination[50];
    int capacite;
    int places_reservees;
    struct Bus *suivant;
} Bus;

typedef struct Reservation {
    int id;
    Bus bus;
    Voyageur voyageur;
    struct Reservation *suivant;
} Reservation;

void afficherReservation(Reservation reservation) {
    printf("\nD�tails de la r�servation :\n");
    printf("ID : %d\n", reservation.id);
    printf("Bus #%d - Destination : %s - Capacit� : %d - Places r�serv�es : %d\n",
           reservation.bus.numero, reservation.bus.destination, reservation.bus.capacite, reservation.bus.places_reservees);
    printf("Voyageur : %s %s - �ge : %d\n", reservation.voyageur.nom, reservation.voyageur.prenom, reservation.voyageur.age);
}

Voyageur *creerVoyageur() {
    Voyageur *nouveauVoyageur = (Voyageur *)malloc(sizeof(Voyageur));
    if (nouveauVoyageur == NULL) {
        perror("Erreur lors de l'allocation m�moire pour le voyageur");
        exit(EXIT_FAILURE);
    }

    printf("Nom du voyageur : ");
    scanf("%s", nouveauVoyageur->nom);
    printf("Pr�nom du voyageur : ");
    scanf("%s", nouveauVoyageur->prenom);
    printf("�ge du voyageur : ");
    scanf("%d", &nouveauVoyageur->age);
    nouveauVoyageur->suivant = NULL;

    return nouveauVoyageur;
}

Bus *creerBus() {
    Bus *nouveauBus = (Bus *)malloc(sizeof(Bus));
    if (nouveauBus == NULL) {
        perror("Erreur lors de l'allocation m�moire pour le bus");
        exit(EXIT_FAILURE);
    }

    printf("Num�ro du bus : ");
    scanf("%d", &nouveauBus->numero);
    printf("Destination : ");
    scanf("%s", nouveauBus->destination);
    printf("Capacit� du bus : ");
    scanf("%d", &nouveauBus->capacite);
    nouveauBus->places_reservees = 0;
    nouveauBus->suivant = NULL;

    return nouveauBus;
}

Reservation *creerReservation(int id) {
    Reservation *nouvelleReservation = (Reservation *)malloc(sizeof(Reservation));
    if (nouvelleReservation == NULL) {
        perror("Erreur lors de l'allocation m�moire pour la r�servation");
        exit(EXIT_FAILURE);
    }

    nouvelleReservation->id = id;
    nouvelleReservation->bus = *creerBus();
    nouvelleReservation->voyageur = *creerVoyageur();
    nouvelleReservation->suivant = NULL;

    return nouvelleReservation;
}

void ajouterVoyageur(Voyageur **listeVoyageurs) {
    Voyageur *nouveauVoyageur = creerVoyageur();
    nouveauVoyageur->suivant = *listeVoyageurs;
    *listeVoyageurs = nouveauVoyageur;
}

void ajouterBus(Bus **listeBus) {
    Bus *nouveauBus = creerBus();
    nouveauBus->suivant = *listeBus;
    *listeBus = nouveauBus;
}

void ajouterReservation(Reservation **listeReservations) {
    static int compteur_id = 1; // Variable statique pour conserver l'ID unique
    Reservation *nouvelleReservation = creerReservation(compteur_id++);
    nouvelleReservation->suivant = *listeReservations;
    *listeReservations = nouvelleReservation;

    if (nouvelleReservation != NULL) {
        nouvelleReservation->bus.places_reservees++;
        printf("Place r�serv�e avec succ�s.\n");
    } else {
        printf("Erreur lors de la cr�ation de la r�servation.\n");
    }
}


void afficherDetailsBus(Bus *listeBus) {
    printf("\nD�tails des bus :\n");
    while (listeBus != NULL) {
        printf("Bus #%d - Destination : %s - Capacit� : %d - Places r�serv�es : %d\n",
               listeBus->numero, listeBus->destination, listeBus->capacite, listeBus->places_reservees);
        listeBus = listeBus->suivant;
    }
}

void afficherEtModifierReservation(Reservation **listeReservations) {
    int id;
    printf("Entrez l'ID de la r�servation � afficher et modifier : ");
    scanf("%d", &id);

    Reservation *reservation = *listeReservations;
    while (reservation != NULL && reservation->id != id) {
        reservation = reservation->suivant;
    }

    if (reservation == NULL) {
        printf("Aucune r�servation trouv�e avec l'ID %d\n", id);
        return;
    }

    // Afficher les d�tails de la r�servation
    afficherReservation(*reservation);

    // Ici, vous pouvez ajouter le code pour modifier les d�tails de la r�servation si n�cessaire

    // Exemple de modification de la destination du bus
    printf("Nouvelle destination pour le bus : ");
    scanf("%s", reservation->bus.destination);

    printf("La r�servation a �t� modifi�e avec succ�s.\n");
}

void afficherToutesReservations(Reservation *listeReservations) {
    printf("\nToutes les r�servations :\n");
    while (listeReservations != NULL) {
        afficherReservation(*listeReservations);
        listeReservations = listeReservations->suivant;
    }
}

void libererMemoire(Voyageur *listeVoyageurs, Bus *listeBus, Reservation *listeReservations) {
    // Lib�rer la m�moire allou�e pour les listes cha�n�es
    while (listeVoyageurs != NULL) {
        Voyageur *temp = listeVoyageurs;
        listeVoyageurs = listeVoyageurs->suivant;
        free(temp);
    }

    while (listeBus != NULL) {
        Bus *temp = listeBus;
        listeBus = listeBus->suivant;
        free(temp);
    }

    while (listeReservations != NULL) {
        Reservation *temp = listeReservations;
        listeReservations = listeReservations->suivant;
        free(temp);
    }
}

void sauvegarderVoyageurs(Voyageur *listeVoyageurs) {
    FILE *file = fopen("voyageurs.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier voyageurs.txt");
        return;
    }

    while (listeVoyageurs != NULL) {
        fprintf(file, "%s %s %d\n", listeVoyageurs->nom, listeVoyageurs->prenom, listeVoyageurs->age);
        listeVoyageurs = listeVoyageurs->suivant;
    }

    fclose(file);
}

void chargerVoyageurs(Voyageur **listeVoyageurs) {
    FILE *file = fopen("voyageurs.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier voyageurs.txt");
        return;
    }

    char nom[50], prenom[50];
    int age;

    while (fscanf(file, "%s %s %d", nom, prenom, &age) == 3) {
        Voyageur *nouveauVoyageur = (Voyageur *)malloc(sizeof(Voyageur));
        if (nouveauVoyageur == NULL) {
            perror("Erreur lors de l'allocation m�moire pour le voyageur");
            exit(EXIT_FAILURE);
        }

        strcpy(nouveauVoyageur->nom, nom);
        strcpy(nouveauVoyageur->prenom, prenom);
        nouveauVoyageur->age = age;

        nouveauVoyageur->suivant = *listeVoyageurs;
        *listeVoyageurs = nouveauVoyageur;
    }

    fclose(file);
}

void sauvegarderBus(Bus *listeBus) {
    FILE *file = fopen("bus.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier bus.txt");
        return;
    }

    while (listeBus != NULL) {
        fprintf(file, "%d %s %d %d\n", listeBus->numero, listeBus->destination, listeBus->capacite, listeBus->places_reservees);
        listeBus = listeBus->suivant;
    }

    fclose(file);
}

void chargerBus(Bus **listeBus) {
    FILE *file = fopen("bus.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier bus.txt");
        return;
    }

    int numero, capacite, places_reservees;
    char destination[50];

    while (fscanf(file, "%d %s %d %d", &numero, destination, &capacite, &places_reservees) == 4) {
        Bus *nouveauBus = (Bus *)malloc(sizeof(Bus));
        if (nouveauBus == NULL) {
            perror("Erreur lors de l'allocation m�moire pour le bus");
            exit(EXIT_FAILURE);
        }

        nouveauBus->numero = numero;
        strcpy(nouveauBus->destination, destination);
        nouveauBus->capacite = capacite;
        nouveauBus->places_reservees = places_reservees;

        nouveauBus->suivant = *listeBus;
        *listeBus = nouveauBus;
    }

    fclose(file);
}

void sauvegarderReservations(Reservation *listeReservations) {
    FILE *file = fopen("reservations.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier reservations.txt");
        return;
    }

    while (listeReservations != NULL) {
        fprintf(file, "%d %d %s %d %s %s %d\n", listeReservations->id,
                listeReservations->bus.numero, listeReservations->bus.destination,
                listeReservations->bus.capacite, listeReservations->voyageur.nom,
                listeReservations->voyageur.prenom, listeReservations->voyageur.age);
        listeReservations = listeReservations->suivant;
    }

    fclose(file);
}
void proposEntreprise() {
    printf("\n� propos de notre soci�t� :\n");
    printf("Notre entreprise de bus est engag�e � fournir un service de transport s�r et fiable.\n");
    printf("Nous nous effor�ons d'offrir des voyages agr�ables et confortables � nos passagers.\n");
    printf("Votre satisfaction est notre priorit� !");
}

void chargerReservations(Reservation **listeReservations) {
    FILE *file = fopen("reservations.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier reservations.txt");
        return;
    }

    int id, numero, capacite, age;
    char destination[50], nom[50], prenom[50];

    while (fscanf(file, "%d %d %s %d %s %s %d", &id, &numero, destination, &capacite, nom, prenom, &age) == 7) {
        Reservation *nouvelleReservation = (Reservation *)malloc(sizeof(Reservation));
        if (nouvelleReservation == NULL) {
            perror("Erreur lors de l'allocation m�moire pour la r�servation");
            exit(EXIT_FAILURE);
        }

        nouvelleReservation->id = id;
        nouvelleReservation->bus.numero = numero;
        strcpy(nouvelleReservation->bus.destination, destination);
        nouvelleReservation->bus.capacite = capacite;
        strcpy(nouvelleReservation->voyageur.nom, nom);
        strcpy(nouvelleReservation->voyageur.prenom, prenom);
        nouvelleReservation->voyageur.age = age;

        nouvelleReservation->suivant = *listeReservations;
        *listeReservations = nouvelleReservation;
    }

    fclose(file);
}
void rechercherReservation(Reservation *listeReservations, int id) {
    Reservation *reservation = listeReservations;
    while (reservation != NULL) {
        if (reservation->id == id) {
            afficherReservation(*reservation);
            return;
        }
        reservation = reservation->suivant;
    }
    printf("Aucune r�servation trouv�e avec l'ID %d\n", id);
}

int main() {
	FILE *fichier;
	fichier=fopen("voyageurs.txt","w");
	fichier=fopen("bus.txt","w");
	fichier=fopen("reservations.txt","w");
    Voyageur *listeVoyageurs = NULL;         // Liste cha�n�e de voyageurs
    Bus *listeBus = NULL;                   // Liste cha�n�e de bus
    Reservation *listeReservations = NULL;  // Liste cha�n�e de r�servations

    chargerVoyageurs(&listeVoyageurs);
    chargerBus(&listeBus);
    chargerReservations(&listeReservations);

    while (1) {
        printf("\nbonjour comment nous pouvons vous aider :\n");
        printf("1. nouveau compte voyageur\n");
        printf("2. ajouter un bus\n");
        printf("3. ajouter une r�servation\n");
        printf("4. Afficher les d�tails des bus\n");
        printf("5.  modifier la destination d'une r�servation\n");
        printf("6. Afficher les r�servations\n");
        printf("7. Rechercher une r�servation par ID\n");
        printf("8. � propos de notre soci�t�\n");
        printf("9. Quitter l application\n");
        int choix;
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterVoyageur(&listeVoyageurs);
                break;
            case 2:
                ajouterBus(&listeBus);
                break;
            case 3:
                ajouterReservation(&listeReservations);
                break;
            case 4:
                afficherDetailsBus(listeBus);
                break;
            case 5:
                afficherEtModifierReservation(&listeReservations);
                break;
            case 6:
                afficherToutesReservations(listeReservations);
                break;
            
            case 7:
                int idToSearch;
                printf("Entrez l'ID de la r�servation � rechercher : ");
                scanf("%d", &idToSearch);
                rechercherReservation(listeReservations, idToSearch);
                break;
            case 8:
                proposEntreprise();
                break;
             case 9:
                sauvegarderVoyageurs(listeVoyageurs);
                sauvegarderBus(listeBus);
                sauvegarderReservations(listeReservations);
                libererMemoire(listeVoyageurs, listeBus, listeReservations);
                printf("merci d utiliser notre application a bientot\n");
                exit(0);
                break;
 

            default:
                printf("Choix invalide. Veuillez r�essayer.\n");
        }}}


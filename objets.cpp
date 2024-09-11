#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>  // For setw
#include "objets.h"
using namespace std;

Cours::Cours()
{
    nom = " ";
    nbEtudiants = 0;
    nbPeriode = 0;
    ordinateur = false;
}

Cours::Cours(string nom, int nbEtudiants, int nbPeriode, bool ordinateur)
{
    this->nom = nom;
    this->nbEtudiants = nbEtudiants;
    this->nbPeriode = nbPeriode;
    this->ordinateur = ordinateur;
}

Local::Local()
{
    nom = " ";
    capacite = 0;
    ordinateur = false;
}

Local::Local(string nom, int capacite, bool ordinateur)
{
    this->nom = nom;
    this->capacite = capacite;
    this->ordinateur = ordinateur;
}

// Function that reads from the file and returns a list of Cours objects
list<Cours> ListeCours() {
    // Create an input file stream
    ifstream file("classes.txt");

    // Check if the file opened correctly
    if (!file) {
        cerr << "Error opening file!" << endl;
        exit(1);
    }

    // Create a list to store Cours objects
    list<Cours> coursList;

    // Temporary variables to hold data from the file
    string nom;
    int nbEtudiants;
    int nbPeriode;
    bool ordinateur;

    // Read the file and create Cours objects, then add them to the list
    while (file >> nom >> nbEtudiants >> nbPeriode >> ordinateur) {
        // Create a Cours object and add it to the list
        coursList.emplace_back(nom, nbEtudiants, nbPeriode, ordinateur);
    }

    // Close the file
    file.close();

    // Return the list of Cours objects
    return coursList;
}


// Function that reads from the file and returns a list of Local objects
list<Local> ListeLocaux() {
    // Create an input file stream
    ifstream file("locaux.txt");

    // Check if the file opened correctly
    if (!file) {
        cerr << "Error opening file!" << endl;
        exit(1);
    }

    // Create a list to store Local objects
    list<Local> locauxList;

    // Temporary variables to hold data from the file
    string nom;
    int capacite;
    bool ordinateur;

    // Read the file and create Local objects, then add them to the list
    while (file >> nom >> capacite >> ordinateur) {
        Local local(nom, capacite, ordinateur);

        // Fill the schedule with some dummy courses for each classroom
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 4; ++j) {
                local.horaireCours[i][j] = Cours("8INF866", 32, 2, true); // Replace with real data as needed
            }
        }

        locauxList.push_back(local);
    }

    // Close the file
    file.close();

    // Return the list of Local objects
    return locauxList;
}

void Cours::afficher()
{
    cout << "Nom du cours : " << nom << endl;
    cout << "Nombre d'etudiants : " << nbEtudiants << endl;
    cout << "Nombre de periodes : " << nbPeriode << endl;
    cout << "Le cours a-t-il besoin d'une classe equipe d'ordinateur? : " << ordinateur << endl;
}

void Local::afficher()
{
    cout << "Nom du local : " << nom << endl;
    cout << "Capacite du local : " << capacite << endl;
    cout << "Le local est-il equipe d'ordinateur? : " << ordinateur << endl;
}

//trier la liste des cours par nombre d'etudiants
void TrierCours(list<Cours> listeCours)
{
    listeCours.sort([](Cours a, Cours b) { return a.nbEtudiants < b.nbEtudiants; });
}

//trier la liste des locaux par capacite
void TrierLocaux(list<Local> listeLocaux)
{
    listeLocaux.sort([](Local a, Local b) { return a.capacite < b.capacite; });
}

//Placer les cours dans l'horaire des locaux en fonction des besoins,chaque local peut prendre 4 cours par jour, il y a 5 jours dans la semaine
list<Local> PlacerCours(list<Cours> listeCours, list<Local> listeLocaux, list<Local> locauxPlein)
{
    list<Cours>::iterator itCours;
    list<Local>::iterator itLocal;
    list<Cours> coursTropGrands;
    int i = 0;
    for (itCours = listeCours.begin(); itCours != listeCours.end(); itCours++)
    {
        for (itLocal = listeLocaux.begin(); itLocal != listeLocaux.end(); itLocal++)
        {
            if (itCours->ordinateur == itLocal->ordinateur)
            {
                //Placer le cours dans l'horaire du local
                //On suppose que chaque local peut prendre 4 cours par jour
                //On suppose qu'il y a 5 jours dans la semaine
                itLocal->horaireCours[i / 4][i % 4] = *itCours;
                i++;
                if (i == 20)
                {
                    i = 0;
                    //si le local est plein on le met dans une liste a part et on le retire de la liste des locaux
                    locauxPlein.push_back(*itLocal);
                    listeLocaux.erase(itLocal);
                }
                
                
            }
            //Si le cours est trop grand pour le local on le met dans une liste a part
            //On suppose que la liste est triee par nombre d'etudiants et par capacite
            else
            {
                coursTropGrands.push_back(*itCours);
            }
            
        }
    }
    //On separes les cours trop grand en 2 pour les placer dans les locaux plus petits
    list<Cours> coursRestants;
    for (itCours = coursTropGrands.begin(); itCours != coursTropGrands.end(); itCours++)
    {
        itCours->nbEtudiants /= 2;
        Cours coursRestant1 = *itCours;
        Cours coursRestant2 = *itCours;
        coursRestant1.nom += "GR_1";
        coursRestant2.nom += "GR_2";
        coursRestants.push_back(coursRestant1);
        coursRestants.push_back(coursRestant2);
    }
    if(coursRestants.size() > 0)
    {
        if(listeLocaux.size() > 0)
        {
            PlacerCours(coursRestants, listeLocaux, locauxPlein);
        }
        else
        {
            cout << "Il n'y a plus de locaux disponibles pour les cours suivants" << endl;
            for (itCours = coursRestants.begin(); itCours != coursRestants.end(); itCours++)
            {
                itCours->afficher();
            }
        }
    }
    return locauxPlein;
}

//extraire une liste de cours a partir des horaires des locaux
list<Cours> ExtraireCours(list<Local> listeLocaux)
{
    list<Cours> listeCours;
    list<Local>::iterator itLocal;
    for (itLocal = listeLocaux.begin(); itLocal != listeLocaux.end(); itLocal++)
    {
        for (size_t i = 0; i < 5; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                listeCours.push_back(itLocal->horaireCours[i][j]);
            }
        }
    }
    return listeCours;
}

//s'assurer que chaque cours est place autant de fois dans l'horaire que son nombre de periodes
bool VerifierCours(list<Local> listeLocaux)
{
    list<Cours> listeCours = ExtraireCours(listeLocaux);
    list<Cours>::iterator itCours;
    list<Local>::iterator itLocal;
    for (itCours = listeCours.begin(); itCours != listeCours.end(); itCours++)
    {
        int nbPeriode = 0;
        for (itLocal = listeLocaux.begin(); itLocal != listeLocaux.end(); itLocal++)
        {
            for (size_t i = 0; i < 5; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    if (itLocal->horaireCours[i][j].nom == itCours->nom)
                    {
                        nbPeriode++;
                    }
                }
            }
        }
        if (nbPeriode != itCours->nbPeriode)
        {
            cout << "Le cours " << itCours->nom << " n'est pas place autant de fois que son nombre de periodes" << endl;
            return false;
        }
        else
        {
            cout << "Le cours " << itCours->nom << " est OK" << endl;
        }
    }
    return true;
}



//Afficher l'horaire des locaux
void AfficherHoraire(list<Local> listeLocaux)
{
    // Display the header for the days of the week
    cout << setw(12) << "Mon" << setw(12) << "Tue" << setw(12) << "Wed" << setw(12) << "Thu" << endl;

    list<Local>::iterator itLocal;
    for (itLocal = listeLocaux.begin(); itLocal != listeLocaux.end(); itLocal++)
    {
        itLocal->afficher();
        for (size_t i = 0; i < 5; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                cout << setw(12) << left << itLocal->horaireCours[i][j].nom;  // Display course name with 12 characters width, left-aligned
                
            }
            cout << endl;  // Move to the next line after each row (day)
        }
    }
}

int main()
{
    list<Cours> listeCours = ListeCours();
    list<Local> listeLocaux = ListeLocaux();
    list<Local> locauxPlein;
    locauxPlein = PlacerCours(listeCours, listeLocaux, locauxPlein);
    if (VerifierCours(locauxPlein))
    {
        cout << "Les cours sont bien places" << endl;
        AfficherHoraire(locauxPlein);
    }
    else
    {
        cout << "Les cours ne sont pas bien places" << endl;
    }
    return 0;
}

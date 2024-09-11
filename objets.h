#ifndef OBJETS_H
#define OBJETS_H
#include <list>
#include <iostream>
using namespace std;

class Cours
{
public:
    string nom;//8 - 9 characters
    int nbEtudiants;// 32 or 64
    int nbPeriode;// 1 - 3
    bool ordinateur;//true or false
    Cours();
    Cours(string nom, int nbEtudiants, int nbPeriode, bool ordinateur);
    void afficher();
};

class Local
{
public:
    string nom;
    int capacite;// 32 or 64
    bool ordinateur;
    Cours horaireCours[5][4];
    Local();
    Local(string nom, int capacite, bool ordinateur);
    void afficher();
};




#endif
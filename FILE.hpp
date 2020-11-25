#ifndef _FILE_H_
#define _FILE_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm> 


using namespace std;
typedef struct Capteur Capteur;
struct Capteur
{
    unsigned int Cout ;
    bool Actif ;
    vector<unsigned int> List_Cible , List_Activer ;
};
class File_Data
{
public:
    File_Data(char File_Name[] );
    void Translate_to_Lp_file(char Lp_File_Name[]);
    void Glouton();
    unsigned int Heuristique();
    bool check_Capteur();
    float Get_Score(unsigned int INDEX);
    void Activate_This_Capteur(unsigned int INDX_Capteur);
    void If_Not_in_Add_Vector( vector<unsigned int >& NEW_VECT , unsigned int Value );
    void INFO();
unsigned int N , M ; // N nombre de capteur activable , M Nombre de cible
unsigned int *Size_per_Capteur;// les coûts de déploiement des N capteurs // nombre de capteur pour chaque cible
unsigned int **Cible_Capteur;// Matrice , chaque cible lier a une list de capteur
Capteur *Data_CAPTEUR ; 
bool *Activated_Cible ;
vector< unsigned int > Solution ;



};
#endif

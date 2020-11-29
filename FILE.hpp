#ifndef _FILE_H_
#define _FILE_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>     


using namespace std;
typedef struct Capteur Capteur;
typedef struct _Solution_ _Solution_;
typedef struct X_XX X_XX;

struct Capteur
{
    unsigned int Cout;
    bool Actif;
    vector<unsigned int> List_Cible, List_Activer;
};
struct _Solution_
{
    unsigned int INDX, Cout;
    bool Actif;
    vector<unsigned int> List_Cible;
};
struct X_XX
{
    unsigned int X, XX;
};
class File_Data
{
public:
    File_Data(char File_Name[]);
    void INFO();
    void Glouton();
    void Sum_Score();
    void Clear_ALL();
    bool check_Capteur();
    void Bool_Solution();
    void Reduction_Solution();
    unsigned int Heuristique();
    void Show_Matrice_Solution();
    void Update_Matrice_Solution();
    bool IF_Useless(unsigned int INDX);
    void Save_Solution(char Out_put[]);
    float Get_Score(unsigned int INDEX);
    float Get_Score_Without(unsigned int INDX);
    void Translate_to_Lp_file(char Lp_File_Name[]);
    void Activate_This_Capteur(unsigned int INDX_Capteur);
    _Solution_ Create_Solution(unsigned int Index_Solution);
    bool Same_As(unsigned int X, unsigned int XX, unsigned int Y);
    void If_Not_in_Add_Vector(vector<unsigned int> &NEW_VECT, unsigned int Value);

    unsigned int N, M, Score;       // N nombre de capteur activable , M Nombre de cible
    unsigned int *Size_per_Capteur; // les coûts de déploiement des N capteurs // nombre de capteur pour chaque cible
    unsigned int **Cible_Capteur;   // Matrice , chaque cible lier a une list de capteur
    Capteur *Data_CAPTEUR;
    char *Activated_Capteur ;
    bool *Activated_Cible ;
    vector<_Solution_> List_Solution;
    unsigned int **Matrice_Solution, *SOM;
};
#endif

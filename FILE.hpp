#ifndef _FILE_H_
#define _FILE_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

class File_Data
{
public:
    File_Data(char File_Name[] );
    void Translate_to_Lp_file(char Lp_File_Name[]);
    void Show_Info();
unsigned int i , j ;
unsigned int N , M ; // N nombre de capteur activable , M Nombre de cible
unsigned int *Couts_N , *Size_per_Cible;// les coûts de déploiement des N capteurs
unsigned int **Cible_Capteur;


};
#endif

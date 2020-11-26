#include "FILE.hpp"
File_Data::File_Data(char File_Name[])
{
    FILE *File;
    unsigned int i, j;

    File = fopen(File_Name, "r");
    if (File == NULL)
    {
        cout << "Erreur lors de la Lecture du fichier  " << File_Name << endl;
        exit(EXIT_FAILURE);
    }
    fscanf(File, "%d %d", &M, &N);
    Data_CAPTEUR = new Capteur[N];
    Size_per_Capteur = new unsigned int[M];
    Activated_Cible = new bool[M];
    Cible_Capteur = new unsigned int *[M];
    unsigned int NB_LIGNE = N / 12 + 1, INDX = 0, geter;
    for (i = 0; i < NB_LIGNE; i++)
    {
        if (i == NB_LIGNE - 1)
        {
            for (j = 0; j < N % 12; j++)
            {
                fscanf(File, "%d ", &Data_CAPTEUR[INDX].Cout);
                Data_CAPTEUR[INDX].Actif = false;
                INDX++;
            }
            continue;
        }
        for (j = 0; j < 12; j++)
        {
            fscanf(File, "%d ", &Data_CAPTEUR[INDX].Cout);
            Data_CAPTEUR[INDX].Actif = false;
            INDX++;
        }
    }
    for (i = 0; i < M; i++)
    {
        Activated_Cible[i] = false;
        fscanf(File, "%d ", &INDX);
        Cible_Capteur[i] = new unsigned int[INDX];
        Size_per_Capteur[i] = INDX;
        for (j = 0; j < INDX; j++)
        {
            fscanf(File, "%d ", &geter);
            Cible_Capteur[i][j] = geter - 1;
            Data_CAPTEUR[geter - 1].List_Cible.push_back(i);
        }
    }
    fclose(File);
}
void File_Data::Translate_to_Lp_file(char Lp_File_Name[])
{
    FILE *File;
    unsigned int i, j;

    File = fopen(Lp_File_Name, "w");
    fprintf(File, "Minimize\n z: %d x%d ", Data_CAPTEUR[0].Cout, 1);
    for (i = 1; i < N; i++)
    {
        fprintf(File, "+ %d x%d ", Data_CAPTEUR[i].Cout, i + 1);
    }
    fprintf(File, "\nSubject To\n");
    for (i = 0; i < M; i++)
    {

        fprintf(File, " c%d: x%d ", i + 1, Cible_Capteur[i][0] + 1);
        for (j = 1; j < Size_per_Capteur[i]; j++)
        {
            fprintf(File, "+ x%d ", Cible_Capteur[i][j] + 1);
        }
        fprintf(File, " >= 1 \n");
    }

    fprintf(File, "Binaries\n");

    for (i = 0; i < M; i++)
    {
        fprintf(File, " x%d \n", i + 1);
    }
    fprintf(File, "End\n");
    fclose(File);
}
float File_Data::Get_Score(unsigned int INDEX)
{
    return (float)(Data_CAPTEUR[INDEX].List_Cible.size() - Data_CAPTEUR[INDEX].List_Activer.size()) / Data_CAPTEUR[INDEX].Cout;
}
unsigned int File_Data::Heuristique()
{
    unsigned int INDX, i;
    float VAR, MAX = 0;
    for (i = 0; i < N; i++)
    {
        if (Data_CAPTEUR[i].Actif == false)
        {
            VAR = Get_Score(i);
            if (MAX < VAR)
            {
                MAX = VAR;
                INDX = i;
            }
        }
    }
    return INDX;
}
void File_Data::If_Not_in_Add_Vector(vector<unsigned int> &NEW_VECT, unsigned int Value)
{
    if (find(NEW_VECT.begin(), NEW_VECT.end(), Value) == NEW_VECT.end())
    {
        NEW_VECT.push_back(Value);
    }
}
void File_Data::Activate_This_Capteur(unsigned int INDX_Capteur)
{
    Data_CAPTEUR[INDX_Capteur].Actif = true;
    unsigned int j, h, INDX_Cible, INDX;
    for (j = 0; j < Data_CAPTEUR[INDX_Capteur].List_Cible.size(); j++)
    {
        INDX_Cible = Data_CAPTEUR[INDX_Capteur].List_Cible[j];
        Activated_Cible[INDX_Cible] = true;
        Data_CAPTEUR[INDX_Capteur].List_Activer.push_back(INDX_Cible);
        for (h = 0; h < Size_per_Capteur[INDX_Cible]; h++)
        {
            INDX = Cible_Capteur[INDX_Cible][h];
            If_Not_in_Add_Vector(Data_CAPTEUR[INDX].List_Activer, INDX_Cible);
        }
    }
}
bool File_Data::check_Capteur()
{
    for (unsigned int i = 0; i < M; i++)
    {
        if (Activated_Cible[i] == false)
        {
            return false;
        }
    }
    return true;
}
void File_Data::Glouton()
{
    unsigned int score = 0, INDX;
    bool STOP = false;
    while (STOP == false)
    {
        INDX = Heuristique();
        score += Data_CAPTEUR[INDX].Cout;
        List_Solution.push_back(Create_Solution(INDX));
        Activate_This_Capteur(INDX);
        STOP = check_Capteur();
    }
    printf("Score %d  ", score);
    INFO();
}
void File_Data::INFO()
{
    unsigned int i , j ;
    printf(" Solution ==> \n");
    for (i = 0; i < List_Solution.size(); i++)
    {
        printf(" %d ===> " , List_Solution[i].INDX);
        for( j = 0 ; j < List_Solution[i].List_Cible.size() ; j++)
        {
            printf(" %d ", List_Solution[i].List_Cible[j]);
        }
        printf(" \n ");
    }
}
_Solution_ File_Data::Create_Solution(unsigned int Index_Solution)
{
    _Solution_ New;
    string Translate;
    New.Cout = Data_CAPTEUR[Index_Solution].Cout;
    New.INDX = Index_Solution;
    New.Actif = true;
    for(unsigned int i = 0; i < Data_CAPTEUR[Index_Solution].List_Cible.size(); i++)
    {
        New.List_Cible = Data_CAPTEUR[Index_Solution].List_Cible ;

    }
    return New;
}
//void File_Data::Reduction_()

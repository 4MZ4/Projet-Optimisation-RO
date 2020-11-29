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
    // Allocation in Memory
    Data_CAPTEUR = new Capteur[N];
    Size_per_Capteur = new unsigned int[M];
    Activated_Cible = new bool[M];
    Cible_Capteur = new unsigned int *[M];
    Matrice_Solution = new unsigned int *[List_Solution.size()];
    SOM = new unsigned int[M];
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
void File_Data::Clear_ALL()
{
    unsigned int i;
    for (i = 0; i < N; i++)
    {
        Data_CAPTEUR[i].Actif = false;
        Data_CAPTEUR[i].List_Activer.clear();
    }
}
void File_Data::Glouton()
{
    unsigned int INDX;
    bool STOP = false;
    while (STOP == false)
    {
        INDX = Heuristique();
        List_Solution.push_back(Create_Solution(INDX));
        Activate_This_Capteur(INDX);
        STOP = check_Capteur();
    }
    Sum_Score();
    printf("\n Score Glouton %d  \n", Score);
    //INFO();
}
void File_Data::INFO()
{
    unsigned int i, j;
    printf(" Solution ==> \n");
    for (i = 0; i < List_Solution.size(); i++)
    {
        printf(" %d ===> ", List_Solution[i].INDX);
        for (j = 0; j < List_Solution[i].List_Cible.size(); j++)
        {
            printf(" %d ", List_Solution[i].List_Cible[j]);
        }
        printf(" \n ");
    }
}
void File_Data::Sum_Score()
{
    Score = 0;
    for (unsigned int i = 0; i < List_Solution.size(); i++)
    {
        if (List_Solution[i].Actif == true)
        {
            Score += List_Solution[i].Cout;
        }
    }
}
_Solution_ File_Data::Create_Solution(unsigned int Index_Solution)
{
    _Solution_ New;
    string Translate;
    New.Cout = Data_CAPTEUR[Index_Solution].Cout;
    New.INDX = Index_Solution;
    New.Actif = true;
    for (unsigned int i = 0; i < Data_CAPTEUR[Index_Solution].List_Cible.size(); i++)
    {
        New.List_Cible = Data_CAPTEUR[Index_Solution].List_Cible;
    }
    return New;
}
bool File_Data::Same_As(unsigned int X, unsigned int XX, unsigned int Y)
{
    unsigned int i;
    for (i = Y; i < M; i++)
    {
        if (Matrice_Solution[X][i] == 1)
        {
            if (Matrice_Solution[XX][i] != 1)
            {
                return false;
            }
        }
    }
    return true;
}
void File_Data::Update_Matrice_Solution()
{
    unsigned int i, j, y;
    for (i = 0; i < List_Solution.size(); i++)
    {
        Matrice_Solution[i] = new unsigned int[M];
        for (j = 0; j < M; j++)
        {
            Matrice_Solution[i][j] = 0;
        }
    }
    for (j = 0; j < M; j++)
    {
        SOM[j] = 0;
    }
    for (i = 0; i < List_Solution.size(); i++)
    {
        if (List_Solution[i].Actif == true)
        {
            for (j = 0; j < List_Solution[i].List_Cible.size(); j++)
            {

                y = List_Solution[i].List_Cible[j];
                Matrice_Solution[i][y]++;
                SOM[y]++;
            }
        }
    }
}
void File_Data::Show_Matrice_Solution()
{
    unsigned int i, j;
    printf("\n");
    for (i = 0; i < List_Solution.size(); i++)
    {
        for (j = 0; j < M; j++)
        {
            printf("%d", Matrice_Solution[i][j]);
        }
        printf("\n");
    }
    for (j = 0; j < M; j++)
    {
        printf("%d", SOM[j]);
    }
}
bool File_Data::IF_Useless(unsigned int INDX)
{
    unsigned int i, j, *SOMM = new unsigned int[M];
    for (j = 0; j < List_Solution.size(); j++)
    {
        SOMM[j] = 0;
    }
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < List_Solution.size(); j++)
        {
            if (j != INDX)
            {
                if (Matrice_Solution[j][i] == 1)
                {
                    SOMM[i]++;
                }
            }
        }
        if (SOMM[i] == 0)
        {
            return false;
        }
    }
    return true;
}
float File_Data::Get_Score_Without(unsigned int INDX)
{
    Update_Matrice_Solution();
    unsigned int i, cible, score = 0;
    for (i = 0; i < List_Solution[INDX].List_Cible.size(); i++)
    {
        cible = List_Solution[INDX].List_Cible[i];
        score += SOM[cible] - 1;
    }
    return score;
}
void File_Data::Reduction_Solution()
{
    unsigned int i;
    double cout = 0, best = 0;
    int to_reduce = -1;
    vector<unsigned int> Choix_Reduction;
    Update_Matrice_Solution();

    do
    {
        to_reduce = -1;
        cout = 0, best = 0;
        for (i = 0; i < List_Solution.size(); i++)
        {
            if (List_Solution[i].Actif == true)
            {
                if (IF_Useless(i) == true)
                {
                    cout = List_Solution[i].Cout;
                    if (cout > best)
                    {
                        best = cout;
                        to_reduce = i;
                    }
                }
            }
        }
        List_Solution[to_reduce].Actif = false;
        Update_Matrice_Solution();

    } while (to_reduce != -1);

    Sum_Score();
    printf("Nouveau Score Glouton Optimise %d \n", Score);
    // Analyse
    /*for (i = 0; i < List_Solution.size(); i++)
    {
        for (j = 0; j < M; j++)
        {
            if (SOM[j] > 1)
            {
                if (Matrice_Solution[i][j] == 1)
                {
                    for (x = i + 1; x < List_Solution.size(); x++)
                    {
                        if (Matrice_Solution[x][j] == 1)
                        {
                            if (Same_As(i, x, j) == true)
                            {
                                printf(" %d %d \n", i, x);
                                res.X = i;
                                res.XX = x;
                                Reduction.push_back(res);
                            }
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
*/
    //Show
}
void File_Data::Bool_Solution()
{
    Activated_Capteur = new char[N];
    unsigned int i, INDX;
    for (i = 0; i < N; i++)
    {
        Activated_Capteur[i] = '0';
    }
    for (i = 0; i < List_Solution.size(); i++)
    {
        INDX = List_Solution[i].INDX;
        if (List_Solution[i].Actif == true)
        {
            Activated_Capteur[INDX] = '1';
        }
    }
}
void File_Data::Save_Solution(char Out_put[])
{
    Bool_Solution();
    FILE *File_OUT;
    File_OUT = fopen(Out_put, "w");
    for (unsigned int i = 0; i < N; i++)
    {
        fprintf(File_OUT, "%c ", Activated_Capteur[i]);
    }
    fclose(File_OUT);
}
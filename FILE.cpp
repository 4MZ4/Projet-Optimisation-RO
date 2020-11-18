#include "FILE.hpp"
File_Data::File_Data(char File_Name[])
{
    FILE *File;
    File = fopen(File_Name, "r");
    if (File == NULL)
    {
        cout << "Erreur lors de la Lecture du fichier  " << File_Name << endl;
        exit(EXIT_FAILURE);
    }
    fscanf(File, "%d %d", &M, &N);
    Couts_N = new unsigned int[N];
    Size_per_Cible = new unsigned int[M];
    Cible_Capteur = new unsigned int * [M];
    unsigned int NB_LIGNE = N / 12 + 1, INDX = 0;
    for (i = 0; i < NB_LIGNE; i++)
    {
        if (i == NB_LIGNE - 1)
        {
            for (j = 0; j < N % 12; j++)
            {
                fscanf(File, "%d ", &Couts_N[INDX]);
                INDX++;
            }
            continue;
        }
        for (j = 0; j < 12; j++)
        {
            fscanf(File, "%d ", &Couts_N[INDX]);
            INDX++;
        }
    }
    for (i = 0; i < M; i++)
    {
        fscanf(File, "%d ", &INDX);
        Cible_Capteur[i] = new unsigned int [INDX];
        Size_per_Cible[i] = INDX ;
        for (j = 0; j < INDX; j++)
        {
            fscanf(File, "%d ", &Cible_Capteur[i][j]);
        }
    }
    fclose(File);
}
void File_Data::Translate_to_Lp_file(char Lp_File_Name[])
{
    FILE *File;
    File = fopen(Lp_File_Name, "w");
    fprintf(File, "Minimize\n z: %d x%d ", Couts_N[0], 1);
    for (i = 1; i < N; i++)
    {
        fprintf(File, "+ %d x%d ", Couts_N[i], i + 1);
    }
    fprintf(File, "\nSubject To\n");
    for (i = 0; i < M; i++)
    {

        fprintf(File, " c%d: x%d ", i + 1, Cible_Capteur[i][0]);
        for (j = 1; j < Size_per_Cible[i] ; j++)
        {
            fprintf(File, "+ x%d ", Cible_Capteur[i][j]);

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
void File_Data::Show_Info()
{
    cout << " SHOW INFO " << endl;
    cout << " N " << N << " M " << M << endl;
    for (i = 0; i < N; i++)
    {
        printf(" c%d: x%d ", i + 1, Cible_Capteur[i][0]);
    }
}
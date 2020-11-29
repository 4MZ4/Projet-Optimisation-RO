#include "FILE.cpp"
//g++ GLPK.cpp -Wall -o GLPK && GLPK inst41.txt Res.txt
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "!!!!! Nombre d'arguments insuffisant !!!!!!" << std::endl;
        return -1;
    }
    printf(" %s ",argv[1]);
    File_Data A = File_Data(argv[2]);
    A.Translate_to_Lp_file(argv[3]);
}
#include "FILE.cpp"
//g++ main.cpp -Wall -o a.exe && a.exe
int main(int argc, char **argv)
{
    char file_name[] = "inst41.txt";
    //char OUT[] = "OUT1.txt";
    File_Data A = File_Data(file_name);
    //A.Translate_to_Lp_file(OUT);
    A.Glouton();
}
#include <cstdio>
#include <cstdlib>
#include "../include/converter.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 2)
    {
        printf("Usage:> %s <file_name> <num_operation>\n",argv[0]);
        printf("<file_name> = Nome do arquivo a ser convetido\n");
        printf("<num_operation> = Numero da operacao a ser realizada\n");
        printf("\t1 = Converter VTP para VTK\n");
        printf("\t2 = Converter VTK para DAT\n");
        exit(-1);
    }
    else
    {
        switch (atoi(argv[2]))
        {
            case 1: {
                        char *outFilename = changeExtension(argv[1],"vtk");
                        VTP_To_VTK(argv[1],outFilename);
                        free(outFilename);
                        break;
                    }
            case 2: {
                        char *outFilename = changeExtension(argv[1],"dat");
                        VTK_To_DAT(argv[1],outFilename);
                        free(outFilename);
                        break;
                    }
        }
        return 0;
    }
}
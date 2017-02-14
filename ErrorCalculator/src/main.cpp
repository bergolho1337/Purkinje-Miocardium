#include <cstdio>
#include <cstdlib>
#include "../include/error.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 2)
    {
        printf("===============================================================================\n");
        printf("Usage:> %s <analitical_result.vtk> <aproximation_result.vtk>\n",argv[0]);
        printf("<analitical_result.vtk> = Arquivo .vtk contendo a solucao analitica\n");
        printf("<aproximation_result.vtk> = Arquivo .vtk contendo a solucao aproximada\n\n");
        printf("Exemplo: ./errorCalculator vent_tempo_r4.vtk result_Benjamin_130_ponto.vtk\n");
        printf("Calcula o erro entre a aproximacao feita pelo GMSH e a arvore original do Benjamin\n");
        printf(" !!!! O arquivo .vtk deve estar representando o numero em ponto flutuante com '.' nao com ',' !!!\n");
        printf("===============================================================================\n");
        exit(-1);
    }
    else
    {
        ErrorCalculator *error = newErrorCalculator(argc,argv);
        calculateError(error);
        freeMemory(error);
        return 0;
    }
}

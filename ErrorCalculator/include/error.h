#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

struct Point
{
    double x, y, z;                     // Coordenadas (x,y,z)
    double aTime;                       // Tempo de ativacao
}typedef Point;

struct Polygons
{
    int id1;                            // Ids que formam o poligono
    int id2;
    int id3;
}typedef Polygons;

struct vtkFile
{
    Point *points;                      // Vetor de pontos do .vtk
    int N;                              // Numero de pontos
    Polygons *polygons;                 // Vetor dos poligonos do .vtk
    int M;                              // Numero de poligonos
}typedef vtkFile;

struct ErrorCalculator
{
    char filename1[500];                // Arquivo da solucao analitica
    char filename2[500];                // Arquivo da solucao aproximada
    char filename3[500];                // Arquivo do erro
    vtkFile *f1;                        // Solucao analitica         
    vtkFile *f2;                        // Solucao aproximada
    vtkFile *f3;                        // Erro
}typedef ErrorCalculator;

ErrorCalculator* newErrorCalculator (int argc, char *argv[]);
vtkFile* readFile (char filename[]);
void calculateError (ErrorCalculator *e);
void errorMsg ();
void freeMemory (ErrorCalculator *error);
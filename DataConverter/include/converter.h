#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

struct Point;
struct MiocardiumReader;

struct Point
{
    double x, y, z;
    double scalar;
}typedef Point;

struct Poly
{
    int v1, v2, v3;
}typedef Poly;

struct VTPReader
{
    int nPoints;
    int nPolys;
    Point *points;
    Poly *polys;

}typedef VTPReader;

void VTK_To_DAT (char *inFilename, char *outFilename);
void VTP_To_VTK (char *inFilename, char *outFilename);
VTPReader* readVTP (char *inFilename);
int readNumberPoints (char *str);
char* changeExtension (char *inFilename, char *ext);
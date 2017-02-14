#include "../include/error.h"

ErrorCalculator* newErrorCalculator (int argc, char *argv[])
{
    printf("=============================================================================\n");
    ErrorCalculator *error = (ErrorCalculator*)malloc(sizeof(ErrorCalculator));
    strcpy(error->filename1,argv[1]);
    strcpy(error->filename2,argv[2]);
    strcpy(error->filename3,"error.vtk");
    return error;
}

vtkFile* readFile (char filename[])
{
    printf("[!] Reading file %s ... ",filename);
    fflush(stdout);
    char str[200], str2[200];
    int trash;
    FILE *in = fopen(filename,"r");
    vtkFile *vtk = (vtkFile*)malloc(sizeof(vtkFile));

    // Ler as coordenadas dos pontos
    while (fscanf(in,"%s",str) != EOF)
        if (strcmp(str,"POINTS") == 0) break;
    if (!fscanf(in,"%d",&vtk->N)) errorMsg();
    vtk->points = (Point*)malloc(sizeof(Point)*vtk->N);
    if (!fscanf(in,"%s",str)) errorMsg();
    for (int i = 0; i < vtk->N; i++)
        if (!fscanf(in,"%lf %lf %lf",&vtk->points[i].x,&vtk->points[i].y,&vtk->points[i].z)) errorMsg();

    // Ler os poligonos
    if (!fscanf(in,"%s %d %s",str,&vtk->M,str2)) errorMsg();
    vtk->polygons = (Polygons*)malloc(sizeof(Polygons)*vtk->M);
    for (int i = 0; i < vtk->M; i++)
        if (!fscanf(in,"%d %d %d %d",&trash,&vtk->polygons[i].id1,&vtk->polygons[i].id2,&vtk->polygons[i].id3)) errorMsg();
    
    // Ler os tempos de ativacao dos pontos
    while (fscanf(in,"%s",str) != EOF)
        if (strcmp(str,"default") == 0) break;
    for (int i = 0; i < vtk->N; i++)
        if (!fscanf(in,"%lf",&vtk->points[i].aTime)) errorMsg();
    fclose(in);

    // Retorna a estrutura montada
    printf("ok\n");
    return vtk;
}

void calculateError (ErrorCalculator *e)
{
    // Ler as entradas
    e->f1 = readFile(e->filename1);
    e->f2 = readFile(e->filename2);
    
    // Alocar as estruturas para o arquivo de saida
    int N = e->f1->N;
    int M = e->f1->M; 
    e->f3 = (vtkFile*)malloc(sizeof(vtkFile));
    e->f3->points = (Point*)malloc(sizeof(Point)*N);
    e->f3->N = N;
    e->f3->polygons = (Polygons*)malloc(sizeof(Polygons)*M);
    e->f3->M = M;

    // Escrever o arquivo de saida (erro)
    printf("[!] Writing output file ...");
    fflush(stdout);
    FILE *out = fopen(e->filename3,"w+");
    fprintf(out,"# vtk DataFile Version 3.0\n");
    fprintf(out,"vtk output\n");
    fprintf(out,"ASCII\n");
    fprintf(out,"DATASET POLYDATA\n");
    fprintf(out,"POINTS %d float\n",N);
    // Pontos
    for (int i = 0; i < N; i++)
        fprintf(out,"%e %e %e\n",e->f1->points[i].x,e->f1->points[i].y,e->f1->points[i].z);
    // Poligonos 
    fprintf(out,"POLYGONS %d %d\n",M,M*4);
    for (int i = 0; i < M; i++)
        fprintf(out,"3 %d %d %d\n",e->f1->polygons[i].id1,e->f1->polygons[i].id2,e->f1->polygons[i].id3);
    // Tempo de ativacao: erro = fabs(analit - aprox)
    fprintf(out,"POINT_DATA %d\n",N);
    fprintf(out,"SCALARS error float\n");
    fprintf(out,"LOOKUP_TABLE default\n");
    for (int i = 0; i < N; i++)
        fprintf(out,"%e\n",fabs(e->f1->points[i].aTime-e->f2->points[i].aTime));
    fclose(out);
    printf("ok\n");
}

void errorMsg ()
{
    printf("[-] Error found!\n");
    exit(-1);
}

void freeMemory (ErrorCalculator *e)
{
    printf("[!] Freeing memory ...");
    fflush(stdout);
    free(e->f1->points);
    free(e->f1->polygons);
    free(e->f2->points);
    free(e->f2->polygons);
    free(e->f3->points);
    free(e->f3->polygons);
    free(e->f1);
    free(e->f2);
    free(e->f3);
    free(e);
    printf("ok\n");
}

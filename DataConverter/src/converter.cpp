#include "../include/converter.h"

void VTK_To_DAT (char *inFilename, char *outFilename)
{
    FILE *inFile = fopen(inFilename,"r");
    FILE *outFile = fopen(outFilename,"w+");
    char str[100];
    int N;
    printf("Converting .vtk to .dat ... ");
    fflush(stdout);
    // Ler os pontos
    while (fscanf(inFile,"%s",str) != EOF)
        if (strcmp(str,"POINTS") == 0) break;
    if (!fscanf(inFile,"%d",&N)) printf("[-] ERRO! Lendo arquivo\n");
    if (!fscanf(inFile,"%s",str)) printf("[-] ERRO! Lendo arquivo\n");
    // Escrever no arquivo de saida
    fprintf(outFile,"%d %d\n",N,3);
    Point points[N];
    for (int i = 0; i < N; i++)
    {
        if (!fscanf(inFile,"%lf %lf %lf",&points[i].x,&points[i].y,&points[i].z)) printf("[-] ERRO! Lendo arquivo\n");;
        fprintf(outFile,"%lf %lf %lf\n",points[i].x,points[i].y,points[i].z);
    }
    printf("ok\n");
    fclose(inFile);
    fclose(outFile);    
}

// Dada uma string retirar os caracteres que sao numeros e converter para inteiro
int readNumberPoints (char *str)
{
    int i, j;
    char numberPoints[50];
    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
        if (str[i] > 47 && str[i] < 58)
        {
            numberPoints[j] = str[i];
            j++;
        }
    }
    return atoi(numberPoints);
}

// Le um arquivo VTP e armazena seus dados em uma estrutura VTPReader
VTPReader* readVTP (char *inFilename)
{
    char str[100];
    VTPReader *vtp = (VTPReader*)malloc(sizeof(VTPReader));
    FILE *file_in = fopen(inFilename,"r");

    if (file_in == NULL)
    {
        printf("[-] ERROR! Reading file %s!\n",inFilename);
        exit(-1);
    }
    // Acha o inicio da tag com o numero de pontos
	while (fscanf(file_in,"%s",str) != EOF)
		if (strcmp(str,"<Piece") == 0) break;
    if (!fscanf(file_in,"%s",str)) printf("[-] ERRO! Lendo arquivo\n");

    // Descobrir o numero de Points
    vtp->nPoints = readNumberPoints(str);

    // Descobrir o numero de Polydata
    for (int i = 0; i < 3; i++) if (!fscanf(file_in,"%s",str)) printf("[-] ERRO! Lendo arquivo\n");
    if (!fscanf(file_in,"%s",str)) printf("[-] ERRO! Lendo arquivo\n");;
    vtp->nPolys = readNumberPoints(str);

    // Acha o inicio da lista de Points
	while (fscanf(file_in,"%s",str) != EOF)
    {
		if (strcmp(str,"<DataArray") == 0) 
        {
            for (int i = 0; i < 6; i++) if (!fscanf(file_in,"%s",str)) printf("[-] ERRO! Lendo arquivo\n");
            break;
        }
    }

    // Alocar memoria para o vetor de Point
    vtp->points = (Point*)malloc(sizeof(Point)*vtp->nPoints);

    // Ler os pontos
    for (int i = 0; i < vtp->nPoints; i++)
        if (!fscanf(file_in,"%lf %lf %lf",&vtp->points[i].x,&vtp->points[i].y,&vtp->points[i].z)) printf("[-] ERRO! Lendo arquivo\n");
    
    // Acha o inicio da lista de PolyData
	while (fscanf(file_in,"%s",str) != EOF)
    {
		if (strcmp(str,"<Polys>") == 0) 
        {
            for (int i = 0; i < 6; i++) if (!fscanf(file_in,"%s",str)) printf("[-] ERRO! Lendo arquivo\n");
            break;
        }
    }

    // Alocar memoria para o vetor de PolyData
    vtp->polys = (Poly*)malloc(sizeof(Poly)*vtp->nPolys);

    // Ler as PolyData
    for (int i = 0; i < vtp->nPolys; i++)
        if (!fscanf(file_in,"%d %d %d",&vtp->polys[i].v1,&vtp->polys[i].v2,&vtp->polys[i].v3)) printf("[-] ERRO! Lendo arquivo\n");

    fclose(file_in);
    return vtp;
}

void VTP_To_VTK (char *inFilename, char *outFilename)
{
    FILE *file_out = fopen(outFilename,"w+");
    VTPReader *vtp = readVTP(inFilename);
    printf("Converting .vtp to .vtk ... ");
    fflush(stdout);
    fprintf(file_out,"# vtk DataFile Version 2.0\n");
    fprintf(file_out,"Ventriculo Recortado 3D do Benjamim Liu\n");
    fprintf(file_out,"ASCII\n");
    fprintf(file_out,"DATASET POLYDATA\n");
    fprintf(file_out,"POINTS %d float\n",vtp->nPoints);
    for (int i = 0; i < vtp->nPoints; i++)
        fprintf(file_out,"%e %e %e\n",vtp->points[i].x,vtp->points[i].y,vtp->points[i].z);
    fprintf(file_out,"POLYGONS %d %d\n",vtp->nPolys,vtp->nPolys*4);
    for (int i = 0; i < vtp->nPolys; i++)
        fprintf(file_out,"3 %d %d %d\n",vtp->polys[i].v1,vtp->polys[i].v2,vtp->polys[i].v3);
    fclose(file_out);
    printf("ok\n");

}

// Muda a extensao do arquivo de entrada de .vtp para .vtk
char* changeExtension (char *inFilename, char *ext)
{
    char *outFilename = (char*)malloc(sizeof(char)*200);
    strcpy(outFilename,inFilename);
    outFilename[strlen(outFilename)-3] = ext[0];
    outFilename[strlen(outFilename)-2] = ext[1];
    outFilename[strlen(outFilename)-1] = ext[2];
    return outFilename;
}
//copyright Michel Chapron ENSEA Cergy-Pontoise
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

FILE *fio,*fir;

int main() 
{
unsigned char chi;
int *io,*ir,LUT[256];
long int hist[256], mmax;
int nbg;
register long int i,j,k;
long int dimx,dimy,size;
char input[35],output[35],string[10];

printf("write input file name without .pgm\n");
scanf("%s",input);
strcat(input,".pgm");

printf("write output file name without .pgm\n");
scanf("%s",output);
strcat(output,".pgm");

fio=fopen(input,"rb"); //"rb" = Read Binary
fir=fopen(output,"wb+");

if(fio !=NULL) printf("in file opening\n"); 
if(fir !=NULL) printf("out file opening\n");

//pgm header reading
fscanf(fio,"%s\n",string);
fprintf(fir,"%s\n",string);

i=0;
while(i<1) 
{
	chi=fgetc(fio);
	fputc(chi,fir);
	if(chi=='\n') i++;
}

fscanf(fio,"%ld %ld\n",&dimx,&dimy);
fprintf(fir,"%ld %ld\n",dimx,dimy);

fscanf(fio,"%d\n",&nbg);
fprintf(fir,"%d\n",nbg);

printf("dimx=%d dimy=%d nbg=%d\n",dimx,dimy,nbg);

//dynamic memory allocation of io and ir images
io=(int *)malloc(dimx*dimy*sizeof(int));
ir=(int *)malloc(dimx*dimy*sizeof(int));
if(ir==NULL) printf("impossible ir allocation\n");

size=dimx*dimy;
printf("size=%ld\n",size);

//pixels reading
for(i=0;i<dimy;i++)
	for(j=0;j<dimx;j++) 
	{
		chi=(unsigned char)fgetc(fio);
		io[i*dimx+j]=(int)chi;
	}


///////////////////////////////////////////////////////////////////
//Processing
//Histogram calculation
for(i=0;i<255;i++) hist[i]=0;

for(i=0;i<dimy;i++)
	for(j=0;j<dimx;j++)
		hist[io[i*dimx+j]]++;

//Histogram creation
mmax=hist[0];
for(k=1;k<255;k++) if(hist[k]>mmax) mmax=hist[k]; //Find the max of the hist
for(k=0;k<255;k++) hist[k]= ((long int)(dimy)*hist[k])/mmax; //Hist normalization depending on the max (144)

for(j=0;j<255;j++){
	for(i=0;i<(dimy-hist[j]);i++) ir[i*dimx+j]=0;
	for(i=(dimy-hist[j]);i<dimy;i++) ir[i*dimx+j]=255;
}

//End of processing
///////////////////////////////////////////////////////////////////
 
//Resulting image saving in fir file
for(i=0;i<dimy;i++)
	for(j=0;j<dimx;j++)
		fputc((unsigned char)ir[i*dimx+j],fir);

//Charts memory release and files closing
free(io); free(ir);
fclose(fio); fclose(fir);
return 0;
}


//
// Program 4piTherm
//
// To obtain "average radiant temperatures" from 4pi stereoradiants thermographic pictures (obtained, for instance, by joining ordinary thermographies)
//
// input file: ascii file: input.dat, contents:
//
//Lines
//1   -   typ: kind of use: <1, the same as 1, but after calculations, reads file with air temperature, radiation on horizontal plane (meteodat.dat), angle of sun, 
//             transmitance (assuming there could be some partial shadow by trees, for instance), and absortance of solar radiation by the subject/cylinder  
//             kind of use: =1, read 4pi thermographic picture as 2 files: Imagez.bmp, zenit file; and Imagen.bmp, nadir file (spherical projections)
//                               Tmin, Tmax (2 lines) giving the minimum and maximum temperatures in the pictures (both the same). Actually,
//								 computes T of a pixel from T= Tmin + (Tmax-Tmin)*(b+g+r)/3/255 
//								 (add some extra lines at the end to assure correct closing of file)
//                          >3, will read (x,y) coordinates of (typ) number of points (max 50) (each coordinate, i.e., x, in a separate line) 
//							    To obtain, from a.bmp, a file a.dat  giving the bgr (r,g,b) values in the picture (to calibrate function relating 
//                              r,g,b to temperatures
// 2   -   Tmin
// 3   -   Tmax                  Consider enlarged Tmax-Tmin because data could be incorrect near the extreme values
// If typ>3 will read coordinates here (one value per line, 2 lines (x,y) each point)
//                                            
// Outputs resul.dat, ascii file, with the spherical average for each  two images, imagez.bmp and imagen.bmp, and total 
// (in the assumption both images are equal size)
// Separate spherical averages, averages over horizontal surface and vertical (cylindrical) surface, to ease averaging over cylinder model of human body, 
// 0.88 m2 lateral (vertical) surface and 0.06 + 0.06 m2 up and down respectively.
// for lineal averages, standard deviation is computed. If average is not far from T^4 (thermal radiation) averages, then  (lineal) standard
// deviation gives an indication of non-uniformity of thermal radiation
// Outputs Tradsola.dat, ascii file with results considering also direct solar radiation for spherical reception and cylinder -model of human- receptions
//
// June 2025 stand alone for processing zenit and nadir thermographic pictures - scaled in RGB gray scale, from Tmin to Tmax
// Include computation with solar radiation, data in devoted separate file
//
// Lines 176-182 of c++ listing has the formula for obtaining temperature from r g b values in thermographic pictures. Modify to your convenience
// and responsability
// This is a research program produced by Aiem group at ETSAB (UPC), June 2025.
// The authors are not liable for the results or interpretation of them. Intended only for specialists with criteria and judgement
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <io.h>
#include <math.h>
//#include <windows.h>
int main(void)  {
double rr2,co,si,si2,cocont,sicont,co2,Tmin,Tmax,T2k,T2avk,T2avsk,T2gn,T2cil,sinaa,cosaa;
double T2gav,xx,yy,r2,T2g,T2,T2av,T2avs,acocont,asicont,bgr,Tair,RadS,angle,transmit,absorb;
double sT2av,sT2avs,sT2g,sT2gav,aT2av,aT2avs,aT2g,asT2av,asT2avs,asT2g,Tgz,Thz,Tvz,Tgn,Thn,Tvn,T1gn,T1cil;
long anchura,altura,anchura2,altura2,nn,ann,xxx[50],yyy[50],k,typ,acumn;
unsigned char bbc,bmax,r,g,b,bb,gg,rr;
char  line[100],c;
FILE *PunteroImagenInici;
FILE *PunteroImagenInicial;
FILE *PunteroDatos;
FILE *PunteroDatoc;
FILE *PunteroClave;
FILE *PunteroMeteo;
;
// To do by the program:   // Read pixel by pixel the picture photos (in Imagez.bmp, Imagen.bmp)
// Convert pixel BGR values to T (grey (0-255)) -> value = (T-Tmin)*255/(Tmax-Tmin) -> T = Tmin+ (Tmax-Tmin)*digit/255  // alternative:put formula
// Obtain cos(theta) for the pixels and add the temperatures to obtain mean (radiant) temperature    // Output of average radiant temperature
printf("Processing of spherical thermographs. (Zenit and Nadir views)  Input: \n");
printf(" \n");
printf("Numerical file (ascii format) input.dat contents: Type (=1, process of thermographs; -1 include direct solar; \n");
printf(">3 number of points to obtain rgb values from thermograph and calibrate according to temperatures obtained (see lines 176-182)\n");
printf("Following two lines: Tmin, Tmax; if typ>3, typ times x,y (two lines) to obtain pixel values \n");
printf("to calibrate pixel values against thermographic temperatures.    Attention: start left, down of pictures \n");
printf(" \n");
printf("Start:  Return(Intro).     Exit: Ctrl-C \n");
c=getchar();
;
PunteroClave = fopen("input.dat","r");
   fgets(line, sizeof(line), PunteroClave);
   sscanf(line, "%ld", &typ);
   printf("typ:  %ld \n",typ);
   fgets(line, sizeof(line), PunteroClave);
   sscanf(line, "%lf", &Tmin);
   fgets(line, sizeof(line), PunteroClave);
   sscanf(line, "%lf", &Tmax);
;   //typ>3 read typ points (x,y; attention starts lower left of picture) to calibrate temperatures to point value
   if(typ>3)
   {
for (k=0; k<typ;k++) {
   fgets(line, sizeof(line), PunteroClave);
   sscanf(line, "%ld", &xxx[k]);
   fgets(line, sizeof(line), PunteroClave);
   sscanf(line, "%ld", &yyy[k]);
//   printf("k, xxx[k], yyy[k] %lf %lf %lf \n", k,xxx[k],yyy[k]);
}
;  
}
;
;
fclose(PunteroClave);  //lecture input file ended
//************************************************************************************************************************************
if (typ<=1)
{ // The actual case: do 2 times, zenith and nadir (Imagez and Imagen)
  	PunteroDatos = fopen("resul.dat","w");
//2 blocks zenit & nadir
PunteroImagenInicial = fopen("Imagez.bmp","rb");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Zenit:  \n");
fprintf(PunteroDatos, " \n");
; //initialize
T2av=0.0;
T2avs=0.0;
T2avk=0.0;
T2avsk=0.0;
T2g=0.0;
T2gav=0.0;
sT2av=0.0;
sT2avs=0.0;
sT2g=0.0;
sT2gav=0.0;
sT2av=0.0;
sT2avs=0.0;
sT2g=0;
sT2gav=0;
sicont=0;
nn=0;
cocont=0;
;
printf("typ, Tmin, Tmax: %ld, %f %f \n", typ, Tmin, Tmax);
printf(" \n");
// Leemos los encabezamientos 
b = fgetc(PunteroImagenInicial);
g = fgetc(PunteroImagenInicial);
//printf to confirm bitmap (generic) -> if NOT BM, take care! no further checkings
printf("1, 2 (Zenit is B M? no further checks): %c %c \n",b,g);
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
bb = fgetc(PunteroImagenInicial); 
gg = fgetc(PunteroImagenInicial); 
rr = fgetc(PunteroImagenInicial); 
bmax = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
anchura=b+256*(g+256*(r+256*bbc));
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial);
altura=b+256*(g+256*(r+256*bbc));
fseek(PunteroImagenInicial,bb+256*(gg+256*(rr+256*bmax)),SEEK_SET);  //goes directly to start of picture data. Assumed rgb bitmap!
altura2=altura/2;
anchura2=anchura/2;
r2=anchura2;
if(r2>altura2) {r2=altura2;};  // the smallest
;
;
r2=r2*r2;  //r2 is squared radius
;  // read total thermal picture
for(int i=0 ; i<altura; i++)  {
	yy=i-altura2;
	yy=yy*yy;
for(int j=0 ; j<anchura; j++) {
b = fgetc(PunteroImagenInicial); // B
g = fgetc(PunteroImagenInicial); // G
r = fgetc(PunteroImagenInicial); // R
xx=j-anchura2;
rr2=xx*xx+yy;  //useful for cos
if(rr2<r2) {
// cos theta is co:
bgr=(b+g+r)/3.0;
si2=rr2/r2;
co2=1.-si2;
co=sqrt(co2);
si=sqrt(si2);
   //si is sin(theta), si2 is sin^2; co is cos, co2 is cos^2
T2=Tmin+(Tmax-Tmin)*bgr/255;
;
T2k=T2+273.15; 
T2k=T2k*T2k*T2k*T2k;   // T in K, radiation goes as T(K)^4
  nn=nn+1;             // no. points
  cocont=cocont+co;    //cocont sum of cos
  sicont=sicont+si;    //sicont sum of sin
  T2g=T2g+T2;  //accumulate to average
  T2gav= T2gav + T2k;
  T2av= T2av + T2*co;
  T2avk=T2avk+T2k*co;
  T2avs= T2avs + T2*si;
  T2avsk=T2avsk+T2k*si;
  sT2g = sT2g+T2*T2;    //accumulate to find standard deviations
  sT2av = sT2av + T2*T2*co2;
  sT2avs = sT2avs + T2*T2*si2;
  ;  
} // end inside circle  
;
;
}   //loop j closes
;
};  //loop i closes
	fclose(PunteroImagenInicial);
;  // Compute accumulations
aT2g=T2g;
asT2g=sT2g;
aT2av=T2av;
asT2av=sT2av;
aT2avs=T2avs;
asT2avs=sT2avs;
ann=nn;
acocont=cocont;
asicont=sicont;
acumn=altura*anchura;
; // compute zenit part values
;
T2g= T2g/nn;
T2gav=pow((T2gav/nn),0.25);
T2av= T2av/cocont;
T2avk=pow((T2avk/cocont),0.25);
T2avs=T2avs/sicont;
T2avsk=pow((T2avsk/sicont),0.25);
//compute standard deviations. Avoid negatives in sqrt!
sT2g=sqrt(sqrt(((sT2g-nn*T2g*T2g)/nn)*(sT2g-nn*T2g*T2g)/nn));
sT2av=sqrt(sqrt(((sT2av-cocont*T2av*T2av)/cocont)*(sT2av-cocont*T2av*T2av)/cocont));
sT2avs=sqrt(sqrt(((sT2avs-sicont*T2avs*T2avs)/sicont)*(sT2avs-sicont*T2avs*T2avs)/sicont));
;  //write results
;
fprintf(PunteroDatos, "typ, Tmin, Tmax: %ld %f %f \n", typ, Tmin, Tmax);
fprintf(PunteroDatos,"Zenit height, width: %ld %ld \n", altura, anchura);
fprintf(PunteroDatos,"r2, r: %f %f \n", r2, sqrt(r2));
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Zenit:  \n");
;
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Selected points, from total:  %ld %ld \n",nn,altura*anchura);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Spheric average, stdesv (ºC)  %f %f \n", T2g,sT2g);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Tav (horiz. average cos, T2av,  stdesv (ºC):  %f %f \n",T2av,sT2av);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Tavs (vert. average sin, T2avs, stdesv (ºC):  %f %f \n",T2avs,sT2avs);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Radiation average   (T(K))^4) ) \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2gav, (Spherical   average   T^4 (K)): %f \n",T2gav);
//fprintf(Punteromud,"  \n");
printf("T2gav %f \n",T2gav);
Tgz=T2gav;
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2avk, (horiz. average  cos   T^4 (K)): %f \n",T2avk);
//fprintf(Punteromud, "  %f \n",T2avk);
Thz=T2avk;
printf("T2avk  %f \n",T2avk);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2avsk (vert.  average  sin   T^4 (K)): %f \n",T2avsk);
Tvz=T2avsk;
//fprintf(Punteromud,"  %f \n",T2avsk);
printf("T2avsk  %f \n",T2avsk);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Cocont= Suma cos; Sicont= Suma sin \n");
fprintf(PunteroDatos,"cocont,sicont: %f %f \n", cocont,sicont);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");

printf("typ, Tmin, Tmax: %ld %f %f \n", typ, Tmin, Tmax);
printf("Height,  width : %ld %ld \n", altura, anchura);
printf("T2g, selected points, from total %f %ld %ld \n",T2g, nn, altura*anchura);
printf("T2av, T2avs:  %f %f \n",T2av,T2avs);
printf("cocont,sicont: %f %f \n", cocont,sicont);
printf(" \n");
printf("Radiation average (T^4), in K \n");
printf("T2gav (Spherical average): %f \n",T2gav);
printf("T2avk (cos average), T2avsk (sin average): %f %f \n",T2avk,T2avs);
acumn=altura*anchura;
;
;
 //2 blocks zenit, nadir ->  Nadir
PunteroImagenInicial = fopen("Imagen.bmp","rb");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Nadir:  \n");
fprintf(PunteroDatos, " \n");
; //initialize
T2av=0.0;
T2avs=0.0;
T2avk=0.0;
T2avsk=0.0;
T2avs=0.0;
T2g=0.0;
T2gav=0.0;
sT2av=0.0;
sT2avs=0.0;
sT2g=0.0;
sT2gav=0.0;
sicont=0.0;
nn=0;
cocont=0.0;
;
printf("typ, Tmin, Tmax: %ld, %f %f \n", typ, Tmin, Tmax);
printf(" \n");
; // Read headings 
b = fgetc(PunteroImagenInicial);
g = fgetc(PunteroImagenInicial);
printf("1, 2 (Nadir is B M? no further checks): %c %c \n",b,g);
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
bb = fgetc(PunteroImagenInicial); 
gg = fgetc(PunteroImagenInicial); 
rr = fgetc(PunteroImagenInicial); 
bmax = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
anchura=b+256*(g+256*(r+256*bbc));
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial);
altura=b+256*(g+256*(r+256*bbc));
fseek(PunteroImagenInicial,bb+256*(gg+256*(rr+256*bmax)),SEEK_SET);
altura2=altura/2;
anchura2=anchura/2;
r2=anchura2;
if(r2>altura2) {r2=altura2;};
;
;
r2=r2*r2; //squared radius
;  // read total thermal picture
for(int i=0 ; i<altura; i++)  {
	yy=i-altura2;
	yy=yy*yy;
for(int j=0 ; j<anchura; j++) {
b = fgetc(PunteroImagenInicial); // B
g = fgetc(PunteroImagenInicial); // G
r = fgetc(PunteroImagenInicial); // R
//Attention inside circle
xx=j-anchura2;
//select
rr2=xx*xx+yy;  //for cos
if(rr2<r2) {
// cos theta:
si2=rr2/r2;
co2=1.-si2;
co=sqrt(co2);
si=sqrt(si2);
bgr=(b+g+r)/3.0;
   //si is sin(theta), si2 is sin^2; co is cos, co2 is cos^2
T2=Tmin+(Tmax-Tmin)*bgr/255; 
;
T2k=T2+273.15;
T2k=T2k*T2k*T2k*T2k;   // T in K, radiation in T(K)^4
  nn=nn+1;             // no. useful points
  cocont=cocont+co;    //cocont sum of cos
  sicont=sicont+si;    //sicont sum of sin
; // Sum
  T2g =  T2g+T2;
  T2gav= T2gav + T2k;
  T2av= T2av + T2*co;
  T2avk=T2avk+T2k*co;
  T2avs= T2avs + T2*si;
  T2avsk=T2avsk+T2k*si;
  sT2g = sT2g+T2*T2;
  sT2av = sT2av + T2*T2*co2;
  sT2avs = sT2avs + T2*T2*si2;
  ;  
} // end inside circle
;
;
}   //close j 
;
;
};  //close i 
;
	fclose(PunteroImagenInicial);
// accumulate
aT2g=aT2g+T2g;
asT2g=asT2g+sT2g;
aT2av=aT2av+T2av;
asT2av=asT2av+sT2av;
aT2avs=aT2avs+T2avs;
asT2avs=asT2avs+sT2avs;
ann=ann+nn;
acocont=acocont+cocont;
asicont=asicont+sicont;
;  // nadir only
T2g= T2g/nn;
T2gav=pow((T2gav/nn),0.25);
T2av= T2av/cocont;
T2avk=pow((T2avk/cocont),0.25);
T2avs=T2avs/sicont;
T2avsk=pow((T2avsk/sicont),0.25);
sT2g=sqrt(sqrt(((sT2g-nn*T2g*T2g)/nn)*(sT2g-nn*T2g*T2g)/nn));
sT2gav=sqrt(sqrt(((sT2gav-nn*T2gav*T2gav)/nn)*(sT2gav-nn*T2gav*T2gav)/nn));
sT2av=sqrt(sqrt(((sT2av-cocont*T2av*T2av)/cocont)*(sT2av-cocont*T2av*T2av)/cocont));
sT2avs=sqrt(sqrt(((sT2avs-sicont*T2avs*T2avs)/sicont)*(sT2avs-sicont*T2avs*T2avs)/sicont));
;  //write down
//	Punteromud= fopen("mudo.dat","w");
fprintf(PunteroDatos, "typ, Tmin, Tmax: %ld %f %f \n", typ, Tmin, Tmax);
fprintf(PunteroDatos,"Nadir height, width: %ld %ld \n", altura, anchura);
fprintf(PunteroDatos,"r2, r: %f %f \n", r2, sqrt(r2));
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Nadir:  \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Selected points of TOTAL picture:  %ld %ld \n",nn,altura*anchura);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2g  Spherical average T2(ºC),  stdesv:  %f  %f \n", T2g, sT2g);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2av  (hor.  average cos), stdesv (ºC):  %f %f \n",T2av,sT2av);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2avs (vert. average sin), stdesv (ºC):  %f %f \n",T2avs,sT2avs);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Radiation average (T(K))^4)) \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2gav  (spherical average  T^4    (K)): %f \n",T2gav);
Tgn=T2gav;
//fprintf(Punteromud, "  %f \n",T2gav);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2avk  (hor. average  cos  T^4    (K)): %f \n",T2avk);
Thn=T2avk;
//fprintf(Punteromud, "  %f \n",T2avk);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "T2avsk (vert. average sin  T^4    (K)): %f \n",T2avsk);
Tvn=T2avsk;
//fprintf(Punteromud, "  %f \n",T2avsk);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Cocont= Sum cos; Sicont= Sum sin \n");
fprintf(PunteroDatos,"cocont,sicont: %f %f \n", cocont,sicont);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
;
printf("typ, Tmin, Tmax: %ld %f %f \n", typ, Tmin, Tmax);
printf("Nadir height, width: %ld %ld \n", altura, anchura);
printf("T2g, selected points, total picture %f %ld %ld \n",T2g, nn, altura*anchura);
printf("T2av, T2avs: %f %f \n",T2av,T2avs);
printf("cocont,sicont: %f %f \n", cocont,sicont);
printf(" \n");
printf("Average with radiation (T^4), in K \n");
printf("T2gav (spheric average): %f \n",T2gav);
printf("T2avk (cos average), T2avsk (sin average) %f %f \n",T2avk,T2avsk);
printf(" \n");
//Write spherical
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "TOTAL SPHERIC:  Selected points, from TOTAL: %ld %ld \n",ann,acumn+altura*anchura);
fprintf(PunteroDatos, " \n");
;
aT2g=aT2g/ann;
asT2g=sqrt(sqrt((asT2g-ann*aT2g*aT2g)*(asT2g-ann*aT2g*aT2g)/ann/ann));
fprintf(PunteroDatos, "Spheric average   T2g,   stdev (ºC):  %f %f \n", aT2g,asT2g);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
aT2av=aT2av/acocont;
asT2av=sqrt(sqrt((asT2av-acocont*aT2av*aT2av)*(asT2av-acocont*aT2av*aT2av)/acocont/acocont));
fprintf(PunteroDatos, "T2av,  (hor. average,  cos ºC), stdev : %f %f \n",aT2av,asT2av);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
aT2avs=aT2avs/asicont;
asT2avs=sqrt(sqrt((asT2avs-asicont*aT2avs*aT2avs)*(asT2avs-asicont*aT2avs*aT2avs)/asicont/asicont));
fprintf(PunteroDatos, "T2avs, (vert. average, sin ºC), stdesv : %f %f \n",aT2avs,asT2avs);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fclose(PunteroDatos);
//printf("End 1:  Pulse intro \n");
//c=getchar();
;
;
;
if(typ<1)
{  //open meteo and file with direct values, compute and write file with Trad including sun 
//printf("Continue:  Intro.   Exit: Ctrl-C \n");
//c=getchar();	
PunteroDatoc = fopen("meteodat.dat","r");
PunteroDatos = fopen("TradSola.dat","w");
//  compute and write down TrradSolar.  Tmud contains Tglob(K), Th(K), Tv(K) for zenit and nadir (6 numbers)
//  air temperature, radiation on horizontal plane (meteodat.dat), angle of sun elevation (º) over horizontal, transmitance
//       Tair, RadS, angle, transmit, absorb
//                              (assuming there could be some partial shadow by trees, for instance), and absortance of solar radiation by the subject/cylinder 
   fgets(line, sizeof(line), PunteroDatoc);
   sscanf(line, "%lf", &Tair);
   fgets(line, sizeof(line), PunteroDatoc);
   sscanf(line, "%lf", &RadS);
   fgets(line, sizeof(line), PunteroDatoc);
   sscanf(line, "%lf", &angle);
   fgets(line, sizeof(line), PunteroDatoc);
   sscanf(line, "%lf", &transmit);
   fgets(line, sizeof(line), PunteroDatoc);
   sscanf(line, "%lf", &absorb);
   printf("Meteo and properties: %f %f %f %f %f \n", Tair,RadS,angle,transmit,absorb);
   // compute: Cylinder: 1m2, lat 0,88 m2, each (top-down) 0,06 m2
   // sigma= 6,67 E-8 W/m2.K4 (Stefan-Boltzmann Law)
   //compute with T^4
   //T1 globe without air not sun, only IR exchanges //to add difuse light if needed  //T1gn / T1cil
   T1gn=pow((Tgz*Tgz*Tgz*Tgz+Tgn*Tgn*Tgn*Tgn)/2,0.25);
   T1cil=pow(0.88*(Tvz*Tvz*Tvz*Tvz+Tvn*Tvn*Tvn*Tvn)/2+0.06*(Thz*Thz*Thz*Thz+Thn*Thn*Thn*Thn),0.25);
 // T2 globe and cyl, T2g, T2cil  with direct solar radiation
 //attention finds solar radiation in normal direction from solar rad in horiz and angle
 // T2gn globe intercepts piR^2 of 4*piR^2 area sphere.
 cosaa=cos(angle*3.1415926/180);
sinaa=pow(1.0-cosaa*cosaa,0.5);
if(cosaa<=0) {cosaa=0.001;
}
;
;
if(sinaa<=0.) {sinaa=0.001;
           } 
           ;
           ;
		   T2gn=pow((T1gn*T1gn*T1gn*T1gn+RadS*transmit*absorb/(0.0000000667*sinaa*4.)),0.25);
                   cosaa=pow((1.-sinaa*sinaa),0.5);
                   if(cosaa<=0.) { cosaa=0.001;
               }
               ;
               ;
               
//T2cil=pow((T1cil*T1cil*T1cil*T1cil+0.06*transmit*absorb*RadS/0.0000000667),0.25);
 T2cil=pow((T1cil*T1cil*T1cil*T1cil+0.06*transmit*absorb*RadS/0.0000000667+0.88*transmit*absorb*RadS*cosaa/(sinaa*0.0000000667*3.1415926)),0.25);
;
;
;
;//write down results	think about Teq globe in air, is there wind? coef from references
// results average Trad for sphere (point)
// results for cylinder human model: 0.88 m2 lateral, 0.06 m2 top and down
fprintf(PunteroDatos, "Results of Radiant temperatures \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Meteo and other data: Tair, RadS, angle, transmit, absorb %f %f %f %f %f \n",Tair,RadS,angle, transmit, absorb);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "No direct Sun, from IR pictures Imagez and Imagen: \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Equivalent Radiant temperature for a sphere: %f (K)    %f (ºC) \n", T1gn, T1gn-273.15);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Equivalent Radiant temperature for a cylinder 0.88m2 lateral, 0.06 m2 up: %f (K)   %f (ºC) \n", T1cil,T1cil-273.15);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "With direct Sun added: \n");
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Horizontal sun radiation %f (W/m2)   angle resp. horiz.  %f (º) \n ", RadS,angle);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Equivalent Radiant temperature for a sphere under Sunlight: %f (K)    %f (ºC) \n", T2gn, T2gn-273.15);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, "Equivalent Radiant temperature for a cylinder 0.88m2 lateral, 0.06 m2 up under Sunlight: %f (K)   %f (ºC) \n", T2cil,T2cil-273.15);
fprintf(PunteroDatos, " \n");
fprintf(PunteroDatos, " sinaa, cosaa: %f %f \n",sinaa,cosaa);
// close
fclose(PunteroDatos);	
fclose(PunteroDatoc);	
}  // close case typ<1
} //close case typ<=1
;
;
 if(typ>3)
{ //ask for typ points, coord xi, yi will give values r g b average 3*3  
//  file for results
PunteroDatoc = fopen("a.dat","w");
PunteroImagenInici = fopen("a.bmp","rb");
printf("input file a.bmp found, with typ>3 number of points whose coordinates x,y were in input.dat, will read and note in  a.dat the bgr values \n");
printf("Start (typ>3):  Intro. Exit: Ctrl-C \n");
c=getchar();
; // Read headings of bmp
b = fgetc(PunteroImagenInicial);
g = fgetc(PunteroImagenInicial);
printf("1, 2 (is BM?)): %c %c \n",b,g);
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
bb = fgetc(PunteroImagenInicial); 
gg = fgetc(PunteroImagenInicial); 
rr = fgetc(PunteroImagenInicial); 
bmax = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial); 
anchura=b+256*(g+256*(r+256*bbc));
b = fgetc(PunteroImagenInicial); 
g = fgetc(PunteroImagenInicial); 
r = fgetc(PunteroImagenInicial); 
bbc = fgetc(PunteroImagenInicial);
altura=b+256*(g+256*(r+256*bbc));
fseek(PunteroImagenInicial,bb+256*(gg+256*(rr+256*bmax)),SEEK_SET);
;
printf("altura, anchura %ld %ld \n",altura,anchura);
// ULL guardar a Rojo[], Verde[], Azul[]
fprintf(PunteroDatoc, " \n");
fprintf(PunteroDatoc,"typ %ld \n",typ);
fprintf(PunteroDatoc, " \n");
fprintf(PunteroDatoc,"altura, anchura %ld %ld \n",altura,anchura);
fprintf(PunteroDatoc, " \n");
;
fprintf(PunteroDatoc, "k, xxx(k), yyy(k),  b,  g,  r\n");
//ara, typ>3, valors de punts:
//c=getchar();
for(int i=0 ; i<altura; ++i) {
	;
for(int j=0 ; j<anchura; ++j){
	;
	bb= fgetc(PunteroImagenInici); // B
	gg= fgetc(PunteroImagenInici); // g
	rr= fgetc(PunteroImagenInici); // r
	;
for (int k=0; k<typ; k++)
{
if ((i==yyy[k]) && (j==xxx[k])) 
{
printf("k, xxx[k], yyy[k], Az, Ve, Ro: %d %ld %ld %d %d %d \n",k+1,xxx[k],yyy[k],bb,gg,rr);
fprintf(PunteroDatoc, " \n");
fprintf(PunteroDatoc, " %d   %ld   %ld  %d  %d  %d  \n", k+1,xxx[k],yyy[k],bb,gg,rr);
};
;
;
}
;
;
};
;
;
};
;
}; //end typ>3
;
printf(" FIN DEL PROGRAMA \n");
}
;

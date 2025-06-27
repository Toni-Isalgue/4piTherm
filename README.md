# 4piTherm
To obtain equivalent radiant temperature from 4pi (spherical) Thermographs

Instructions for the program 4piTherm (app)

Program 4piTherm has been produced to obtain "average radiant temperatures" from 4pi stereo-radiant thermographic pictures (obtained, for instance, by joining ordinary thermographs).
It is written in C++, has been compiled on windows 10 and windows 11 with Dev-C++ version 6.3, and on Linux Ubuntu 24.04 LTS with Geany 2.0 using g+ (May 2025) (attention should be used in compiling as some #include <> might be found in different directories in each case)

This is a research program produced by A. Isalgue (Toni), and  Arch. François M. Julien, Aiem group at ETSAB (UPC), June 2025. Thanks are given to Prof. Dr. Arch. Helena Coch for encouragement.
The authors are not liable for the results or interpretation of the results. Please use good quality input data, the program uses physics effects to relate the terms and does not check the values for meaningfulness. The results are sensitive to changes in input data.
The program is Intended only for knowledgeable users and educational (non-commercial) purposes, to be used or supervised by specialists with criteria and judgement



The program uses different files. For easy of use, they should be in the same directory as the application (4piTherm.exe in windows)
Files:

-input file: ascii file: input.dat (can be created and edited with notepad or a plain file editor), contents:
Lines
1   -   typ: Integer, 	kind of use: <1, the same as 1, but after calculations, reads file with air temperature, radiation on horizontal plane (meteodat.dat), angle of sun, transmittance (assuming there could be some partial shadow by trees, for instance), and absorbance of solar radiation by the subject/cylinder, one value in each line (5 lines). Please use real data, results depend strongly on the entered values.  
                        kind of use: =1, read 4pi thermographic picture as 2 files: 
                        Imagez.bmp, Zenit file; and                         
                        Imagen.bmp, nadir file (spherical projections). Should be the same size or very nearly (see below)

2-3  Tmin, Tmax (2 float, 2 lines) giving the minimum and maximum temperatures in the pictures (both the same). Actually, computes T of a pixel from T= Tmin + (Tmax-Tmin)*(b+g+r)/3/255    
(Consider using enlarged Tmax-Tmin because data could be incorrect near the extreme values)
(Add some extra lines at the end to assure correct closing of file)

Kind of use >=3, will read (x,y) coordinates (integer) of (typ) number of points (max 50) (each coordinate, i.e., x, in a separate line)  To obtain, from a.bmp, a file a.dat  giving the bgr (r,g,b) values in the picture (to calibrate function relating  r,g,b to temperatures

-input file: Imagez.bmp Bitmap file where to read the hemispheric thermograph corresponding to the Zenit. Might be a large file, the program only uses the bmp convention to address finding height and width, and the starting of the data. 3 unsigned char (bytes, number from 0 to 255), b, g and r for each pixel. The values of r,g,b, are converted to temperature in the program (could be modified according to your needs, Modify to your convenience and responsibility). Checked with “Iron” scale converted to grey scale (rgb) with Photoshop or Paint, from Testo and Flir pictures (only IR pictures).
The spherical-projected image should be centred and diameter of the projected image= width= height of the bitmap image

-input file: Imagen.bmp: Bitmap image corresponding to the hemispheric projection to the Nadir, the same as imagez.bmp. Is expected that the dimensions of both images are the same (or very nearly the same)


Output files: 
-resul.dat, ascii file (can be opened with Notepad, or file editor), with the spherical average for each of two images, imagez.bmp and imagen.bmp, and total. Partial values for averaged temperatures are also there. 
Separate spherical averages, averages over horizontal surface and vertical (cylindrical) surface, to ease averaging over cylinder model of human body, made as follows:  0.88 m2 lateral (vertical) surface and 0.06 + 0.06 m2 (circular) up and down respectively.
for lineal averages, standard deviation is computed. If average is not far from T^4 (thermal radiation) averages, then the lineal standard deviation gives an indication of non-uniformity of thermal radiation
Output file: Tradsola.dat, ascii file with results considering also direct solar radiation for spherical reception and cylinder -model of human- receptions

The program Includes possible computation with solar radiation (if typ<1), data in devoted separate file
Meteodat.dat (ascii file), contains meteorology and observer data: each value in a line, 5 float (Real) values:
Tair (air temperature in ºC, not used in present version), 
RadS (solar radiation on horizontal plane, in W/m2)
Angle (elevation angle of the Sun direction, angle from the horizontal, in ºC)
Transmit (transmittance of any obstacles to sunlight, as tree canopies, in per one)
Absorbance (absorbance of the cylinder to model the standing human body, in per one)
In this case, another file will be produced as result:

Output file: -TradSola.dat, ascii file, giving the equivalent radiant temperatures in the case without solar contribution, and with direct solar contribution, both for a receiving sphere and a receiving cylinder (human standing body model)

Note: .exe provided for Windows

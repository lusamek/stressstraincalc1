


/*
***********************************************************
**                                                       ** 
**  C Code to model the true stress-strain curve         **
**                                                       ** 
**  Author: L.Samek, 2016                                ** 
**                                                       ** 
***********************************************************

The code is a simple demonstration for education purposes, to show the strengthening mechanims based on glide.

It can be used for a fully bainitic steel or a ferritic steel.
To make a steel with two constituents, e.g. bainite and ferrite, just add the mixture law.



TO COMPILE:
   -On linux:
	  gcc -lm glide.c -o glide.bin; ./glide.bin  ; pdflatex graph.tex ; mupdf graph.pdf 
   -On Win32
	  It shall be similar.


LIBS AND REQUIREMENTS:
   {GCC, Math lib,
    PDFLATEX (optional)}

   - It has a relatively minimal lib requirement.
   - It needs GCC and math.h (math lib)
   - It can run on any platforms, since the plot of the flow curve is made by using PDFLATEX

*/









int OSENVIRONMENT ;  // 1:Linux, 2:Win32, 3:Mac


/// important to calculate
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//////////////////////////// LIST OF MANY LIBS ON LINUX (<-- needed for later on)
#define PATH_MAX 2040
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <string.h> 
#include <time.h> 
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>




/////// Some constants/Variables
        double const_m = 3; 
        double burgers = 2.48e-10 ; 
        double const_k = 1.5e-2;
        double const_f = 3;
        double const_n = 1; //(this is the step number)

        double epsilon_p = 5e-3; 
        double epsilon_p_step = 5e-3; 
        double dgamma = 0 ; 

        double const_grainsize , ro_0 , ro;
        double dro_grainsize , dro_forest  , dro_annihilation , dro_total ;

        double sigmap0 , sigmap ;

        FILE *fp;





  ///// VOIDS
  void graph_head(){
    fp = fopen( "graph.tex" , "wb+");
    fputs( "\\documentclass{article}\n", fp );
    fputs( "\\usepackage{tikz,pgfplots}\n", fp );
    fputs( "\\begin{document}\n", fp );
    fputs( "\\vspace{1cm}\n" , fp);
    fputs( "\\section{Plot the true stress-strain curve}\n" , fp);
    fputs( "\\vspace{1cm}\n" , fp);
    fputs( "\\begin{tikzpicture}\n", fp );
    fputs( "\\begin{axis}[%\n", fp );
    fputs( "scatter/classes={%\n", fp );
    fputs( "    a={mark=o,draw=black}}]\n", fp );
    fputs( "\\addplot[scatter,only marks,%\n", fp );
    fputs( "    scatter src=explicit symbolic]%\n", fp );
    fputs( "table[meta=label] {\n", fp );
    fputs( "x y label\n", fp );
  }


 void graph_print( char *line ){
    int  foo ; 
    char charo[PATH_MAX] ; 
    foo = snprintf( charo , PATH_MAX-1 , "%g %g a\n" , epsilon_p , sigmap );
    fputs( charo, fp );
 }


 void graph_tail(){
   int  foo ; 
   char charo[PATH_MAX] ; 
   fputs( "};\n", fp );
   fputs( "\\end{axis}\n", fp );
   fputs( "\\end{tikzpicture} \\par \n", fp );
   fputs( "(Mecking-Kocks Model with Glide only, ", fp );
   foo = snprintf( charo , PATH_MAX-1 , "D=%gm $\\rho$=%g)\n" , const_grainsize , ro_0 );
   fputs( charo, fp );
   fputs( "\\end{document}\n", fp );
   fclose( fp );
 }




 void function_display_title( ){
	printf("***************** \n");
	printf("DISLOCATION GLIDE \n");
	printf("***************** \n");
 }


 void function_display_step( int plotmode ){
        // increment and dgamma
        if ( const_n >=2 ) epsilon_p = epsilon_p + epsilon_p_step ; 
        dgamma  = const_m * epsilon_p_step; 

        // increase of dislocation density 
        dro_grainsize = 1 / ( burgers * const_grainsize ); 
        dro_forest = const_k * sqrt( ro ) / burgers ; 
        dro_annihilation =  ro * const_f ;  
        dro_total = ( dro_grainsize + dro_forest - dro_annihilation ) * dgamma  ; 
        if ( const_n == 1 )
          ro = ro_0 ; 
        else
          ro = ro + dro_total;
 
        // sigma (plastic)
        if ( const_n == 1 )
          sigmap = sigmap0;
        else
          sigmap = sigmap0 + 0.35 * 2.48e-10 * 8.26e4 * 3 * sqrt( ro );

        // display
        printf( " \nStep #%g", const_n);
        printf( " epsilon_p: %g,", epsilon_p );
        printf( " dgamma: %g \n", dgamma  );
        printf( " (+) dro grainsize: %g \n", dro_grainsize ) ; 
        printf( " (+) dro forest: %g \n",    dro_forest );
        printf( " (-) dro annihilation: %g \n", dro_annihilation );
        printf( " ______________________________ \n" );
        printf( " dro total: %g \n", dro_total );
        printf( " ro  (sum): %g \n", ro );
        printf( " sigma  (plastic): %g \n", sigmap );

        if ( plotmode == 1 ) graph_print( "" );
        const_n++;
  }




int main() {
        double iterations, i;
        int displayplot = 1;
        iterations = 102;


        // ferrite
        sigmap0 = 60; sigmap = 0;
        const_grainsize = 20e-6;
        ro_0 = 1e8; 

        // bainite
        sigmap0 = 200; sigmap = 0;
        const_grainsize = 1e-6;
        ro_0 = 1e13; 

     
        if ( displayplot == 1 )  graph_head();
        function_display_title();

        /// main loop
        for( i = 1; i <= iterations ; i++)
           function_display_step( displayplot );

        if ( displayplot == 1 )  graph_tail();

	return 0;
}






